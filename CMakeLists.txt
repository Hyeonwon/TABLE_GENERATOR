cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME TableGenerator)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_XCC_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

project(${PROJECT_NAME})

#소스 파일 경로
file(GLOB SOURCE_FILES
    src/*.cpp
    src/*/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

find_program(CLANG_FORMAT "clang-format")
if (CLANG_FORMAT)
    add_custom_target(
        format
        COMMAND ${CLANG_FORMAT} -i ${SOURCE_FILES}
        COMMENT "Running clang-format on all sources."
    )
    add_dependencies(${PROJECT_NAME} format)
else()
    message(WARNING "ClangFormat not found.")
endif()