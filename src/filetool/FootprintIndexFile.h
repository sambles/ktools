#ifndef KTOOLS_FILETOOL_FOOTPRINT_INDEX_FILE_H_
#define KTOOLS_FILETOOL_FOOTPRINT_INDEX_FILE_H_

#include <sstream>
#include "BaseFileReader.h"
#include "../include/oasis.h"

namespace ktools { namespace filetool {

class FootprintIndexFile : public BaseFileReader<EventIndex>
{
public:
    FootprintIndexFile(const std::string& prefix, bool zip)
     : BaseFileReader("footprint index", prefix, (zip ? ZFOOTPRINT_IDX_FILE : FOOTPRINT_IDX_FILE))
    {}

    virtual ~FootprintIndexFile() {}
};

template<>
struct CsvFormatter<EventIndex> {
    std::string header() {
        return  "Event Id,Offset,Size";
    }

    std::string row(const EventIndex& rec) {
        std::stringstream ss;
        ss << rec.event_id << ',' << rec.offset << ',' << rec.size;

        return ss.str();
    }
};

}}

#endif
