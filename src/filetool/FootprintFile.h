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

typedef CsvFileReader<EventRow> FootprintCsvFileReader;

class FootprintBinFileReader : public BinFileReader<EventRow> {
public:
    FootprintBinFileReader(const std::string& prefix)
        : BinFileReader(prefix), _num_records(0), _current_record(0) {
        _stream.read(reinterpret_cast<char*>(&_num_intensity_bins),
                     sizeof(_num_intensity_bins));
        _stream.read(reinterpret_cast<char*>(&_has_intensity_uncertainty),
                     sizeof(_has_intensity_uncertainty));
    }

    virtual ~FootprintBinFileReader() {}

    int num_intensity_bins() const { return _num_intensity_bins; }
    int has_intensity_uncertainty() const { return _has_intensity_uncertainty; }

    void init(const EventIndex& event_index) {
        BaseFileReader<EventRow>::_stream.seekg(event_index.offset);
        if (_stream.bad()) {
            LOG_ERROR << "Error setting offset: " << strerror(errno);
        }

        _current_record = 0;
        _num_records = event_index.size / sizeof(EventRow);
    }

    // Overrides from BaseFileReader

    virtual size_t num_records() const { return _num_records; }

    virtual bool read(EventRow& rec) {
        if (_current_record >= _num_records) {
            return false;
        }

        bool more_to_read = BinFileReader<EventRow>::read(rec);

        if (more_to_read) {
            _current_record++;
        }

        return more_to_read;
    }

private:
    int _num_intensity_bins;
    int _has_intensity_uncertainty;
    size_t _num_records;
    size_t _current_record;
};

template <>
struct FileReaderSpecialization<EventRow> {
    static std::string object_name() { return "footprint index"; }
    static std::string bin_filename() { return FOOTPRINT_FILE; }
    static std::string csv_header() {
        return "areaperil_id,intensity_bin_id,probability";
    }

    static BaseFileReader<EventRow>* csv_reader(const std::string& path) {
        return new FootprintCsvFileReader(path);
    }

    static BaseFileReader<EventRow>* bin_reader(const std::string& prefix) {
        return new FootprintBinFileReader(prefix);
    }

    static void to_csv(const EventRow& rec, std::stringstream& ss) {
        ss << rec.areaperil_id << ',' << rec.intensity_bin_id << ','
           << rec.probability;
    }

    static void from_csv(EventRow& s, const std::string& field_name,
                         const std::string& field_value) {
        if (field_name == "areaperil_id") {
            s.areaperil_id = std::stoi(field_value);
        } else if (field_name == "intensity_bin_id") {
            s.intensity_bin_id = std::stoi(field_value);
        } else if (field_name == "probability") {
            s.probability = std::stof(field_value);
        }
    }
};

}  // namespace filetool
}  // namespace ktools
