#ifndef BMP_LOADER_H
#define BMP_LOADER_H

#include <vector>
#include <fstream>
#include <stdexcept>
#include <cstdint>
#include <algorithm>
#include "glad.h"

class BMPLoader {
private:
    // BMP file header structures (packed to ensure exact memory layout)
    #pragma pack(push, 1)
    struct BMPFileHeader {
        uint16_t file_type;     // File type, should be 0x4D42 (BMP signature)
        uint32_t file_size;     // Size of the file in bytes
        uint16_t reserved1;     // Reserved, must be zero
        uint16_t reserved2;     // Reserved, must be zero
        uint32_t offset_data;   // Offset to image data in bytes
    };

    struct BMPInfoHeader {
        uint32_t header_size;       // Size of this header
        int32_t width;              // Image width
        int32_t height;             // Image height
        uint16_t planes;            // Must be 1
        uint16_t bit_count;         // Bits per pixel
        uint32_t compression;       // Compression method
        uint32_t image_size;        // Size of the raw bitmap data
        int32_t x_pixels_per_meter; // Horizontal resolution
        int32_t y_pixels_per_meter; // Vertical resolution
        uint32_t colors_used;       // Number of colors in the color palette
        uint32_t colors_important;  // Number of important colors
    };
    #pragma pack(pop)

    // Supported compression methods
    enum class Compression {
        BI_RGB = 0,        // No compression
        BI_RLE8 = 1,       // 8-bit RLE compression
        BI_RLE4 = 2,       // 4-bit RLE compression
        BI_BITFIELDS = 3   // Bit field compression
    };

public:
    /**
     * Load BMP file and prepare for OpenGL texture
     * 
     * @param filename Path to BMP file
     * @param width Output width of image
     * @param height Output height of image
     * @return Vector of decoded image data in RGBA
     */
    static std::vector<uint8_t> loadBMP(const std::string& filename, int& width, int& height) {
        // Open file in binary mode
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Could not open BMP file: " + filename);
        }

        // Read file header
        BMPFileHeader file_header;
        file.read(reinterpret_cast<char*>(&file_header), sizeof(BMPFileHeader));
        
        // Validate BMP signature
        if (file_header.file_type != 0x4D42) {
            throw std::runtime_error("Invalid BMP file: Incorrect signature");
        }

        // Read info header
        BMPInfoHeader info_header;
        file.read(reinterpret_cast<char*>(&info_header), sizeof(BMPInfoHeader));

        // Set output dimensions
        width = std::abs(info_header.width);
        height = std::abs(info_header.height);

        // Check compression and bit depth
        if (info_header.compression != static_cast<uint32_t>(Compression::BI_RGB)) {
            throw std::runtime_error("Compressed BMPs are not supported");
        }

        // Determine bytes per pixel and padding
        int bytes_per_pixel = info_header.bit_count / 8;
        int padding = (4 - (width * bytes_per_pixel) % 4) % 4;

        // Seek to image data
        file.seekg(file_header.offset_data);

        // Read pixel data
        std::vector<uint8_t> pixel_data(width * height * bytes_per_pixel);
        std::vector<uint8_t> rgba_data(width * height * 4);

        // Read rows in reverse order (BMPs are stored bottom-up)
        for (int y = height - 1; y >= 0; --y) {
            // Read a row of pixels
            file.read(reinterpret_cast<char*>(pixel_data.data() + y * width * bytes_per_pixel), 
                      width * bytes_per_pixel);
            
            // Skip padding bytes
            file.seekg(padding, std::ios_base::cur);
        }

        // Convert to RGBA
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                size_t src_idx = (y * width + x) * bytes_per_pixel;
                size_t dst_idx = (y * width + x) * 4;

                if (bytes_per_pixel == 3) {  // BGR format
                    rgba_data[dst_idx + 0] = pixel_data[src_idx + 2];  // R
                    rgba_data[dst_idx + 1] = pixel_data[src_idx + 1];  // G
                    rgba_data[dst_idx + 2] = pixel_data[src_idx + 0];  // B
                    rgba_data[dst_idx + 3] = 255;                      // A
                }
                else if (bytes_per_pixel == 4) {  // BGRA format
                    rgba_data[dst_idx + 0] = pixel_data[src_idx + 2];  // R
                    rgba_data[dst_idx + 1] = pixel_data[src_idx + 1];  // G
                    rgba_data[dst_idx + 2] = pixel_data[src_idx + 0];  // B
                    rgba_data[dst_idx + 3] = pixel_data[src_idx + 3];  // A
                }
                else {
                    throw std::runtime_error("Unsupported BMP bit depth");
                }
            }
        }

        return rgba_data;
    }

    /**
     * Load BMP directly into OpenGL texture
     * 
     * @param filename Path to BMP file
     * @param textureID OpenGL texture ID to bind
     */
    static void loadBMPTexture(const std::string& filename, GLuint textureID) {
        int width, height;
        std::vector<uint8_t> imageData = loadBMP(filename, width, height);

        glBindTexture(GL_TEXTURE_2D, textureID);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(
            GL_TEXTURE_2D,      // Target
            0,                  // Mipmap level 
            GL_RGBA,            // Internal format
            width,              // Width
            height,             // Height
            0,                  // Border (must be 0)
            GL_RGBA,            // Format of the pixel data
            GL_UNSIGNED_BYTE,   // Data type of the pixel data
            imageData.data()    // Pointer to image data
        );
    }
};

#endif // BMP_LOADER_H