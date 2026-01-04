#include <gtest/gtest.h>
#include <image.h>

TEST(IMAGE, TestP6){
    std::string test_ppm_path {"/home/harunie/Documents/imaging/images/ppm/tree_p6.ppm"};
    Image tree;
    PPMLoader ppm;
    ASSERT_EQ(ppm.load(test_ppm_path, &tree), IMGError::SUCCESS);
    Display::term(&tree);
}