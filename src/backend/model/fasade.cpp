#include "../headers/fasade.h"

#include "../headers/transform_matrix.h"

namespace s21 {

FacadeOperationResult::FacadeOperationResult(std::string error_message,
                                             bool is_success)
    : error_message_(error_message), is_success_(is_success) {}

std::string FacadeOperationResult::GetErrorMessage() const {
  return error_message_;
}

bool FacadeOperationResult::IsSuccess() const { return is_success_; }

Facade::Facade() { scene_.AddObserver(this); }

Facade::~Facade() {
  if (file_reader_) {
    delete file_reader_;
  }
}

Facade* Facade::GetInstance() {
  static Facade instance;
  return &instance;
}

void Facade::SetSceneDrawer(SceneDrawerBase* drawer) {
  if (scene_drawer_) {
    delete scene_drawer_;
  }
  scene_drawer_ = drawer;
}

FacadeOperationResult Facade::DrawScene() {
  try {
    if (!scene_drawer_) {
      throw "SceneDrawer wasnt set";
    }
    scene_drawer_->DrawScene(scene_);
  } catch (const char* error_message) {
    return FacadeOperationResult(error_message, false);
  }
  return FacadeOperationResult("", true);
}

FacadeOperationResult Facade::LoadScene(const std::string& path,
                                        NormalizationParameters params) {
  if (file_reader_) {
    delete file_reader_;
    file_reader_ = nullptr;
  }
  try {
    SelectFileReader(path);
    scene_ = file_reader_->ReadScene(path, params);
    int vertices = 0;
    int edges = 0;
    for (const auto& fig : scene_.GetFigures()) {
      vertices += fig.GetVertices().size();
      edges += fig.GetEdges().size();
    }

    const std::string str = "File: " + path +
                            "  Vertices: " + std::to_string(vertices) +
                            "  Edges: " + std::to_string(edges);
    return FacadeOperationResult(str, true);
  } catch (const char* error_message) {
    return FacadeOperationResult(error_message, false);
  }
}

FacadeOperationResult Facade::MoveScene(float x, float y, float z) {
  try {
    scene_.TransformFigures(TransformMatrixBuilder::CreateMoveMatrix(x, y, z));
  } catch (const char* error_message) {
    return FacadeOperationResult(error_message, false);
  }
  return FacadeOperationResult("", true);
}

FacadeOperationResult Facade::RotateScene(float x, float y, float z) {
  try {
    scene_.TransformFigures(
        TransformMatrixBuilder::CreateRotationMatrix(x, y, z));
  } catch (const char* error_message) {
    return FacadeOperationResult(error_message, false);
  }
  return FacadeOperationResult("", true);
}

FacadeOperationResult Facade::ScaleScene(float x, float y, float z) {
  try {
    scene_.TransformFigures(TransformMatrixBuilder::CreateScaleMatrix(x, y, z));
  } catch (const char* error_message) {
    return FacadeOperationResult(error_message, false);
  }
  return FacadeOperationResult("", true);
}

void Facade::Update() { DrawScene(); }

void Facade::SelectFileReader(const std::string& path) {
  if (!file_reader_) {
    if (path.find(".obj") != std::string::npos) {
      file_reader_ = new FileReader();
    } else {
      throw "Unsupported file format";
    }
  }
}

}  // namespace s21
