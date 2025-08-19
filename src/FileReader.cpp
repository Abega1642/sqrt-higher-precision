#include "../include/FileReader.hpp"

#include <cstdio>
#include <stdexcept>
#include <utility>

FileReader::FileReader(std::string filename) : filename(std::move(filename)) {}

std::string FileReader::read_all() const {
  FILE* file = std::fopen(filename.c_str(), "rb");
  if (!file) {
    throw std::runtime_error("Failed to open file: " + filename);
  }

  std::fseek(file, 0, SEEK_END);
  const long size = std::ftell(file);
  std::rewind(file);

  if (size < 0) {
    std::fclose(file);
    throw std::runtime_error("Failed to determine file size");
  }

  std::string buffer(size, '\0');
  const size_t read = std::fread(&buffer[0], 1, size, file);
  std::fclose(file);

  if (read != static_cast<size_t>(size)) {
    throw std::runtime_error("Failed to read entire file: " + filename);
  }

  return buffer;
}
