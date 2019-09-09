#ifndef BASE_FILE_READER_H_
#define BASE_FILE_READER_H_

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

        return static_cast<uint32_t>(sz / sizeof(T));
    }

    virtual bool read(T& rec) {
        _stream.read(reinterpret_cast<char *>(&rec), sizeof(T));

        bool fail = !_stream;

        return !fail;
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

#endif
