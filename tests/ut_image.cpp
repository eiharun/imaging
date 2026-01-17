#include <gtest/gtest.h>
#include <image.h>

TEST(Image, TestP6){
    std::string test_ppm_path {"/home/harunie/Documents/imaging/images/ppm/tree_p6.ppm"};
    Image tree;
    PPMLoader ppm;
    ASSERT_EQ(ppm.load(test_ppm_path, &tree), IMGError::SUCCESS);
    ImgDisplay::term(&tree);
}

TEST(Image, TestBMP){
    std::string test_bmp_path {"/home/harunie/Documents/imaging/images/bmp/snail.bmp"};
    Image snail;
    BMPLoader bmp;
    ASSERT_EQ(bmp.load(test_bmp_path, &snail), IMGError::SUCCESS);
    ImgDisplay::qt(&snail);
}

TEST(Image, TestPNG){
    std::string test_png_path {"/home/harunie/Documents/imaging/images/png/mountain.png"};
    Image mountain;
    PNGLoader png;
    ASSERT_EQ(png.load(test_png_path, &mountain), IMGError::SUCCESS);
    ImgDisplay::qt(&mountain);
}
