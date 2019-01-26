#include "main.h"

#ifndef RING_H
#define RING_H


class Ring {
public:
    Ring() {}
    Ring(float x, float y);
    glm::vec3 position;
    float rotation;
    bounding_box_t bb;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    bool will_draw;
private:
    VAO *object;
    VAO *object1;
};

#endif  //PLATFORM_H
