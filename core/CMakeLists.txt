cmake_minimum_required(VERSION 3.16)

project(priority2030_core)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(connection)
add_subdirectory(parser)
add_subdirectory(server)
add_subdirectory(session)

file(GLOB INCLUDE_DIRS
        ${CONNECTION_INCLUDE_DIRS}
        ${PARSER_INCLUDE_DIRS}
        ${SERVER_INCLUDE_DIRS}
        ${SESSION_INCLUDE_DIRS}
)

file(GLOB SOURCE_FILES 
        ${CONNECTION_SOURCE_FILES}
        ${PARSER_SOURCE_FILES}
        ${SERVER_SOURCE_FILES}
        ${SESSION_SOURCE_FILES}
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

# target_link_libraries(${PROJECT_NAME} sfml-window sfml-graphics sfml-system pthread crypto)

set(CORE_LIBRARIES ${PROJECT_NAME} PARENT_SCOPE)
set(CORE_INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)