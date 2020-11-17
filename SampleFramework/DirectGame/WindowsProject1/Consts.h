#pragma once

#include "resource.h"
#include <string>

#define TITLE L"Mario"

#define CATEGORY_TEXTURE "Textures"
#define CATEGORY_SPRITE "Sprites"
#define CATEGORY_ANIMATION "Animations"
#define CATEGORY_SCENE "Scenes"
#define CATEGORY_CONFIG "Config"

#define TEXTURE_MARIO "tex-mario"
#define TEXTURE_ENEMY "tex-enemy"
#define TEXTURE_MISC "tex-misc"
#define TEXTURE_FIREBALL "tex-fire-ball"
#define TEXTURE_EFFECT "tex-effect"

#define FIREBALL_TRANS_COLOR D3DCOLOR_XRGB(0, 0, 0)
#define DEFAULT_TRANS_COLOR D3DCOLOR_XRGB(255, 255, 255)

#define DB_SPRITE_MARIO "spr-mario"
#define DB_SPRITE_ENEMY "spr-enemy"
#define DB_SPRITE_MISC "spr-misc"
#define DB_SPRITE_EFFECT "spr-effect"

#define DB_ANIMATION_MARIO "ani-mario"
#define DB_ANIMATION_ENEMY "ani-enemy"
#define DB_ANIMATION_MISC "ani-misc"
#define DB_ANIMATION_EFFECT "ani-effect"

#define SC_WOLRD_1_1 "world-1-1"

#define CG_GLOBAL_CONFIG "global-config"

#define KEYBOARD_BUFFER_SIZE 1024

#define DIRECTINPUT_VERSION 0x0800