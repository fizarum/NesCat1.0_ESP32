#ifndef nes_launcher_h
#define nes_launcher_h

#include <app.h>
#include <nes_wrapper.h>

class NesLauncher : public App {
 private:
  void init();
  nes_t *nes;

  const char *path = "/nes";
  const uint8_t filesPerPage = 8;
  int8_t cursorPos = 0;

 protected:
  void draw();
  void onUpdate();

 public:
  NesLauncher() {
    this->id = nesEmulatorId;
    this->name = emulatorTitle;
    this->running = false;
  }

  bool handle(uint16_t keyState);
};

#endif  // nes_launcher_h