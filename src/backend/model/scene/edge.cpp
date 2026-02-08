#include "../../../headers/scene.h"

namespace s21 {

Edge::Edge(Vertex& begin, Vertex& end) : begin_(begin), end_(end) {}
Vertex& Edge::GetBegin() const { return begin_; }
Vertex& Edge::GetEnd() const { return end_; }

}  // namespace s21
