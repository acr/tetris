#ifndef GAME_UI_H
#define GAME_UI_H

#include "gl_defns.h"
#include "gl_utils.h"
#include "DrawableHierarchy.h"
#include "ActivePiece.h"
#include "NullDrawable.h"
#include "Grid.h"
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
  const GLint model_uniform_location;

  gfx::DrawableHierarchy* root;
  gfx::ActivePiece* active_piece;
  const float block_area_width;
  const float block_area_height;
  const int width;
  const int height;
  gfx::Grid* grid;
  gfx::NullDrawable* block_scale_area;

  std::set<gfx::DrawableHierarchy*> allocated_drawables;

  bool handle_user_input();
  void scanGridForMatches();
  bool isGameOver();
  gfx::ActivePiece* generateNewPiece();

public:
  GameUI(SDL_Window* window, GLint mul, int _width, int _height);
  ~GameUI();
  void run();
};

}

#endif
