cmake_minimum_required(VERSION 3.12...3.45)

project(ftn2xml VERSION 0.1.0)

# Output directories.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/data")

# Defines and options
if(NOT DEFINED VERSION)
	set(VERSION "${CMAKE_PROJECT_VERSION}")
endif()

if(NOT DEFINED CSS_PATH)
	set(CSS_PATH "${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}")
	message(STATUS "${CSS_PATH}")
endif()

# Depends
include(FeatureSummary)

find_package(CLI11 REQUIRED)
find_package(podofo REQUIRED)

feature_summary(
	WHAT ALL
	FATAL_ON_MISSING_REQUIRED_PACKAGES
)

# Sources
add_subdirectory(src)
add_subdirectory(css)
