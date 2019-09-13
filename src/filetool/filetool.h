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

#include <map>
#include <string>
#include <functional>

namespace ktools { namespace filetool {

class FileTool
{
public:

	enum Format
	{
		FMT_UNKNOWN,
		FMT_CSV,
		FMT_BIN,
		FMT_JSON
	};

	enum FileType
	{
		FT_UNKOWN,
		FT_EVENT_GEN,
		FT_EVENTS,
		FT_VULNERABILITIES,
		FT_DAMAGEBINDICT,
		FT_COVERAGES,
		FT_FOOTPRINT,
		FT_FOOTPRINT_INDEX,
		FT_FM_POLICY_TC,
		FT_FM_PROFILE
	};

private:
	typedef std::function<void(const std::string&, Format)> ConvertFunction;
	typedef std::map<std::string, FileType> FileTypeMap;
	typedef std::map<std::string, Format> FormatMap;
	typedef std::map<FileType, ConvertFunction> ExecutorMap;

public:
	FileTool(const std::string& prefix, const Format input_format, const Format output_format, const FileType file_type);
	virtual ~FileTool();

	void run();

	static std::string get_supported_filetypes();
	static Format convert_format(const std::string& format_str);
	static FileType convert_filetype(const std::string& filetype_str);
private:
	static const FileTypeMap _file_type_mapping;
	static const FormatMap _format_mapping;
	static const ExecutorMap _executor_mapping;

	const std::string _prefix;
	const Format _input_format;
	const Format _output_format;
	const FileType _file_type;
};

}}