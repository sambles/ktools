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

#include "binfilereader.h"
#include "csvfilereader.h"
#include "../include/oasis.h"

namespace ktools {
namespace filetool {

struct coveragedata {
    int coverage_id;
    OASIS_FLOAT tiv;
};

typedef CsvFileReader<coveragedata> CoveragesCsvFileReader;

class CoveragesBinFileReader : public BinFileReader<coveragedata> {
public:
    CoveragesBinFileReader(const std::string& prefix) : BinFileReader(prefix) {}

    virtual ~CoveragesBinFileReader() {}

    virtual bool read(coveragedata& rec) {
        _stream.read(reinterpret_cast<char*>(&rec.tiv), sizeof(rec.tiv));
        rec.coverage_id = _coverage_id++;

        bool fail = !_stream;

        return !fail;
    }

protected:
    virtual int record_size() const {
        coveragedata rec;
        return sizeof(rec.tiv);
    }

private:
    int _coverage_id = 1;
};

template <>
struct FileReaderSpecialization<coveragedata> {
    static std::string object_name() { return "coverages"; };
    static std::string bin_filename() { return COVERAGES_FILE; }
    static std::string csv_header() { return "coverage_id,tiv"; }

    static BaseFileReader<coveragedata>* csv_reader(const std::string& path) {
        return new CoveragesCsvFileReader(path);
    }

    static BaseFileReader<coveragedata>* bin_reader(const std::string& prefix) {
        return new CoveragesBinFileReader(prefix);
    }

    static void to_csv(const coveragedata& rec, std::stringstream& ss) {
        ss << rec.coverage_id << "," << rec.tiv;
    }

    static void from_csv(coveragedata& s, const std::string& field_name,
                         const std::string& field_value) {
        if (field_name == "coverage_id") {
            s.coverage_id = std::stoi(field_value);
        } else if (field_name == "tiv") {
            s.tiv = std::stof(field_value);
        }
    }
};

}  // namespace filetool
}  // namespace ktools
