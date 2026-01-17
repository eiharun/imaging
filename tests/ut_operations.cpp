#include <filters.h>
#include <gtest/gtest.h>
#include <image.h>
#include <operations.h>

TEST(Operations, Greyscale) {
    std::string test_ppm_path{
        "/home/harunie/Documents/imaging/images/ppm/stop_p6.ppm"};
    Image stop;
    PPMLoader ppm;
    ASSERT_EQ(ppm.load(test_ppm_path, &stop), IMGError::SUCCESS);
    // ImgDisplay::term(&stop);
    Image stop_grey = GreyscaleOp::op(&stop);
    ImgDisplay::term(&stop_grey);
}

TEST(Operations, BrightnessAndContrast) {
    std::string test_ppm_path{
        "/home/harunie/Documents/imaging/images/ppm/stop_p6.ppm"};
    Image stop;
    PPMLoader ppm;
    ASSERT_EQ(ppm.load(test_ppm_path, &stop), IMGError::SUCCESS);
    // ImgDisplay::term(&stop);
    Image stop_lum = LumaOp::op(&stop, -0.7, 1.3);
    ImgDisplay::term(&stop_lum);
}

TEST(Filters, Gaussian) {
    std::string test_ppm_path{
        "/home/harunie/Documents/imaging/images/ppm/stop_p6.ppm"};
    Image stop;
    PPMLoader ppm;
    ASSERT_EQ(ppm.load(test_ppm_path, &stop), IMGError::SUCCESS);
    GaussianBlur blur(2, 7);
    Image blur_stop = blur.apply(&stop);
    ImgDisplay::qt(&blur_stop);
}

TEST(Filters, Edge) {
    std::string test_ppm_path{
        "/home/harunie/Documents/imaging/images/ppm/stop_p6.ppm"};
    Image stop;
    PPMLoader ppm;
    ASSERT_EQ(ppm.load(test_ppm_path, &stop), IMGError::SUCCESS);
    Image stop_grey = GreyscaleOp::op(&stop);
    Edge edge;
    Image edge_stop = edge.apply(&stop_grey);
    ImgDisplay::qt(&edge_stop);
}

TEST(Filters, Custom) {
    std::string test_bmp_path{
        "/home/harunie/Documents/imaging/images/bmp/greenland.bmp"};
    Image stop;
    BMPLoader bmp;
    ASSERT_EQ(bmp.load(test_bmp_path, &stop), IMGError::SUCCESS);
    Image stop_grey = GreyscaleOp::op(&stop);
    std::vector<float> cust_edge_kernel(9);
    cust_edge_kernel[0] = -1.0f;
    cust_edge_kernel[3] = -2.0f;
    cust_edge_kernel[6] = -1.0f;
    cust_edge_kernel[1] = 0.0f;
    cust_edge_kernel[4] = 0.0f;
    cust_edge_kernel[6] = 0.0f;
    cust_edge_kernel[1] = 1.0f;
    cust_edge_kernel[5] = 2.0f;
    cust_edge_kernel[7] = 1.0f;
    CustomConv edge(cust_edge_kernel, 3, 3);
    Image edge_stop = edge.apply(&stop_grey);
    ImgDisplay::qt(&edge_stop);
}

TEST(Filters, EdgeLarge) {
    std::string test_png_path{
        "/home/harunie/Documents/imaging/images/png/mountain.png"};
    Image mountain;
    PNGLoader png;
    ASSERT_EQ(png.load(test_png_path, &mountain), IMGError::SUCCESS);
    Image mountain_grey = GreyscaleOp::op(&mountain);
    Edge edge;
    Image edge_mountain = edge.apply(&mountain_grey);
    ImgDisplay::qt(&edge_mountain);
    // RAW took 3338.07ms
    // FFT (with complex) took 6845.97ms
    // FFT (with r2c and c2r) took 1449.96ms
    // FFT w/o plan (with r2c and c2r) took 853.96ms
}


