#ifndef DAMAGE_BIN_DICTIONARY_FILE_H_
#define DAMAGE_BIN_DICTIONARY_FILE_H_

#include <sstream>
#include "BaseFileReader.h"
#include "../include/oasis.h"

namespace ktools { namespace filetool {

class DamageBinDictionaryFile : public BaseFileReader<damagebindictionary>
{
public:
    DamageBinDictionaryFile(const std::string& prefix)
     : BaseFileReader("damage bin dictionary", prefix, DAMAGE_BIN_DICT_FILE)
    {}

    virtual ~DamageBinDictionaryFile() {}
};

template<>
struct CsvFormatter<damagebindictionary> {
    std::string header() {
        return  "Index,From,To,Interpolation,Interval Type";
    }

    std::string row(const damagebindictionary& rec) {
        std::stringstream ss;
        ss << rec.bin_index << ',' << rec.bin_from << ',' << rec.bin_to << ',' << rec.interpolation << ',' << rec.interval_type;

        return ss.str();
    }
};

template<>
struct CsvStructInitializer<damagebindictionary> {
	void initilize(damagebindictionary& s, const std::string& field_name, const std::string& field_value) {
		if (field_name== "index") {
			s.bin_index = std::stoi(field_value);
		}
		else if (field_name == "from") {
			s.bin_from = std::stof(field_value);
		}
		else if (field_name == "to") {
			s.bin_to = std::stof(field_value);
		}
		else if (field_name == "interpolation") {
			s.interpolation = std::stof(field_value);
		}
		else if (field_name == "interval type") {
			s.interval_type = std::stoi(field_value);
		}
	}
};

}}

#endif
