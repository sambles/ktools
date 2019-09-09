#ifndef KTOOLS_FILETOOL_FOOTPRINT_FILE_H_
#define KTOOLS_FILETOOL_FOOTPRINT_FILE_H_

#include <sstream>
#include "BaseFileReader.h"
#include "../include/oasis.h"

namespace ktools { namespace filetool {

class FootprintFile : public BaseFileReader<EventRow>
{
public:
    FootprintFile(const std::string& prefix, bool zip)
     : BaseFileReader("footprint", prefix, (zip ? ZFOOTPRINT_FILE : FOOTPRINT_FILE)), _zip(zip)
    {
        _stream.read(reinterpret_cast<char*>(&_num_intensity_bins), sizeof(_num_intensity_bins));
        _stream.read(reinterpret_cast<char*>(&_has_intensity_uncertainty), sizeof(_has_intensity_uncertainty));
    }

    virtual ~FootprintFile() {}

    int num_intensity_bins() const { return _num_intensity_bins; }
    int has_intensity_uncertainty() const { return _has_intensity_uncertainty; }

    void init(const EventIndex& event_index) {
        if (_zip) {
            // TODO Read the the whole buffer and uncompress
        } else {
            _stream.seekg(event_index.offset);
            if (_stream.bad()) {
                LOG_ERROR << "Error setting offset: " << strerror(errno);
            }
        }

        _current_record = 0;
        _num_records = event_index.size / sizeof(EventRow);
    }

    // Overrides from BaseFileReader
    
    virtual uint32_t num_records() const { return _num_records; }

    virtual bool read(EventRow& rec) {
        if (_current_record >= _num_records) {
            return false;
        }

        bool more_to_read = true;
        if (_zip) {
            // TODO Read from memory buffer
        } else {
            more_to_read = BaseFileReader::read(rec);
        }

        if (more_to_read) {
            _current_record++;
        }

        return more_to_read;
    }

private:
    bool _zip;
    int _num_intensity_bins;
    int _has_intensity_uncertainty;
    uint32_t _num_records;
    uint32_t _current_record;
};

template<>
struct CsvFormatter<EventRow> {
    std::string header() {
        return  "event_id,areaperil_id,intensity_bin_id,probability";
    }

    std::string row(const EventRow& rec) {
        std::stringstream ss;
        ss << rec.areaperil_id << ',' << rec.intensity_bin_id << ',' << rec.probability;

        return ss.str();
    }
};

}}

#endif
