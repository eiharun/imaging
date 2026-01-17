#include <iterator>
#define STB_IMAGE_IMPLEMENTATION
#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <image.h>
#include <iostream>
#include <limits>
#include <stb_image.h>

bool ImageLoader::read_token(std::ifstream *in, std::string &out) {
    out.clear();
    char c{};
    while (in->get(c)) {
        if (std::isspace(static_cast<unsigned char>(c))) {
            continue;
        }
        if (c == '#') {
            in->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        out.push_back(c);
        break;
    }
    if (out.empty())
        return false;

    while (in->get(c)) {
        if (std::isspace(static_cast<unsigned char>(c))) {
            break;
        }
        if (c == '#') {
            in->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
        out.push_back(c);
    }

    return true;
}

IMGError PPMLoader::save(const Image *img, std::string filename) {
    // Implementation for saving a PPM image - P6 by default
    return IMGError::SUCCESS;
}

IMGError PPMLoader::save(const Image *img, std::string filename, PPMMode mode) {
    // Implementation for saving a PPM image
    if (mode == PPMMode::P6) {
        return save(img, filename); // Default
    }

    return IMGError::SUCCESS;
}

IMGError PPMLoader::load(std::string filename, Image *img) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        return IMGError::FILENOTFOUND;
    }

    uint16_t magicNumber;

    file.read(reinterpret_cast<char *>(&magicNumber), sizeof(magicNumber));
    magicNumber =
        (magicNumber >> 8) |
        (magicNumber << 8); // Convert from little-endian to host-endian
    if (magicNumber < 0x5031 || magicNumber > 0x5036) {
        std::cerr << "Incorrect magic number\n";
        return IMGError::BADFORMAT;
    }
    PPMMode mode = static_cast<PPMMode>(magicNumber - 0x5030);
    // TODO handle P3 vs P6
    // handle magic number interp
    switch (mode) {
    case PPMMode::P1: {
    } break;
    case PPMMode::P2: {
    } break;
    case PPMMode::P3: {
    } break;
    case PPMMode::P4: {
    } break;
    case PPMMode::P5: {
    } break;
    case PPMMode::P6: {
        return load_p6(&file, img);
    } break;

    default: {
        std::cerr << "Unhandled ppm mode";
        return IMGError::BADFORMAT;
    }
    }

    return IMGError::SUCCESS;
}

IMGError PPMLoader::load_p6(std::ifstream *file, Image *img) {
    std::string token{};
    read_token(file, token);
    img->width = std::stoi(token);
    read_token(file, token);
    img->height = std::stoi(token);
    read_token(file, token);
    m_max_val = std::stoi(token);

    img->channels = 3;
    img->data.resize(img->width * img->height * img->channels);
    bool is_short{false};
    if (m_max_val > 255) {
        is_short = true;
    }

    size_t i{};
    if (is_short) {
        unsigned short val;
        img->bitdepth = 16;
        for (size_t _{}; _ < img->width * img->height * img->channels; _++) {
            file->read(reinterpret_cast<char *>(&val), sizeof(val));
            img->data[i++] =
                static_cast<float>(val) / ((1u << img->bitdepth) - 1u);
        }
    } else {
        unsigned char val;
        img->bitdepth = 8;
        for (size_t _{}; _ < img->width * img->height * img->channels; _++) {
            file->read(reinterpret_cast<char *>(&val), sizeof(val));
            img->data[i++] =
                static_cast<float>(val) / ((1u << img->bitdepth) - 1u);
        }
    }

    return IMGError::SUCCESS;
}

IMGError BMPLoader::save(const Image *img, std::string filename) {
    return IMGError::SUCCESS;
}

IMGError BMPLoader::load(std::string filename, Image *img) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        return IMGError::FILENOTFOUND;
    }
    BMPHeader header;
    BMPInfoHeader header_info;
    file.read(reinterpret_cast<char *>(&header), sizeof(BMPHeader));
    file.read(reinterpret_cast<char *>(&header_info), sizeof(BMPInfoHeader));
    if (header.magic_number != 0x4D42) {
        std::cerr << "Incorrect magic number\n";
        return IMGError::BADFORMAT;
    }
    if (header_info.compression != 0) {
        std::cerr << "Compression mode not supported\n";
        return IMGError::BADFORMAT;
    }
    if (header_info.bits_p_pixel != 24 && header_info.bits_p_pixel != 32) {
        std::cerr << "Bit count not supported\n";
        return IMGError::BADFORMAT;
    }

    img->width = header_info.width;
    img->height = static_cast<uint32_t>(std::abs(header_info.height));
    img->bitdepth = 8;
    img->channels =
        3; // Only support 24/32 bit depth with no compression or alpha channel
    int bytes_p_pixel = header_info.bits_p_pixel / 8;
    int row_size = (img->width * bytes_p_pixel + 3) & ~3; // 4 byte alignment

    std::vector<uint8_t> data(row_size * img->height);
    file.seekg(header.data_offset);
    file.read(reinterpret_cast<char *>(data.data()), data.size());
    // Format in BGR
    // Copy data from buffer to img.data
    img->data.resize(img->width * img->height * img->channels);
    for (size_t y{}; y < img->height; ++y) {
        int s_y = header_info.height > 0 ? (img->height - 1 - y) : y;
        for (size_t x{}; x < img->width; ++x) {
            size_t src_idx = s_y * row_size;
            size_t dest_idx = (y * img->width + x) * img->channels;
            for (int c{}; c < img->channels; ++c) {
                int src_chan = (img->channels - 1) - c;
                img->data[dest_idx + c] =
                    static_cast<float>(
                        data[src_idx + x * bytes_p_pixel + src_chan]) /
                    ((1u << img->bitdepth) - 1u);
            }
        }
    }
    return IMGError::SUCCESS;
}

IMGError PNGLoader::save(const Image *img, std::string filename) {
    return IMGError::SUCCESS;
}

IMGError PNGLoader::load(std::string filename, Image *img) {
    int width, height, channels;
    unsigned char *raw_data =
        stbi_load(filename.c_str(), &width, &height, &channels, 0);// No support for 4 chan alpha yet
    if (!raw_data) {
        return IMGError::UNKNOWNERROR;
    }
    img->bitdepth = 8; // Only support 8-bit png so far
    img->width = width;
    img->height = height;
    img->channels = channels;
    img->data.reserve(width * height * channels);
    std::transform(raw_data, raw_data + (width * height * channels),
                   std::back_inserter(img->data), [img](unsigned char element) {
                       return static_cast<float>(element) /
                              ((1u << img->bitdepth) - 1u);
                   });
    stbi_image_free(raw_data);
    return IMGError::SUCCESS;
}

void ImgDisplay::term(const Image *img) {
    std::cout << "Warning, in BETA\n";
    auto to_u8 = [](float v) {
        v = std::clamp(v, 0.0f, 1.0f);
        return static_cast<int>(v * 255.0f + 0.5f);
    };

    for (size_t row = 0; row < img->height; ++row) {
        for (size_t col = 0; col < img->width; ++col) {
            size_t idx = (row * img->width + col) * img->channels;
            if (img->channels == 1) {
                int v = to_u8(img->data[idx]);
                std::cout << "\x1b[48;2;" << v << ";" << v << ";" << v << "m  ";
            } else {
                int r = to_u8(img->data[idx]);
                int g = to_u8(img->data[idx + 1]);
                int b = to_u8(img->data[idx + 2]);
                std::cout << "\x1b[48;2;" << r << ";" << g << ";" << b << "m  ";
            }
        }
        std::cout << "\x1b[0m\n";
    }
}

void ImgDisplay::initQtApp() {
    // Only create app if it doesn't already exist
    if (QApplication::instance() == nullptr) {
        static int argc = 1;
        static char *argv[] = {(char *)"Floating", nullptr};
        // Static instance persists for the program lifetime
        static QApplication app(argc, argv);
        app.setApplicationName("Floating");
    }
}

struct ResizeFilter : QObject {
    QLabel *label;
    QPixmap pix;

    bool eventFilter(QObject *obj, QEvent *ev) override {
        if (ev->type() == QEvent::Resize) {
            label->setPixmap(pix.scaled(label->size(), Qt::KeepAspectRatio,
                                        Qt::SmoothTransformation));
        }
        return false;
    }
};

void ImgDisplay::qt(const Image *img) {
    initQtApp();
    std::vector<unsigned char> char_img_data(img->width * img->height *
                                             img->channels);

    QImage::Format format;
    switch (img->channels) {
    case 1:
        format = QImage::Format_Grayscale8;
        break; // For 8-bit grayscale
    case 3:
        format = QImage::Format_RGB888;
        break; // Standard 24-bit RGB
    case 4:
        format = QImage::Format_RGBA8888;
        break; // 32-bit with Alpha
    default:
        return;
    }

    // 2. Allocate 8-bit buffer and convert float [0, 1] to uchar [0, 255]
    for (size_t i = 0; i < img->data.size(); ++i) {
        // Clamp and scale: ensures values outside [0, 1] don't wrap around
        char_img_data[i] = static_cast<unsigned char>(
            std::clamp(img->data[i] * 255.0f, 0.0f, 255.0f));
    }
    qsizetype bytes_p_line = static_cast<qsizetype>(img->width) * img->channels;
    QImage image(char_img_data.data(), img->width, img->height, bytes_p_line,
                 format);
    QDialog dialog;
    dialog.setWindowTitle(QString("Preview"));
    // dialog.setWindowFlags(dialog.windowFlags() | Qt::Window);
    const int maxW = 900;
    const int maxH = 700;
    dialog.resize(std::min(image.width(), maxW),
                  std::min(image.height(), maxH));

    QScreen *s = dialog.screen();
    QRect g = s->availableGeometry();

    dialog.setMinimumSize(200, 200);
    dialog.setMaximumSize(g.size());
    dialog.setBaseSize(dialog.size());
    dialog.setSizeIncrement(1, 1);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(0, 0, 0, 0);
    // layout->setSpacing(0);

    QLabel *label = new QLabel(nullptr);
    label->setAlignment(Qt::AlignCenter);
    label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    label->setMinimumSize(1, 1);

    QPixmap original = QPixmap::fromImage(image);
    label->setPixmap(original);

    layout->addWidget(label);
    // dialog.setFixedSize(image.size());

    ResizeFilter *rf = new ResizeFilter;
    rf->label = label;
    rf->pix = original;
    dialog.installEventFilter(rf);
    rf->setParent(&dialog);

    dialog.exec();
}
