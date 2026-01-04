#include <filters.h>
#define _USE_MATH_DEFINES
#include <cmath>

Image ConvolutionOperation::apply(const Image* img) {
    build_kernel();
    return convolve(img);
}

Image ConvolutionOperation::convolve(const Image* img){
    Image result;
    int w_padding = (m_k_width-1)/2;
    int h_padding = (m_k_height-1)/2;

    result.width = img->width + 2*w_padding - m_k_width + 1;
    result.height = img->height + 2*h_padding - m_k_height + 1;
    result.channels = img->channels;
    result.bitdepth = img->bitdepth;

    for(size_t w{}; w<result.width; ++w){
        for(size_t h{}; h<result.height; ++h){
            double sum{};
            for(size_t kw{}; kw<m_k_width; ++kw){
                for(size_t kh{}; kh<m_k_height; ++kh){
                    int kernel_w_offset = (kw-m_k_center_idx)*(m_k_width/2);
                    int kernel_h_offset = (kh-m_k_center_idx)*(m_k_height/2);
                    int w_idx = w+kernel_w_offset;
                    int h_idx = h+kernel_h_offset;
                    size_t input_idx = (w_idx + result.width*h_idx);
                    sum += img->data[input_idx] * m_kernel[kw + m_k_width*kh];
                }
            }
            result.data[w + result.width*h] = sum;
        }
    }

    return result;
}

GaussianBlur::GaussianBlur(float sigma, int width): m_sigma(sigma), m_width(width){}

void GaussianBlur::build_kernel(){
    m_k_width = m_width;
    m_k_height = m_width;
    m_k_center_idx = m_width/2;

    m_kernel.resize(m_k_width*m_k_height);
    signed int x_begin = static_cast<signed int>(m_k_center_idx - (m_k_width-1));
    signed int x_end = static_cast<signed int>(m_k_width - m_k_center_idx);
    signed int y_begin = static_cast<signed int>(m_k_center_idx - (m_k_height-1));
    signed int y_end = static_cast<signed int>(m_k_height - m_k_center_idx);

    for(signed int x{x_begin}; x<x_end; ++x){
        for(signed int y{y_begin}; y<y_end; ++y){
            m_kernel[(x+m_k_center_idx) + m_k_width*(y+m_k_center_idx)] = (1.0/(2*M_PI*m_sigma*m_sigma)) * std::exp(-1*((x*x + y*y)/(2.0*m_sigma*m_sigma)));
        }
    }

}