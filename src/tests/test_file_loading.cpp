#include <gtest/gtest.h>

#include <cmath>

#include "../backend/headers/file_reader.h"
#include "../headers/controller.h"
#include "../headers/scene.h"

namespace s21 {

bool PointEqual(const _3DPoint& a, const _3DPoint& b, float eps = 1e-5f) {
  return std::fabs(a.x - b.x) < eps && std::fabs(a.y - b.y) < eps &&
         std::fabs(a.z - b.z) < eps;
}

TEST(FileReaderTest, SimpleObjLoad) {
  const char* filename = "tests/files/simple.obj";

  FileReader reader;
  NormalizationParameters params;
  params.min = 0.0f;
  params.max = 1.0f;
  params.dx_step = 1.0f;
  params.dy_step = 1.0f;

  Scene scene = reader.ReadScene(filename, params);

  ASSERT_EQ(scene.GetFigures().size(), 1);
  const auto& verts = scene.GetFigures()[0].GetVertices();
  ASSERT_EQ(verts.size(), 3);

  EXPECT_TRUE(PointEqual(verts[0].GetPosition(), {0.0f, 0.0f, 0.0f}));
  EXPECT_TRUE(PointEqual(verts[1].GetPosition(), {1.0f, 0.0f, 0.0f}));
  EXPECT_TRUE(PointEqual(verts[2].GetPosition(), {0.0f, 1.0f, 0.0f}));

  const auto& edges = scene.GetFigures()[0].GetEdges();
  ASSERT_EQ(edges.size(), 3);
}

TEST(FileReaderTest, SimpleNegativeIndexObjLoad) {
  const char* filename = "tests/files/with_negative_index.obj";

  FileReader reader;
  NormalizationParameters params;
  params.min = 0.0f;
  params.max = 1.0f;
  params.dx_step = 1.0f;
  params.dy_step = 1.0f;

  Scene scene = reader.ReadScene(filename, params);

  ASSERT_EQ(scene.GetFigures().size(), 1);
  const auto& verts = scene.GetFigures()[0].GetVertices();
  ASSERT_EQ(verts.size(), 3);

  EXPECT_TRUE(PointEqual(verts[0].GetPosition(), {0.0f, 0.0f, 0.0f}));
  EXPECT_TRUE(PointEqual(verts[1].GetPosition(), {1.0f, 0.0f, 0.0f}));
  EXPECT_TRUE(PointEqual(verts[2].GetPosition(), {0.0f, 1.0f, 0.0f}));

  const auto& edges = scene.GetFigures()[0].GetEdges();
  ASSERT_EQ(edges.size(), 3);
}

TEST(FileReaderTest, QuadWithNormalsAndFaces) {
  const char* filename = "tests/files/with_normals.obj";

  FileReader reader;
  NormalizationParameters params{0.0f, 1.0f, 1.0f, 1.0f};
  Scene scene = reader.ReadScene(filename, params);

  ASSERT_EQ(scene.GetFigures().size(), 1);
  const auto& verts = scene.GetFigures()[0].GetVertices();
  ASSERT_EQ(verts.size(), 4);

  EXPECT_TRUE(PointEqual(verts[0].GetPosition(), {0.0f, 0.0f, 0.0f}));
  EXPECT_TRUE(PointEqual(verts[1].GetPosition(), {1.0f, 0.0f, 0.0f}));
  EXPECT_TRUE(PointEqual(verts[2].GetPosition(), {1.0f, 1.0f, 0.0f}));
  EXPECT_TRUE(PointEqual(verts[3].GetPosition(), {0.0f, 1.0f, 0.0f}));

  const auto& edges = scene.GetFigures()[0].GetEdges();
  ASSERT_EQ(edges.size(), 4);
}

TEST(FileReaderTest, WrongExtension) {
  const char* filename = "nothing.obj";

  FileReader reader;
  NormalizationParameters params{0.0f, 1.0f, 1.0f, 1.0f};
  EXPECT_THROW({ reader.ReadScene(filename, params); }, const char*);
}

}  // namespace s21
