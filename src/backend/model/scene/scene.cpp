#include "../../../headers/scene.h"

#include <iostream>
#include <unordered_map>

#include "../../headers/transform_matrix.h"

namespace s21 {

SceneObject::~SceneObject() = default;

Scene::Scene(const std::vector<Figure>& figures) : figures_(figures) {}

const std::vector<Figure>& Scene::GetFigures() const { return figures_; }

void Scene::TransformFigures(const TransformMatrix& matrix) {
  for (auto& fig : figures_) {
    fig.Transform(matrix);
  }
  NotifyUpdate();
}

Scene::Scene(Scene&& other) noexcept : figures_(std::move(other.figures_)) {}

Scene& Scene::operator=(const Scene& other) {
  if (this != &other) {
    figures_ = other.figures_;
  }
  return *this;
}

}  // namespace s21
