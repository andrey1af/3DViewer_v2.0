#include <gtest/gtest.h>

#include <cmath>
#include <vector>

#include "../backend/headers/transform_matrix.h"
#include "../headers/scene.h"

namespace s21 {

bool PointEq(const _3DPoint& a, const _3DPoint& b, float eps = 1e-5f) {
  return std::fabs(a.x - b.x) < eps && std::fabs(a.y - b.y) < eps &&
         std::fabs(a.z - b.z) < eps;
}

TEST(SceneAffineTest, ScaleFigure) {
  Figure fig({Vertex(1, 2, 3)}, {});
  Scene scene({fig});
  auto scale = TransformMatrixBuilder::CreateScaleMatrix(2.0f, 2.0f, 2.0f);
  scene.TransformFigures(scale);

  const auto& verts = scene.GetFigures()[0].GetVertices();
  ASSERT_EQ(verts.size(), 1);
  _3DPoint expected = {2.0f, 4.0f, 6.0f};
  EXPECT_TRUE(PointEq(verts[0].GetPosition(), expected));
}

TEST(SceneAffineTest, TranslateFigure) {
  Figure fig({Vertex(1, 2, 3)}, {});
  Scene scene({fig});
  auto move = TransformMatrixBuilder::CreateMoveMatrix(1.0f, -2.0f, 3.0f);
  scene.TransformFigures(move);

  const auto& verts = scene.GetFigures()[0].GetVertices();
  ASSERT_EQ(verts.size(), 1);
  _3DPoint expected = {2.0f, 0.0f, 6.0f};
  EXPECT_TRUE(PointEq(verts[0].GetPosition(), expected));
}

TEST(SceneAffineTest, RotateZFigure90deg) {
  Figure fig({Vertex(1, 0, 0)}, {});
  Scene scene({fig});
  float angle = static_cast<float>(M_PI_2);
  auto rot = TransformMatrixBuilder::CreateRotationMatrix(0.0f, 0.0f, angle);
  scene.TransformFigures(rot);

  const auto& verts = scene.GetFigures()[0].GetVertices();
  ASSERT_EQ(verts.size(), 1);
  _3DPoint expected = {0.0f, 1.0f, 0.0f};
  EXPECT_TRUE(PointEq(verts[0].GetPosition(), expected));
}

TEST(SceneAffineTest, RotateXFigure180deg) {
  Figure fig({Vertex(0, 1, 0)}, {});
  Scene scene({fig});
  float angle = static_cast<float>(M_PI);
  auto rot = TransformMatrixBuilder::CreateRotationMatrix(angle, 0.0f, 0.0f);
  scene.TransformFigures(rot);

  const auto& verts = scene.GetFigures()[0].GetVertices();
  ASSERT_EQ(verts.size(), 1);
  _3DPoint expected = {0.0f, -1.0f, 0.0f};
  EXPECT_TRUE(PointEq(verts[0].GetPosition(), expected));
}

TEST(SceneAffineTest, RotateYFigure180deg) {
  Figure fig({Vertex(1, 0, 0)}, {});
  Scene scene({fig});
  float angle = static_cast<float>(M_PI);
  auto rot = TransformMatrixBuilder::CreateRotationMatrix(0.0f, angle, 0.0f);
  scene.TransformFigures(rot);

  const auto& verts = scene.GetFigures()[0].GetVertices();
  ASSERT_EQ(verts.size(), 1);
  _3DPoint expected = {-1.0f, 0.0f, 0.0f};
  EXPECT_TRUE(PointEq(verts[0].GetPosition(), expected));
}

}  // namespace s21
