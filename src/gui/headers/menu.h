#ifndef GUI_HEADERS_MENU_H
#define GUI_HEADERS_MENU_H

#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>

#include "settings_panel.h"

namespace s21 {

class ViewerMenu : public QMenuBar {
  Q_OBJECT
 public:
  SettingsPanel *settings_panel;

  explicit ViewerMenu(QWidget *parent = nullptr);
  ~ViewerMenu() = default;

 signals:
  void fileOpened(QString path);
  void screenshotPressed();
  void screencastPressed();

 private:
  QMenu *SetupMenuFile();

 private slots:
  void handleSettingsActionClicked();
  void handleOpenFileActionClicked();
  void handleScreenshotActionPressed();
  void handleScreencastActionPressed();
};

}  // namespace s21

#endif  // GUI_HEADERS_MENU_H
