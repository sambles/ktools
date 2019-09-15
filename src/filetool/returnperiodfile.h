/*
* Copyright (c)2015 - 2019 Oasis LMF Limited
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*
*   * Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.
*
*   * Neither the original author of this software nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
* THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*/
#pragma once

#include <sstream>
#include "BaseFileReader.h"
#include "../include/oasis.h"

namespace ktools { namespace filetool {

struct return_period 
{
	int return_period;
};

class ReturnPeriodFile : public BaseFileReader<return_period>
{
public:
	ReturnPeriodFile(const std::string& prefix)
		: BaseFileReader("return periods file", prefix, RETURNPERIODS_FILE)
	{}

	virtual ~ReturnPeriodFile() {}
};

template<>
struct CsvFormatter<return_period> {
	std::string header() {

		return "return_period";
	}

	std::string row(const return_period& rec) {
		std::stringstream ss;
		ss << rec.return_period;

		return ss.str();
	}
};

template<>
struct CsvStructInitializer<return_period> {
	void initilize(return_period& s, const std::string& field_name, const std::string& field_value) {
		if (field_name == "return_period") {
			s.return_period = std::stoi(field_value);
		}
	}
};

}}
