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
#include "ItemsFile.h"
#include "VulnerabilitiesFile.h"
#include "DamageBinDictionaryFile.h"
#include "CoveragesFile.h"
#include "FootprintFile.h"
#include "FootprintIndexFile.h"
#include "FmPolicyTCFile.h"
#include "EventFile.h"
#include "fmprofilefile.h"
#include "fmprogrammefile.h"
#include "fmsummaryxreffile.h"
#include "fmxreffile.h"

// ---------------------------------------------------------------------------
// Template functions

template<typename T>
void read_and_print_csv(ktools::filetool::BaseFileReader<T>& reader) {
	ktools::filetool::CsvFormatter<T> formatter;

	std::cout << formatter.header() << std::endl;
	T rec;
	while (reader.read(rec)) {
		std::cout << formatter.row(rec) << std::endl;
	}
	std::cout.flush();
}

template<typename T>
void read_and_print_bin(ktools::filetool::BaseFileReader<T>& reader) {
	T rec;
	while (reader.read(rec)) {
		std::cout.write(reinterpret_cast<const char*>(&rec), sizeof(rec));
	}
	std::cout.flush();
}

template<typename T>
void read_and_print(ktools::filetool::BaseFileReader<T>& reader, const ktools::filetool::FileTool::Format output) {
	switch (output) {
	case ktools::filetool::FileTool::FMT_CSV:
		read_and_print_csv(reader);
		break;
	case ktools::filetool::FileTool::FMT_BIN:
		read_and_print_bin(reader);
		break;
	default:
		std::cerr << "Unhandled output type specified" << std::endl;
	}
}

// ---------------------------------------------------------------------------
// FileTool static attribute initialization

const ktools::filetool::FileTool::FileTypeMap ktools::filetool::FileTool::_file_type_mapping = {
	{"events",          FT_EVENTS},
	{"eventids",        FT_EVENT_GEN},
	{"vulnerabilities", FT_VULNERABILITIES},
	{"damagebindict",   FT_DAMAGEBINDICT},
	{"coverages",       FT_COVERAGES},
	{"footprint",       FT_FOOTPRINT},
	{"footprintidx",    FT_FOOTPRINT_INDEX},
	{"fmpolicytc",      FT_FM_POLICY_TC},
	{"fmprofile",       FT_FM_PROFILE},
	{"fmprogramme",		FT_FM_PROGRAMME},
	{"fmsummaryxref",   FT_FM_SUMMARY_XREF},
	{"fmxref",			FT_FM_XREF}
};

const ktools::filetool::FileTool::FormatMap ktools::filetool::FileTool::_format_mapping = {
	{"csv", FMT_CSV},
	{"bin", FMT_BIN},
	{"json", FMT_JSON}
};

const ktools::filetool::FileTool::ExecutorMap ktools::filetool::FileTool::_executor_mapping = {
	{FT_EVENTS,
	 [](const std::string& prefix, const Format output) {
		ktools::filetool::ItemsFile file(prefix);
		read_and_print(file, output);
	}},
	{FT_EVENT_GEN,
	 [](const std::string& prefix, const Format output) {
		ktools::filetool::EventFile file(prefix);
		read_and_print(file, output);
	}},
	{FT_VULNERABILITIES,
	 [](const std::string& prefix, const Format output) {
		ktools::filetool::VulnerabilitiesFile file(prefix);
		read_and_print(file, output);
	}},
	{FT_DAMAGEBINDICT,
	 [](const std::string& prefix, const Format output) {
		ktools::filetool::DamageBinDictionaryFile file(prefix);
		read_and_print(file, output);
	}},
	{FT_COVERAGES,
	 [](const std::string& prefix, const Format output) {
		ktools::filetool::CoveragesFile file(prefix);
		read_and_print(file, output);
	}},
	{FT_FOOTPRINT_INDEX,
	 [](const std::string& prefix, const Format output) {
		ktools::filetool::FootprintIndexFile file(prefix, false);
		read_and_print(file, output);
	}},
	{FT_FOOTPRINT,
	 [](const std::string& prefix, const Format output) {
		ktools::filetool::FootprintIndexFile index_file(prefix, false);
		ktools::filetool::FootprintFile file(prefix, false);

		EventIndex index;
		while (index_file.read(index)) {
			if (index.size == 0) {
				continue;
			}
			file.init(index);
			read_and_print(file, output);
		}
	}},
	{FT_FM_POLICY_TC,
	 [](const std::string& prefix, const Format output) {
		ktools::filetool::FMPolicyTCFile file(prefix);
		read_and_print(file, output);
	}},
	{FT_FM_PROFILE,
	 [](const std::string& prefix, const Format output) {
		ktools::filetool::FMProfileFile file(prefix);
		read_and_print(file, output);
	}},
	{FT_FM_PROGRAMME,
	 [](const std::string& prefix, const Format output) {
		ktools::filetool::FMProgrammeFile file(prefix);
		read_and_print(file, output);
	}},
	{FT_FM_SUMMARY_XREF,
	 [](const std::string& prefix, const Format output) {
		ktools::filetool::FMSummaryXrefFile file(prefix);
		read_and_print(file, output);
	}},
	{FT_FM_XREF,
	 [](const std::string& prefix, const Format output) {
		ktools::filetool::FMXrefFile file(prefix);
		read_and_print(file, output);
	}} };

// ---------------------------------------------------------------------------
// FileTool method implementations

ktools::filetool::FileTool::FileTool(const std::string&prefix, 
								     const ktools::filetool::FileTool::Format input_format, 
									 const ktools::filetool::FileTool::Format output_format, 
									 const ktools::filetool::FileTool::FileType file_type)
	: _prefix(prefix), _input_format(input_format), _output_format(output_format), _file_type(file_type)
{
}

ktools::filetool::FileTool::~FileTool()
{

}

void ktools::filetool::FileTool::run()
{
	auto itr = _executor_mapping.find(_file_type);
	if (itr != _executor_mapping.end()) {
		itr->second(_prefix, _output_format);
	}
}

std::string ktools::filetool::FileTool::get_supported_filetypes()
{
	std::stringstream ss;
	ss << '[';
	for (auto const& kv : _file_type_mapping) {
		ss << kv.first << ",";
	}
	ss << ']';

	return ss.str();
}

ktools::filetool::FileTool::Format ktools::filetool::FileTool::convert_format(const std::string& format_str)
{
	auto itr = _format_mapping.find(format_str);
	if (itr == _format_mapping.end()) {
		return FMT_UNKNOWN;
	}

	return itr->second;
}

ktools::filetool::FileTool::FileType ktools::filetool::FileTool::convert_filetype(const std::string& filetype_str)
{
	auto itr = _file_type_mapping.find(filetype_str);
	if (itr == _file_type_mapping.end()) {
		return FT_UNKOWN;
	}

	return itr->second;
}
