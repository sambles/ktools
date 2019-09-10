#include "ItemsFile.h"
#include "CoveragesFile.h"
#include "DamageBinDictionaryFile.h"
#include "VulnerabilitiesFile.h"
#include "FootprintIndexFile.h"
#include "FootprintFile.h"

#define BOOST_TEST_MODULE TestFiles
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( TestFiles );

BOOST_AUTO_TEST_CASE(read_items_file)
{
    ktools::filetool::ItemsFile file("examples");

    BOOST_REQUIRE_EQUAL(89, file.num_records());

    item item_rec;
    int count = 0;
    while (file.read(item_rec)) {
        count++;
    }

    BOOST_REQUIRE_EQUAL(89, count);
}

/*
BOOST_AUTO_TEST_CASE(read_covrages_file)
{
    ktools::filetool::CoveragesFile file("examples");

    uint32_t nrec = file.num_records();
    BOOST_REQUIRE_EQUAL(89, nrec);

	ktools::filetool::coveragesdata rec;
	int count = 0;
    while (file.read(rec)) {
        count++;
    }

    BOOST_REQUIRE_EQUAL(nrec, count);
}
*/

BOOST_AUTO_TEST_CASE(read_damage_bin_dictionary_file)
{
    ktools::filetool::DamageBinDictionaryFile file("examples");

    uint32_t nrec = file.num_records();
    BOOST_REQUIRE_EQUAL(102, nrec);

    damagebindictionary rec;
    int count = 0;
    while (file.read(rec)) {
        count++;
    }

    BOOST_REQUIRE_EQUAL(nrec, count);
}

BOOST_AUTO_TEST_CASE(read_vulnerabilities_file)
{
    ktools::filetool::VulnerabilitiesFile file("examples");

    uint32_t nrec = file.num_records();
    BOOST_REQUIRE_EQUAL(2283270, nrec);

    Vulnerability rec;
    int count = 0;
    while (file.read(rec)) {
        count++;
    }

    BOOST_REQUIRE_EQUAL(nrec, count);
}

BOOST_AUTO_TEST_CASE(read_footprint_index_file)
{
    ktools::filetool::FootprintIndexFile file("examples", false);

    uint32_t nrec = file.num_records();
    BOOST_REQUIRE_EQUAL(249, nrec);

    EventIndex rec;
    int count = 0;
    while (file.read(rec)) {
        count++;
    }

    BOOST_REQUIRE_EQUAL(nrec, count);
}

BOOST_AUTO_TEST_CASE(read_footprint_file)
{
    ktools::filetool::FootprintIndexFile index_file("examples", false);
    ktools::filetool::FootprintFile file("examples", false);

    EventIndex index;
    while (index_file.read(index)) {
        if (index.size == 0) {
            continue;
        }
        file.init(index);
        
        EventRow row;
        int count = 0;
        while (file.read(row)) {
            count++;
        }

        BOOST_REQUIRE_EQUAL(file.num_records(), count);
    }
}

BOOST_AUTO_TEST_SUITE_END()
