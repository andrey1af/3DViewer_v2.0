#include "../../headers/settings.h"

#include <QtCore/QFile>
#include <QtCore/QSettings>

namespace s21 {

ViewerSettings::ViewerSettings() = default;
ViewerSettings::~ViewerSettings() = default;

ViewerSettings* ViewerSettings::GetInstance() {
  static ViewerSettings instance;
  static bool loaded = false;
  if (!loaded) {
    instance.LoadSettings();
    loaded = true;
  }
  return &instance;
}

VerticesType ViewerSettings::GetVerticesType() const { return vertices_type_; }

QString ViewerSettings::GetVerticesColor() const { return vertices_color_; }

int ViewerSettings::GetVerticesSize() const { return vertices_size_; }

EdgesType ViewerSettings::GetEdgesType() const { return edges_type_; }

QString ViewerSettings::GetEdgesColor() const { return edges_color_; }

int ViewerSettings::GetEdgesWeight() const { return edges_weight_; }

QString ViewerSettings::GetBackgroundColor() const { return background_color_; }

ProjectionType ViewerSettings::GetProjection() const { return projection_; }

void ViewerSettings::SetVerticesType(const int type) {
  if (type >= static_cast<int>(VerticesType::kNone) &&
      type <= static_cast<int>(VerticesType::kCircle)) {
    vertices_type_ = static_cast<VerticesType>(type);
    NotifyUpdate();
  }
}
void ViewerSettings::SetVerticesColor(const QString color) {
  vertices_color_ = color;
  NotifyUpdate();
}
void ViewerSettings::SetVerticesSize(const int size) {
  vertices_size_ = size;
  NotifyUpdate();
}
void ViewerSettings::SetEdgesType(const int type) {
  if (type >= static_cast<int>(EdgesType::kSolid) &&
      type <= static_cast<int>(EdgesType::kDotted)) {
    edges_type_ = static_cast<EdgesType>(type);
    NotifyUpdate();
  }
}
void ViewerSettings::SetEdgesColor(const QString color) {
  edges_color_ = color;
  NotifyUpdate();
}
void ViewerSettings::SetEdgesWeight(const int weight) {
  edges_weight_ = weight;
  NotifyUpdate();
}
void ViewerSettings::SetBackgroundColor(const QString color) {
  background_color_ = color;
  NotifyUpdate();
}
void ViewerSettings::SetProjection(const int type) {
  if (type >= static_cast<int>(ProjectionType::kParallel) &&
      type <= static_cast<int>(ProjectionType::kCentral)) {
    projection_ = static_cast<ProjectionType>(type);
    NotifyUpdate();
  }
}

void ViewerSettings::LoadSettings() {
  auto settings_file = QSettings("settings.ini", QSettings::IniFormat);
  SetVerticesType(settings_file.value("vertices/type", "0").toInt());
  vertices_color_ = settings_file.value("vertices/color", "#000000").toString();
  vertices_size_ = settings_file.value("vertices/size", "5").toInt();
  SetEdgesType(settings_file.value("edges/type", "0").toInt());
  edges_color_ = settings_file.value("edges/color", "#000000").toString();
  edges_weight_ = settings_file.value("edges/weight", "2").toInt();
  background_color_ =
      settings_file.value("main/background_color", "#ffffff").toString();
  SetProjection(settings_file.value("main/projection", "0").toInt());
  NotifyUpdate();
}

void ViewerSettings::SaveSettings() {
  auto settings_file = QSettings("settings.ini", QSettings::IniFormat);
  settings_file.setValue("vertices/type", static_cast<int>(vertices_type_));
  settings_file.setValue("vertices/color", vertices_color_);
  settings_file.setValue("vertices/size", vertices_size_);
  settings_file.setValue("edges/type", static_cast<int>(edges_type_));
  settings_file.setValue("edges/color", edges_color_);
  settings_file.setValue("edges/weight", edges_weight_);
  settings_file.setValue("main/background_color", background_color_);
  settings_file.setValue("main/projection", static_cast<int>(projection_));

  settings_file.sync();
}

ConfigWatcher::ConfigWatcher(const QString& config_path, QObject* parent)
    : QObject(parent), config_path_(config_path) {
  watcher_.addPath(config_path_);
  connect(&watcher_, &QFileSystemWatcher::fileChanged, this,
          &ConfigWatcher::onFileChanged);
}

void ConfigWatcher::onFileChanged(const QString& path) {
  emit configChanged();

  watcher_.addPath(path);
}

}  // namespace s21
