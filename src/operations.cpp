#include <algorithm>
#include <operations.h>

Image GreyscaleOp::op(const Image *img) {
    Image result;
    result.width = img->width;
    result.height = img->height;
    result.channels = 1;
    result.bitdepth = img->bitdepth;
    size_t size = result.width * result.height;
    result.data.resize(size);

    for (int i{}; i < size; ++i) {
        size_t src_index = i * img->channels;
        result.data[i] = (img->data[src_index] + img->data[src_index + 1] +
                          img->data[src_index + 2]) /
                         3;
    }

    return result;
}

Image LumaOp::op(const Image *img, float brightness, float contrast) {
    Image result = *img;

    size_t size = result.width * result.height * result.channels;
    for (int i{}; i < size; ++i) {
        float new_val = contrast * result.data[i] + brightness;
        new_val = std::clamp(new_val, 0.0f, 1.0f);
        result.data[i] = new_val;
    }

    return result;
}

FlipOp::FlipOp(FlipAxis axis) : m_axis(axis) {}

Image FlipOp::op(const Image *img, FlipAxis axis) {
    Image result = *img;
    if (axis == FlipAxis::HORIZONTAL) {
        for (size_t y = 0; y < img->height; ++y) {
            for (size_t x = 0; x < img->width / 2; ++x) {
                size_t left = (y * img->width + x) * img->channels;
                size_t right =
                    (y * img->width + (img->width - 1 - x)) * img->channels;
                for (size_t c = 0; c < img->channels; ++c) {
                    std::swap(result.data[left + c], result.data[right + c]);
                }
            }
        }
    } else {
        for (size_t y = 0; y < img->height / 2; ++y) {
            for (size_t x = 0; x < img->width; ++x) {
                size_t top = (y * img->width + x) * img->channels;
                size_t bottom =
                    ((img->height - 1 - y) * img->width + x) * img->channels;
                for (size_t c = 0; c < img->channels; ++c) {
                    std::swap(result.data[top + c], result.data[bottom + c]);
                }
            }
        }
    }
    return result;
}

Image FlipOp::apply(const Image *img) { return op(img, m_axis); }
