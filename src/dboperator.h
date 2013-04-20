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

#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <stdexcept>
#include "operation.h"

namespace Laretz
{
	class DBResult;

	class DB;
	typedef std::shared_ptr<DB> DB_ptr;

	class DBOpError : public std::runtime_error
	{
	public:
		enum ErrorCode
		{
			SeqOutdated = 100
		};
	private:
		ErrorCode m_ec;
		std::string m_reason;
	public:
		DBOpError (ErrorCode, const std::string&);
		~DBOpError () throw ();

		ErrorCode getEC () const;
	};

	class DBOperator
	{
		DB_ptr m_db;

		const std::map<OpType, std::function<std::vector<DBResult> (Operation)>> m_op2func;
	public:
		DBOperator (DB_ptr);

		std::vector<DBResult> operator() (const std::vector<Operation>&);
	private:
		std::vector<DBResult> apply (const Operation&);

		std::vector<DBResult> list (const Operation&);
		std::vector<DBResult> fetch (const Operation&);
		std::vector<DBResult> append (const Operation&);
		std::vector<DBResult> update (const Operation&);
	};
}
