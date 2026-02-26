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
    // ImgDisplay::qt(&stop);
    Image stop_grey = GreyscaleOp::op(&stop);
    ImgDisplay::qt(&stop_grey);
}

TEST(Operations, BrightnessAndContrast) {
    std::string test_ppm_path{
        "/home/harunie/Documents/imaging/images/ppm/stop_p6.ppm"};
    Image stop;
    PPMLoader ppm;
    ASSERT_EQ(ppm.load(test_ppm_path, &stop), IMGError::SUCCESS);
    // ImgDisplay::qt(&stop);
    Image stop_lum = LumaOp::op(&stop, -0.7, 1.3);
    ImgDisplay::qt(&stop_lum);
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

TEST(Operations, FlipHorizontal) {
    // 2x2 single-channel image: row0=[0.1, 0.2], row1=[0.3, 0.4]
    Image img;
    img.width = 2;
    img.height = 2;
    img.channels = 1;
    img.bitdepth = 8;
    img.type = StorageType::INTERLEAVED;
    img.data = {0.1f, 0.2f, 0.3f, 0.4f};

    Image flipped = FlipOp::op(&img, FlipAxis::HORIZONTAL);

    // After horizontal flip: row0=[0.2, 0.1], row1=[0.4, 0.3]
    EXPECT_FLOAT_EQ(flipped.data[0], 0.2f);
    EXPECT_FLOAT_EQ(flipped.data[1], 0.1f);
    EXPECT_FLOAT_EQ(flipped.data[2], 0.4f);
    EXPECT_FLOAT_EQ(flipped.data[3], 0.3f);
    EXPECT_EQ(flipped.width, img.width);
    EXPECT_EQ(flipped.height, img.height);
    EXPECT_EQ(flipped.channels, img.channels);
}

TEST(Operations, FlipVertical) {
    // 2x2 single-channel image: row0=[0.1, 0.2], row1=[0.3, 0.4]
    Image img;
    img.width = 2;
    img.height = 2;
    img.channels = 1;
    img.bitdepth = 8;
    img.type = StorageType::INTERLEAVED;
    img.data = {0.1f, 0.2f, 0.3f, 0.4f};

    Image flipped = FlipOp::op(&img, FlipAxis::VERTICAL);

    // After vertical flip: row0=[0.3, 0.4], row1=[0.1, 0.2]
    EXPECT_FLOAT_EQ(flipped.data[0], 0.3f);
    EXPECT_FLOAT_EQ(flipped.data[1], 0.4f);
    EXPECT_FLOAT_EQ(flipped.data[2], 0.1f);
    EXPECT_FLOAT_EQ(flipped.data[3], 0.2f);
    EXPECT_EQ(flipped.width, img.width);
    EXPECT_EQ(flipped.height, img.height);
    EXPECT_EQ(flipped.channels, img.channels);
}

TEST(Operations, FlipMultiChannel) {
    // 2x1 RGB image: pixel0=[0.1, 0.2, 0.3], pixel1=[0.4, 0.5, 0.6]
    Image img;
    img.width = 2;
    img.height = 1;
    img.channels = 3;
    img.bitdepth = 8;
    img.type = StorageType::INTERLEAVED;
    img.data = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f};

    Image flipped = FlipOp::op(&img, FlipAxis::HORIZONTAL);

    // After horizontal flip: pixel0=[0.4, 0.5, 0.6], pixel1=[0.1, 0.2, 0.3]
    EXPECT_FLOAT_EQ(flipped.data[0], 0.4f);
    EXPECT_FLOAT_EQ(flipped.data[1], 0.5f);
    EXPECT_FLOAT_EQ(flipped.data[2], 0.6f);
    EXPECT_FLOAT_EQ(flipped.data[3], 0.1f);
    EXPECT_FLOAT_EQ(flipped.data[4], 0.2f);
    EXPECT_FLOAT_EQ(flipped.data[5], 0.3f);
}

TEST(Filters, Sharpen) {
    std::string test_ppm_path{
        "/home/harunie/Documents/imaging/images/ppm/stop_p6.ppm"};
    Image stop;
    PPMLoader ppm;
    ASSERT_EQ(ppm.load(test_ppm_path, &stop), IMGError::SUCCESS);
    SharpenFilter sharpen;
    Image sharp_stop = sharpen.apply(&stop);
    ImgDisplay::qt(&sharp_stop);
}
