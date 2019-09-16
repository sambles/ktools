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

namespace ktools { namespace filetool {
struct fm_policyTC {
	int level_id;
	int agg_id;
	int layer_id;
	int policytc_id;
};

typedef BinFileReader<fm_policyTC> FMPolicyTCBinFileReader;
typedef CsvFileReader<fm_policyTC> FMPolicyTCCsvFileReader;

template<>
struct FileReaderSpecialization<fm_policyTC> {
	static std::string object_name() { return "fm policy tc"; }
	static std::string bin_filename() { return FMPOLICYTC_FILE; }
	static std::string csv_header() { return "layer_id,level_id,agg_id,policytc_id"; }

	static BaseFileReader<fm_policyTC>* csv_reader(const std::string& path) {
		return new FMPolicyTCCsvFileReader(path);
	}

	static BaseFileReader<fm_policyTC>* bin_reader(const std::string& prefix) {
		return new FMPolicyTCBinFileReader(prefix);
	}

	static void to_csv(const fm_policyTC& rec, std::stringstream& ss) {
		ss << rec.layer_id << ',' << rec.level_id << ',' << rec.agg_id << ',' << rec.policytc_id;
	}

	static void from_csv(fm_policyTC& s, const std::string& field_name, const std::string& field_value) {
		if (field_name == "layer_id") {
			s.layer_id = std::stoi(field_value);
		}
		else if (field_name == "level_id") {
			s.level_id = std::stoi(field_value);
		}
		else if (field_name == "agg_id") {
			s.agg_id = std::stoi(field_value);
		}
		else if (field_name == "policytc_id") {
			s.policytc_id = std::stoi(field_value);
		}
	}
};
}}
