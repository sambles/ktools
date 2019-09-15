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
#include <map>
#include <string.h>
#include <errno.h>

#include "log.h"

namespace ktools { namespace filetool {

/**
 * Data type based choice for csv file formatting
*/
template<typename T>
struct CsvFormatter {
	std::string header();
	std::string row(const T& row);
};

template<typename T>
struct CsvStructInitializer {
	void initilize(T& s, const std::string& field_name, const std::string& field_value);
};

template<typename T>
class BaseFileReader
{
protected:
    BaseFileReader(const std::string& object_name, const std::string& prefix, const std::string& object_file) {
        std::string filename = prefixed_path(prefix, object_file);
        _stream.open(filename, std::ios::in | std::ios::binary);
        if (!_stream) {
            LOG_ERROR << "Can not open " << filename << ": " << strerror(errno);
            exit(EXIT_FAILURE);
        }

    	LOG_INFO << "Reading " << object_name << " from " << filename;
    }

    virtual ~BaseFileReader() {
        _stream.close();
    }

public:
    uint32_t num_records() {
        _stream.seekg(0, _stream.end);
        int sz = _stream.tellg();
        _stream.seekg(0, _stream.beg);

        return static_cast<uint32_t>(sz / record_size());
    }

    virtual bool read(T& rec) {
        _stream.read(reinterpret_cast<char *>(&rec), sizeof(T));

        bool fail = !_stream;

        return !fail;
    }

protected:

	virtual int record_size() const {
		return sizeof(T);
	}

private:

    std::string prefixed_path(const std::string& prefix, const std::string& filename) {
        if (!prefix.empty()) 
        {
            return prefix + "/" + filename;
        }

        return filename;
    }

protected:
    std::ifstream _stream;
};

}}
