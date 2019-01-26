#include "main.h"

#ifndef FIRELINE_H
#define FIRELINE_H


class Fireline {
public:
    Fireline() {}
    Fireline(float x, float y, float angle);
    glm::vec3 position;
    float rotation;
    float angle;
    int up;
    bounding_box_t bb;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    bool will_draw;
private:
    VAO *object;
    VAO *object1;
    VAO *object2;
};

#endif  //PLATFORM_H
