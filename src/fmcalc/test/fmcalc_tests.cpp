
#include "fmcalc.h"

#define BOOST_TEST_MODULE TestFMCalc
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(TestFMCalc);

BOOST_AUTO_TEST_CASE(apply_calc_rules)
{
	profile_rec_new profile;
	LossRec l;
	int calcrule_id = 1;
	int layer = 1;
	profile.calcrule_id = 1;
	add_tc(deductible_1,1000, profile.tc_vec);
	add_tc(limit_1, 35000, profile.tc_vec);
	l.loss = 40000;
	applycalcrule(profile, l, layer);
	
	BOOST_REQUIRE_EQUAL(l.loss, 35000.0);
	BOOST_REQUIRE_EQUAL(l.effective_deductible, 1000);

}
BOOST_AUTO_TEST_SUITE_END()
