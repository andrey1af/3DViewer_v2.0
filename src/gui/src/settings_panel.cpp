#include "../headers/settings_panel.h"

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>

#include "../headers/settings.h"

namespace s21 {

SettingsPanel::SettingsPanel(QWidget *parent) : QDialog(parent) {
  auto btn_save = new QPushButton("Save", this);
  auto btn_cancel = new QPushButton("Cancel", this);
  setFixedSize(800, 400);

  SetupGeneralTab();
  SetupVerticesTab();
  SetupEdgesTab();

  auto *settings_area_widget = new QWidget(this);
  settings_area_widget->setObjectName("settings-area");
  auto *settings_layout = new QHBoxLayout(settings_area_widget);

  auto *list_widget = new QListWidget(this);
  list_widget->setViewMode(QListView::ListMode);
  list_widget->addItem("General");
  list_widget->addItem("Vertecies");
  list_widget->addItem("Edges");
  list_widget->setSpacing(4);
  list_widget->setProperty("class", "category-list");
  list_widget->setFocusPolicy(Qt::NoFocus);

  auto *stacked_widget = new QStackedWidget(this);
  stacked_widget->addWidget(general_tab);
  stacked_widget->addWidget(vertices_tab);
  stacked_widget->addWidget(edges_tab);

  settings_layout->addWidget(list_widget);
  settings_layout->addWidget(stacked_widget);

  connect(list_widget, &QListWidget::currentRowChanged, stacked_widget,
          &QStackedWidget::setCurrentIndex);

  auto *btn_layout = new QHBoxLayout();
  btn_layout->addStretch();
  QFont labelFont("Inter", 12, QFont::DemiBold);
  btn_save->setFont(labelFont);
  btn_cancel->setFont(labelFont);
  btn_save->setFixedSize(120, 40);
  btn_cancel->setFixedSize(150, 40);
  btn_layout->addWidget(btn_save);
  btn_layout->addWidget(btn_cancel);

  auto *main_layout = new QVBoxLayout(this);
  main_layout->addWidget(settings_area_widget);
  main_layout->addLayout(btn_layout);

  connect(btn_save, &QPushButton::clicked, this, [this]() {
    ViewerSettings::GetInstance()->SaveSettings();
    accept();
  });
  connect(btn_cancel, &QPushButton::clicked, this, [this]() {
    reject();
    ViewerSettings::GetInstance()->LoadSettings();
  });
}

void SettingsPanel::SetupGeneralTab() {
  general_tab = new QWidget(this);
  general_tab->setProperty("class", "settings-tab");
  auto *layout = new QFormLayout(general_tab);

  auto settings = ViewerSettings::GetInstance();

  projection = new QComboBox(general_tab);
  projection->addItem("Parallel", static_cast<int>(ProjectionType::kParallel));
  projection->addItem("Central", static_cast<int>(ProjectionType::kCentral));
  projection->setCurrentIndex(static_cast<int>(settings->GetProjection()));

  connect(projection, QOverload<int>::of(&QComboBox::currentIndexChanged),
          general_tab, [](int value) {
            ViewerSettings::GetInstance()->SetProjection(value);
          });

  background_color_button = new QPushButton(general_tab);
  background_color_button->setStyleSheet(
      QString("background-color: %1").arg(settings->GetBackgroundColor()));

  connect(background_color_button, &QPushButton::clicked, general_tab,
          [this]() {
            auto settings = ViewerSettings::GetInstance();
            QColor color = QColorDialog::getColor(
                settings->GetBackgroundColor(), general_tab, "Choose color");
            if (color.isValid()) {
              ViewerSettings::GetInstance()->SetBackgroundColor(color.name());
            }
          });

  layout->addRow("Projection type:", projection);
  layout->addRow("Background color:", background_color_button);
}

void SettingsPanel::SetupVerticesTab() {
  vertices_tab = new QWidget(this);
  vertices_tab->setProperty("class", "settings-tab");
  auto *layout = new QFormLayout(vertices_tab);

  auto settings = ViewerSettings::GetInstance();

  vertices_type = new QComboBox(vertices_tab);
  vertices_type->addItem("None", static_cast<int>(VerticesType::kNone));
  vertices_type->addItem("Square", static_cast<int>(VerticesType::kSquare));
  vertices_type->addItem("Circle", static_cast<int>(VerticesType::kCircle));
  vertices_type->setCurrentIndex(static_cast<int>(settings->GetVerticesType()));

  connect(vertices_type, QOverload<int>::of(&QComboBox::currentIndexChanged),
          vertices_tab, [](int value) {
            ViewerSettings::GetInstance()->SetVerticesType(value);
          });

  vertices_color_button = new QPushButton(vertices_tab);
  vertices_color_button->setStyleSheet(
      QString("background-color: %1").arg(settings->GetVerticesColor()));

  connect(vertices_color_button, &QPushButton::clicked, vertices_tab, [this]() {
    auto settings = ViewerSettings::GetInstance();
    QColor color = QColorDialog::getColor(settings->GetVerticesColor(),
                                          vertices_tab, "Choose color");
    if (color.isValid()) {
      ViewerSettings::GetInstance()->SetVerticesColor(color.name());
    }
  });

  vertices_size = new QSpinBox(vertices_tab);
  vertices_size->setMinimum(1);
  vertices_size->setMaximum(15);
  vertices_size->setSingleStep(1);
  vertices_size->setValue(settings->GetVerticesSize());

  connect(
      vertices_size, QOverload<int>::of(&QSpinBox::valueChanged), vertices_tab,
      [](int value) { ViewerSettings::GetInstance()->SetVerticesSize(value); });

  layout->addRow("Vertecies type:", vertices_type);
  layout->addRow("Color:", vertices_color_button);
  layout->addRow("Vertecies size:", vertices_size);
}

void SettingsPanel::SetupEdgesTab() {
  edges_tab = new QWidget(this);
  edges_tab->setProperty("class", "settings-tab");
  auto *layout = new QFormLayout(edges_tab);

  auto settings = ViewerSettings::GetInstance();

  edges_line_type = new QComboBox(edges_tab);
  edges_line_type->addItem("Solid", static_cast<int>(EdgesType::kSolid));
  edges_line_type->addItem("Dotted", static_cast<int>(EdgesType::kDotted));
  edges_line_type->setCurrentIndex(static_cast<int>(settings->GetEdgesType()));

  connect(edges_line_type, QOverload<int>::of(&QComboBox::currentIndexChanged),
          edges_tab, [](int value) {
            ViewerSettings::GetInstance()->SetEdgesType(value);
          });

  edges_color_button = new QPushButton(edges_tab);
  edges_color_button->setStyleSheet(
      QString("background-color: %1").arg(settings->GetEdgesColor()));

  connect(edges_color_button, &QPushButton::clicked, edges_tab, [this]() {
    auto settings = ViewerSettings::GetInstance();
    QColor color = QColorDialog::getColor(settings->GetEdgesColor(), edges_tab,
                                          "Choose color");
    if (color.isValid()) {
      ViewerSettings::GetInstance()->SetEdgesColor(color.name());
    }
  });

  edges_line_weight = new QSpinBox(edges_tab);
  edges_line_weight->setMinimum(1);
  edges_line_weight->setMaximum(15);
  edges_line_weight->setSingleStep(1);
  edges_line_weight->setValue(settings->GetEdgesWeight());

  connect(
      edges_line_weight, QOverload<int>::of(&QSpinBox::valueChanged), edges_tab,
      [](int value) { ViewerSettings::GetInstance()->SetEdgesWeight(value); });

  layout->addRow("Line type:", edges_line_type);
  layout->addRow("Color:", edges_color_button);
  layout->addRow("Line weight:", edges_line_weight);
}

void SettingsPanel::LoadSettings() {
  auto settings = ViewerSettings::GetInstance();

  if (projection) {
    projection->setCurrentIndex(static_cast<int>(settings->GetProjection()));
  }
  if (background_color_button) {
    background_color_button->setStyleSheet(
        QString("background-color: %1").arg(settings->GetBackgroundColor()));
  }

  if (vertices_type) {
    vertices_type->setCurrentIndex(
        static_cast<int>(settings->GetVerticesType()));
  }
  if (vertices_color_button) {
    vertices_color_button->setStyleSheet(
        QString("background-color: %1").arg(settings->GetVerticesColor()));
  }
  if (vertices_size) {
    vertices_size->setValue(settings->GetVerticesSize());
  }

  if (edges_line_type) {
    edges_line_type->setCurrentIndex(
        static_cast<int>(settings->GetEdgesType()));
  }
  if (edges_color_button) {
    edges_color_button->setStyleSheet(
        QString("background-color: %1").arg(settings->GetEdgesColor()));
  }
  if (edges_line_weight) {
    edges_line_weight->setValue(settings->GetEdgesWeight());
  }
}

}  // namespace s21
