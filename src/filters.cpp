#include <algorithm>
#include <cassert>
#include <filters.h>
#include <ratio>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <chrono>
extern "C"{
#include <fftw3.h>
}

Image ConvolutionOperation::apply(const Image *img) {
    build_kernel();
    return convolve(img); // Revert to convolve() after fft is done
}

inline int next_pow2(int n) {
    if (n <= 0)
        return 1;
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n + 1;
}

Image ConvolutionOperation::convolve(const Image *img) {
#ifdef D_TIME
    std::cout << "Using FFT convolve\n";
    // Implement fftw3 here
    auto start = std::chrono::high_resolution_clock::now();
#endif
    if(img->channels != 1){
        std::cerr << "Only support for 1 channel so far\n";
    }
    int f_w = next_pow2(img->width + m_k_width - 1);
    int f_h = next_pow2(img->height + m_k_height - 1);

    fftwf_complex *A = fftwf_alloc_complex(f_h * f_w);
    fftwf_complex *B = fftwf_alloc_complex(f_h * f_w);
    fftwf_complex *C = fftwf_alloc_complex(f_h * f_w);
    std::vector<float> Ar(f_w*f_h);
    std::vector<float> Br(f_w*f_h);
    std::vector<float> Cr(f_w*f_h);
    memset(A, 0, f_w * f_h * sizeof(fftwf_complex));
    memset(B, 0, f_w * f_h * sizeof(fftwf_complex));

    for (size_t y{}; y < img->height; ++y) {
        for (size_t x{}; x < img->width; ++x) {
            Ar[y * f_w + x] = img->data[y * img->width + x];
        }
    }

    for (int y = 0; y < m_k_height; y++)
        for (int x = 0; x < m_k_width; x++) {
            int sx = (x - m_k_width / 2 + f_w) % f_w;
            int sy = (y - m_k_height / 2 + f_h) % f_h;
            Br[sy * f_w + sx] = m_kernel[y * m_k_width + x];
        }

    auto pA = fftwf_plan_dft_r2c_2d(f_h, f_w, Ar.data(), A, FFTW_ESTIMATE);
    auto pB = fftwf_plan_dft_r2c_2d(f_h, f_w, Br.data(), B, FFTW_ESTIMATE);
    auto pC = fftwf_plan_dft_c2r_2d(f_h, f_w, C, Cr.data(), FFTW_ESTIMATE);

    fftwf_execute(pA);
    fftwf_execute(pB);

    for (int i = 0; i < f_w * f_h; i++) {
        float ar = A[i][0], ai = A[i][1];
        float br = B[i][0], bi = B[i][1];
        C[i][0] = ar * br - ai * bi;
        C[i][1] = ar * bi + ai * br;
    }
    fftwf_execute(pC);
    float s = 1.0f / (f_w * f_h);
    Image out;
    out.width = img->width;
    out.height = img->height;
    out.channels = img->channels;
    out.bitdepth = img->bitdepth;
    out.data.resize(img->width * img->height);
    for (int y = 0; y < img->height; y++)
        for (int x = 0; x < img->width; x++)
            out.data[y * img->width + x] = Cr[y * f_w + x] * s;
    fftwf_destroy_plan(pA);
    fftwf_destroy_plan(pB);
    fftwf_destroy_plan(pC);
    fftwf_free(A);
    fftwf_free(B);
    fftwf_free(C);
#ifdef D_TIME
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "FFT Conv took: " << duration.count() << " ms\n";
#endif
    return out;
}

Image ConvolutionOperation::convolve_raw(const Image *img) {
#ifdef D_TIME
    std::cout << "Using convolve raw\n";
    auto start = std::chrono::high_resolution_clock::now();
#endif
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

#ifdef D_TIME
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "ConvRaw took: " << duration.count() << " ms\n";
#endif
    
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

CustomConv::CustomConv(const std::vector<float> &cust_kernel, int width,
                       int height) {
    m_k_width = width;
    m_k_height = height;
    m_k_center_x = m_k_width / 2;
    m_k_center_y = m_k_height / 2;
    m_kernel = cust_kernel;
}

void CustomConv::build_kernel() {} // empty
