add_executable(fmcalc)
target_sources(fmcalc
	PRIVATE
		fmcalc.cpp fmcalc.h main.cpp fmcalcrules.cpp fmcalcdoit.cpp fmstructs.h
)

# -------------------------------------------------------------------------
# Test
if (BUILD_TESTING)

add_executable(fmcalc_tests)
target_sources(fmcalc_tests
	PRIVATE
		test/fmcalc_tests.cpp fmcalcrules.cpp
)
target_include_directories(fmcalc_tests PRIVATE .)
target_link_libraries(fmcalc_tests Boost::log Boost::unit_test_framework)

endif()