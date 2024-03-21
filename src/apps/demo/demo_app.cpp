#include "demo_app.h"

#include <Esp.h>
#include <log.h>
#include <scheduler.h>
#include <sprite.h>

#include "demo_app_settings.h"
#include "scene_holder.h"

DisplayDevice *_display = nullptr;
TaskHandle_t loopTaskHandler;
ObjectId playerSprite = 0;
ObjectId addonSprite = 0;

SceneHolder *sceneHolder = nullptr;
Palette *palette = nullptr;

volatile bool _loopRunning = false;
volatile bool _terminated = false;

void _loopTask(void *params);
void printDebugInfo();
void setupPlayerSprite();
void setupSimpleSprite();
void setupAdditionalSprite();
void redrawPixel(uint8_t x, uint8_t y, Color color);
void setupPalette();

void DemoApp::onOpen() {
  _display = nullptr;
  _loopRunning = false;
  _terminated = false;

  setupPalette();
  sceneHolder = new SceneHolder(palette, &redrawPixel);

  setupPlayerSprite();
  setupSimpleSprite();
  setupAdditionalSprite();

  createTaskOnCore0(&_loopTask, "loop_task", 10000, RENDER_TASK_PRIORITY,
                    &loopTaskHandler);
  printDebugInfo();
}

void DemoApp::onDraw(DisplayDevice *display) {
  if (_display == nullptr) {
    _display = display;
    display->fillScreen(COLOR_BLACK);
    vTaskDelay(toMillis(100));
    _loopRunning = true;
  }
}

bool DemoApp::onHandleInput(InputDevice *inputDevice) {
  if (inputDevice->isLeftPressed()) {
    sceneHolder->moveSpriteBy(playerSprite, -1, 0);
  } else if (inputDevice->isRightPressed()) {
    sceneHolder->moveSpriteBy(playerSprite, 1, 0);
  } else if (inputDevice->isUpPressed()) {
    sceneHolder->moveSpriteBy(playerSprite, 0, -1);
  } else if (inputDevice->isDownPressed()) {
    sceneHolder->moveSpriteBy(playerSprite, 0, 1);
  }
  return true;
}

void DemoApp::onClose() {
  _terminated = true;
  _loopRunning = false;

  vTaskDelete(loopTaskHandler);
  delete sceneHolder;
  debug("demo app closing");

  // temporary solution used to make sure that resources are cleaned up
  ESP.restart();
}

void redrawPixel(uint8_t x, uint8_t y, Color color) {
  _display->fillRectangle(x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE,
                          PIXEL_SIZE, color);
}

uint8_t maxAnimationIterations = 60;
uint8_t animationIteration = 0;

void _loopTask(void *params) {
  int64_t startedAt, finihedAt;
  uint32_t diffInMillis;

  while (_terminated == false) {
    if (_loopRunning == true) {
      startedAt = esp_timer_get_time();
      // cat animation
      if (animationIteration < maxAnimationIterations) {
        sceneHolder->moveSpriteBy(addonSprite, 1, 0);
        animationIteration++;
      }

      sceneHolder->bakeCanvas();
      sceneHolder->removeAllDurtyRegions();
      finihedAt = esp_timer_get_time();

      // vPortYield();
      vTaskDelay(toMillis(20));
    }
  }
}

void setupPlayerSprite() {
  ColorIndex pixels[] = {6, 119};
  playerSprite = sceneHolder->createSprite(2, 2, pixels, 4, 20, 20);
}

void setupSimpleSprite() {
  // tree sprite, 4x4 pixels
  ColorIndex pixels[] = {5, 80, 85, 85, 85, 85, 10, 160};
  sceneHolder->createBackgroundSprite(4, 4, pixels, 16, 5, 5);
}

void setupAdditionalSprite() {
  // cat sprite 7x7 pixels
  // warning! all sprites should have sizes dividable by 2! (2, 4, 6, 8, etc.)
  // so this sprite is rendered incorrectly
  ColorIndex pixels[] = {
      128, 128, 8,   136, 136, 0, 8, 136, 128, 0, 128, 136, 136, 136,
      8,   136, 136, 128, 128, 0, 8, 8,   0,   0, 128, 128, 0,   8,
  };

  addonSprite = sceneHolder->createSprite(7, 7, pixels, 56, 15, 5);
}

void printDebugInfo() {
  debug("----------------------");
  uint stackSize = uxTaskGetStackHighWaterMark(loopTaskHandler);
  debug("render requires:%u bytes in stack", stackSize);
  debug("pixel size %u", PIXEL_SIZE);
  debug("resolution width: %u", WIDTH_IN_V_PIXELS);
  debug("resolution height: %u", HEIGHT_IN_V_PIXELS);
  debug("----------------------");
}

void setupPalette() {
  palette = new Palette();
  palette->setColor(0, COLOR_BLACK);
  palette->setColor(1, COLOR_PEARL);
  palette->setColor(2, COLOR_WATERMELON_RED);
  palette->setColor(3, COLOR_PEWTER_BLUE);
  palette->setColor(4, COLOR_PURPLE_TAUPE);
  palette->setColor(5, COLOR_FOREST_GREEN);
  palette->setColor(6, COLOR_INDIGO);
  palette->setColor(7, COLOR_SUNRAY);
  palette->setColor(8, COLOR_LIGHT_TAUPE);
  palette->setColor(9, COLOR_FELDGRAU);
  palette->setColor(10, COLOR_CEDAR_CHEST);
  palette->setColor(11, COLOR_DARK_CHARCOAL);
  palette->setColor(12, COLOR_SONIC_SILVER);
  palette->setColor(13, COLOR_ASPARAGUS);
  palette->setColor(14, COLOR_SEA_SERPENT);
  palette->setColor(15, COLOR_GRAY);
}