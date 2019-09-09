#ifndef ITEMS_FILE_H_
#define ITEMS_FILE_H_

#include <sstream>
#include "BaseFileReader.h"
#include "../include/oasis.h"

namespace ktools { namespace filetool {

class ItemsFile : public BaseFileReader<item>
{
public:
    ItemsFile(const std::string& prefix)
     : BaseFileReader("vulnerability items", prefix, ITEMS_FILE)
    {}

    virtual ~ItemsFile() {}
};

template<>
struct CsvFormatter<item> {
    std::string header() {
        return  "Id,Coverage Id, Area Peril Id, Vulnerability Id, Group Id";
    }

    std::string row(const item& rec) {
        std::stringstream ss;
        ss << rec.id << ',' << rec.coverage_id << ',' << rec.areaperil_id << ',' << rec.vulnerability_id << ',' << rec.group_id;

        return ss.str();
    }
};

}}

#endif
