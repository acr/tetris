add_executable(nt main.cpp TransformStack.cpp DrawableHierarchy.cpp NullDrawable.cpp
  HollowRectangle.cpp FilledSquare.cpp PieceI.cpp PieceJ.cpp PieceL.cpp PieceO.cpp
  PieceS.cpp PieceT.cpp PieceZ.cpp GameUI.cpp Grid.cpp GridSquare.cpp ActivePiece.cpp
  TextRenderer.cpp)
target_link_libraries(nt ${GL_LIB} ${SDL2_MAIN} ${SDL2_LIB} ${GLEW_LIBRARY} ft2 gl_utils)