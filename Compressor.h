#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <vector>
#include <string>

class Compressor {
public:
    // Función para comprimir los datos usando Zstandard
    static std::vector<char> compress(const std::string& data);

    // Función para descomprimir los datos usando Zstandard
    static std::string decompress(const std::vector<char>& compressedData);
};

#endif
