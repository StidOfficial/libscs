cmake_minimum_required(VERSION 3.0.0)

project(libSCS VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH bin/)
set(LIBRARY_OUTPUT_PATH bin/)
set(LIBPBO_SRCS
    src/entry.cpp
	src/scs.cpp
)

include_directories(include/)
add_library(scs SHARED ${LIBPBO_SRCS})

target_include_directories(scs PUBLIC include PRIVATE include/libscs)

add_executable(scsinfo tools/scsinfo/src/scsinfo.cpp)

target_link_libraries(scs stdc++fs z cityhash)

target_link_libraries(scsinfo stdc++fs z cityhash scs)

install(TARGETS scs LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install(DIRECTORY include/libscs DESTINATION include)