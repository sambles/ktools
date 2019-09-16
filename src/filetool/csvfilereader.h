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

#include "BaseFileReader.h"

namespace ktools { namespace filetool {

template<typename T>
class CsvFileReader : public BaseFileReader<T> {
public:
	CsvFileReader(const std::string& path)
		: BaseFileReader<T>(path) {
		// Read the first line as the header
		read_and_split_line(_header);
	}

	virtual bool read(T& rec) {
		std::vector<std::string> line;
		if (!read_and_split_line(line)) {
			return false;
		}

		for (size_t i = 0; i < _header.size(); i++) {
			const std::string& field_name = _header.at(i);
			const std::string& field_value = line.at(i);

			FileReaderSpecialization<T>::from_csv(rec, field_name, field_value);
		}

		return true;
	}

private:

	bool read_and_split_line(std::vector<std::string>& result) {
		std::string read_line;
		std::getline(BaseFileReader<T>::_stream, read_line);
		bool fail = !BaseFileReader<T>::_stream;

		// Exit if read operation failed
		if (fail) {
			return false;
		}

		split_line(read_line, result);
		return true;
	}

	void split_line(std::string& line, std::vector<std::string>& result) {
		// Split the line and put individual items in result
		result.clear();
		std::istringstream split(line);
		std::string s;
		while (getline(split, s, ',')) {
			result.push_back(s);
		}
	}

protected:
	std::vector<std::string> _header;
	std::ifstream _stream;
};

}}