add_executable(filetool)
target_sources(filetool
	PRIVATE main.cpp
	PUBLIC BaseFileReader.h CoveragesFile.h DamageBinDictionaryFile.h FootprintFile.h FootprintIndexFile.h Interfaces.h ItemsFile.h VulnerabilitiesFile.h
)

if(Boost_FOUND)
	target_link_libraries(filetool Boost::log z)
else()
	target_link_libraries(filetool z)
endif()
