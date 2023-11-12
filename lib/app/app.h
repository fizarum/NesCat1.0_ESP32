#ifndef app_h
#define app_h

#include <input_device/input_device.h>
#include <stdint.h>

#include "../device/display/display_device.h"
#include "apps_registry.h"

class App {
 private:
  /** called on open() function call */
  virtual void init() {}

 protected:
  int id;
  const char *name;
  bool running;

  /** flag indicating if application needs to be redrawn */
  bool needsToRedraw = false;

  /** called on app's close() */
  void (*onCloseListener)() = nullptr;

  /** app specific implementation of update */
  virtual void onUpdate(){};

  /** app specific implementation of draw */
  virtual void onDraw(DisplayDevice *display) = 0;

  /** app specific implementation of input handling */
  virtual bool onHandleInput(InputDevice *inputDevice) = 0;

  /** app specific implementation of event before app closing */
  virtual void onClose() {}

 public:
  int getId();
  const char *getName();
  bool isRunning();

  void open(void (*listener)());
  void close();

  /** used for updating application state */
  virtual void update() final;

  /* draw main content */
  virtual void draw(DisplayDevice *display) final;
  /* draw background of app */
  virtual void drawBackground(DisplayDevice *display) {}
  /* draw title bar if its implemented */
  virtual void drawTitle(DisplayDevice *display) {}
  /* draw status bar if its implemented */
  virtual void drawStatusBar(DisplayDevice *display) {}

  virtual bool needsToBeRedrawn();

  /** set flag needsToRedraw to true, on next update it will be redrawn */
  void requestRedraw();

  /**
   * @brief By default, app should be closed by pressing "menu" button.
   * This flag allows to prevent such behavior and application won't be closed
   * in such cases. Its useful for some special app - menu, for example.
   * If application has preventClosing = true, then it should call close()
   * manually.
   *
   *
   * @return true if application should behave as normal one
   * @return false for cases when handling "menu" button should be avoided
   */
  virtual bool preventClosingByUser() { return false; }

  /**
   * @brief handles user input
   *
   * @returns true if command is consumed and can't be feed to another part
   * false if it's not accepted by application
   * and can be processed by someone else
   */
  virtual bool handleInput(InputDevice *inputDevice) final;

  virtual ~App() {}
};

#endif  // app_h