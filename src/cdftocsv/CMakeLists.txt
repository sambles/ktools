add_executable(cdftocsv)
target_sources(cdftocsv
	PRIVATE
		cdftocsv.cpp main.cpp
)
target_link_libraries(cdftocsv Boost::log)
if (WIN32)
	target_link_libraries(cdftocsv Boost::log wingetopt)
endif()
