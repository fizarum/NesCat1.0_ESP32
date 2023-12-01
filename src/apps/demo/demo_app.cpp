#include "demo_app.h"

#include <log.h>
#include <scheduler.h>
#include <sprite.h>

#include "canvas.h"
#include "demo_app_settings.h"
#include "scene_holder.h"

DisplayDevice *_display = nullptr;
QueueHandle_t queue;
TaskHandle_t videoTaskHandler;
TaskHandle_t updateTaskHandler;
//
Sprite *playerSprite = nullptr;
SceneHolder *sceneHolder = nullptr;

inline void drawVirtualPixel(uint16_t x, uint16_t y, Color color);
void renderTask(void *params);
void writeFrameTask(void *params);

void printDebugInfo();
void printQueueStatus(const char *message = "");

void setupPlayerSprite();
void setupSimpleSprite();

void DemoApp::onOpen() {
  queue = xQueueCreate(1, sizeof(Canvas));
  sceneHolder = new SceneHolder();

  setupPlayerSprite();
  setupSimpleSprite();

  if (queue != nullptr) {
    createTaskOnCore0(&renderTask, "render_task", 10000, RENDER_TASK_PRIORITY,
                      &videoTaskHandler);
    createTaskOnCore0(&writeFrameTask, "update_task", 10000,
                      WRITE_FRAME_TASK_PRIORITY, &updateTaskHandler);
    printDebugInfo();
  }
}

void DemoApp::onDraw(DisplayDevice *display) {
  if (_display == nullptr) {
    _display = display;
    display->fillScreen(COLOR_BLACK);
    display->drawString(DISPLAY_H_CENTER, DISPLAY_V_CENTER, "loading...",
                        COLOR_WHITE, MC_DATUM);
  }
}

bool DemoApp::onHandleInput(InputDevice *inputDevice) {
  if (inputDevice->isLeftPressed()) {
    playerSprite->moveBy(-1, 0);
  } else if (inputDevice->isRightPressed()) {
    playerSprite->moveBy(1, 0);
  } else if (inputDevice->isUpPressed()) {
    playerSprite->moveBy(0, -1);
  } else if (inputDevice->isDownPressed()) {
    playerSprite->moveBy(0, 1);
  }

  return true;
}

void DemoApp::onClose() {
  vQueueDelete(queue);
  _display = nullptr;
  vTaskDelay(50);
  vTaskDelete(updateTaskHandler);
  vTaskDelete(videoTaskHandler);
  vTaskDelay(50);
}

void renderTask(void *params) {
  uint16_t x = 0;
  uint16_t y = 0;
  for (;;) {
    if (xQueueReceive(queue, sceneHolder->canvas, portMAX_DELAY) == pdPASS) {
      if (_display != nullptr) {
        x = 0;
        y = 0;

        for (auto index = 0; index < sceneHolder->canvas->getPixelsCount();
             ++index) {
          drawVirtualPixel(x, y, sceneHolder->canvas->getPixel(index));

          x += PIXEL_SIZE;
          if (x == DISPLAY_WIDTH) {
            x = 0;
            y += PIXEL_SIZE;
          }
        }
      }
    }
    // vPortYield();
    vTaskDelay(toMillis(33));
  }
  vTaskDelete(videoTaskHandler);
}

void writeFrameTask(void *param) {
  uint16_t index = 0;
  ColorIndex pixelData = 0;
  for (;;) {
    index = 0;
    sceneHolder->bakeCanvas();
    if (xQueueSend(queue, sceneHolder->canvas, 100) == pdPASS) {
      vTaskDelay(toMillis(50));
    } else {
      debug("failed to send data!");
    }
    // vPortYield();
  }
  vTaskDelete(updateTaskHandler);
}

inline void drawVirtualPixel(uint16_t x, uint16_t y, Color color) {
  if (_display != nullptr) {
    _display->fillRectangle(x, y, PIXEL_SIZE, PIXEL_SIZE, color);
  }
}

void setupPlayerSprite() {
  playerSprite = sceneHolder->createSprite(2, 2);
  ColorIndex pixels[] = {3, 3, 2, 2};
  playerSprite->setPixels(pixels, 4);
}

void setupSimpleSprite() {
  Sprite *sprite = sceneHolder->createSprite(4, 4);
  // tree sprite, 4x4 pixels
  ColorIndex pixels[] = {0, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5, 0, 10, 10, 0};
  sprite->setPixels(pixels, 16);
  sprite->moveBy(5, 5);
}

void printDebugInfo() {
  debug("----------------------");
  uint stackSize = uxTaskGetStackHighWaterMark(videoTaskHandler);
  debug("render requires:%u bytes in stack", stackSize);
  stackSize = uxTaskGetStackHighWaterMark(updateTaskHandler);
  debug("updater requires:%u bytes in stack", stackSize);
  debug("pixel size %u", PIXEL_SIZE);
  debug("resolution width: %u", WIDTH_IN_V_PIXELS);
  debug("resolution height: %u", HEIGHT_IN_V_PIXELS);
  debug("----------------------");
}
