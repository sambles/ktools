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

namespace ktools {
	namespace filetool {
		struct fm_policyTC {
			int level_id;
			int agg_id;
			int layer_id;
			int policytc_id;
		};

		class FMPolicyTCFile : public BaseFileReader<fm_policyTC>
		{
		public:
			FMPolicyTCFile(const std::string& prefix)
				: BaseFileReader("fm policy tc", prefix, FMPOLICYTC_FILE)
			{}

			virtual ~FMPolicyTCFile() {}
		};

		template<>
		struct CsvFormatter<fm_policyTC> {
			std::string header() {
				return  "layer_id,level_id,agg_id,policytc_id";
			}

			std::string row(const fm_policyTC& rec) {
				std::stringstream ss;
				ss << rec.layer_id << ',' << rec.level_id << ',' << rec.agg_id << ',' << rec.policytc_id;

				return ss.str();
			}
		};

		template<>
		struct CsvStructInitializer<fm_policyTC> {
			void initilize(fm_policyTC& s, const std::string& field_name, const std::string& field_value) {
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

	}
}
