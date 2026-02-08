#ifndef GUI_HEADERS_SETTINGS_PANEL_H
#define GUI_HEADERS_SETTINGS_PANEL_H

#include <QtCore/QSettings>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <string>

namespace s21 {

class QTabWidget;

class SettingsPanel : public QDialog {
  Q_OBJECT
 public:
  explicit SettingsPanel(QWidget *parent = nullptr);
  ~SettingsPanel() = default;

  void LoadSettings();
  void SaveSettings();

 private:
  QWidget *vertices_tab;
  QWidget *edges_tab;
  QWidget *general_tab;

  QComboBox *vertices_type;
  QPushButton *vertices_color_button;
  QSpinBox *vertices_size;

  QComboBox *edges_line_type;
  QPushButton *edges_color_button;
  QSpinBox *edges_line_weight;

  QComboBox *projection;
  QPushButton *background_color_button;

  void SetupGeneralTab();
  void SetupVerticesTab();
  void SetupEdgesTab();
};

}  // namespace s21

#endif  // GUI_HEADERS_SETTINGS_PANEL_H
