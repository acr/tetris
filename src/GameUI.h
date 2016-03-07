#ifndef GAME_UI_H
#define GAME_UI_H

#include "gl_defns.h"
#include "gl_utils.h"
#include "DrawableHierarchy.h"
#include "ActivePiece.h"
#include "NullDrawable.h"
#include "Grid.h"
#include "TextRenderer.h"
#include <SDL2/SDL_mutex.h>
#include <SDL2/SDL_thread.h>
#include <set>

namespace gui {

class GameUI {
public:
  enum CustomEvent {
    UPDATE_POSITION,
    KEYPRESS_UPDATE,
    NUM_CUSTOM_EVENTS
  };

  enum KeyPresses {
    LEFT,
    RIGHT,
    DOWN,
    NUM_KEY_PRESSES
  };

private:

  SDL_Window* sdl_window;
  GLint model_mat_location;

  gfx::TextRenderer* textRenderer;
  gfx::DrawableHierarchy* root;
  gfx::ActivePiece* active_piece;
  const float block_area_width;
  const float block_area_height;
  const int width;
  const int height;
  gfx::Grid* grid;
  gfx::NullDrawable* block_scale_area;
  GLint font_shader_program;
  GLint default_shader_program;
  int score;
  int level;
  Uint32 game_start_ticks;
  bool last_clear_was_a_tetris;
  Uint32 notification_start_ticks;
  std::string notification_text;
  SDL_mutex* mutex;
  SDL_TimerID block_move_timer_id;
  SDL_TimerID keypress_timer_id;
  std::string debug_text;
  bool is_game_over;
  int keys_pressed[NUM_KEY_PRESSES];

  std::set<gfx::DrawableHierarchy*> allocated_drawables;

  bool initSelf();
  bool destroySelf();
  bool confirmPlayAgain();
  bool initNewPieceAndScanForGameEnd();
  bool handleUserInput();
  void scanGridForMatches();
  gfx::ActivePiece* generateNewPiece();
  void renderTextBoxes();
  void incrementScoreByLevel(int basePoints);

public:
  GameUI(SDL_Window* window, int _width, int _height);
  ~GameUI();
  void run();

  Uint32 blockMoveTimerCallback();
};

}

#endif
