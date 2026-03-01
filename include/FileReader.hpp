#pragma once
#include <string>

class FileReader {
 public:
  explicit FileReader(std::string path);
  [[nodiscard]] std::string read_all() const;

 private:
  std::string path_;
};