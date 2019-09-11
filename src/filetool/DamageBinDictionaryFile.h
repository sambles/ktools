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

class DamageBinDictionaryFile : public BaseFileReader<damagebindictionary>
{
public:
    DamageBinDictionaryFile(const std::string& prefix)
     : BaseFileReader("damage bin dictionary", prefix, DAMAGE_BIN_DICT_FILE)
    {}

    virtual ~DamageBinDictionaryFile() {}
};

template<>
struct CsvFormatter<damagebindictionary> {
    std::string header() {
        return  "Index,From,To,Interpolation,Interval Type";
    }

    std::string row(const damagebindictionary& rec) {
        std::stringstream ss;
        ss << rec.bin_index << ',' << rec.bin_from << ',' << rec.bin_to << ',' << rec.interpolation << ',' << rec.interval_type;

        return ss.str();
    }
};

template<>
struct CsvStructInitializer<damagebindictionary> {
	void initilize(damagebindictionary& s, const std::string& field_name, const std::string& field_value) {
		if (field_name== "index") {
			s.bin_index = std::stoi(field_value);
		}
		else if (field_name == "from") {
			s.bin_from = std::stof(field_value);
		}
		else if (field_name == "to") {
			s.bin_to = std::stof(field_value);
		}
		else if (field_name == "interpolation") {
			s.interpolation = std::stof(field_value);
		}
		else if (field_name == "interval type") {
			s.interval_type = std::stoi(field_value);
		}
	}
};

}}
