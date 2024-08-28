cmake_minimum_required(VERSION 3.29)
project(project1_roman_evan)

set(CMAKE_CXX_STANDARD 20)
add_executable(project1_roman_evan main.cpp)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(project1_roman_evan PRIVATE nlohmann_json::nlohmann_json)

find_package(wxwidgets CONFIG REQUIRED)
target_link_libraries(project1_roman_evan PRIVATE wx::core wx::base)

