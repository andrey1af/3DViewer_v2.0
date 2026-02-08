#include "../headers/model_menu.h"

#include <QtGui/QPixmap>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>

namespace s21 {

ModelMenu::ModelMenu(QWidget *parent) : QWidget(parent) {
  ClearValue();
  SetupGrid();

  ConfigSpinBox(&move_x_);
  ConfigSpinBox(&move_y_);
  ConfigSpinBox(&move_z_);
  ConfigSpinBox(&rotate_x_);
  ConfigSpinBox(&rotate_y_);
  ConfigSpinBox(&rotate_z_);

  scale_x_.setSingleStep(0.01);
  scale_x_.setMinimum(0.01);
  scale_y_.setSingleStep(0.01);
  scale_y_.setMinimum(0.01);
  scale_z_.setSingleStep(0.01);
  scale_z_.setMinimum(0.01);
}

void ModelMenu::SetupGrid() {
  auto layout = new QVBoxLayout(this);
  layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

  QPixmap pixmap(":/icons/logo.png");
  auto imageLabel = new QLabel;
  imageLabel->setPixmap(
      pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
  imageLabel->setAlignment(Qt::AlignCenter);
  layout->addWidget(imageLabel);
  layout->addSpacing(20);

  QFont labelFontHead("Inter", 16, QFont::DemiBold);
  QFont labelFont("Inter", 12, QFont::DemiBold);

  auto moveHeader = new QLabel("Move");
  moveHeader->setFont(labelFontHead);
  moveHeader->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
  moveHeader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  layout->addWidget(moveHeader);

  auto form_layout_move = new QFormLayout;
  form_layout_move->setLabelAlignment(Qt::AlignRight);
  form_layout_move->setFormAlignment(Qt::AlignTop | Qt::AlignLeft);
  form_layout_move->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
  form_layout_move->addRow("X ", &move_x_);
  form_layout_move->addRow("Y ", &move_y_);
  form_layout_move->addRow("Z ", &move_z_);
  layout->addLayout(form_layout_move);
  layout->addSpacing(30);

  auto rotateHeader = new QLabel("Rotate");
  rotateHeader->setFont(labelFontHead);
  rotateHeader->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
  rotateHeader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  layout->addWidget(rotateHeader);

  auto form_layout_rotate = new QFormLayout;
  form_layout_rotate->setLabelAlignment(Qt::AlignRight);
  form_layout_rotate->setFormAlignment(Qt::AlignTop | Qt::AlignLeft);
  form_layout_rotate->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
  form_layout_rotate->addRow("X ", &rotate_x_);
  form_layout_rotate->addRow("Y ", &rotate_y_);
  form_layout_rotate->addRow("Z ", &rotate_z_);
  layout->addLayout(form_layout_rotate);
  layout->addSpacing(30);

  auto scaleHeader = new QLabel("Scale");
  scaleHeader->setFont(labelFontHead);
  scaleHeader->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
  scaleHeader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  layout->addWidget(scaleHeader);

  auto form_layout_scale = new QFormLayout;
  form_layout_scale->setLabelAlignment(Qt::AlignRight);
  form_layout_scale->setFormAlignment(Qt::AlignTop | Qt::AlignLeft);
  form_layout_scale->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
  form_layout_scale->addRow("X ", &scale_x_);
  form_layout_scale->addRow("Y ", &scale_y_);
  form_layout_scale->addRow("Z ", &scale_z_);
  layout->addLayout(form_layout_scale);

  for (auto form_layout :
       {form_layout_move, form_layout_rotate, form_layout_scale}) {
    for (int i = 0; i < form_layout->rowCount(); ++i) {
      auto label_item = form_layout->itemAt(i, QFormLayout::LabelRole);
      if (label_item && label_item->widget()) {
        QLabel *label = qobject_cast<QLabel *>(label_item->widget());
        if (label) {
          label->setFont(labelFont);
          label->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
          label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        }
      }
    }
  }

  int spinboxWidth = 150;
  for (auto spin : {&move_x_, &move_y_, &move_z_, &rotate_x_, &rotate_y_,
                    &rotate_z_, &scale_x_, &scale_y_, &scale_z_}) {
    spin->setMaximumWidth(spinboxWidth);
  }

  connect(&move_x_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &ModelMenu::handleMoveXChange);
  connect(&rotate_x_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          this, &ModelMenu::handleRotateXChange);
  connect(&scale_x_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &ModelMenu::handleScaleXChange);
  connect(&move_y_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &ModelMenu::handleMoveYChange);
  connect(&rotate_y_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          this, &ModelMenu::handleRotateYChange);
  connect(&scale_y_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &ModelMenu::handleScaleYChange);
  connect(&move_z_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &ModelMenu::handleMoveZChange);
  connect(&rotate_z_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          this, &ModelMenu::handleRotateZChange);
  connect(&scale_z_, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &ModelMenu::handleScaleZChange);
}

void ModelMenu::ConfigSpinBox(QDoubleSpinBox *spin_box) {
  spin_box->setMinimum(std::numeric_limits<double>::lowest());
  spin_box->setMaximum(std::numeric_limits<double>::max());
  spin_box->setSingleStep(0.01);
}

void ModelMenu::ClearValue() {
  move_x_value_ = 0.0;
  rotate_x_value_ = 0.0;
  scale_x_value_ = 1.0;
  move_y_value_ = 0.0;
  rotate_y_value_ = 0.0;
  scale_y_value_ = 1.0;
  move_z_value_ = 0.0;
  rotate_z_value_ = 0.0;
  scale_z_value_ = 1.0;

  move_x_.setValue(move_x_value_);
  move_y_.setValue(move_y_value_);
  move_z_.setValue(move_z_value_);
  rotate_x_.setValue(rotate_x_value_);
  rotate_y_.setValue(rotate_y_value_);
  rotate_z_.setValue(rotate_z_value_);
  scale_x_.setValue(scale_x_value_);
  scale_y_.setValue(scale_y_value_);
  scale_z_.setValue(scale_z_value_);
}

void ModelMenu::handleMoveXChange(double value) {
  double increment = value - move_x_value_;
  move_x_value_ = value;
  emit moveChanged(static_cast<float>(increment), 0.0f, 0.0f);
}
void ModelMenu::handleMoveYChange(double value) {
  double increment = value - move_y_value_;
  move_y_value_ = value;
  emit moveChanged(0.0f, static_cast<float>(increment), 0.0f);
}
void ModelMenu::handleMoveZChange(double value) {
  double increment = value - move_z_value_;
  move_z_value_ = value;
  emit moveChanged(0.0f, 0.0f, static_cast<float>(increment));
}
void ModelMenu::handleRotateXChange(double value) {
  double increment = value - rotate_x_value_;
  rotate_x_value_ = value;
  emit rotateChanged(static_cast<float>(increment), 0.0f, 0.0f);
}
void ModelMenu::handleRotateYChange(double value) {
  double increment = value - rotate_y_value_;
  rotate_y_value_ = value;
  emit rotateChanged(0.0f, static_cast<float>(increment), 0.0f);
}
void ModelMenu::handleRotateZChange(double value) {
  double increment = value - rotate_z_value_;
  rotate_z_value_ = value;
  emit rotateChanged(0.0f, 0.0f, static_cast<float>(increment));
}
void ModelMenu::handleScaleXChange(double value) {
  double increment = 1.0 + value - scale_x_value_;
  scale_x_value_ = value;
  emit scaleChanged(static_cast<float>(increment), 1.0f, 1.0f);
}
void ModelMenu::handleScaleYChange(double value) {
  double increment = 1.0 + value - scale_y_value_;
  scale_y_value_ = value;
  emit scaleChanged(1.0f, static_cast<float>(increment), 1.0f);
}
void ModelMenu::handleScaleZChange(double value) {
  double increment = 1.0 + value - scale_z_value_;
  scale_z_value_ = value;
  emit scaleChanged(1.0f, 1.0f, static_cast<float>(increment));
}

}  // namespace s21
