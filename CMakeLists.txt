cmake_minimum_required(VERSION 3.0.0)

project(libSCS VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH bin/)
set(LIBRARY_OUTPUT_PATH bin/)
set(LIBPBO_SRCS
    src/entry.cpp
	src/scs.cpp
    src/sii.cpp
)

include(CTest)
enable_testing()

include_directories(include/)

add_library(scs SHARED ${LIBPBO_SRCS})
target_include_directories(scs PUBLIC include PRIVATE include/libscs)
target_link_libraries(scs stdc++fs z cityhash)

add_executable(scsinfo tools/scsinfo/src/scsinfo.cpp)
target_link_libraries(scsinfo stdc++fs z cityhash scs)

add_executable(scs_extractor tools/scs_extractor/src/scs_extractor.cpp)
target_link_libraries(scs_extractor stdc++fs z cityhash scs)

add_executable(parsing_test tests/parsing.cpp)
target_link_libraries(parsing_test stdc++fs scs)

add_test(parsing bin/parsing_test ../tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

install(TARGETS scs LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install(DIRECTORY include/libscs DESTINATION include)