
cmake_minimum_required(VERSION 3.21.1)
project(OpenLesson LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets Xml)

file(GLOB_RECURSE BASE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/openlesson-base/src/*.cpp"
)

qt_add_executable(openlesson-base ${BASE_SOURCES})

target_compile_features(openlesson-base PRIVATE cxx_std_17)

target_include_directories(openlesson-base PRIVATE
    openlesson-base/include
)

target_link_libraries(openlesson-base PRIVATE
    Qt6::Widgets
    Qt6::Xml
)

if(WIN32)
    target_link_libraries(openlesson-base PRIVATE advapi32)
endif()

include(cmake/package.cmake)
