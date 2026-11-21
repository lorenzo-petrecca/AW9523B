cmake_minimum_required(VERSION 3.16)

set(EXTRA_COMPONENT_DIRS
    "${CMAKE_CURRENT_LIST_DIR}/../../../adapters/esp_idf"
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(aw9523b_basic)
