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
#include "../include/oasis.h"
#include "binfilereader.h"
#include "csvfilereader.h"

namespace ktools {
namespace filetool {

class VulnerabilityBinFileReader : public BinFileReader<Vulnerability> {
public:
    VulnerabilityBinFileReader(const std::string& prefix)
        : BinFileReader(prefix) {
        _stream.read(reinterpret_cast<char*>(&_num_damage_bins),
                     sizeof(_num_damage_bins));
    }

    virtual ~VulnerabilityBinFileReader() {}

    inline int num_damage_bins() const { return _num_damage_bins; }

private:
    int _num_damage_bins;
};

typedef CsvFileReader<Vulnerability> VulnerabilityCsvFileReader;

template <>
struct FileReaderSpecialization<Vulnerability> {
    static std::string object_name() { return "vulnerabilities"; }
    static std::string bin_filename() { return VULNERABILITY_FILE; }
    static std::string csv_header() {
        return "vulnerability_id,intensity_bin_id,damage_bin_id,probablility";
    }

    static BaseFileReader<Vulnerability>* csv_reader(const std::string& path) {
        return new VulnerabilityCsvFileReader(path);
    }

    static BaseFileReader<Vulnerability>* bin_reader(
        const std::string& prefix) {
        return new VulnerabilityBinFileReader(prefix);
    }

    static void to_csv(const Vulnerability& rec, std::stringstream& ss) {
        ss << rec.vulnerability_id << ',' << rec.intensity_bin_id << ','
           << rec.damage_bin_id << ',' << rec.probability;
    }

    static void from_csv(Vulnerability& s, const std::string& field_name,
                         const std::string& field_value) {
        if (field_name == "vulnerability_id") {
            s.vulnerability_id = std::stoi(field_value);
        } else if (field_name == "intensity_bin_id") {
            s.intensity_bin_id = std::stoi(field_value);
        } else if (field_name == "damage_bin_id") {
            s.damage_bin_id = std::stoi(field_value);
        } else if (field_name == "probability") {
            s.probability = std::stof(field_value);
        }
    }
};

}  // namespace filetool
}  // namespace ktools
