#include "../headers/transform_matrix.h"

#include <cmath>

#include "../../headers/scene.h"

namespace s21 {

TransformMatrix::TransformMatrix(const S21Matrix m) {
  if (m.GetCols() != 4 || m.GetRows() != 4) {
    throw std::invalid_argument("Matrix should be 4x4");
  }
  matrix_ = m;
}

_3DPoint TransformMatrix::TransformPoint(_3DPoint point) const {
  float x = matrix_(0, 0) * point.x + matrix_(0, 1) * point.y +
            matrix_(0, 2) * point.z + matrix_(0, 3);
  float y = matrix_(1, 0) * point.x + matrix_(1, 1) * point.y +
            matrix_(1, 2) * point.z + matrix_(1, 3);
  float z = matrix_(2, 0) * point.x + matrix_(2, 1) * point.y +
            matrix_(2, 2) * point.z + matrix_(2, 3);
  float w = matrix_(3, 0) * point.x + matrix_(3, 1) * point.y +
            matrix_(3, 2) * point.z + matrix_(3, 3);

  if (w != 0.0f && w != 1.0f) {
    x /= w;
    y /= w;
    z /= w;
  }

  return _3DPoint{x, y, z};
}

const S21Matrix& TransformMatrixBuilder::IdentityMatrix() {
  static const S21Matrix identity = [] {
    S21Matrix result(4, 4);
    for (int i = 0; i < 4; ++i) result(i, i) = 1.0f;
    return result;
  }();
  return identity;
}

TransformMatrix TransformMatrixBuilder::CreateRotationMatrix(float x, float y,
                                                             float z) {
  S21Matrix rotate_x = IdentityMatrix();
  S21Matrix rotate_y = IdentityMatrix();
  S21Matrix rotate_z = IdentityMatrix();

  float cx = std::cos(x);
  float sx = std::sin(x);
  rotate_x(1, 1) = cx;
  rotate_x(1, 2) = -sx;
  rotate_x(2, 1) = sx;
  rotate_x(2, 2) = cx;

  float cy = std::cos(y);
  float sy = std::sin(y);
  rotate_y(0, 0) = cy;
  rotate_y(0, 2) = sy;
  rotate_y(2, 0) = -sy;
  rotate_y(2, 2) = cy;

  float cz = std::cos(z);
  float sz = std::sin(z);
  rotate_z(0, 0) = cz;
  rotate_z(0, 1) = -sz;
  rotate_z(1, 0) = sz;
  rotate_z(1, 1) = cz;

  return TransformMatrix(rotate_z * rotate_y * rotate_x);
}

TransformMatrix TransformMatrixBuilder::CreateMoveMatrix(float x, float y,
                                                         float z) {
  S21Matrix result = IdentityMatrix();
  result(0, 3) = x;
  result(1, 3) = y;
  result(2, 3) = z;
  return TransformMatrix(result);
}
TransformMatrix TransformMatrixBuilder::CreateScaleMatrix(float x, float y,
                                                          float z) {
  S21Matrix result = IdentityMatrix();
  result(0, 0) = x;
  result(1, 1) = y;
  result(2, 2) = z;
  return TransformMatrix(result);
}

}  // namespace s21
