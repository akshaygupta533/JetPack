#include "main.h"

#ifndef LIFE_H
#define LIFE_H


class Life {
public:
    Life() {}
    Life(float x, float y, int health);
    glm::vec3 position;
    float rotation;
    int type;
    bounding_box_t bb;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    bool will_draw;
private:
    VAO *object;
    VAO *object1;
};

#endif  //PLATFORM_H
