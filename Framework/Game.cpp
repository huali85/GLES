//
// Created by lihua_000 on 3/1/2017.
//

#include "Game.h"

Game* g_game = nullptr;

void Game::Resize(int width, int height) {
    _timer.Reset();
    _width = width;
    _height = height;
    glViewport(0, 0, width, height);
}

void Game::Step() {
    double dt = _timer.GetDeltaTime();
    Update((float)dt);
}

void Game::Render() {
    Step();

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Draw();
}

void Game::OnTouchMove(float dx, float dy, bool is_left) {
    if (is_left) {
        _left_input.x = dx;
        _left_input.y = dy;
    } else {
        _right_input.x = dx;
        _right_input.y = dy;
    }
}
