#include <gtest/gtest.h>
#include <image.h>

TEST(IMAGE, PPMLoad){
    std::string test_ppm_path {"/home/harunie/Documents/Imaging/images/ppm/tree_1.ppm"};
    PPM file;
    ASSERT_EQ(file.load(test_ppm_path), IMGError::SUCCESS);    
    file.display();
}