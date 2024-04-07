add_executable(ftn2xml ftn2xml.cc)

configure_file(ftn2xml.h.in ftn2xml.h)

add_library(fountain fountain.cc auxiliary.cc)

target_link_libraries(ftn2xml fountain podofo)

target_include_directories(
  ftn2xml PUBLIC
  "${CMAKE_CURRENT_BINARY_DIR}"
)

install(
  TARGETS ftn2xml
  DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
)

file(CREATE_LINK ftn2xml "${CMAKE_CURRENT_BINARY_DIR}/ftn2fdx" SYMBOLIC)
file(CREATE_LINK ftn2xml "${CMAKE_CURRENT_BINARY_DIR}/ftn2html" SYMBOLIC)
file(CREATE_LINK ftn2xml "${CMAKE_CURRENT_BINARY_DIR}/ftn2pdf" SYMBOLIC)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/ftn2fdx"
    "${CMAKE_CURRENT_BINARY_DIR}/ftn2html"
    "${CMAKE_CURRENT_BINARY_DIR}/ftn2pdf"
  DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
)
