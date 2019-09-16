add_executable(getmodel)
target_sources(getmodel
	PRIVATE
		getmodel.cpp getmodel.h main.cpp
)

target_link_libraries(getmodel Boost::log z)

if (WIN32)
	target_link_libraries(getmodel Boost::log wingetopt z)
endif()

