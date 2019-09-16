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
#include "binfilereader.h"
#include "csvfilereader.h"
#include "../include/oasis.h"

namespace ktools { namespace filetool {

typedef BinFileReader<Periods> PeriodsBinFileReader;
typedef CsvFileReader<Periods> PeriodsCsvFileReader;

template<>
struct FileReaderSpecialization<Periods> {
	static std::string object_name() { return "periods"; }
	static std::string bin_filename() { return PERIODS_FILE; }
	static std::string csv_header() { return "period_no,weighting"; }

	static BaseFileReader<Periods>* csv_reader(const std::string& path) {
		return new PeriodsCsvFileReader(path);
	}

	static BaseFileReader<Periods>* bin_reader(const std::string& prefix) {
		return new PeriodsBinFileReader(prefix);
	}

	static void to_csv(const Periods& rec, std::stringstream& ss) {
		ss << rec.period_no << ',' << rec.weighting;
	}

	static void from_csv(Periods& s, const std::string& field_name, const std::string& field_value) {
		if (field_name == "period_no") {
			s.period_no = std::stoi(field_value);
		}
		else if (field_name == "weighting") {
			s.weighting = std::stod(field_value);
		}
	}
};

}}
