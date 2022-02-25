cmake_minimum_required(VERSION 3.12)

project (Homework)

file (GLOB MY_SOURCES *.cpp)

foreach  (source_file ${MY_SOURCES})
    get_filename_component(source_name ${source_file} NAME_WE)

    add_executable (${source_name} ${source_file})
    target_link_libraries (${source_name} m)
endforeach (source_file ${MY_SOURCES})

