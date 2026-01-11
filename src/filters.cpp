#include <algorithm>
#include <cassert>
#include <filters.h>
#define _USE_MATH_DEFINES
#include <cmath>

Image ConvolutionOperation::apply(const Image *img) {
    build_kernel();
    return convolve(img);
}

Image ConvolutionOperation::convolve(const Image *img) {
    Image result;
    int w_padding = (m_k_width - 1) / 2;
    int h_padding = (m_k_height - 1) / 2;

    result.width = img->width;
    result.height = img->height;
    result.channels = img->channels;
    result.bitdepth = img->bitdepth;
    result.data.resize(result.width * result.height * result.channels);
    for (size_t h{}; h < result.height; ++h) {
        for (size_t w{}; w < result.width; ++w) {
            for (size_t c{}; c < result.channels; ++c) {
                double sum{};
                for (size_t kh{}; kh < m_k_height; ++kh) {
                    for (size_t kw{}; kw < m_k_width; ++kw) {
                        int kernel_w_offset = kw - m_k_center_x;
                        int kernel_h_offset = kh - m_k_center_y;
                        signed long int w_idx =
                            std::clamp((signed long int)w + kernel_w_offset,
                                       (signed long int)0,
                                       (signed long int)img->width - 1);
                        signed long int h_idx =
                            std::clamp((signed long int)h + kernel_h_offset,
                                       (signed long int)0,
                                       (signed long int)img->height - 1);
                        size_t input_idx = (w_idx + result.width * h_idx);
                        sum += img->data[input_idx * result.channels + c] *
                               m_kernel[kw + m_k_width * kh];
                    }
                }
                result.data[(w + result.width * h) * result.channels + c] = sum;
            }
        }
    }

    return result;
}

GaussianBlur::GaussianBlur(float sigma, int width)
    : m_sigma(sigma), m_width(width) {
    assert(width % 2 == 1);
}

void GaussianBlur::build_kernel() {
    m_k_width = m_width;
    m_k_height = m_width;
    m_k_center_x = m_k_center_y = m_width / 2;

    m_kernel.resize(m_k_width * m_k_height);
    signed int x_begin = static_cast<signed int>(-(m_k_width / 2));
    signed int x_end = static_cast<signed int>(m_k_width / 2);
    signed int y_begin = static_cast<signed int>(-(m_k_height / 2));
    signed int y_end = static_cast<signed int>(m_k_height / 2);

    for (signed int y{y_begin}; y < y_end + 1; ++y) {
        for (signed int x{x_begin}; x < x_end + 1; ++x) {
            m_kernel[(x + m_k_center_x) + m_k_width * (y + m_k_center_y)] =
                std::exp(-1 * ((x * x + y * y) / (2.0 * m_sigma * m_sigma)));
            // m_kernel[(x+m_k_center_x) + m_k_width*(y+m_k_center_y)] =
            // (1.0/(2*M_PI*m_sigma*m_sigma)) * std::exp(-1*((x*x +
            // y*y)/(2.0*m_sigma*m_sigma)));
        }
    }
    double sum = 0.0;
    for (float v : m_kernel)
        sum += v;
    for (float &v : m_kernel)
        v /= sum;
}

void Edge::build_kernel() {
    // TODO: expand -> Edge is just a test operator
    m_k_width = 3;
    m_k_height = 3;
    m_k_center_x = m_k_center_y = m_k_width / 2;
    m_kernel.resize(m_k_width * m_k_height);
    m_kernel[0] = -1.0f;
    m_kernel[3] = -2.0f;
    m_kernel[6] = -1.0f;
    m_kernel[1] = 0.0f;
    m_kernel[4] = 0.0f;
    m_kernel[6] = 0.0f;
    m_kernel[1] = 1.0f;
    m_kernel[5] = 2.0f;
    m_kernel[7] = 1.0f;
}

CustomConv::CustomConv(const std::vector<float>& cust_kernel, int width, int height){
    m_k_width = width;
    m_k_height = height;
    m_k_center_x = m_k_width/2;
    m_k_center_y = m_k_height/2;
    m_kernel = cust_kernel;
}

void CustomConv::build_kernel(){} //empty
