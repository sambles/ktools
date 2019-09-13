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
#include "ItemsFile.h"
#include "CoveragesFile.h"
#include "DamageBinDictionaryFile.h"
#include "VulnerabilitiesFile.h"
#include "FootprintIndexFile.h"
#include "FootprintFile.h"
#include "FMPolicyTCFile.h"
#include "EventFile.h"
#include "fmprofilefile.h"

#define BOOST_TEST_MODULE TestFiles
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( TestFiles );

BOOST_AUTO_TEST_CASE(read_items_file)
{
    ktools::filetool::ItemsFile file("examples");

    BOOST_REQUIRE_EQUAL(89, file.num_records());

    item rec;
    int count = 0;
    while (file.read(rec)) {
        count++;
    }

    BOOST_REQUIRE_EQUAL(89, count);
}

BOOST_AUTO_TEST_CASE(read_events_file)
{
	ktools::filetool::EventFile file("examples");

	BOOST_REQUIRE_EQUAL(249, file.num_records());

	ktools::filetool::eventdata rec;
	int count = 0;
	while (file.read(rec)) {
		count++;
	}

	BOOST_REQUIRE_EQUAL(249, count);
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


BOOST_AUTO_TEST_CASE(read_fm_policy_tc_file)
{
	ktools::filetool::FMPolicyTCFile file("examples");

	uint32_t nrec = file.num_records();
	BOOST_REQUIRE_EQUAL(96, nrec);

	ktools::filetool::fm_policyTC rec;
	int count = 0;
	while (file.read(rec)) {
		count++;
	}

	BOOST_REQUIRE_EQUAL(nrec, count);
}

BOOST_AUTO_TEST_CASE(read_fmprofile_file)
{
	ktools::filetool::FMProfileFile file("examples");

	uint32_t nrec = file.num_records();
	BOOST_REQUIRE_EQUAL(89, nrec);

	fm_profile_new rec;
	int count = 0;
	while (file.read(rec)) {
		count++;
	}

	BOOST_REQUIRE_EQUAL(nrec, count);
}

BOOST_AUTO_TEST_SUITE_END()
