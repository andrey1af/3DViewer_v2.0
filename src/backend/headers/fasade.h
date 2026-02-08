#ifndef BACKEND_HEADERS_FASADE_H
#define BACKEND_HEADERS_FASADE_H

#include <string>

#include "../../headers/scene.h"
#include "file_reader.h"

namespace s21 {

class Facade : public Observer {
 public:
  static Facade *GetInstance();

  void SetSceneDrawer(SceneDrawerBase *drawer);
  FacadeOperationResult DrawScene();
  FacadeOperationResult LoadScene(const std::string &path,
                                  NormalizationParameters params);
  FacadeOperationResult MoveScene(float x, float y, float z);
  FacadeOperationResult RotateScene(float x, float y, float z);
  FacadeOperationResult ScaleScene(float x, float y, float z);

  void Update() override;

 protected:
  Facade();
  ~Facade() override;
  Facade(const Facade &) = delete;
  Facade &operator=(const Facade &) = delete;
  Facade(Facade &&) = delete;
  Facade &operator=(Facade &&) = delete;

 private:
  void SelectFileReader(const std::string &path);
  BaseFileReader *file_reader_ = nullptr;
  SceneDrawerBase *scene_drawer_ = nullptr;
  Scene scene_;
};

}  // namespace s21

#endif  // BACKEND_HEADERS_FASADE_H
