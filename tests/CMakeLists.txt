add_executable(unit_tests 
    ut_image.cpp    
    ut_operations.cpp    
)

target_link_libraries(unit_tests PRIVATE
    core_lib
    GTest::gtest
    GTest::gtest_main
)
target_link_libraries(unit_tests PRIVATE 
    Qt6::Core 
    Qt6::Gui 
    Qt6::Widgets
)

target_include_directories(unit_tests PUBLIC ${CMAKE_SOURCE_DIR}/include)

gtest_discover_tests(unit_tests)
