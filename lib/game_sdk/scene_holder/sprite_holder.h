#ifndef GAME_SDK_SPRITE_HOLDER_H
#define GAME_SDK_SPRITE_HOLDER_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_BACKGROUND_SPRITES_COUNT 32
#define MAX_FOREGROUND_SPRITES_COUNT 32

typedef struct SpritesHolder_t SpritesHolder_t;

SpritesHolder_t *SpriteHolderCreate();
void SpriteHolderDestroy(SpritesHolder_t *holder);

//..

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_SPRITE_HOLDER_H