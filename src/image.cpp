#include <cctype>
#include <image.h>
#include <fstream>
#include <iostream>


IMGError PPMLoader::save(const Image* img, std::string filename)  {
    // Implementation for saving a PPM image - P6 by default
    return IMGError::SUCCESS;
}

IMGError PPMLoader::save(const Image* img, std::string filename, PPMMode mode) {
    // Implementation for saving a PPM image
    if(mode==PPMMode::P6){
        return save(img, filename); // Default
    }

    return IMGError::SUCCESS;
}

IMGError PPMLoader::load(std::string filename, Image* img) {
    std::ifstream file(filename, std::ios::in);
    if (!file.is_open()) {
        return IMGError::FILENOTFOUND;
    }

    char magicNumber[2];
    
    file.read(magicNumber, 2);
    // TODO handle P3 vs P6
    if (magicNumber[0] != 'P' || magicNumber[1] != '6') {
        return IMGError::BADFORMAT;
    }

    m_is_open = true;

    file >> m_w;
    file >> m_h;
    file >> m_max_val;
    m_data.resize(m_w*m_h*m_channels);

    bool is_short{false};
    if(m_max_val > sizeof(uint8_t)){
        is_short = true;
    }

    size_t i{};
    if (is_short) {
        unsigned short val;
        while(file.read(reinterpret_cast<char*>(&val), sizeof(val))) {
            m_data[i++] = (float)val / sizeof(uint16_t);
        }
    } 
    else {
        unsigned char val;
        while(file.read(reinterpret_cast<char*>(&val), sizeof(val))) {
            m_data[i++] = (float)val / sizeof(uint8_t);
        }
    }

    return IMGError::SUCCESS;
}

// void PPMLoader::display() {
//     std::cout << "Warning, in BETA\n";
//     for (size_t row = 0; row < m_h; ++row) {
//         for (size_t col = 0; col < m_w; ++col) {
//             size_t idx = (row * m_w + col) * m_channels;
//             int r = m_data[idx];
//             int g = m_data[idx + 1];
//             int b = m_data[idx + 2];
//             std::cout << "\x1b[48;2;" << r << ";" << g << ";" << b << "m  ";
//         }
//         std::cout << "\x1b[0m\n";
//     }
// }