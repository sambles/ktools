add_executable(filetool)
target_sources(filetool
	PRIVATE main.cpp Interfaces.h filetool.h filetool.cpp
	PUBLIC BaseFileReader.h CoveragesFile.h DamageBinDictionaryFile.h FootprintFile.h FootprintIndexFile.h ItemsFile.h VulnerabilitiesFile.h
)

target_link_libraries(filetool Boost::log)
if (WIN32)
	target_link_libraries(filetool Boost::log wingetopt)
endif()

# -------------------------------------------------------------------------
# Test

add_executable(filetool_tests)
target_sources(filetool_tests
	PRIVATE
		test/TestFiletoolFiles.cpp
)
target_include_directories(filetool_tests PRIVATE .)
target_link_libraries(filetool_tests Boost::log Boost::unit_test_framework z)
