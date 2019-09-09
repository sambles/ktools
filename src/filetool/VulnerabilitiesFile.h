#ifndef KTOOLS_FILETOOL_VULNERABILITY_H
#define KTOOLS_FILETOOL_VULNERABILITY_H

#include <sstream>
#include "BaseFileReader.h"
#include "../include/oasis.h"

namespace ktools { namespace filetool {

class VulnerabilitiesFile : public BaseFileReader<Vulnerability>
{
public:
    VulnerabilitiesFile(const std::string& prefix)
     : BaseFileReader("vulnerabilities", prefix, VULNERABILITY_FILE)
    {
        _stream.read(reinterpret_cast<char *>(&_num_damage_bins), sizeof(_num_damage_bins));
    }

    virtual ~VulnerabilitiesFile() {}

    inline int num_damage_bins() const { return _num_damage_bins; }
private:
    int _num_damage_bins;
};

template<>
struct CsvFormatter<Vulnerability> {
    std::string header() {
        return  "Vulnerability Id,Intensity Bin Id,Damage Bin Id,Probablility";
    }

    std::string row(const Vulnerability& rec) {
        std::stringstream ss;
        ss << rec.vulnerability_id << ',' << rec.intensity_bin_id << ',' << rec.damage_bin_id << ',' << rec.probability;

        return ss.str();
    }
};

}}

#endif
