#include "../../../headers/scene.h"
#include "../../headers/transform_matrix.h"

namespace s21 {

Vertex::Vertex(float x, float y, float z) : position_({x, y, z}) {}

_3DPoint Vertex::GetPosition() const { return position_; }

void Vertex::Transform(const TransformMatrix& matrix) {
  position_ = matrix.TransformPoint(position_);
}

}  // namespace s21
