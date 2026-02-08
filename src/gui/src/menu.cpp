#include "../headers/menu.h"

#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidgetAction>

#include "../headers/settings_panel.h"

namespace s21 {

ViewerMenu::ViewerMenu(QWidget *parent) : QMenuBar(parent) {
  auto menu_file = SetupMenuFile();
  settings_panel = new SettingsPanel(this);
  settings_panel->setWindowTitle("Settings");
  auto action_settings = new QAction(this);

  this->addMenu(menu_file);
  this->addAction(action_settings);

  connect(action_settings, &QAction::triggered, this,
          &ViewerMenu::handleSettingsActionClicked);

  action_settings->setText("Settings");

  QFile style_file(":/style.css");
}

QMenu *ViewerMenu::SetupMenuFile() {
  auto menu_file = new QMenu(this);
  menu_file->setTitle("File");
  menu_file->setObjectName("fileMenu");

  auto action_open_file = new QAction(menu_file);
  action_open_file->setText("Open");
  action_open_file->setToolTip("Open file");
  connect(action_open_file, &QAction::triggered, this,
          &ViewerMenu::handleOpenFileActionClicked);

  auto action_screenshot = new QAction(menu_file);
  action_screenshot->setText("Screenshot");
  action_screenshot->setToolTip("Make Screenshot");
  connect(action_screenshot, &QAction::triggered, this,
          &ViewerMenu::handleScreenshotActionPressed);

  auto action_screencast = new QAction(menu_file);
  action_screencast->setText("Screencast");
  action_screencast->setToolTip("Make Screencast");
  connect(action_screencast, &QAction::triggered, this,
          &ViewerMenu::handleScreencastActionPressed);

  menu_file->addAction(action_open_file);
  menu_file->addAction(action_screenshot);
  menu_file->addAction(action_screencast);

  return menu_file;
}

void ViewerMenu::handleOpenFileActionClicked() {
  QString file_path = QFileDialog::getOpenFileName(
      this, "Choose file", QDir::homePath(), "3D files (*.obj)");
  emit fileOpened(file_path);
}

void ViewerMenu::handleSettingsActionClicked() { settings_panel->exec(); }
void ViewerMenu::handleScreenshotActionPressed() { emit screenshotPressed(); }
void ViewerMenu::handleScreencastActionPressed() { emit screencastPressed(); }

}  // namespace s21
