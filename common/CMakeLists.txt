add_library(common_lib
    Common.cpp
)

target_include_directories(common_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
