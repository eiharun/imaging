#include <gtest/gtest.h>
#include <image.h>

TEST(Image, TestP6){
    std::string test_ppm_path {TEST_DATA_DIR "/ppm/tree_p6.ppm"};
    Image tree;
    PPMLoader ppm;
    ASSERT_EQ(ppm.load(test_ppm_path, &tree), IMGError::SUCCESS);
#ifndef HEADLESS_TESTS
    ImgDisplay::qt(&tree);
#endif
}

TEST(Image, TestBMP){
    std::string test_bmp_path {TEST_DATA_DIR "/bmp/snail.bmp"};
    Image snail;
    BMPLoader bmp;
    ASSERT_EQ(bmp.load(test_bmp_path, &snail), IMGError::SUCCESS);
#ifndef HEADLESS_TESTS
    ImgDisplay::qt(&snail);
#endif
}

TEST(Image, TestPNG){
    std::string test_png_path {TEST_DATA_DIR "/png/mountain.png"};
    Image mountain;
    PNGLoader png;
    ASSERT_EQ(png.load(test_png_path, &mountain), IMGError::SUCCESS);
#ifndef HEADLESS_TESTS
    ImgDisplay::qt(&mountain);
#endif
}
