#ifndef GUI_HEADERS_SETTINGS_H
#define GUI_HEADERS_SETTINGS_H

#include <QtCore/QFileSystemWatcher>
#include <QtCore/QObject>
#include <QtCore/QString>

#include "../../headers/observer.h"

namespace s21 {

enum class ProjectionType {
  kParallel,
  kCentral,
};

enum class VerticesType {
  kNone,
  kSquare,
  kCircle,
};

enum class EdgesType {
  kSolid,
  kDotted,
};

class ViewerSettings : public Observable {
 public:
  static ViewerSettings* GetInstance();
  VerticesType GetVerticesType() const;
  QString GetVerticesColor() const;
  int GetVerticesSize() const;
  EdgesType GetEdgesType() const;
  QString GetEdgesColor() const;
  int GetEdgesWeight() const;
  QString GetBackgroundColor() const;
  ProjectionType GetProjection() const;

  void SetVerticesType(const int type);
  void SetVerticesColor(const QString color);
  void SetVerticesSize(const int size);
  void SetEdgesType(const int type);
  void SetEdgesColor(const QString color);
  void SetEdgesWeight(const int weight);
  void SetBackgroundColor(const QString color);
  void SetProjection(const int type);

  void LoadSettings();
  void SaveSettings();

 protected:
  ViewerSettings();
  ~ViewerSettings();

 private:
  VerticesType vertices_type_ = VerticesType::kNone;
  QString vertices_color_ = "#000000";
  int vertices_size_ = 5;
  EdgesType edges_type_ = EdgesType::kSolid;
  QString edges_color_ = "#000000";
  int edges_weight_ = 2;
  QString background_color_ = "#ffffff";
  ProjectionType projection_ = ProjectionType::kParallel;
};

class ConfigWatcher : public QObject {
  Q_OBJECT

 public:
  ConfigWatcher(const QString& config_path, QObject* parent = nullptr);

  void GetFile();

 signals:
  void configChanged();

 private slots:
  void onFileChanged(const QString& path);

 private:
  QFileSystemWatcher watcher_;
  QString config_path_;
};

}  // namespace s21

#endif  // GUI_HEADERS_SETTINGS_H
