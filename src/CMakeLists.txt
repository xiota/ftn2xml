add_executable(ftn2xml ftn2xml.cc)

configure_file(ftn2xml.h.in ftn2xml.h)

add_library(fountain fountain.cc auxiliary.cc)

target_link_libraries(ftn2xml fountain podofo)

target_include_directories(
  ftn2xml PUBLIC
  "${CMAKE_CURRENT_BINARY_DIR}"
)


add_custom_target(link_target ALL
  COMMAND ${CMAKE_COMMAND} -E create_symlink ftn2xml ftn2fdx
  COMMAND ${CMAKE_COMMAND} -E create_symlink ftn2xml ftn2html
  COMMAND ${CMAKE_COMMAND} -E create_symlink ftn2xml ftn2pdf
  WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)

# install(TARGETS ftn2xml DESTINATION "${CMAKE_INSTALL_BINDIR}")

install(
  DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
  DESTINATION "${CMAKE_INSTALL_PREFIX}"
  FILES_MATCHING PATTERN "ftn2*"
  PERMISSIONS
    OWNER_READ
    OWNER_WRITE
    OWNER_EXECUTE
    GROUP_READ
    GROUP_EXECUTE
    WORLD_READ
    WORLD_EXECUTE
)
