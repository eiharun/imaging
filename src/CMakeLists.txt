
set(CORE_SOURCES
    image.cpp
    operations.cpp
    filters.cpp
)

add_executable(main main.cpp)

add_library(core_lib STATIC ${CORE_SOURCES})

target_link_libraries(core_lib PUBLIC 
    Qt6::Core 
    Qt6::Gui 
    Qt6::Widgets
)
target_link_libraries(core_lib PUBLIC fftw3 fftw3f)
target_link_libraries(main PRIVATE core_lib)
target_compile_definitions(core_lib PRIVATE D_TIME)
target_include_directories(core_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/include)
