#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct Image {
    // PPM BMP etc become adapters for the Image
    uint32_t width;
    uint32_t height;
    uint32_t channels;
    uint32_t bitdepth;
    std::vector<float> data; // normalized
};

enum class IMGError { SUCCESS = 0, FILENOTFOUND, BADFORMAT, UNKNOWNERROR };

class ImageLoader {
  public:
    virtual IMGError save(const Image *img, std::string filename) = 0;
    virtual IMGError load(std::string filename, Image *img) = 0;

  protected:
    bool read_token(std::ifstream *in, std::string &out);
};

/* PPM */

enum class PPMMode : short { P1 = 0x01, P2, P3, P4, P5, P6 };

class PPMLoader : public ImageLoader {
    // https://netpbm.sourceforge.net/doc/ppm.html
  public:
    IMGError save(const Image *img, std::string filename) override;
    IMGError save(const Image *img, std::string filename, PPMMode mode);
    IMGError load(std::string filename, Image *img) override;

  private:
    IMGError load_p6(std::ifstream *file, Image *img);
    size_t m_max_val;
};

/* BMP */
class BMPLoader : public ImageLoader {
  public:
    IMGError save(const Image *img, std::string filename) override;
    IMGError load(std::string filename, Image *img) override;

  private:
    struct BMPHeader {
        uint16_t magic_number;
        uint32_t file_size;
        uint32_t reserved;
        uint32_t data_offset;
    } __attribute__((packed));
    struct BMPInfoHeader {
        uint32_t size;
        uint32_t width;
        int32_t height;
        uint16_t n_planes;
        uint16_t bits_p_pixel;
        uint32_t compression;
        uint32_t img_size_cmprs;
        uint32_t x_pxl_p_m;
        uint32_t y_pxl_p_m;
        uint32_t color_used;
        uint32_t important_colors;
    } __attribute__((packed));
};

class PNGLoader : public ImageLoader {
  public:
    IMGError save(const Image *img, std::string filename) override;
    IMGError load(std::string filename, Image *img) override;

  private:
};

#include <QApplication>
#include <QDialog>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QScreen>
#include <QVBoxLayout>

class ImgDisplay {
  public:
    static void term(const Image *img);
    static void qt(const Image *img);
    static void initQtApp();
};
