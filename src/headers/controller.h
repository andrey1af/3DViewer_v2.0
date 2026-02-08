#ifndef HEADERS_CONTROLLER_H
#define HEADERS_CONTROLLER_H

#include <string>

#include "scene.h"

namespace s21 {

enum class UserKeys {
  kRight,
  kLeft,
  kUp,
  kDown,
  kLeftRotate,
  kRightRotate,
  kScaleUp,
  kScaleDown
};

class Controller {
 public:
  FacadeOperationResult LoadFromFile(const std::string &path,
                                     NormalizationParameters params);
  FacadeOperationResult UserInput(UserKeys key);
  FacadeOperationResult MoveScene(float x, float y, float z);
  FacadeOperationResult RotateScene(float x, float y, float z);
  FacadeOperationResult ScaleScene(float x, float y, float z);
  FacadeOperationResult DrawScene();
  void SetSceneDrawer(SceneDrawerBase *drawer);
};

}  // namespace s21

#endif  // HEADERS_CONTROLLER_H
