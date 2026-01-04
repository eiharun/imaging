#pragma once
#include <image.h>

class Operation{
public:
    virtual Image apply(const Image* img) = 0;
    virtual ~Operation() = default;
};

class GreyscaleOp : public Operation{
public:
    static Image op(const Image* img);
    Image apply(const Image* img) override;
private:
    struct Color {
        float r;
        float g;
        float b;
    };
    Color m_avg { 1.0/3, 1.0/3, 1.0/3};
};

class LumaOp : public Operation{
public:
    static Image op(const Image* img, float brightness=0.0f, float contrast=1.0f);
    Image apply(const Image* img) override;
private:

};