#pragma once
#include <cstdint>
#include <vector>
#include <string>

struct Image{
 // PPM BMP etc become adapters for the Image
    uint32_t width;
    uint32_t height;
    uint32_t channels;
    uint32_t bitdepth;
    std::vector<float> data; // normalized
};


enum class IMGError {SUCCESS=0, FILENOTFOUND, BADFORMAT, UNKNOWNERROR};

class ImageLoader {
public:
    virtual IMGError save(const Image* img, std::string filename) = 0;
    virtual IMGError load(std::string filename, Image* img) = 0;
protected:
    size_t m_h;
    size_t m_w;
    std::vector<float> m_data;
    bool m_is_open{false};
};

/* PPM */

enum class PPMMode {P1=0, P2, P3, P4, P5, P6};

class PPMLoader : public ImageLoader {
// https://netpbm.sourceforge.net/doc/ppm.html
public:
    IMGError save(const Image* img, std::string filename) override;
    IMGError save(const Image* img, std::string filename, PPMMode mode);
    IMGError load(std::string filename, Image* img) override;
private:
    size_t m_max_val;
    const int m_channels = 3;
};

class PGMLoader : public ImageLoader {
// https://netpbm.sourceforge.net/doc/pgm.html
public:
    IMGError save(const Image* img, std::string filename) override;
    IMGError save(std::string filename, PPMMode mode);
    IMGError load(std::string filename, Image* img) override;
private:
    size_t m_max_val;
    const int m_channels = 1;
};

class PBMLoader : public ImageLoader {
// https://netpbm.sourceforge.net/doc/pbm.html
public:
    IMGError save(const Image* img, std::string filename) override;
    IMGError save(std::string filename, PPMMode mode);
    IMGError load(std::string filename, Image* img) override;
private:
    size_t m_max_val;
    const int m_channels = 1;
};

/* BMP */
class BMPLoader : public ImageLoader {
public:
private:

};