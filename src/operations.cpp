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
