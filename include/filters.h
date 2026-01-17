#pragma once
#include <image.h>
#include <operations.h>

class ConvolutionOperation : public Operation {
  public:
    Image apply(const Image *img) override;

  protected:
    std::vector<float> m_kernel;
    size_t m_k_width;
    size_t m_k_height;
    size_t m_k_center_x;
    size_t m_k_center_y;

    virtual void build_kernel() = 0;

  private:
    Image convolve(const Image *img);
    Image convolve_raw(const Image *img);
};

class GaussianBlur : public ConvolutionOperation {
  public:
    explicit GaussianBlur(float sigma, int width);

  protected:
    void build_kernel() override;

  private:
    float m_sigma;
    int m_width;
};

class Edge : public ConvolutionOperation {
  public:
  protected:
    void build_kernel() override;

  private:
};

class CustomConv : public ConvolutionOperation {
  public:
      CustomConv(const std::vector<float>& cust_kernel, int width, int height);
  protected:
      void build_kernel() override;
  private:
};
