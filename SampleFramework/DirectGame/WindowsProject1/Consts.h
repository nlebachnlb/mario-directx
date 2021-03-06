#pragma once

#include "resource.h"
#include <string>

#define TITLE L"Super Mario Bros. 3 (NES)"

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
#define TEXTURE_PIPE "tex-pipe"
#define TEXTURE_HUD "tex-ui"
#define TEXTURE_BOX "tex-box"
#define TEXTURE_REWARD "tex-reward"
#define TEXTURE_OVERWORLD_MAP "tex-overworld"
#define TEXTURE_MENU "tex-menu"
#define TEXTURE_DIALOG "tex-dialog"

#define FIREBALL_TRANS_COLOR D3DCOLOR_XRGB(0, 0, 0)
#define DEFAULT_TRANS_COLOR D3DCOLOR_XRGB(254, 254, 254)

#define DB_SPRITE_MARIO "spr-mario"
#define DB_SPRITE_ENEMY "spr-enemy"
#define DB_SPRITE_MISC "spr-misc"
#define DB_SPRITE_EFFECT "spr-effect"
#define DB_SPRITE_HUD "spr-hud"
#define DB_SPRITE_OVERWORLD_MAP "spr-overworld"
#define DB_SPRITE_MENU "spr-menu"

#define DB_ANIMATION_MARIO "ani-mario"
#define DB_ANIMATION_ENEMY "ani-enemy"
#define DB_ANIMATION_MISC "ani-misc"
#define DB_ANIMATION_EFFECT "ani-effect"
#define DB_ANIMATION_HUD "ani-hud"
#define DB_ANIMATION_OVERWORLD_MAP "ani-overworld"
#define DB_ANIMATION_MENU "ani-menu"

#define SC_OVERWORLD_MAP_1 "overworld-1"
#define SC_WOLRD_1_1 "world-1-1"
#define SC_WOLRD_1_4 "world-1-4"
#define SC_MENU "main-menu"
#define SC_INTRO "intro"

#define CG_GLOBAL_CONFIG "global-config"

#define KEYBOARD_BUFFER_SIZE 1024

#define DIRECTINPUT_VERSION 0x0800

const int SCORE_VALUES[9] = { 100, 200, 400, 800, 1000, 2000, 4000, 8000, 8000 };