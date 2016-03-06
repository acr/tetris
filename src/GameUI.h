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
private:
  enum CustomEvent {
    UPDATE_POSITION,
    NUM_CUSTOM_EVENTS
  };

  SDL_Window* sdl_window;
  GLint model_mat_location;

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
  std::string notificationText;
  SDL_mutex* mutex;
  SDL_TimerID timerID;
  std::string debugText;

  std::set<gfx::DrawableHierarchy*> allocated_drawables;

  bool initNewPieceAndScanForGameEnd();
  bool handle_user_input();
  void scanGridForMatches();
  bool isGameOver();
  gfx::ActivePiece* generateNewPiece();
  void renderTextBoxes(gfx::TextRenderer& textRenderer);
  void incrementScoreByLevel(int basePoints);

public:
  GameUI(SDL_Window* window, int _width, int _height);
  ~GameUI();
  void run();

  Uint32 timerCallback();
};

}

#endif
