/*
 * Net Puzzle Arena
 *
 * Copyright (C) 2016 NovaSquirrel
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef PUZZLE_HEADER
#define PUZZLE_HEADER
#define NO_STDIO_REDIRECT

#define ENABLE_AUDIO 1
#define ENABLE_SOUNDS 1
//#define ENABLE_MUSIC 1

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <stdarg.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#ifdef ENABLE_AUDIO
#include <SDL2/SDL_mixer.h>
#endif
#include <SDL2/SDL_ttf.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

extern int ScreenWidth, ScreenHeight;
extern SDL_Window *window;
extern SDL_Renderer *ScreenRenderer;
extern int retraces;
extern int TILE_W, TILE_H;
extern SDL_Renderer *ScreenRenderer;
extern SDL_Texture *TileSheet;
extern int DirX[];
extern int DirY[];

#ifdef ENABLE_AUDIO
extern Mix_Chunk *SampleSwap, *SampleDrop, *SampleDisappear, *SampleMove;
#endif

enum GameTypes {
  FRENZY,
  AVALANCHE,
  PILLARS,
  DICE_MATCH,
  REVERSI_BALL,
  COOKIE,
};

enum Directions {
  EAST,
  SOUTHEAST,
  SOUTH,
  SOUTHWEST,
  WEST,
  NORTHWEST,
  NORTH,
  NORTHEAST
};

enum GameKey {
  KEY_LEFT,
  KEY_DOWN,
  KEY_UP,
  KEY_RIGHT,
  KEY_OK,
  KEY_BACK,
  KEY_PAUSE,
  KEY_ACTION,
  KEY_ROTATE_L, // counter clockwise
  KEY_ROTATE_R, // clockwise
  KEY_LIFT,
  KEY_COUNT,

  KEY_SWAP = KEY_ACTION
};

enum BlockColor {
  BLOCK_EMPTY,
  BLOCK_RED,
  BLOCK_GREEN,
  BLOCK_YELLOW,
  BLOCK_CYAN,
  BLOCK_PURPLE,
  BLOCK_BLUE,
  BLOCK_EXTRA1,
  BLOCK_EXTRA2,
  BLOCK_METAL,
  BLOCK_GARBAGE,
  BLOCK_GRAY,
  BLOCK_DISABLED
};

enum GameplayOptions {
  SWAP_INSTANTLY = 1,
  LIFT_WHILE_CLEARING = 2,
  PULL_BLOCK_HORIZONTAL = 4
};

struct FallingChunk {
  int Timer, X, Y, Height;
  struct FallingChunk *Next;
};

struct GarbageSlab {
  int X, Y, Width, Height;
  struct GarbageSlab *Next;
};

struct MatchRow {
  int Color; // color of the row
  int X, Y;  // position in the playfield of the row
  int Width; // how many blocks are in the row
  int Timer1; // timer for how long until blocks disappear
  int Timer2; // timer for making individual blocks disappear
  int DisplayX;     // \ used to make blocks gradually disappear
  int DisplayWidth; // /
  struct MatchRow *Child, *Next;
};

struct Playfield {
  int GameType;
  int Width, Height;
  int CursorX, CursorY;
  int Paused;
  uint32_t Flags;
  int KeyDown[KEY_COUNT];
  int KeyLast[KEY_COUNT];
  int KeyNew[KEY_COUNT];
  int KeyRepeat[KEY_COUNT];
  int *Playfield;

  // Frenzy
  int Rise;
  int SwapTimer, SwapColor1, SwapColor2;
  struct MatchRow *Match;
  struct FallingChunk *FallingData;

  // Falling blocks
  int FallTimer;
  int LockTimer;
  int Active;
  int Direction; // for Avalanche
  int SwapColor3; // for pillars

  // Game rules
  int MinMatchSize; // how many matching tiles are needed
  int ColorCount;
};

void SDL_MessageBox(int Type, const char *Title, SDL_Window *Window, const char *fmt, ...);
void strlcpy(char *Destination, const char *Source, int MaxLength);
SDL_Surface *SDL_LoadImage(const char *FileName, int Flags);
SDL_Texture *LoadTexture(const char *FileName, int Flags);
void rectfill(SDL_Renderer *Bmp, int X1, int Y1, int X2, int Y2);
void rect(SDL_Renderer *Bmp, int X1, int Y1, int X2, int Y2);
void sblit(SDL_Surface* SrcBmp, SDL_Surface* DstBmp, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height);
void blit(SDL_Texture* SrcBmp, SDL_Renderer* DstBmp, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height);
void blitf(SDL_Texture* SrcBmp, SDL_Renderer* DstBmp, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height, SDL_RendererFlip Flip);
void blitz(SDL_Texture* SrcBmp, SDL_Renderer* DstBmp, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height, int Width2, int Height2);
void blitfull(SDL_Texture* SrcBmp, SDL_Renderer* DstBmp, int DestX, int DestY);
void UpdatePlayfield(struct Playfield *P);
int GetTile(struct Playfield *P, int X, int Y);
void SetTile(struct Playfield *P, int X, int Y, int Value);
void RandomizeRow(struct Playfield *P, int y);

int RandomTileColor(struct Playfield *P);
void UpdatePuzzleFrenzy(struct Playfield *P);
void UpdateAvalanche(struct Playfield *P);
void UpdatePillars(struct Playfield *P);
void UpdateCookie(struct Playfield *P);
void UpdateReversiBall(struct Playfield *P);
void UpdateDiceMatch(struct Playfield *P);
int Random(int Choices);

int CountConnected(struct Playfield *P, int X, int Y, int *Used);
void ClearConnected(struct Playfield *P, int X, int Y);
int MakeBlocksFall(struct Playfield *P);
int TestBlocksFall(struct Playfield *P);
int ClearAvalancheStyle(struct Playfield *P);