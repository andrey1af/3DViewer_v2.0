#include "../../headers/drag_spinbox.h"

#include <QtGui/QCursor>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>

namespace s21 {

DragSpinBox::DragSpinBox(QWidget *parent) : QDoubleSpinBox(parent) {
  setMouseTracking(true);
  setFocusPolicy(Qt::StrongFocus);

  installEventFilter(this);
  edit_ = lineEdit();
  edit_->installEventFilter(this);
  edit_->setCursor(Qt::SizeHorCursor);
}

bool DragSpinBox::eventFilter(QObject *watched, QEvent *event) {
  if (watched == edit_) {
    if (event->type() == QEvent::MouseButtonPress) {
      QMouseEvent *mouse_event = static_cast<QMouseEvent *>(event);
      if (mouse_event->button() == Qt::LeftButton) {
        StartDrag();
        return true;
      }
    }
  }
  if (dragging) {
    if (event->type() == QEvent::MouseMove) {
      UpdateDrag();
      return true;
    }
    if (event->type() == QEvent::MouseButtonRelease) {
      EndDrag();
      return true;
    }
  }
  return QDoubleSpinBox::eventFilter(watched, event);
}

void DragSpinBox::StartDrag() {
  dragging = true;
  drag_start_pos = QCursor::pos();
  drags_start_value = value();
  setFocusPolicy(Qt::NoFocus);
  QPixmap pixmap(1, 1);
  pixmap.fill(QColor(0, 0, 0, 1));
  QCursor invisibleCursor(pixmap);
  QApplication::setOverrideCursor(invisibleCursor);
  grabMouse();
}

void DragSpinBox::UpdateDrag() {
  QPoint currentPos = QCursor::pos();
  int dx = currentPos.x() - drag_start_pos.x();
  double sensitivity = singleStep();
  setValue(drags_start_value + dx * sensitivity);
}

void DragSpinBox::EndDrag() {
  dragging = false;
  QApplication::restoreOverrideCursor();
  releaseMouse();
  setFocusPolicy(Qt::StrongFocus);
}

}  // namespace s21
