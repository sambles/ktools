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

#include <fstream>
#include <string>
#include <list>
#include <functional>
#include <vector>
#include <cstring>
#include <errno.h>

#include "log.h"

namespace ktools { namespace filetool {

	template<typename T>
	struct FileReaderSpecialization;

template<typename T>
class BaseFileReader
{
protected:
    BaseFileReader(const std::string& path) {
        _stream.open(path, std::ios::in | std::ios::binary);
        if (!_stream) {
            LOG_ERROR << "Can not open " << path << ": " << std::strerror(errno);
            exit(EXIT_FAILURE);
        }

		LOG_INFO << "Reading " << FileReaderSpecialization<T>::object_name() << " from " << path;
    }

public:

    virtual ~BaseFileReader() {
        _stream.close();
    }

public:
	virtual bool read(T& rec) = 0;

protected:
    std::ifstream _stream;
};

template<typename T>
struct FileReaderSpecialization {
	static std::string object_name() { return ""; }
	static std::string bin_filename() { return ""; }
	static std::string csv_header() { return ""; }

	static BaseFileReader<T>* csv_reader(const std::string& ) { return nullptr; }
	static BaseFileReader<T>* bin_reader(const std::string& ) { return nullptr;  }

	static void to_csv(const T& , std::stringstream& ) {}
	static void from_csv(T& s, const std::string& field_name, const std::string& field_value) {}
};

}}
