cmake_minimum_required(VERSION 3.16)

project(priority2030)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(core)
add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CORE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CORE_LIBRARIES})