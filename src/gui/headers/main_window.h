#ifndef GUI_HEADERS_MAIN_WINDOW_H
#define GUI_HEADERS_MAIN_WINDOW_H

#include <QtCore/QSettings>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

#include "../../headers/controller.h"
#include "../../headers/observer.h"
#include "menu.h"
#include "settings.h"
#include "view.h"

namespace s21 {

class MainWindow : public QMainWindow, public Observer {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() = default;

  void Update() override;

  View *view;
  ViewerMenu *menubar;
  QStatusBar *statusbar;

 protected:
  void keyPressEvent(QKeyEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  void closeEvent(QCloseEvent *event) override;

 private:
  Controller *controller_;

  void UserInput(const UserKeys &key);
};

}  // namespace s21

#endif  // GUI_HEADERS_MAIN_WINDOW_H
