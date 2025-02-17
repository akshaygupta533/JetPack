#include "main.h"

#ifndef COIN_H
#define COIN_H


class Coin {
public:
    Coin() {}
    Coin(float x, float y, int type);
    glm::vec3 position;
    float rotation;
    int type;
    float xvel;
    float yvel;
    bounding_box_t bb;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    bool will_draw;
private:
    VAO *object;
    VAO *object1;
};

#endif  //PLATFORM_H
