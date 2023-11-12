#ifndef nes_launcher_h
#define nes_launcher_h

#include <app.h>
#include <nes_wrapper.h>

class NesLauncher : public App {
 private:
  nes_t *nes;

  const char *path = "/nes";
  const uint8_t filesPerPage = 8;
  uint8_t pageCount = 0;
  int8_t cursorPos = 0;
  char *fullPathToSelectedNes;
  bool isLoading = false;

  void init();
  const char *selectFile();

 protected:
  void onDraw(DisplayDevice *display);
  void drawBackground(DisplayDevice *display);
  void drawTitle(DisplayDevice *display);
  void onUpdate();
  bool onHandleInput(InputDevice *inputDevice);

 public:
  NesLauncher() {
    this->id = nesEmulatorId;
    this->name = emulatorTitle;
    this->running = false;
    this->fullPathToSelectedNes = new char[256];
  }

  void resetLoadingStats(bool startLoading = false);
};

#endif  // nes_launcher_h