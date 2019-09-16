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

typedef BinFileReader<gulsummaryxref> GulSummaryXrefBinFileReader;
typedef CsvFileReader<gulsummaryxref> GulSummaryXrefCsvFileReader;

template<>
struct FileReaderSpecialization<gulsummaryxref> {
	static std::string object_name() { return "gul summary xref"; }
	static std::string bin_filename() { return GULSUMMARYXREF_FILE; }
	static std::string csv_header() { return "coverage_id,summary_id,summaryset_id"; }

	static BaseFileReader<gulsummaryxref>* csv_reader(const std::string& path) {
		return new GulSummaryXrefCsvFileReader(path);
	}

	static BaseFileReader<gulsummaryxref>* bin_reader(const std::string& prefix) {
		return new GulSummaryXrefBinFileReader(prefix);
	}

	static void to_csv(const gulsummaryxref& rec, std::stringstream& ss) {
		ss << rec.coverage_id << ',' << rec.summary_id << ',' << rec.summaryset_id;
	}

	static void from_csv(gulsummaryxref& s, const std::string& field_name, const std::string& field_value) {
		if (field_name == "coverage_id") {
			s.coverage_id = std::stoi(field_value);
		}
		else if (field_name == "summary_id") {
			s.summary_id = std::stoi(field_value);
		}
		else if (field_name == "summaryset_id") {
			s.summaryset_id = std::stoi(field_value);
		}
	}
};

}}
