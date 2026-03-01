#include "../include/FileReader.hpp"

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <utility>

FileReader::FileReader(std::string path) : path_(std::move(path)) {}

std::string FileReader::read_all() const {
  FILE* file = std::fopen(path_.c_str(), "rb");
  if (file == nullptr)
    throw std::runtime_error("Failed to open file: " + path_ + " (" +
                             std::strerror(errno) + ")");

  if (std::fseek(file, 0, SEEK_END) != 0) {
    std::fclose(file);
    throw std::runtime_error("Failed to seek in file: " + path_ + " (" +
                             std::strerror(errno) + ")");
  }

  const long size = std::ftell(file);
  if (size < 0) {
    std::fclose(file);
    throw std::runtime_error("Failed to determine size of file: " + path_ +
                             " (" + std::strerror(errno) + ")");
  }

  std::rewind(file);

  std::string buffer(static_cast<std::size_t>(size), '\0');

  const std::size_t read =
      std::fread(buffer.data(), 1, static_cast<std::size_t>(size), file);
  std::fclose(file);

  if (read != static_cast<std::size_t>(size))
    throw std::runtime_error("Failed to read entire file: " + path_);

  return buffer;
}