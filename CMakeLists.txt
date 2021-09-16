cmake_minimum_required(VERSION 2.8.12)

PROJECT(TTT)

# include(/home/blackweb/Documents/ProgLibs/dlib-19.22/dlib)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "lib/")
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "lib/")
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")

set(CMAKE_BUILD_TYPE RelWithDebInfo)

# find_package(Armadillo REQUIRED)
#target_link_libraries(foo ${ARMADILLO_LIBRARIES})

# include_directories(${armadillo_DIR})
add_subdirectory(../../ProgLibs/armadillo-10.6.2 arma_build)
include_directories(${ARMADILLO_INCLUDE_DIRS})

#find_package(DLIB CONFIG REQUIRED)
# include_directories(${DLIB_INCLUDE_DIRECTORY})

#target_link_libraries(your_project ${DLIB_LIBRARIES} dlib)
add_subdirectory(../../ProgLibs/dlib-19.22/dlib dlib_build)
include_directories(${DLIB_INCLUDE_DIRECTORY})

add_executable(source Source.cpp)

target_link_libraries(source dlib::dlib)
# target_link_libraries(source armadillo::armadillo)
target_link_libraries(source armadillo)