/**********************************************************************
 * Copyright 2013 Georg Rudoy <0xd34df00d@gmail.com>
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 **********************************************************************/

#include "dboperator.h"
#include <boost/lexical_cast.hpp>
#include "db.h"
#include "operation.h"
#include "dbresult.h"

namespace Laretz
{
	DBOpError::DBOpError (ErrorCode ec, const std::string& reason)
	: runtime_error ("DB operation error " + boost::lexical_cast<std::string> (ec) + "; " + reason)
	, m_ec (ec)
	, m_reason (reason)
	{
	}

	DBOpError::~DBOpError () throw ()
	{
	}

	DBOpError::ErrorCode DBOpError::getEC () const
	{
		return m_ec;
	}

	DBOperator::DBOperator (DB_ptr db)
	: m_db { db }
	, m_op2func {
			{ OpType::List, [this] (const Operation& op) { return list (op); } },
			{ OpType::Fetch, [this] (const Operation& op) { return fetch (op); } },
			{ OpType::Append, [this] (const Operation& op) { return append (op); } },
			{ OpType::Modify, [this] (const Operation& op) { return update (op); } }
		}
	{
	}

	std::vector<DBResult> DBOperator::operator() (const std::vector<Operation>& ops)
	{
		std::vector<DBResult> result;
		for (const auto& op : ops)
		{
			const auto& opResult = apply (op);
			std::copy (opResult.begin (), opResult.end (), std::back_inserter (result));
		}
		return result;
	}

	std::vector<DBResult> DBOperator::apply (const Operation& op)
	{
		return m_op2func.find (op.getType ())->second (op);
	}

	std::vector<DBResult> DBOperator::list (const Operation& op)
	{
		if (op.getItems ().empty ())
			return {};

		const auto& reqItem = op.getItems ().front ();
		return { { m_db->enumerateItems (reqItem.getSeq (), reqItem.getParentId ()) } };
	}

	std::vector<DBResult> DBOperator::fetch (const Operation& op)
	{
		DBResult res;
		for (const auto& item : op.getItems ())
			if (const auto optItem = m_db->loadItem (item.getId ()))
				res << ResultSet_t { std::vector<Item> { *optItem } };

		return { res };
	}

	std::vector<DBResult> DBOperator::append (const Operation& op)
	{
		const auto& items = op.getItems ();
		if (items.empty ())
			return {};

		for (const auto& item : items)
		{
			const auto& parentItem = m_db->loadItem (item.getParentId ());
			if (!parentItem)
				throw DBOpError (DBOpError::ErrorCode::UnknownParent,
						"cannot insert new item into unknown parent");

			if (parentItem->getChildrenSeq () > item.getSeq ())
				throw DBOpError (DBOpError::ErrorCode::SeqOutdated,
						"cannot insert new item: parent has newer sequence id");
		}
		DBResult res;
		for (const auto& item : items)
			res.updateCurSeq (m_db->addItem (item));
		return { res };
	}

	std::vector<DBResult> DBOperator::update (const Operation& op)
	{
		const auto& items = op.getItems ();
		if (items.empty ())
			return {};

		for (const auto& item : items)
			if (m_db->getSeqNum (item.getId ()) > item.getSeq ())
				throw DBOpError (DBOpError::ErrorCode::SeqOutdated,
						"cannot update item: stored item has newer sequence id; refetch and retry");

		DBResult res;
		for (const auto& item : items)
			res.updateCurSeq (m_db->modifyItem (item));

		return { res };
	}

	std::vector<DBResult> DBOperator::remove (const Operation& op)
	{
	}
}
