add_executable(summarycalc)
target_sources(summarycalc
	PRIVATE
		summarycalc.cpp summarycalc.h main.cpp
)

target_link_libraries(summarycalc Boost::log)
if (WIN32)
	target_link_libraries(summarycalc Boost::log wingetopt)
endif()
