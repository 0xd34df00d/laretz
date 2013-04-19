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

#include "item.h"
#include <mongo/client/dbclient.h>

namespace Laretz
{
	ShortItem::ShortItem ()
	{
	}

	ShortItem::ShortItem (std::string&& id)
	: m_id (id)
	{
	}

	ShortItem::ShortItem (const std::string& id)
	: m_id (id)
	{
	}

	Item::Item ()
	: m_seq (0)
	{
	}

	Item::Item (const std::string& id, const std::string& parentId, uint64_t seq)
	: m_id (id)
	, m_parentId (parentId)
	, m_seq (seq)
	{
	}

	std::string Item::getId () const
	{
		return m_id;
	}

	void Item::setId (const std::string& id)
	{
		m_id = id;
	}

	std::string Item::getParentId () const
	{
		return m_parentId;
	}

	void Item::setParentId (const std::string& parentId)
	{
		m_parentId = parentId;
	}

	uint64_t Item::getSeq () const
	{
		return m_seq;
	}

	void Item::setSeq (uint64_t seq)
	{
		m_seq = seq;
	}

	Field_t Item::operator[] (const std::string& name) const
	{
		const auto pos = m_fields.find (name);
		return pos == m_fields.end () ? Field_t () : pos->second;
	}

	Field_t& Item::operator[] (const std::string& name)
	{
		return m_fields [name];
	}

	void Item::setField (const std::string& name, const mongo::BSONElement& elem)
	{
		Field_t field;
		switch (elem.type ())
		{
		case mongo::BSONType::NumberDouble:
			field = elem.Double ();
			break;
		case mongo::BSONType::NumberInt:
			field = static_cast<int32_t> (elem.Int ());
			break;
		case mongo::BSONType::String:
			field = elem.String ();
			break;
		case mongo::BSONType::BinData:
		{
			int length = 0;
			const char *data = elem.binData (length);
			std::vector<char> vec;
			vec.reserve (length);
			std::copy (data, data + length, std::back_inserter (vec));
			field = vec;
		}
		default:
		{
			const auto& numStr = boost::lexical_cast<std::string> (elem.type ());
			throw std::runtime_error ("unknown field data type" + numStr);
		}
		}
		m_fields [name] = field;
	}
}
