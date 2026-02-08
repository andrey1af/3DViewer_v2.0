#include "../headers/file_reader.h"

#include <fstream>
#include <list>
#include <sstream>
#include <vector>

#include "../../headers/scene.h"

namespace s21 {

BaseFileReader::~BaseFileReader() = default;

size_t FileReader::ConvertIndex(int index, size_t max_index) {
  if (index < 0) {
    index += max_index;
  } else {
    index -= 1;
  }
  return index;
}

void FileReader::Normalize(float *x, float *y, float *z,
                           NormalizationParameters params) {
  *x = (*x - params.min) / (params.max - params.min);
  *y = (*y - params.min) / (params.max - params.min);
  *z = (*z - params.min) / (params.max - params.min);

  *x *= params.dx_step;
  *y *= params.dy_step;
  *z *= std::min(params.dx_step, params.dy_step);
}

Scene FileReader::ReadScene(std::string path, NormalizationParameters params) {
  std::ifstream file(path);

  if (!file.is_open()) {
    throw "Cant open file";
  }

  std::vector<Vertex> vertices;
  std::vector<std::vector<int>> face_indices;
  std::vector<std::pair<size_t, size_t>> index_edges;
  std::vector<Figure> figures;

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix;
    if (prefix == "v") {
      float x, y, z;
      iss >> x >> y >> z;
      Normalize(&x, &y, &z, params);
      vertices.emplace_back(x, y, z);
    } else if (prefix == "f") {
      std::vector<int> indices;
      std::string token;
      while (iss >> token) {
        size_t slash_pos = token.find('/');
        if (slash_pos != std::string::npos) {
          token.resize(slash_pos);
        }
        indices.push_back(std::stoi(token));
      }
      face_indices.emplace_back(std::move(indices));
    }
  }
  file.close();

  for (const auto &indices : face_indices) {
    if (indices.size() < 2) continue;

    for (size_t i = 0; i < indices.size(); ++i) {
      size_t idx1 = ConvertIndex(indices[i], vertices.size());
      size_t idx2 =
          ConvertIndex(indices[(i + 1) % indices.size()], vertices.size());
      index_edges.emplace_back(idx1, idx2);
    }
  }

  figures.emplace_back(std::move(vertices), index_edges);

  return Scene(figures);
}

}  // namespace s21
