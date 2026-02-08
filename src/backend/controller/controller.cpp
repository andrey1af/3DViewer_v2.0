#include "../../headers/controller.h"

#include "../headers/fasade.h"

namespace s21 {

FacadeOperationResult Controller::LoadFromFile(const std::string &path,
                                               NormalizationParameters params) {
  Facade *facade = Facade::GetInstance();
  return facade->LoadScene(path, params);
}

FacadeOperationResult Controller::UserInput(UserKeys key) {
  Facade *facade = Facade::GetInstance();
  FacadeOperationResult result;
  switch (key) {
    case UserKeys::kRight:
      result = facade->MoveScene(0.1, 0, 0);
      break;
    case UserKeys::kLeft:
      result = facade->MoveScene(-0.1, 0, 0);
      break;
    case UserKeys::kUp:
      result = facade->MoveScene(0, 0.1, 0);
      break;
    case UserKeys::kDown:
      result = facade->MoveScene(0, -0.1, 0);
      break;
    case UserKeys::kRightRotate:
      result = facade->RotateScene(0, 0.1, 0);
      break;
    case UserKeys::kLeftRotate:
      result = facade->RotateScene(0, -0.1, 0);
      break;
    case UserKeys::kScaleUp:
      result = facade->ScaleScene(1.1, 1.1, 1.1);
      break;
    case UserKeys::kScaleDown:
      result = facade->ScaleScene(0.9, 0.9, 0.9);
      break;
    default:
      break;
  }
  return result;
}

void Controller::SetSceneDrawer(SceneDrawerBase *drawer) {
  Facade *facade = Facade::GetInstance();
  facade->SetSceneDrawer(drawer);
}

FacadeOperationResult Controller::MoveScene(float x, float y, float z) {
  Facade *facade = Facade::GetInstance();
  return facade->MoveScene(x, y, z);
}
FacadeOperationResult Controller::RotateScene(float x, float y, float z) {
  Facade *facade = Facade::GetInstance();
  return facade->RotateScene(x, y, z);
}
FacadeOperationResult Controller::ScaleScene(float x, float y, float z) {
  Facade *facade = Facade::GetInstance();
  return facade->ScaleScene(x, y, z);
}
FacadeOperationResult Controller::DrawScene() {
  Facade *facade = Facade::GetInstance();
  return facade->DrawScene();
}

}  // namespace s21
