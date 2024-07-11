#ifndef DEMC_MANAGER_H
#define DEMC_MANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class DEMC_Manager {
public:
    bool compressFiles(const std::string& demcPath, const std::vector<std::string>& files);
    bool decompressFile(const std::string& demcPath, const std::string& outputDir);

private:
    void writeFileToDemc(std::ofstream& outFile, const std::string& filePath);
    void readFileFromDemc(std::ifstream& inFile, const std::string& outputDir);
};

bool DEMC_Manager::compressFiles(const std::string& demcPath, const std::vector<std::string>& files) {
    std::ofstream outFile(demcPath, std::ios::binary);
    if (!outFile) {
        std::cerr << "Could not open file for writing: " << demcPath << "\n";
        return false;
    }

    for (const auto& file : files) {
        if (!fs::exists(file)) {
            std::cerr << "File does not exist: " << file << "\n";
            outFile.close();
            return false;
        }
        try {
            writeFileToDemc(outFile, file);
        } catch (const std::exception& e) {
            std::cerr << "Error writing file " << file << " to archive: " << e.what() << "\n";
            outFile.close();
            return false;
        }
    }

    outFile.close();
    return true;
}

void DEMC_Manager::writeFileToDemc(std::ofstream& outFile, const std::string& filePath) {
    std::ifstream inFile(filePath, std::ios::binary | std::ios::ate);
    if (!inFile) {
        throw std::runtime_error("Could not open file for reading: " + filePath);
    }

    std::string fileName = fs::path(filePath).filename().string();
    std::streamsize fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);

    // Write the filename length and filename
    uint32_t fileNameLength = fileName.size();
    outFile.write(reinterpret_cast<const char*>(&fileNameLength), sizeof(fileNameLength));
    outFile.write(fileName.c_str(), fileNameLength);

    // Write the file size
    outFile.write(reinterpret_cast<const char*>(&fileSize), sizeof(fileSize));

    // Write the file content
    char buffer[4096];
    while (inFile.read(buffer, sizeof(buffer))) {
        outFile.write(buffer, inFile.gcount());
    }
    outFile.write(buffer, inFile.gcount());

    inFile.close();
}

bool DEMC_Manager::decompressFile(const std::string& demcPath, const std::string& outputDir) {
    std::ifstream inFile(demcPath, std::ios::binary);
    if (!inFile) {
        std::cerr << "Could not open file for reading: " << demcPath << "\n";
        return false;
    }

    while (inFile.peek() != std::ifstream::traits_type::eof()) {
        try {
            readFileFromDemc(inFile, outputDir);
        } catch (const std::exception& e) {
            std::cerr << "Error reading file from archive: " << e.what() << "\n";
            inFile.close();
            return false;
        }
    }

    inFile.close();
    return true;
}

void DEMC_Manager::readFileFromDemc(std::ifstream& inFile, const std::string& outputDir) {
    // Read the filename length and filename
    uint32_t fileNameLength;
    inFile.read(reinterpret_cast<char*>(&fileNameLength), sizeof(fileNameLength));
    
    std::vector<char> fileNameBuffer(fileNameLength);
    inFile.read(fileNameBuffer.data(), fileNameLength);
    std::string fileName(fileNameBuffer.data(), fileNameLength);

    // Read the file size
    std::streamsize fileSize;
    inFile.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));

    // Prepare output file path
    std::string outputPath = fs::path(outputDir) / fileName;
    fs::create_directories(fs::path(outputPath).parent_path());

    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile) {
        throw std::runtime_error("Could not open file for writing: " + outputPath);
    }

    // Read the file content and write to the output file
    char buffer[4096];
    std::streamsize remaining = fileSize;
    while (remaining > 0) {
        std::streamsize toRead = std::min(remaining, static_cast<std::streamsize>(sizeof(buffer)));
        inFile.read(buffer, toRead);
        outFile.write(buffer, toRead);
        remaining -= toRead;
    }

    outFile.close();
}

#endif // DEMC_MANAGER_H