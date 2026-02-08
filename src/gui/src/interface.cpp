#include <QtCore/QFile>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QVBoxLayout>

#include "../headers/main_window.h"

void SetStyle(QApplication *app);

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  SetStyle(&app);

  app.setWindowIcon(QIcon(":/icons/logo.png"));

  auto *window = new s21::MainWindow();

  window->show();

  return app.exec();
}

void SetStyle(QApplication *app) {
  QFile style_file(":/style.qss");
  if (style_file.open(QFile::ReadOnly)) {
    QString style = QLatin1String(style_file.readAll());
    app->setStyleSheet(style);
  }
}
