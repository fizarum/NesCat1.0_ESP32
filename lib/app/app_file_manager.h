#ifndef app_file_manager_h
#define app_file_manager_h

#include "app.h"

class FileManager : public App {
 protected:
  void draw();
  void onUpdate();

 public:
  FileManager() {
    this->id = fileManagerId;
    this->name = fileManagerTitle;
    this->running = false;
  }

  bool handle(uint16_t keyState);
};

#endif  // app_file_manager_h