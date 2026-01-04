
set(CORE_SOURCES
    image.cpp
    operations.cpp
    filters.cpp
)

add_executable(main main.cpp)

add_library(core_lib STATIC ${CORE_SOURCES})

target_link_libraries(main PRIVATE core_lib)

target_include_directories(core_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/include)