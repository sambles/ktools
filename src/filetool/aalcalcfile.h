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

#include "csvfilereader.h"
#include "binfilereader.h"
#include "../include/oasis.h"

namespace ktools { namespace filetool {

typedef CsvFileReader<coveragedata> AALCalcCsvFileReader;

class AALCalcBinFileReader : public BinFileReader<aal_rec>
{
public:
	AALCalcBinFileReader(const std::string& prefix)
		: BinFileReader(prefix)	{
		_stream.read(reinterpret_cast<char*>(&_sample_size), sizeof(_sample_size));
	}

	virtual ~AALCalcBinFileReader() {}

	int sample_size() const { return _sample_size; }

protected:

private:
	int _sample_size;
};

template<>
struct FileReaderSpecialization<aal_rec> {
	static std::string object_name() { return "aalcalc"; };
	static std::string bin_filename() { return "aalcalc.bin"; }
	static std::string csv_header() { return "summary_id,type,mean,mean_squared,max_exposure_value"; }

	static BaseFileReader<aal_rec>* csv_reader(const std::string& path) {
		return new AALCalcCsvFileReader(path);
	}

	static BaseFileReader<aal_rec>* bin_reader(const std::string& prefix) {
		return new AALCalcBinFileReader(prefix);
	}

	static void to_csv(const aal_rec& rec, std::stringstream& ss) {
		ss << rec.summary_id << "," << rec.type << ',' << rec.mean << ',' << rec.mean_squared << ',' << rec.max_exposure_value;
	}

	static void from_csv(aal_rec& s, const std::string& field_name, const std::string& field_value) {
		if (field_name == "summary_id") {
			s.summary_id = std::stoi(field_value);
		}
		else if (field_name == "type") {
			s.type = std::stoi(field_value);
		}
		else if (field_name == "mean") {
			s.mean = std::stod(field_value);
		}
		else if (field_name == "mean_squared") {
			s.mean_squared = std::stod(field_value);
		}
		else if (field_name == "max_exposure_value") {
			s.max_exposure_value = std::stod(field_value);
		}
	}
};

}}
