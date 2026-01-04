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
    bool read_token(std::ifstream* in, std::string& out);
};

/* PPM */

enum class PPMMode : short {P1=0x01, P2, P3, P4, P5, P6};

class PPMLoader : public ImageLoader {
// https://netpbm.sourceforge.net/doc/ppm.html
public:
    IMGError save(const Image* img, std::string filename) override;
    IMGError save(const Image* img, std::string filename, PPMMode mode);
    IMGError load(std::string filename, Image* img) override;
private:
    IMGError load_p6(std::ifstream* file, Image* img);
    size_t m_max_val;
};

/* BMP */
class BMPLoader : public ImageLoader {
public:
private:
};

class Display{
public:
    static void term(const Image* img);
};
