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

typedef BinFileReader<fm_profile_new> FMProfileBinFileReader;
typedef CsvFileReader<fm_profile_new> FMProfileCsvFileReader;

template<>
struct FileReaderSpecialization<fm_profile_new> {
	static std::string object_name() { return "fm profile"; }
	static std::string bin_filename() { return FMPROFILE_FILE_NEW; }
	static std::string csv_header() {
		return "profile_id,calcrule_id,deductible1,deductible2,"
			"deductible3,attachment,limit,share1,share2,share3";
	}

	static BaseFileReader<fm_profile_new>* csv_reader(const std::string& path) {
		return new FMProfileCsvFileReader(path);
	}

	static BaseFileReader<fm_profile_new>* bin_reader(const std::string& prefix) {
		return new FMProfileBinFileReader(prefix);
	}

	static void to_csv(const fm_profile_new& rec, std::stringstream& ss) {
		ss << rec.profile_id << ',' << rec.calcrule_id << ',' << rec.deductible1 << ',' << rec.deductible2 << ',' <<
			rec.deductible3 << ',' << rec.attachment << ',' << rec.limit << ',' << rec.share1 << ',' << rec.share2 << ',' <<
			rec.share3;
	}

	static void from_csv(fm_profile_new& s, const std::string& field_name, const std::string& field_value) {
		if (field_name == "profile_id") {
			s.profile_id = std::stoi(field_value);
		}
		else if (field_name == "calcrule_id") {
			s.calcrule_id = std::stoi(field_value);
		}
		else if (field_name == "deductible1") {
			s.deductible1 = std::stof(field_value);
		}
		else if (field_name == "deductible2") {
			s.deductible2 = std::stof(field_value);
		}
		else if (field_name == "deductible3") {
			s.deductible3 = std::stof(field_value);
		}
		else if (field_name == "attachment") {
			s.attachment = std::stof(field_value);
		}
		else if (field_name == "limit") {
			s.limit = std::stof(field_value);
		}
		else if (field_name == "share1") {
			s.share1 = std::stof(field_value);
		}
		else if (field_name == "share2") {
			s.share2 = std::stof(field_value);
		}
		else if (field_name == "share3") {
			s.share3 = std::stof(field_value);
		}

	}
};

}}
