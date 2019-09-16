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

typedef BinFileReader<fm_programme> FMProgrammeBinFileReader;
typedef CsvFileReader<fm_programme> FMProgrammeCsvFileReader;

template<>
struct FileReaderSpecialization<fm_programme> {
	static std::string object_name() { return "fm programme"; }
	static std::string bin_filename() { return FMPROGRAMME_FILE; }
	static std::string csv_header() { return "from_agg_id,level_id,to_agg_id"; }

	static BaseFileReader<fm_programme>* csv_reader(const std::string& path) {
		return new FMProgrammeCsvFileReader(path);
	}

	static BaseFileReader<fm_programme>* bin_reader(const std::string& prefix) {
		return new FMProgrammeCsvFileReader(prefix);
	}

	static void to_csv(const fm_programme& rec, std::stringstream& ss) {
		ss << rec.from_agg_id << ',' << rec.level_id << ',' << rec.to_agg_id;
	}

	static void from_csv(fm_programme& s, const std::string& field_name, const std::string& field_value) {
		if (field_name == "from_agg_id") {
			s.from_agg_id = std::stoi(field_value);
		}
		else if (field_name == "level_id") {
			s.level_id = std::stoi(field_value);
		}
		else if (field_name == "to_agg_id") {
			s.to_agg_id = std::stoi(field_value);
		}
	}
};

}}
