//
// Created by lihua_000 on 3/1/2017.
//

#ifndef GAME_H
#define GAME_H

#include "glm/glm.hpp"
#include "Platform.h"

class Game {
public:
    virtual ~Game() {}

    virtual bool Init() = 0;
    virtual void Resize(int width, int height);
    void Render();

    void OnTouchMove(float dx, float dy);

protected:
    Game() {}

    virtual void Update(float dt) {};
    virtual void Draw() = 0;

private:
    void Step();

protected:
    int     _width;
    int     _height;
    Timer   _timer;

    // Input
    glm::vec2 _input;
};

extern Game* CreateGame();

#endif //GAME_H
