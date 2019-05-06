cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(nana_creator VERSION      0.15.1
        DESCRIPTION "RAD tool for Nana C++ Library"
        HOMEPAGE_URL https://github.com/besh81/nana-creator
        LANGUAGES CXX         )

#######################     Main setting of sources, targets and install

add_library(pugixml ${CMAKE_CURRENT_LIST_DIR}/libs/pugixml/pugixml.cpp)
target_include_directories(pugixml PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libs)

add_executable(nana_creator)
target_include_directories(nana_creator PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)

set(nana_creator_SOURCE_DIR         ${CMAKE_CURRENT_LIST_DIR}/src)
set(nana_creator_SOURCE_SUBDIRS     /.
                                    /ctrls
                                    /nana_extra
                                    )

# collect all source files in the source-sub-dir
foreach(subdir ${nana_creator_SOURCE_SUBDIRS})
    aux_source_directory(${nana_creator_SOURCE_DIR}${subdir} SOURCES) # todo: use GLOB to add headers too ??
endforeach()

target_sources(nana_creator PRIVATE ${SOURCES})

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../nana ${CMAKE_CURRENT_LIST_DIR}/../cmake-nana-build-${CMAKE_BUILD_TYPE} )
target_link_libraries(nana_creator PUBLIC nana pugixml)



install(TARGETS nana_creator RUNTIME DESTINATION "bin")

if (NANA_CMAKE_VERBOSE_PREPROCESSOR)

    include(CMakePrintHelpers)
    cmake_print_properties(TARGETS nana_creator  PROPERTIES
            COMPILE_DEFINITIONS   COMPILE_OPTIONS    COMPILE_FLAGS  LINK_LIBRARIES
            INCLUDE_DIRECTORIES   INSTALL_NAME_DIR   LINK_FLAGS     VERSION)
    cmake_print_properties(TARGETS pugixml  PROPERTIES
            COMPILE_DEFINITIONS   COMPILE_OPTIONS    COMPILE_FLAGS  LINK_LIBRARIES
            INCLUDE_DIRECTORIES   INSTALL_NAME_DIR   LINK_FLAGS     VERSION)
endif()