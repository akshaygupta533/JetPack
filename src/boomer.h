#include "main.h"

#ifndef BOOMER_H
#define BOOMER_H


class Boomer {
public:
    Boomer() {}
    Boomer(float x, float y);
    glm::vec3 position;
    float rotation;
    float acc;
    float xvel;
    float yvel;
    int up;
    bounding_box_t bb;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    bool will_draw;
private:
    VAO *object;
};

#endif  //PLATFORM_H
