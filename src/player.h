#include "main.h"

#ifndef PLAYER_H
#define PLAYER_H


class Player {
public:
    Player() {}
    Player(float x, float y, color_t color);
    glm::vec3 position;
    int health;
    float velocity;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick_left();
    void tick_right();
    void gravity();
    void jet();
    double speed;
    bounding_box_t bb;
private:
    VAO *object;
};

#endif // BALL_H
