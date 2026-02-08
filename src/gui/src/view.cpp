#include "../headers/view.h"

#include <QCoreApplication>
#include <QMessageBox>
#include <QMouseEvent>
#include <QtWidgets/QFileDialog>
#include <cmath>

#include "../headers/settings.h"

extern "C" {
#include "../headers/gif.h"
}

namespace s21 {

View::View(QWidget* parent) : QOpenGLWidget(parent) {}

void View::initializeGL() {
  initializeOpenGLFunctions();

  line_program_.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                        ":/shaders/lines_vertex.glsl");
  line_program_.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                        ":/shaders/lines_fragment.glsl");
  line_program_.addShaderFromSourceFile(QOpenGLShader::Geometry,
                                        ":/shaders/lines_geometry.glsl");
  line_program_.link();

  line_vao_.create();
  line_vbo_.create();
  line_vbo_.setUsagePattern(QOpenGLBuffer::DynamicDraw);
  line_vertices_.reserve(2000000);
  line_created_ = false;

  point_program_.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                         ":/shaders/points_vertex.glsl");
  point_program_.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                         ":/shaders/points_fragment.glsl");
  point_program_.link();

  point_vao_.create();
  point_vbo_.create();
  point_vbo_.setUsagePattern(QOpenGLBuffer::DynamicDraw);
  point_vertices_.reserve(1000000);
  point_created_ = false;
}

void View::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void View::paintGL() {
  auto settings = ViewerSettings::GetInstance();

  QColor background = settings->GetBackgroundColor();
  glClearColor(background.redF(), background.greenF(), background.blueF(),
               1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  auto projection_type = settings->GetProjection();

  const auto& figures = scene_.GetFigures();

  auto vertices_type = static_cast<int>(settings->GetVerticesType());

  QMatrix4x4 model;
  model.setToIdentity();

  QMatrix4x4 view;
  view.lookAt(QVector3D(0, 0, 5), QVector3D(0, 0, 0), QVector3D(0, 1, 0));

  QMatrix4x4 projection;
  if (projection_type == ProjectionType::kCentral) {
    projection.perspective(45.0f, width() / float(height()), 0.1f, 100.0f);
  } else {
    float orthoSize = 2.0f;
    projection.ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, 0.1f,
                     100.0f);
  }

  line_vertices_.clear();
  point_vertices_.clear();

  for (const auto& fig : figures) {
    for (const auto& edge : fig.GetEdges()) {
      line_vertices_.emplace_back(edge.GetBegin().GetPosition());
      line_vertices_.emplace_back(edge.GetEnd().GetPosition());
    }
    DrawLines(model, view, projection);

    if (vertices_type) {
      for (const auto& vertex : fig.GetVertices()) {
        point_vertices_.emplace_back(vertex.GetPosition());
      }
      DrawPoints(vertices_type, model, view, projection);
    }
  }
}

void View::DrawLines(QMatrix4x4 model, QMatrix4x4 view, QMatrix4x4 projection) {
  if (line_vertices_.empty()) return;

  auto settings = ViewerSettings::GetInstance();

  QColor edges_color(settings->GetEdgesColor());
  auto edges_type = static_cast<int>(settings->GetEdgesType());
  float edges_weight = settings->GetEdgesWeight();

  line_vao_.bind();

  if (!line_created_) {
    line_vbo_.bind();
    line_vbo_.allocate(line_vertices_.data(),
                       line_vertices_.size() * sizeof(QVector3D));
    line_created_ = true;
  } else {
    line_vbo_.bind();
    line_vbo_.write(0, line_vertices_.data(),
                    line_vertices_.size() * sizeof(QVector3D));
  }

  line_program_.bind();

  line_program_.enableAttributeArray(0);
  line_program_.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));

  line_program_.setUniformValue("u_model", model);
  line_program_.setUniformValue("u_view", view);
  line_program_.setUniformValue("u_projection", projection);
  line_program_.setUniformValue("u_resolution", QVector2D(width(), height()));
  line_program_.setUniformValue("u_dash_length", 10.0f);
  line_program_.setUniformValue("u_gap_length", 10.0f);
  line_program_.setUniformValue("u_dotted", edges_type);
  line_program_.setUniformValue("u_thickness", edges_weight);
  line_program_.setUniformValue(
      "u_color",
      QVector3D(edges_color.redF(), edges_color.greenF(), edges_color.blueF()));

  glDrawArrays(GL_LINES, 0, line_vertices_.size());

  line_program_.disableAttributeArray(0);
  line_program_.release();
  line_vbo_.release();
  line_vao_.release();
}

void View::DrawPoints(int vertices_type, QMatrix4x4 model, QMatrix4x4 view,
                      QMatrix4x4 projection) {
  if (!vertices_type || point_vertices_.empty()) return;

  auto settings = ViewerSettings::GetInstance();

  QColor vertices_color(settings->GetVerticesColor());
  float vertices_size = settings->GetVerticesSize();

  glEnable(GL_PROGRAM_POINT_SIZE);
  point_vao_.bind();

  if (!point_created_) {
    point_vbo_.bind();
    point_vbo_.allocate(point_vertices_.data(),
                        point_vertices_.size() * sizeof(QVector3D));
    point_created_ = true;
  } else {
    point_vbo_.bind();
    point_vbo_.write(0, point_vertices_.data(),
                     point_vertices_.size() * sizeof(QVector3D));
  }

  point_program_.bind();
  point_program_.enableAttributeArray(0);
  point_program_.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));

  point_program_.setUniformValue("u_model", model);
  point_program_.setUniformValue("u_view", view);
  point_program_.setUniformValue("u_projection", projection);
  point_program_.setUniformValue("u_type", vertices_type);
  point_program_.setUniformValue("u_size", vertices_size);
  point_program_.setUniformValue(
      "u_color", QVector3D(vertices_color.redF(), vertices_color.greenF(),
                           vertices_color.blueF()));

  glDrawArrays(GL_POINTS, 0, point_vertices_.size());

  point_program_.disableAttributeArray(0);
  point_program_.release();
  point_vbo_.release();
  point_vao_.release();
}

void View::DrawScene(const Scene& scene) {
  scene_ = scene;
  update();
}

void View::PrepareNewScene() {
  line_created_ = false;
  point_created_ = false;
}

void View::MakeScreenshot() {
  QImage screenshot = grabFramebuffer();

  QString filePath = QFileDialog::getSaveFileName(
      this, "Сохранить скриншот", QDir::homePath() + "/screenshot.jpg",
      "JPEG (*.jpg *.jpeg);;BMP (*.bmp)");

  if (!filePath.isEmpty()) {
    screenshot.save(filePath);
  }
}

void View::MakeScreencast() {
  screencast_file_path_ = QFileDialog::getSaveFileName(
      this, "Сохранить скринкаст", QDir::homePath() + "/screencast.gif",
      "GIF (*.gif)");

  if (screencast_file_path_.isEmpty()) return;

  const int width = 640;
  const int height = 480;
  const int fps = 10;
  const int duration_sec = 5;
  screencast_total_frames_ = fps * duration_sec;
  screencast_frame_ = 0;

  QVariant ptrVar = this->property("gif_writer_ptr");
  if (ptrVar.isValid()) {
    GifWriter* oldGifWriter = static_cast<GifWriter*>(ptrVar.value<void*>());
    GifEnd(oldGifWriter);
    delete oldGifWriter;
    this->setProperty("gif_writer_ptr", QVariant());
  }

  GifWriter* gif_writer = new GifWriter;
  if (!GifBegin(gif_writer, screencast_file_path_.toUtf8().constData(), width,
                height, 10)) {
    delete gif_writer;
    return;
  }
  this->setProperty("gif_writer_ptr",
                    QVariant::fromValue(static_cast<void*>(gif_writer)));

  if (!screencast_timer_) {
    screencast_timer_ = new QTimer(this);
    connect(screencast_timer_, &QTimer::timeout, this,
            &View::recordScreencastFrame);
  }
  screencast_timer_->start(100);
}

void View::recordScreencastFrame() {
  const int width = 640;
  const int height = 480;
  const int delay_cs = 10;

  QVariant ptrVar = this->property("gif_writer_ptr");
  if (!ptrVar.isValid()) return;
  GifWriter* gif_writer = static_cast<GifWriter*>(ptrVar.value<void*>());
  if (!gif_writer) return;

  QImage frame = grabFramebuffer()
                     .scaled(width, height, Qt::IgnoreAspectRatio,
                             Qt::SmoothTransformation)
                     .convertToFormat(QImage::Format_RGBA8888);

  GifWriteFrame(gif_writer, frame.bits(), width, height, delay_cs);

  screencast_frame_++;
  if (screencast_frame_ >= screencast_total_frames_) {
    screencast_timer_->stop();
    GifEnd(gif_writer);
    delete gif_writer;
    this->setProperty("gif_writer_ptr", QVariant());
    screencast_frame_ = 0;
    QMessageBox::information(this, "Screencast",
                             "Screencast saved:\n" + screencast_file_path_);

    unsetCursor();
    setCursor(Qt::ArrowCursor);
  }
}

void View::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    last_mouse_pos_ = event->pos();
    is_dragging_ = true;
    setCursor(Qt::ClosedHandCursor);
  }
  QOpenGLWidget::mousePressEvent(event);
}

void View::mouseMoveEvent(QMouseEvent* event) {
  if (!is_dragging_) return;

  QPoint delta = event->pos() - last_mouse_pos_;
  last_mouse_pos_ = event->pos();

  bool shift = (event->modifiers() & Qt::ShiftModifier);

  if (event->buttons() & Qt::LeftButton) {
    if (shift) {
      emit rotateRequested(delta.x() * 0.005f, delta.y() * 0.005f);
    } else {
      emit moveRequested(delta.x() * 0.003f, -delta.y() * 0.003f);
    }
  }

  QOpenGLWidget::mouseMoveEvent(event);
}

void View::mouseReleaseEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    is_dragging_ = false;
    setCursor(Qt::ArrowCursor);
  }
  QOpenGLWidget::mouseReleaseEvent(event);
}

}  // namespace s21
