#ifndef DEMO_RESOURCES_H
#define DEMO_RESOURCES_H

#include "palette.h"
#include "primitives/sprite_data.h"

// 8x8
SpriteData_t bush = {.width = 8, .height = 8, .indicesCount = 32};
_ci bush_pixels[] = {0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
                     0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,
                     0xd,  0xdd, 0x0,  0x0,  0xda, 0xdd, 0xd0, 0x0,
                     0xdd, 0xda, 0xd5, 0x50, 0xd,  0xdd, 0x5,  0x50};

// 16x16
SpriteData_t cat = {.width = 16, .height = 16, .indicesCount = 128};
_ci cat_pixels[] = {
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xa,  0xaa, 0x0,
    0xa0, 0x0,  0x0,  0xa0, 0xa,  0xa0, 0x0,  0x0,  0xa0, 0xaa, 0xa0, 0xa0,
    0xa0, 0x0,  0x0,  0x0,  0xaa, 0xaa, 0xaa, 0xa0, 0xa0, 0x0,  0x0,  0x0,
    0xaa, 0xaa, 0xaa, 0xa0, 0xa0, 0x0,  0x0,  0x0,  0xaa, 0xaa, 0xaa, 0xa0,
    0xa0, 0x0,  0x0,  0x0,  0xa,  0xaa, 0xaa, 0x0,  0xa,  0x0,  0x0,  0x0,
    0x0,  0xaa, 0xa0, 0x0,  0x0,  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x0,
    0x0,  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x0,  0x0,  0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0x0,  0x0,  0xa0, 0xa0, 0x0,  0x0,  0xa,  0xa,  0x0,
    0xa,  0x0,  0xa,  0x0,  0x0,  0xa,  0x0,  0xa0, 0xa,  0x0,  0xa,  0x0,
    0x0,  0xa,  0x0,  0xa0, 0xa,  0xa0, 0xa,  0xa0, 0x0,  0xa,  0xa0, 0xaa,
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0};

// 16x16
SpriteData_t grass = {.width = 16, .height = 16, .indicesCount = 128};
_ci grass_pixels[] = {
    0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0xd5, 0x55, 0x55,
    0x5d, 0x55, 0x55, 0x55, 0x5d, 0x55, 0x55, 0x55, 0xd5, 0x55, 0x55, 0x55,
    0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x25, 0x55,
    0x55, 0x55, 0x55, 0xa5, 0x55, 0x5d, 0x55, 0x55, 0x55, 0x55, 0x5d, 0x55,
    0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
    0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0xd5, 0x55, 0xd5, 0x55, 0x55, 0x55,
    0x55, 0x5d, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
    0x55, 0xd5, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x5d, 0x55, 0x55, 0x55,
    0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x5d, 0x55, 0x55, 0x5d, 0xe5, 0x55,
    0x55, 0x55, 0xd5, 0x55, 0x55, 0xd5, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
    0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55};

// 8x8
SpriteData_t player = {.width = 8, .height = 8, .indicesCount = 32};
_ci player_pixels[] = {0x0,  0xc,  0xc0, 0x9,  0xc,  0xcc, 0xcc, 0x9,
                       0xc,  0x71, 0x71, 0x9,  0x0,  0x77, 0x77, 0x9,
                       0x66, 0x22, 0x22, 0x67, 0x70, 0x22, 0x22, 0x9,
                       0x0,  0x20, 0x2,  0x0,  0x0,  0xcc, 0xc,  0xc0};

// 16x32 - animated
SpriteData_t player16 = {.width = 16, .height = 16, .indicesCount = 256};
_ci player16_pixels[] = {
    0x0,  0x0,  0xc,  0xcc, 0xcc, 0xcc, 0x0,  0x90, 0x0,  0x0,  0xc,  0xcc,
    0xcc, 0xcc, 0x0,  0x90, 0x0,  0x0,  0xc,  0x71, 0x71, 0x7c, 0x0,  0x90,
    0x0,  0x0,  0xc,  0x77, 0x77, 0x7c, 0x0,  0x90, 0x0,  0x0,  0x0,  0x87,
    0x77, 0x80, 0x0,  0x90, 0x0,  0x0,  0x0,  0x8,  0x88, 0x0,  0x0,  0x90,
    0x0,  0x0,  0xc,  0x66, 0x66, 0x6c, 0x0,  0x90, 0x0,  0x0,  0x6,  0x66,
    0x66, 0x66, 0x0,  0x77, 0x0,  0x0,  0x6,  0x66, 0x66, 0x66, 0x66, 0x77,
    0x0,  0x0,  0x6,  0xcc, 0xcc, 0xc0, 0x0,  0x90, 0x0,  0x0,  0x77, 0x22,
    0x22, 0x20, 0x0,  0x90, 0x0,  0x0,  0x77, 0x22, 0x22, 0x20, 0x0,  0x90,
    0x0,  0x0,  0x0,  0xc2, 0x2,  0xc0, 0x0,  0x0,  0x0,  0x0,  0x0,  0xc2,
    0x2,  0xc0, 0x0,  0x0,  0x0,  0x0,  0x0,  0x22, 0x2,  0x20, 0x0,  0x0,
    0x0,  0x0,  0xc,  0xcc, 0xc,  0xcc, 0x0,  0x0,  0x0,  0x0,  0xc,  0xcc,
    0xcc, 0xcc, 0x0,  0x90, 0x0,  0x0,  0xc,  0xcc, 0xcc, 0xcc, 0x0,  0x90,
    0x0,  0x0,  0xc,  0x71, 0x71, 0x7c, 0x0,  0x90, 0x0,  0x0,  0xc,  0x77,
    0x77, 0x7c, 0x0,  0x90, 0x0,  0x0,  0x0,  0x87, 0x77, 0x80, 0x0,  0x90,
    0x0,  0x0,  0x0,  0x8,  0x88, 0x0,  0x0,  0x90, 0x0,  0x0,  0xc,  0x66,
    0x66, 0x6c, 0x0,  0x90, 0x0,  0x0,  0x6,  0x66, 0x66, 0x66, 0x0,  0x77,
    0x0,  0x0,  0x60, 0x66, 0x66, 0x66, 0x66, 0x77, 0x0,  0x0,  0x60, 0xcc,
    0xcc, 0xc0, 0x0,  0x90, 0x0,  0x7,  0x70, 0x22, 0x22, 0x20, 0x0,  0x90,
    0x0,  0x7,  0x70, 0x22, 0x22, 0x22, 0x0,  0x90, 0x0,  0x0,  0x0,  0xc2,
    0x0,  0x2c, 0x0,  0x0,  0x0,  0x0,  0x0,  0xc2, 0x0,  0x2c, 0x0,  0x0,
    0x0,  0x0,  0x0,  0x22, 0x0,  0x22, 0x0,  0x0,  0x0,  0x0,  0xc,  0xcc,
    0x0,  0xcc, 0xc0, 0x0};

// 8x8
SpriteData_t tree = {.width = 8, .height = 8, .indicesCount = 32};
_ci tree_pixels[] = {0x0,  0xd5, 0x55, 0x0,  0xd,  0x2d, 0x55, 0x50,
                     0xd5, 0x55, 0x55, 0x55, 0x55, 0xd2, 0x55, 0x25,
                     0x5,  0x55, 0x5d, 0x50, 0x0,  0x52, 0x55, 0x0,
                     0x0,  0x7,  0x80, 0x0,  0x0,  0x7,  0x80, 0x0};

// TODO: make this part as auto generated
void prepareSprites() {
  bush.indices = bush_pixels;
  cat.indices = cat_pixels;
  grass.indices = grass_pixels;
  player.indices = player_pixels;
  player16.indices = player16_pixels;
  tree.indices = tree_pixels;
}

#endif  // DEMO_RESOURCES_H