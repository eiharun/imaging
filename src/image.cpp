#include <cctype>
#include <image.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <limits>

bool ImageLoader::read_token(std::ifstream* in, std::string& out){
    out.clear();
    char c{};
    while(in->get(c)){
        if(std::isspace(static_cast<unsigned char>(c))){
            continue;
        }
        if(c == '#'){
            in->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        out.push_back(c);
        break;
    }
    if(out.empty()) return false;

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
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        return IMGError::FILENOTFOUND;
    }

    uint16_t magicNumber;
    
    file.read(reinterpret_cast<char*>(&magicNumber), sizeof(magicNumber));
    magicNumber = (magicNumber >> 8) | (magicNumber << 8); // Convert from little-endian to host-endian
    if(magicNumber < 0x5031 || magicNumber > 0x5036){
        return IMGError::BADFORMAT;
    }
    PPMMode mode = static_cast<PPMMode>(magicNumber-0x5030);
    // TODO handle P3 vs P6
    // handle magic number interp
    switch(mode){
        case PPMMode::P1: {} break;
        case PPMMode::P2: {} break;
        case PPMMode::P3: {} break;
        case PPMMode::P4: {} break;
        case PPMMode::P5: {} break;
        case PPMMode::P6: { return load_p6(&file, img); } break;
        
        default:{std::cerr << "Unhandled ppm mode"; return IMGError::BADFORMAT;}
    }

    return IMGError::SUCCESS;
}

IMGError PPMLoader::load_p6(std::ifstream* file, Image* img){
    std::string token{};
    read_token(file, token);
    img->width = std::stoi(token);
    read_token(file, token);
    img->height = std::stoi(token);
    read_token(file, token);
    m_max_val = std::stoi(token);

    img->channels = 3;
    img->data.resize(img->width*img->height*img->channels);
    bool is_short{false};
    if(m_max_val > 255){
        is_short = true;
    }

    size_t i{};
    if (is_short) {
        unsigned short val;
        img->bitdepth = 16;
        for(size_t _{}; _<img->width*img->height*img->channels; _++){
            file->read(reinterpret_cast<char*>(&val), sizeof(val));
            img->data[i++] = (float)val / ((1u << img->bitdepth) -1u);
        }
    } 
    else {
        unsigned char val;
        img->bitdepth = 8;
        for(size_t _{}; _<img->width*img->height*img->channels; _++){
            file->read(reinterpret_cast<char*>(&val), sizeof(val));
            img->data[i++] = (float)val / ((1u << img->bitdepth) -1u);
        }
    }

    return IMGError::SUCCESS;
}



void Display::term(const Image* img){
    std::cout << "Warning, in BETA\n";
    auto to_u8 = [](float v) {
        v = std::clamp(v, 0.0f, 1.0f);
        return static_cast<int>(v * 255.0f + 0.5f);
    };

    for (size_t row = 0; row < img->height; ++row) {
        for (size_t col = 0; col < img->width; ++col) {
            size_t idx = (row * img->width + col) * img->channels;
            if(img->channels == 1){
                int v = to_u8(img->data[idx]);
                std::cout << "\x1b[48;2;" << v << ";" << v << ";" << v << "m  ";
            }
            else{
                int r = to_u8(img->data[idx]);
                int g = to_u8(img->data[idx + 1]);
                int b = to_u8(img->data[idx + 2]);
                std::cout << "\x1b[48;2;" << r << ";" << g << ";" << b << "m  ";
            }
        }
        std::cout << "\x1b[0m\n";
    }
}