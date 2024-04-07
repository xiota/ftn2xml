cmake_minimum_required(VERSION 3.12...3.45)

project(ftn2xml VERSION 0.1.0)

if(NOT DEFINED VERSION)
	set(VERSION "${CMAKE_PROJECT_VERSION}")
endif()

include(FeatureSummary)

find_package(CLI11 REQUIRED)
find_package(podofo REQUIRED)

feature_summary(
	WHAT ALL
	FATAL_ON_MISSING_REQUIRED_PACKAGES
)

add_subdirectory(src)
