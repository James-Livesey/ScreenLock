cmake_minimum_required(VERSION 3.25)
project(ScreenLock)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-4.0)

include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*.cpp)

add_executable(ScreenLock ${SOURCE_FILES})
target_include_directories(ScreenLock PRIVATE src)
target_link_libraries(ScreenLock ${GTKMM_LIBRARIES} crypt)