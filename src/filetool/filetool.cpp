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

#include "filetool.h"
#include <iostream>
#include "CoveragesFile.h"
#include "DamageBinDictionaryFile.h"
#include "EventFile.h"
#include "FmPolicyTCFile.h"
#include "FootprintFile.h"
#include "FootprintIndexFile.h"
#include "ItemsFile.h"
#include "VulnerabilitiesFile.h"
#include "fmprofilefile.h"
#include "fmprogrammefile.h"
#include "fmsummaryxreffile.h"
#include "fmxreffile.h"
#include "gulsummaryxreffile.h"
#include "periodsfile.h"
#include "returnperiodfile.h"

// ---------------------------------------------------------------------------
// Template functions

template <typename T>
void print_data(ktools::filetool::BaseFileReader<T>& reader,
                const ktools::filetool::FileTool::Format output) {
    std::stringstream ss;

    // For CSV output, print the header
    if (output == ktools::filetool::FileTool::FMT_CSV) {
        std::cout << ktools::filetool::FileReaderSpecialization<T>::csv_header()
                  << std::endl;
    }

    T rec;
    while (reader.read(rec)) {
        switch (output) {
            case ktools::filetool::FileTool::FMT_BIN:
                std::cout.write(reinterpret_cast<const char*>(&rec),
                                sizeof(rec));
                break;
            case ktools::filetool::FileTool::FMT_CSV:
                ss.clear();
                ktools::filetool::FileReaderSpecialization<T>::to_csv(rec, ss);
                std::cout << ss.str() << std::endl;
                break;
            case ktools::filetool::FileTool::FMT_UNKNOWN:
            case ktools::filetool::FileTool::FMT_JSON:
                std::cerr << "Unhandled output format" << std::endl;
                break;
        }
    }
}

template <typename T>
ktools::filetool::BaseFileReader<T>* create_reader(
    const std::string& prefix, const ktools::filetool::FileTool::Format input) {
    switch (input) {
        case ktools::filetool::FileTool::FMT_BIN:
            return ktools::filetool::FileReaderSpecialization<T>::bin_reader(prefix);
            break;
        case ktools::filetool::FileTool::FMT_CSV:
            return ktools::filetool::FileReaderSpecialization<T>::csv_reader(prefix);
            break;
        case ktools::filetool::FileTool::FMT_UNKNOWN:
        case ktools::filetool::FileTool::FMT_JSON:
        default:
            return nullptr;
    }
}

template <typename T>
void read_and_print(const std::string& prefix,
                    const ktools::filetool::FileTool::Format input,
                    const ktools::filetool::FileTool::Format output) {
    ktools::filetool::BaseFileReader<T>* reader =
        create_reader<T>(prefix, input);

    print_data<T>(*reader, output);

    delete reader;
}

// ---------------------------------------------------------------------------
// FileTool static attribute initialization

const ktools::filetool::FileTool::FileTypeMap
    ktools::filetool::FileTool::_file_type_mapping = {
        {"events", FT_EVENTS},
        {"eventids", FT_EVENT_GEN},
        {"vulnerabilities", FT_VULNERABILITIES},
        {"damagebindict", FT_DAMAGEBINDICT},
        {"coverages", FT_COVERAGES},
        {"footprint", FT_FOOTPRINT},
        {"footprintidx", FT_FOOTPRINT_INDEX},
        {"fmpolicytc", FT_FM_POLICY_TC},
        {"fmprofile", FT_FM_PROFILE},
        {"fmprogramme", FT_FM_PROGRAMME},
        {"fmsummaryxref", FT_FM_SUMMARY_XREF},
        {"fmxref", FT_FM_XREF},
        {"gulsummaryxref", FT_GUL_SUMMARY_XREF},
        {"periods", FT_PERIODS},
        {"returnperiod", FT_RETURN_PERIOD},
        {"aacalc", FT_AALCALC}};

const ktools::filetool::FileTool::FormatMap
    ktools::filetool::FileTool::_format_mapping = {
        {"csv", FMT_CSV}, {"bin", FMT_BIN}, {"json", FMT_JSON}};

const ktools::filetool::FileTool::ExecutorMap 
	ktools::filetool::FileTool::_executor_mapping = {
		{FT_EVENTS,
		[](const std::string& prefix, const Format input, const Format output) {
			read_and_print<item>(prefix, input, output);
		}},
		{FT_EVENT_GEN,
		[](const std::string& prefix, const Format input, const Format output) {
			read_and_print<eventdata>(prefix, input, output);
		}},
		{FT_DAMAGEBINDICT,
		[](const std::string& prefix, const Format input, const Format output) {
			read_and_print<damagebindictionary>(prefix, input, output);
		}},
		{FT_COVERAGES,
		[](const std::string& prefix, const Format input, const Format output) {
			read_and_print<ktools::filetool::coveragedata>(prefix, input, output);
		}},
		{FT_FOOTPRINT_INDEX,
		[](const std::string& prefix, const Format input, const Format output) {
			read_and_print<EventIndex>(prefix, input, output);
		}},
		{FT_FOOTPRINT,
		[](const std::string& prefix, const Format input, const Format output) {
			if (input ==
				ktools::filetool::FileTool::FMT_CSV) {
				read_and_print<EventRow>(prefix, input,output);
			} else {
				ktools::filetool::FootprintIndexBinFileReader index_file(prefix);
				ktools::filetool::FootprintBinFileReader file(prefix);

				EventIndex index;
				while (index_file.read(index)) {
					if (index.size == 0) {
						continue;
					}
					file.init(index);
					print_data(file, output);
				}
			}
		}},
		{FT_FM_POLICY_TC,
		[](const std::string& prefix, const Format input, const Format output) {
			read_and_print<ktools::filetool::fm_policyTC>(prefix, input, output);
		}},
		{FT_FM_PROFILE,
		[](const std::string& prefix, const Format input, const Format output) {
			read_and_print<fm_profile_new>(prefix, input, output);
		}},
		{FT_FM_PROGRAMME,
		[](const std::string& prefix, const Format input, const Format output) {
			read_and_print<fm_programme>(prefix, input, output);
		}},
		{FT_FM_SUMMARY_XREF,
		[](const std::string& prefix, const Format input, const Format output) {
			read_and_print<fmsummaryxref>(prefix, input, output);
		}},
		{FT_FM_XREF,
		[](const std::string& prefix, const Format input, const Format output) {
			read_and_print<fmXref>(prefix, input, output);
		}},
		{FT_GUL_SUMMARY_XREF,
		[](const std::string& prefix, const Format input, const Format output) {
			read_and_print<gulsummaryxref>(prefix, input, output);
		}},
		{FT_PERIODS,
		[](const std::string& prefix, const Format input, const Format output) {
			read_and_print<Periods>(prefix, input, output);
		}},
		{FT_RETURN_PERIOD,
		[](const std::string& prefix, const Format input, const Format output) {
			read_and_print<ktools::filetool::return_period>(prefix, input, output);
		}},
		{FT_VULNERABILITIES,
		[](const std::string& prefix, const Format input, const Format output) {
			read_and_print<Vulnerability>(prefix, input, output);
		}},
		{FT_AALCALC,
		[](const std::string& prefix, const Format input, const Format output) {
			read_and_print<aal_rec>(prefix, input, output);
		}}
	};

// ---------------------------------------------------------------------------
// FileTool method implementations

ktools::filetool::FileTool::FileTool(
    const std::string& prefix,
    const ktools::filetool::FileTool::Format input_format,
    const ktools::filetool::FileTool::Format output_format,
    const ktools::filetool::FileTool::FileType file_type)
    : _prefix(prefix),
      _input_format(input_format),
      _output_format(output_format),
      _file_type(file_type) {}

ktools::filetool::FileTool::~FileTool() {}

void ktools::filetool::FileTool::run() {
    auto itr = _executor_mapping.find(_file_type);
    if (itr != _executor_mapping.end()) {
        itr->second(_prefix, _input_format, _output_format);
    }
}

std::string ktools::filetool::FileTool::get_supported_filetypes() {
    std::stringstream ss;
    ss << '[';
    for (auto const& kv : _file_type_mapping) {
        ss << kv.first << ",";
    }
    ss << ']';

    return ss.str();
}

ktools::filetool::FileTool::Format ktools::filetool::FileTool::convert_format(
    const std::string& format_str) {
    auto itr = _format_mapping.find(format_str);
    if (itr == _format_mapping.end()) {
        return FMT_UNKNOWN;
    }

    return itr->second;
}

ktools::filetool::FileTool::FileType
ktools::filetool::FileTool::convert_filetype(const std::string& filetype_str) {
    auto itr = _file_type_mapping.find(filetype_str);
    if (itr == _file_type_mapping.end()) {
        return FT_UNKOWN;
    }

    return itr->second;
}
