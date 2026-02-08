#ifndef GUI_HEADERS_VIEW_H
#define GUI_HEADERS_VIEW_H

#include <QtCore/QTimer>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QtWidgets/QOpenGLWidget>

#include "../../headers/scene.h"

namespace s21 {

class View : public QOpenGLWidget,
             protected QOpenGLFunctions,
             public SceneDrawerBase {
  Q_OBJECT

 public:
  explicit View(QWidget* parent = nullptr);
  ~View() = default;

  void DrawScene(const Scene& scene) override;
  void PrepareNewScene();
  void MakeScreenshot();
  void MakeScreencast();

 signals:
  void moveRequested(float dx, float dy);
  void rotateRequested(float dx, float dy);

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

 private slots:
  void recordScreencastFrame();

 private:
  Scene scene_;
  QOpenGLShaderProgram line_program_;
  QOpenGLShaderProgram point_program_;
  QOpenGLVertexArrayObject line_vao_;
  QOpenGLVertexArrayObject point_vao_;
  QOpenGLBuffer line_vbo_;
  QOpenGLBuffer point_vbo_;
  std::vector<_3DPoint> line_vertices_;
  std::vector<_3DPoint> point_vertices_;
  bool line_created_;
  bool point_created_;

  // Move mouse
  QPoint last_mouse_pos_;
  bool is_dragging_ = false;

  // Gif
  QTimer* screencast_timer_ = nullptr;
  int screencast_frame_ = 0;
  int screencast_total_frames_ = 0;
  QString screencast_file_path_;

  void DrawLines(QMatrix4x4 model, QMatrix4x4 view, QMatrix4x4 projection);
  void DrawPoints(int vertices_type, QMatrix4x4 model, QMatrix4x4 view,
                  QMatrix4x4 projection);
};

}  // namespace s21

#endif  // GUI_HEADERS_VIEW_H
