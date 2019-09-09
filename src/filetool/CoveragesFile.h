#ifndef KTOOLS_FILETOOL_COVRAGES_FILE_H_
#define KTOOLS_FILETOOL_COVRAGES_FILE_H_

namespace ktools { namespace filetool {

struct coveragesdata
{
	int coverage_id;
	OASIS_FLOAT tiv;
};

class CoveragesFile : public BaseFileReader<coveragesdata>
{
public:
    CoveragesFile(const std::string& prefix)
     : BaseFileReader("coverages", prefix, COVERAGES_FILE)
    {}

    virtual ~CoveragesFile() {}
};

template<>
struct CsvFormatter<coveragesdata> {
    std::string header() {
        return  "coverage_id,tiv";
    }

    std::string row(const coveragesdata& rec) {
        std::stringstream ss;
        ss << rec.coverage_id << "," << rec.tiv;

        return ss.str();
    }
};

template<>
struct CsvStructInitializer<coveragesdata> {
	void initilize(coveragesdata& s, const std::string& field_name, const std::string& field_value) {
		if (field_name == "coverage_id") {
			s.coverage_id = std::stoi(field_value);
		}
		else if (field_name == "tiv") {
			s.tiv = std::stof(field_value);
		}
	}
};

}}

#endif
