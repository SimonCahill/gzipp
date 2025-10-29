/**
 * @file gzip_compress.hpp
 * @author Simon Cahill (s.cahill@procyon-systems.de)
 * @brief Contains the implementation of the Compressor class.
 * @version 0.1
 * @date 2025-04-25
 * 
 * @copyright Copyright (c) 2025 Procyon Systems All Rights Reserved.
 * @license BSD-2-Clause
 */

#ifndef GZIPP_INCLUDE_GZIPP_GZIP_COMPRESS_HPP
#define GZIPP_INCLUDE_GZIPP_GZIP_COMPRESS_HPP

/////////////////////////////////
//       SYSTEM INCLUDES       //
/////////////////////////////////
// stl
#include <stdexcept>
#include <string>

// libc
#include <stdint.h>

// libz
#include <zlib.h>

/////////////////////////////////
//       LOCAL  INCLUDES       //
/////////////////////////////////
#include "gzipp/gzipp_config.hpp"

namespace gzipp {

    template<typename Str>
    concept Stringable = requires(Str s) { { s.data() + s.size() } -> std::convertible_to<const char*>; };

    template<typename Bytes>
    concept Binary = requires(Bytes b) { { b.data() + b.size() } -> std::convertible_to<const uint8_t*>; };

    template<typename T>
    concept Gzipable = Stringable<T> || Binary<T>;

    using std::string;
    using std::string_view;

    /**
     * @brief Compresses a string or binary data using the Gzip format.
     * 
     * @tparam T The type of the input data. Must be either a string or binary data.
     * @param input The input data to compress.
     * @param level The compression level (0-9). Default is 6.
     * @return The compressed data as a string.
     */
    template<Gzipable T>
    inline string compress(const T& input, int level = Z_DEFAULT_COMPRESSION, size_t maxSize = DEFAULT_MAX_COMPRESSABLE_SIZE) {
        // Check if the input is empty
        if (input.size() == 0) {
            return {};
        }

        if (input.size() > maxSize) {
            throw std::runtime_error("Input size exceeds maximum compressable size");
        }

        z_stream deflateStream{};
        deflateStream.zalloc = Z_NULL;
        deflateStream.zfree = Z_NULL;
        deflateStream.opaque = Z_NULL;
        deflateStream.avail_in = 0;
        deflateStream.next_in = Z_NULL;

        constexpr const size_t windowBits = 15 + 16; // 15 for window size, 16 for gzip header
        constexpr const size_t memLevel = 8; // Default memory level
        
        if (deflateInit2(&deflateStream, level, Z_DEFLATED, windowBits, memLevel, Z_DEFAULT_STRATEGY) != Z_OK) {
            throw std::runtime_error("Failed to initialize deflate stream");
        }

        deflateStream.avail_in = static_cast<size_t>(input.size());
        deflateStream.next_in = std::bit_cast<uint8_t*>(const_cast<char*>(input.data()));

        size_t compressedSize{0};

        string compressedData{};
        do {
            // Resize the output buffer if necessary
            const size_t bufferGrowthAmount = input.size() / 2 + 1024;

            if (compressedData.size() < compressedSize + bufferGrowthAmount) {
                compressedData.resize(compressedSize + bufferGrowthAmount);
            }

            // We're limited by zlib to an unsigned 32-bit integer
            deflateStream.avail_out = static_cast<uint32_t>(bufferGrowthAmount);
            deflateStream.next_out = std::bit_cast<uint8_t*>(compressedData.data() + compressedSize);

            static_cast<void>(deflate(&deflateStream, Z_FINISH)); // deflate cannot fail; cast to void to prevent compiler errors.
            compressedSize += bufferGrowthAmount - deflateStream.avail_out;
        } while (deflateStream.avail_out == 0);

        // Clean up
        if (deflateEnd(&deflateStream) != Z_OK) {
            throw std::runtime_error("Failed to finalize deflate stream");
        }

        compressedData.resize(compressedSize);

        return compressedData;
    }

    /**
     * @brief Compresses a string or binary data using the Gzip format.
     * 
     * @param input The input data to compress.
     * @param size The size of the input data.
     * @param level The compression level (0-9). Default is 6.
     * @return The compressed data as a string.
     */
    inline string compress(const char* input, size_t size, int level = Z_DEFAULT_COMPRESSION) { return compress(string_view(input, size), level); }

    /**
     * @brief Compresses a string or binary data using the Gzip format.
     * 
     * @param input The input data to compress.
     * @param size The size of the input data.
     * @param level The compression level (0-9). Default is 6.
     * @return The compressed data as a string.
     */
    inline string compress(const uint8_t* input, size_t size, int level = Z_DEFAULT_COMPRESSION) { return compress(std::bit_cast<const char*>(input), size, level); }

    /**
     * @brief Decompresses a string or binary data using the Gzip format.
     * 
     * @tparam T The type of the input data. Must be either a string or binary data.
     * @param input The input data to decompress.
     * @return The decompressed data as a string.
     */
    template<Gzipable T>
    inline string decompress(const T& input) {
        // Check if the input is empty
        if (input.size() == 0) {
            return string();
        }

        // Create a buffer for the decompressed data
        uLongf decompressedSize = input.size() * 2; // Initial guess for size
        string decompressedData{};
        decompressedData.reserve(decompressedSize);
        decompressedData.resize(decompressedSize);

        z_stream inflateStream{};
        inflateInit2(&inflateStream, 16 + MAX_WBITS); // 16 + MAX_WBITS to enable gzip decoding

        inflateStream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));

        if (input.size() > DEFAULT_MAX_COMPRESSABLE_SIZE || input.size() * 2 > DEFAULT_MAX_COMPRESSABLE_SIZE) {
            throw std::runtime_error("Input size exceeds maximum decompressable size");
        }

        inflateStream.avail_in = static_cast<uInt>(input.size());

        size_t totalDecompressedSize = 0;

        do {
            // Resize the output buffer if necessary
            if (decompressedData.size() < totalDecompressedSize + decompressedSize) {
                decompressedData.resize(totalDecompressedSize + decompressedSize);
            }

            inflateStream.avail_out = static_cast<uInt>(decompressedSize);
            inflateStream.next_out = reinterpret_cast<Bytef*>(decompressedData.data() + totalDecompressedSize);

            int ret = inflate(&inflateStream, Z_NO_FLUSH);
            if (ret == Z_STREAM_END) {
                totalDecompressedSize += (decompressedSize - inflateStream.avail_out);
                break;
            }
            else if (ret != Z_OK) {
                inflateEnd(&inflateStream);
                throw std::runtime_error("Failed to decompress data");
            }

            totalDecompressedSize += (decompressedSize - inflateStream.avail_out);
        } while (inflateStream.avail_out == 0);

        // Clean up
        inflateEnd(&inflateStream);

        // Resize the decompressed data to the actual size
        decompressedData.resize(totalDecompressedSize);
        decompressedData.shrink_to_fit();
        return decompressedData;
    }

    /**
     * @brief Decompresses a string or binary data using the Gzip format.
     * 
     * @param input The input data to decompress.
     * @param size The size of the input data.
     * @return The compressed data as a string.
     */
    inline string decompress(const char* input, size_t size) { return decompress(string_view(input, size)); }

    /**
     * @brief Decompresses a string or binary data using the Gzip format.
     * 
     * @param input The input data to decompress.
     * @param size The size of the input data.
     * @return The compressed data as a string.
     */
    inline string decompress(const uint8_t* input, size_t size) { return decompress(std::bit_cast<const char*>(input), size); }

    /**
     * @brief Determines if the input data is compressed.
     * 
     * @tparam T The type of the input data. Must be either a string or binary data.
     * @param input The input data to check.
     * @return True if the data is compressed, false otherwise.
     */
    template<Gzipable T>
    inline bool isCompressed(const T& input) {
        // Check if the input is empty
        if (input.size() == 0) {
            return false;
        }

        // Check the Gzip magic number
        return (
            input.size() >= 2 && (
                // Zlib Magic Number
                (
                    input[0] == '\x78' && (input[1] == 0x01 || input[1] == 0x9c || input[1] == 0xda || input[1] == 0x5e)
                ) ||

                // Gzip Magic Number
                (
                    input[0] == '\x1f' && input[1] == '\x8b'
                )
            )
        );
    }

    /**
     * @brief Determines if the input data is compressed.
     * 
     * @param input The input data to check.
     * @param size The size of the input data.
     * @return True if the data is compressed, false otherwise.
     */
    inline bool isCompressed(const char* input, size_t size) { return isCompressed(string_view(input, size)); }

    /**
     * @brief Determines if the input data is compressed.
     * 
     * @param input The input data to check.
     * @param size The size of the input data.
     * @return True if the data is compressed, false otherwise.
     */
    inline bool isCompressed(const uint8_t* input, size_t size) { return isCompressed(std::bit_cast<const char*>(input), size); }

}

#endif // GZIPP_INCLUDE_GZIPP_GZIP_COMPRESS_HPP