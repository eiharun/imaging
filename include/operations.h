#pragma once
#include <image.h>

class Operation {
  public:
    virtual Image apply(const Image *img) = 0;
    virtual ~Operation() = default;
};

class GreyscaleOp : public Operation {
  public:
    static Image op(const Image *img);
    Image apply(const Image *img) override;

  private:
    struct Color {
        float r;
        float g;
        float b;
    };
    Color m_avg{1.0 / 3, 1.0 / 3, 1.0 / 3};
};

class LumaOp : public Operation {
  public:
    static Image op(const Image *img, float brightness = 0.0f,
                    float contrast = 1.0f);
    Image apply(const Image *img) override;

  private:
};

enum class FlipAxis { HORIZONTAL, VERTICAL };

class FlipOp : public Operation {
  public:
    explicit FlipOp(FlipAxis axis);
    static Image op(const Image *img, FlipAxis axis);
    Image apply(const Image *img) override;

  private:
    FlipAxis m_axis;
};
