#ifndef BACKEND_HEADERS_TRANSFORM_MATRIX_H
#define BACKEND_HEADERS_TRANSFORM_MATRIX_H

#include "s21_matrix_oop.h"

namespace s21 {

class _3DPoint;

class TransformMatrix {
 public:
  TransformMatrix() = default;
  explicit TransformMatrix(const S21Matrix);
  ~TransformMatrix() = default;

  _3DPoint TransformPoint(_3DPoint point) const;

 private:
  S21Matrix matrix_;
};

class TransformMatrixBuilder {
 public:
  static TransformMatrix CreateRotationMatrix(float x, float y, float z);
  static TransformMatrix CreateMoveMatrix(float x, float y, float z);
  static TransformMatrix CreateScaleMatrix(float x, float y, float z);

 private:
  static const S21Matrix& IdentityMatrix();
};

}  // namespace s21

#endif  // BACKEND_HEADERS_TRANSFORM_MATRIX_H
