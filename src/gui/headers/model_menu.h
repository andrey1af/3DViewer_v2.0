#ifndef GUI_HEADERS_MODEL_MENU_H
#define GUI_HEADERS_MODEL_MENU_H

#include <QtWidgets/QWidget>

#include "../headers/drag_spinbox.h"

namespace s21 {

class ModelMenu : public QWidget {
  Q_OBJECT
 public:
  explicit ModelMenu(QWidget *parent = nullptr);
  ~ModelMenu() = default;
  void ClearValue();

 signals:
  void moveChanged(float x, float y, float z);
  void rotateChanged(float x, float y, float z);
  void scaleChanged(float x, float y, float z);

 private:
  DragSpinBox move_x_;
  DragSpinBox rotate_x_;
  DragSpinBox scale_x_;
  DragSpinBox move_y_;
  DragSpinBox rotate_y_;
  DragSpinBox scale_y_;
  DragSpinBox move_z_;
  DragSpinBox rotate_z_;
  DragSpinBox scale_z_;

  double move_x_value_;
  double rotate_x_value_;
  double scale_x_value_;
  double move_y_value_;
  double rotate_y_value_;
  double scale_y_value_;
  double move_z_value_;
  double rotate_z_value_;
  double scale_z_value_;

  void SetupGrid();
  void ConfigSpinBox(QDoubleSpinBox *spin_box);

 private slots:
  void handleMoveXChange(double value);
  void handleMoveYChange(double value);
  void handleMoveZChange(double value);
  void handleRotateXChange(double value);
  void handleRotateYChange(double value);
  void handleRotateZChange(double value);
  void handleScaleXChange(double value);
  void handleScaleYChange(double value);
  void handleScaleZChange(double value);
};

}  // namespace s21

#endif  // GUI_HEADERS_MODEL_MENU_H
