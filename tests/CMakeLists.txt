add_library(lib_test
    ${PROJECT_SOURCE_DIR}/tests/lib/lib_test.cpp
)

target_include_directories(lib_test
    PUBLIC
        ${PROJECT_SOURCE_DIR}/
)

add_executable(test
    ${PROJECT_SOURCE_DIR}/tests/test.cpp
)

target_include_directories(test
    PRIVATE
        ${PROJECT_SOURCE_DIR}/tests/lib
)

target_link_libraries(test
        PRIVATE
            lib_test
)

if(UNIX)
    target_link_libraries(test
        PRIVATE
            pthread
    )
endif()