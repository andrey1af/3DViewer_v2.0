#include "../headers/main_window.h"

#include <QtCore/QVariant>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <iostream>

#include "../headers/menu.h"
#include "../headers/model_menu.h"
#include "../headers/settings.h"

namespace s21 {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  resize(800, 600);
  setFocusPolicy(Qt::StrongFocus);
  auto central_widget = new QWidget(this);

  auto horizontal_layout = new QHBoxLayout(central_widget);

  view = new View(this);

  auto model_menu = new ModelMenu(central_widget);
  model_menu->setMaximumWidth(250);

  connect(
      model_menu, &ModelMenu::moveChanged, this,
      [this](float x, float y, float z) { controller_->MoveScene(x, y, z); });
  connect(
      model_menu, &ModelMenu::rotateChanged, this,
      [this](float x, float y, float z) { controller_->RotateScene(x, y, z); });
  connect(
      model_menu, &ModelMenu::scaleChanged, this,
      [this](float x, float y, float z) { controller_->ScaleScene(x, y, z); });

  // Move mouse
  connect(view, &View::moveRequested, this,
          [this](float dx, float dy) { controller_->MoveScene(dx, dy, 0); });
  connect(view, &View::rotateRequested, this,
          [this](float dx, float dy) { controller_->RotateScene(dy, dx, 0); });

  horizontal_layout->addWidget(view);
  horizontal_layout->addWidget(model_menu);

  setCentralWidget(central_widget);
  menubar = new ViewerMenu(this);
  menubar->setGeometry(QRect(0, 0, 800, 19));

  setMenuBar(menubar);

  statusbar = new QStatusBar(this);

  auto label = new QLabel();
  label->setProperty("class", "status");

  statusbar->addPermanentWidget(label);

  setStatusBar(statusbar);
  setWindowTitle(QCoreApplication::translate("3DViewer", "3DViewer", nullptr));

  controller_->SetSceneDrawer(view);

  connect(menubar, &ViewerMenu::fileOpened, this,
          [this, label, model_menu](QString path) {
            auto result =
                controller_->LoadFromFile(path.toStdString(), {0, 5, 1, 1});
            if (result.IsSuccess()) {
              model_menu->ClearValue();
              label->setText(QString::fromStdString(result.GetErrorMessage()));
              view->PrepareNewScene();
              result = controller_->DrawScene();
            }
            if (!result.IsSuccess()) {
              QMessageBox::warning(
                  this, "Error",
                  QString::fromStdString(result.GetErrorMessage()));
            }
          });
  connect(menubar, &ViewerMenu::screenshotPressed, this,
          [this]() { view->MakeScreenshot(); });

  connect(menubar, &ViewerMenu::screencastPressed, view, &View::MakeScreencast);

  ViewerSettings::GetInstance()->AddObserver(this);

  auto watcher = new ConfigWatcher("settings.ini", this);

  connect(watcher, &ConfigWatcher::configChanged, this,
          []() { ViewerSettings::GetInstance()->LoadSettings(); });
}

void MainWindow::UserInput(const UserKeys &key) {
  FacadeOperationResult result = controller_->UserInput(key);
  if (!result.IsSuccess()) {
    QMessageBox::warning(this, "Error",
                         QString::fromStdString(result.GetErrorMessage()));
  }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_ZoomOut:
      UserInput(UserKeys::kScaleDown);
      break;
    case Qt::Key_ZoomIn:
      UserInput(UserKeys::kScaleUp);
      break;
    case Qt::Key_W:
    case Qt::Key_Up:
      UserInput(UserKeys::kUp);
      break;
    case Qt::Key_S:
    case Qt::Key_Down:
      UserInput(UserKeys::kDown);
      break;
    case Qt::Key_A:
    case Qt::Key_Left:
      UserInput(UserKeys::kLeft);
      break;
    case Qt::Key_D:
    case Qt::Key_Right:
      UserInput(UserKeys::kRight);
      break;
    case Qt::Key_E:
      UserInput(UserKeys::kRightRotate);
      break;
    case Qt::Key_Q:
      UserInput(UserKeys::kLeftRotate);
      break;
    default:
      break;
  }
}

void MainWindow::wheelEvent(QWheelEvent *event) {
  int delta = event->angleDelta().y();

  if (delta > 0) {
    UserInput(UserKeys::kScaleUp);
  } else if (delta < 0) {
    UserInput(UserKeys::kScaleDown);
  }
}

void MainWindow::Update() {
  if (menubar->settings_panel) {
    menubar->settings_panel->LoadSettings();
  }
  if (view) {
    view->update();
  }
}

void MainWindow::closeEvent(QCloseEvent *event) {
  controller_->SetSceneDrawer(nullptr);
  QMainWindow::closeEvent(event);
}

}  // namespace s21
