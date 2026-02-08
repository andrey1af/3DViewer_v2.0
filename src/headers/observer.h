#ifndef HEADERS_OBSERVER_H
#define HEADERS_OBSERVER_H

#include <vector>

namespace s21 {

class Observer {
 public:
  virtual void Update() = 0;
  virtual ~Observer() = default;
};

class Observable {
 public:
  ~Observable() = default;
  void AddObserver(Observer *observer) { _observers.push_back(observer); }
  void NotifyUpdate() {
    int size = _observers.size();
    for (int i = 0; i < size; i++) {
      _observers[i]->Update();
    }
  }

 private:
  std::vector<Observer *> _observers;
};

}  // namespace s21

#endif  // HEADERS_OBSERVER_H
