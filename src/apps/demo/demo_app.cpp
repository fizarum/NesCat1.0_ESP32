#include "demo_app.h"

#include <Esp.h>
#include <freertos/queue.h>
#include <log.h>
#include <scheduler.h>

#include "demo_app_settings.h"
#include "primitives/pixel.h"
#include "resources.h"
#include "scene_holder.h"

DisplayDevice *_display = nullptr;

QueueHandle_t xQueue;
TaskHandle_t loopTaskHandler;
TaskHandle_t drawTaskHandler;
ObjectId playerId = 0;
ObjectId addonSprite = 0;

SceneHolder *sceneHolder = nullptr;
Palette_t *palette = NULL;

void _loopTask(void *params);
void _drawTask(void *pvParameters);

void printDebugInfo();
void setupSprites();
void redrawPixel(uint8_t x, uint8_t y, Color color);
void setupPalette();

void DemoApp::onOpen() {
  _display = nullptr;

  setupPalette();
  sceneHolder = new SceneHolder(palette, &redrawPixel);

  setupSprites();

  xQueue = xQueueCreate(FRAME_BUFFER_SIZE, sizeof(Pixel_t));

  printDebugInfo();
}

void DemoApp::onDraw(DisplayDevice *display) {
  if (_display == nullptr) {
    _display = display;
    display->fillScreen(COLOR_BLACK);

    sceneHolder->bakeCanvas();
    sceneHolder->removeAllDurtyRegions();

    vTaskDelay(pdMS_TO_TICKS(100));

    xTaskCreate(_loopTask, "loop_task", 2048, NULL, RENDER_TASK_PRIORITY,
                &loopTaskHandler);

    xTaskCreate(_drawTask, "draw_task", 2048, NULL, RENDER_TASK_PRIORITY,
                &drawTaskHandler);
  }
}

bool DemoApp::onHandleInput(InputDevice *inputDevice) {
  if (inputDevice->isLeftKeyDown()) {
    sceneHolder->moveGameObjectBy(playerId, -1, 0);
  } else if (inputDevice->isRightKeyDown()) {
    sceneHolder->moveGameObjectBy(playerId, 1, 0);
  } else if (inputDevice->isUpKeyDown()) {
    sceneHolder->moveGameObjectBy(playerId, 0, -1);
  } else if (inputDevice->isDownKeyDown()) {
    sceneHolder->moveGameObjectBy(playerId, 0, 1);
  }
  return true;
}

void DemoApp::onClose() {
  vTaskDelete(drawTaskHandler);
  vTaskDelete(loopTaskHandler);
  vQueueDelete(xQueue);

  PaletteDestroy(palette);
  delete sceneHolder;
  debug("demo app closing");

  // temporary solution used to make sure that resources are cleaned up
  ESP.restart();
}

Pixel_t pixel;

void redrawPixel(uint8_t x, uint8_t y, Color color) {
  BaseType_t xStatus;
  pixel.x = x;
  pixel.y = y;
  pixel.color = color;

  xStatus = xQueueSend(xQueue, (void *)(&pixel), 0);

  if (xStatus != pdPASS) {
    debug("can not add pixel to queue!\n");
  }
}

uint8_t maxAnimationIterations = 60;
uint8_t animationIteration = 0;
// 33 millis means 30 fps for game
// 20 millis means 50 fps for game
const uint16_t delayBetweenUpdates = 20;

void _loopTask(void *params) {
  int64_t startedAt, finihedAt;
  int64_t diffInMillis;

  while (1) {
    startedAt = esp_timer_get_time();
    // cat animation
    if (animationIteration < maxAnimationIterations) {
      sceneHolder->moveSpriteBy(addonSprite, 1, 0);
      animationIteration++;
    }

    sceneHolder->bakeCanvas();
    sceneHolder->removeAllDurtyRegions();
    finihedAt = esp_timer_get_time();

    diffInMillis = finihedAt - startedAt;

    if (diffInMillis < delayBetweenUpdates) {
      diffInMillis = delayBetweenUpdates - diffInMillis;
      vTaskDelay(pdMS_TO_TICKS(diffInMillis));
    } else {
      vTaskDelay(pdMS_TO_TICKS(delayBetweenUpdates));
    }
  }
}

void _drawTask(void *pvParameters) {
  Pixel_t pixelToDraw;
  BaseType_t xStatus;

  while (1) {
    xStatus = xQueueReceive(xQueue, &pixelToDraw, portMAX_DELAY);

    if (xStatus == pdPASS) {
      _display->fillRectangle(pixelToDraw.x * PIXEL_SIZE,
                              pixelToDraw.y * PIXEL_SIZE, PIXEL_SIZE,
                              PIXEL_SIZE, pixelToDraw.color);
    }
  }
}

void setupSprites() {
  // player
  playerId = sceneHolder->createGameObject(8, 8, player, 64, true, true);

  // player's statue - animated (wip)
  sceneHolder->createSprite(16, 16, player16, 256, 50, 50);

  // tree sprite, 8x8 pixels
  ObjectId treeObject =
      sceneHolder->createGameObject(8, 8, tree, 64, true, true);
  sceneHolder->moveGameObjectTo(treeObject, 50, 10);

  // bush, 8x8
  sceneHolder->createBackgroundSprite(8, 8, bush, 64, 30, 20);

  sceneHolder->createBackgroundSprite(16, 16, grass, 256, 0, 20);

  addonSprite = sceneHolder->createSprite(16, 16, cat, 256, 15, 5);
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
  palette = PaletteCreate(0);

  PaletteSetColor(palette, 0, COLOR_BLACK);
  PaletteSetColor(palette, 1, COLOR_PEARL);
  PaletteSetColor(palette, 2, COLOR_WATERMELON_RED);
  PaletteSetColor(palette, 3, COLOR_PEWTER_BLUE);
  PaletteSetColor(palette, 4, COLOR_PURPLE_TAUPE);
  PaletteSetColor(palette, 5, COLOR_FOREST_GREEN);
  PaletteSetColor(palette, 6, COLOR_INDIGO);
  PaletteSetColor(palette, 7, COLOR_SUNRAY);
  PaletteSetColor(palette, 8, COLOR_LIGHT_TAUPE);
  PaletteSetColor(palette, 9, COLOR_FELDGRAU);
  PaletteSetColor(palette, 10, COLOR_CEDAR_CHEST);
  PaletteSetColor(palette, 11, COLOR_DARK_CHARCOAL);
  PaletteSetColor(palette, 12, COLOR_SONIC_SILVER);
  PaletteSetColor(palette, 13, COLOR_ASPARAGUS);
  PaletteSetColor(palette, 14, COLOR_SEA_SERPENT);
  PaletteSetColor(palette, 15, COLOR_GRAY);
}