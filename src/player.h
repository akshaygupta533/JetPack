#include "main.h"

#ifndef PLAYER_H
#define PLAYER_H


class Player {
public:
    Player() {}
    Player(float x, float y, color_t color);
    glm::vec3 position;
    float velocity;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick_left();
    void tick_right();
    void gravity();
    void jet();
    double speed;
private:
    VAO *object;
};

#endif // BALL_H
