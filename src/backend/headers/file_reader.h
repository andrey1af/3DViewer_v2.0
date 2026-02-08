#ifndef BACKEND_HEADERS_FILE_READER_H
#define BACKEND_HEADERS_FILE_READER_H

#include <string>

#include "../../headers/controller.h"

namespace s21 {

class Scene;

class BaseFileReader {
 public:
  virtual Scene ReadScene(std::string path, NormalizationParameters params) = 0;
  virtual ~BaseFileReader();
};

class FileReader : public BaseFileReader {
 public:
  FileReader() = default;
  ~FileReader() override = default;
  Scene ReadScene(std::string path, NormalizationParameters params) override;

 private:
  size_t ConvertIndex(int index, size_t max_index);
  void Normalize(float *x, float *y, float *z, NormalizationParameters params);
};

}  // namespace s21

#endif  // BACKEND_HEADERS_FILE_READER_H
