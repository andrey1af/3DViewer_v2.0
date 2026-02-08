#ifndef GUI_HEADERS_DRAG_SPINBOX_H
#define GUI_HEADERS_DRAG_SPINBOX_H

#include <QtCore/QPoint>
#include <QtGui/QCursor>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QDoubleSpinBox>

namespace s21 {

class DragSpinBox : public QDoubleSpinBox {
  Q_OBJECT

 public:
  explicit DragSpinBox(QWidget *parent = nullptr);

 protected:
  bool eventFilter(QObject *watched, QEvent *event) override;

 private:
  QLineEdit *edit_;
  bool dragging = false;
  QPoint drag_start_pos;
  double drags_start_value = 0.0;

  void StartDrag();
  void UpdateDrag();
  void EndDrag();
};

}  // namespace s21

#endif  // GUI_HEADERS_DRAG_SPINBOX_H
