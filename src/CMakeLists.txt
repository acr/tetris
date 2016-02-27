add_executable(nt main.cpp)
target_link_libraries(nt ${GL_LIB} ${SDL2_MAIN} ${SDL2_LIB} ${GLEW_LIBRARY})