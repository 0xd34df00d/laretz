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

#include "packetgenerator.h"
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

namespace Laretz
{
	PacketGenerator::PacketGenerator ()
	{
	}

	PacketGenerator::PacketGenerator (std::map<std::string, std::string>&& fields)
	: m_fields (fields)
	{
	}

	PacketGenerator& PacketGenerator::operator<< (const std::pair<std::string, std::string>& field)
	{
		m_fields.insert (field);
		return *this;
	}

	PacketGenerator& PacketGenerator::operator<< (const Operation& op)
	{
		m_operations.push_back (op);
		return *this;
	}

	std::string PacketGenerator::operator() () const
	{
		std::ostringstream ostr;
		for (const auto& field : m_fields)
			ostr << field.first << ": " << field.second << "\n";
		ostr << "\n";

		if (!m_operations.empty ())
		{
			boost::archive::text_oarchive oars (ostr);
			oars << m_operations;
		}

		return ostr.str ();
	}
}
