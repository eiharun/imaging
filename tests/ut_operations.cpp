#include <gtest/gtest.h>
#include <image.h>
#include <operations.h>
#include <filters.h>

TEST(Operations, Greyscale){
    std::string test_ppm_path {"/home/harunie/Documents/imaging/images/ppm/stop_p6.ppm"};
    Image stop;
    PPMLoader ppm;
    ASSERT_EQ(ppm.load(test_ppm_path, &stop), IMGError::SUCCESS);
    // Display::term(&stop);
    Image stop_grey = GreyscaleOp::op(&stop);
    Display::term(&stop_grey);
}

TEST(Operations, BrightnessAndContrast){
    std::string test_ppm_path {"/home/harunie/Documents/imaging/images/ppm/stop_p6.ppm"};
    Image stop;
    PPMLoader ppm;
    ASSERT_EQ(ppm.load(test_ppm_path, &stop), IMGError::SUCCESS);
    // Display::term(&stop);
    Image stop_grey = LumaOp::op(&stop, -0.7, 1.3);
    Display::term(&stop_grey);
}

TEST(Filters, Gaussian){
    std::string test_ppm_path {"/home/harunie/Documents/imaging/images/ppm/stop_p6.ppm"};
    Image stop;
    PPMLoader ppm;
    ASSERT_EQ(ppm.load(test_ppm_path, &stop), IMGError::SUCCESS);
    GaussianBlur blur(1.0, 3);
    Image blur_stop = blur.apply(&stop);
    Display::term(&blur_stop);
}