#ifndef app_h
#define app_h

#include <stdint.h>

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
  virtual void onUpdate() = 0;

 public:
  int getId();
  const char *getName();
  bool isRunning();

  void open(void (*listener)());
  void close();

  /** used for updating application state */
  virtual void update() final;

  /* drawing section */
  /* draw main content */
  virtual void draw() = 0;
  /* draw background of app */
  virtual void drawBackground() {}
  /* draw title bar if its implemented */
  virtual void drawTitle() {}
  /* draw status bar if its implemented */
  virtual void drawStatusBar() {}
  virtual bool needsToBeRedrawn();

  /** set flag needsToRedraw to true, on next update it will be redrawn */
  void requestRedraw();

  /**
  handles user input, returns:
  true if command is consumed and can't be feed to another part
  false if it's not accepted by application
  and can be processed by someone else
  */
  virtual bool handle(uint16_t keyState) = 0;

  virtual ~App() {}
};

#endif  // app_h