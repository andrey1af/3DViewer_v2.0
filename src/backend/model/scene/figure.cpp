#include "../../../headers/scene.h"

namespace s21 {

Figure::Figure(std::vector<Vertex> vertices,
               const std::vector<std::pair<size_t, size_t>>& index_edges)
    : vertices_(std::move(vertices)) {
  edges_.reserve(index_edges.size());

  for (const auto& [begin_idx, end_idx] : index_edges) {
    if (begin_idx >= vertices_.size() || end_idx >= vertices_.size()) {
      throw "Vertex index is out of range";
    }

    edges_.emplace_back(vertices_[begin_idx], vertices_[end_idx]);
  }
}

const std::vector<Vertex>& Figure::GetVertices() const { return vertices_; }

const std::vector<Edge>& Figure::GetEdges() const { return edges_; }

void Figure::Transform(const TransformMatrix& matrix) {
  for (auto& vertex : vertices_) {
    vertex.Transform(matrix);
  }
}

Figure::Figure(const Figure& other) : vertices_(other.vertices_) {
  edges_.clear();
  for (const auto& edge : other.edges_) {
    const Vertex* begin_ptr = &edge.GetBegin();
    const Vertex* end_ptr = &edge.GetEnd();

    auto begin_idx = std::distance(other.vertices_.data(), begin_ptr);
    auto end_idx = std::distance(other.vertices_.data(), end_ptr);

    edges_.emplace_back(vertices_.at(begin_idx), vertices_.at(end_idx));
  }
}

Figure::Figure(Figure&& other) noexcept
    : vertices_(std::move(other.vertices_)), edges_(std::move(other.edges_)) {}

Figure& Figure::operator=(const Figure& other) {
  if (this == &other) return *this;

  vertices_ = other.vertices_;

  edges_.clear();
  for (const auto& edge : other.edges_) {
    const Vertex* begin_ptr = &edge.GetBegin();
    const Vertex* end_ptr = &edge.GetEnd();

    auto begin_idx = std::distance(other.vertices_.data(), begin_ptr);
    auto end_idx = std::distance(other.vertices_.data(), end_ptr);

    edges_.emplace_back(vertices_.at(begin_idx), vertices_.at(end_idx));
  }

  return *this;
}

}  // namespace s21
