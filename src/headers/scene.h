#ifndef HEADERS_SCENE_H
#define HEADERS_SCENE_H

#include <string>
#include <vector>

#include "observer.h"

namespace s21 {

class TransformMatrix;

struct NormalizationParameters {
  float min;
  float max;
  float dx_step;
  float dy_step;
};

class FacadeOperationResult {
 public:
  FacadeOperationResult() = default;
  FacadeOperationResult(std::string error_message, bool is_success);
  ~FacadeOperationResult() = default;

  std::string GetErrorMessage() const;
  bool IsSuccess() const;

 private:
  std::string error_message_;
  bool is_success_ = false;
};

struct _3DPoint {
  float x;
  float y;
  float z;
};

class SceneObject {
 public:
  virtual void Transform(const TransformMatrix &) = 0;
  virtual ~SceneObject();
};

class Vertex : public SceneObject {
 public:
  Vertex() = default;
  Vertex(float x, float y, float z);
  ~Vertex() override = default;

  _3DPoint GetPosition() const;
  void Transform(const TransformMatrix &matrix) override;

 private:
  _3DPoint position_ = {0.0f, 0.0f, 0.0f};
};

class Edge {
 public:
  Edge() = default;
  Edge(Vertex &begin, Vertex &end);
  ~Edge() = default;

  Vertex &GetBegin() const;
  Vertex &GetEnd() const;

 private:
  Vertex &begin_;
  Vertex &end_;
};

class Figure : public SceneObject {
 public:
  Figure() = default;
  Figure(std::vector<Vertex> vertices,
         const std::vector<std::pair<size_t, size_t>> &index_edges);
  ~Figure() override = default;
  Figure(const Figure &other);
  Figure(Figure &&other) noexcept;
  Figure &operator=(const Figure &other);

  const std::vector<Vertex> &GetVertices() const;
  const std::vector<Edge> &GetEdges() const;
  void Transform(const TransformMatrix &matrix) override;

 private:
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
};

class Scene : public Observable {
 public:
  Scene() = default;
  explicit Scene(const std::vector<Figure> &figures);
  ~Scene() = default;
  Scene(Scene &&other) noexcept;
  Scene &operator=(const Scene &other);

  const std::vector<Figure> &GetFigures() const;
  void TransformFigures(const TransformMatrix &matrix);

 private:
  std::vector<Figure> figures_;
};

class SceneDrawerBase {
 public:
  virtual void DrawScene(const Scene &scene) = 0;
  virtual ~SceneDrawerBase() = default;
};

}  // namespace s21

#endif  // HEADERS_SCENE_H
