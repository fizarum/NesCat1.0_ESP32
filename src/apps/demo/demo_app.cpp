#include "demo_app.h"

#include <log.h>
#include <scheduler.h>
#include <sprite.h>

#include "canvas.h"
#include "demo_app_settings.h"

DisplayDevice *_display = nullptr;
QueueHandle_t queue;
TaskHandle_t videoTaskHandler;
TaskHandle_t updateTaskHandler;
//
Sprite *sprite;
Canvas *canvas;

uint32_t renderDelay = toMillis(33);
uint32_t updateDelay = toMillis(50);

inline void drawVirtualPixel(uint16_t x, uint16_t y, Color color);
void renderTask(void *params);
void writeFrameTask(void *params);

void printDebugInfo();
void setupSprite();

void DemoApp::onOpen() {
  queue = xQueueCreate(1, sizeof(Canvas));
  canvas = new Canvas();
  setupSprite();
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
    sprite->moveBy(-1, 0);
  } else if (inputDevice->isRightPressed()) {
    sprite->moveBy(1, 0);
  } else if (inputDevice->isUpPressed()) {
    sprite->moveBy(0, -1);
  } else if (inputDevice->isDownPressed()) {
    sprite->moveBy(0, 1);
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
    if (xQueueReceive(queue, canvas, portMAX_DELAY) == pdPASS) {
      if (_display != nullptr) {
        x = 0;
        y = 0;

        for (auto index = 0; index < canvas->getPixelsCount(); ++index) {
          drawVirtualPixel(x, y, canvas->getPixel(index));

          x += PIXEL_SIZE;
          if (x == DISPLAY_WIDTH) {
            x = 0;
            y += PIXEL_SIZE;
          }
        }
      }
    }
    // vPortYield();
    vTaskDelay(renderDelay);
  }
  vTaskDelete(videoTaskHandler);
}

void writeFrameTask(void *param) {
  uint16_t index = 0;
  ColorIndex pixelData = 0;
  for (;;) {
    index = 0;
    // pixelData = 0;
    if (_display != nullptr) {
      for (uint8_t y = 0; y < HEIGHT_IN_V_PIXELS; ++y) {
        for (uint8_t x = 0; x < WIDTH_IN_V_PIXELS; ++x) {
          if (sprite->contains(x, y) == true) {
            pixelData = sprite->getPixel(x, y);
            canvas->setPixel(index, pixelData);
          } else {
            canvas->setPixel(index, 0);
          }
          ++index;
        }
      }
    }

    xQueueSend(queue, canvas, 0);
    vTaskDelay(updateDelay);
    // vPortYield();
  }
  vTaskDelete(updateTaskHandler);
}

inline void drawVirtualPixel(uint16_t x, uint16_t y, Color color) {
  if (_display != nullptr) {
    _display->fillRectangle(x, y, PIXEL_SIZE, PIXEL_SIZE, color);
  }
}

void setupSprite() {
  ColorIndex pixels[] = {3, 3, 2, 2};
  sprite = new Sprite(2, 2);
  sprite->setPixels(pixels, 4);
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
