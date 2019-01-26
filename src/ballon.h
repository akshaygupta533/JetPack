#include "main.h"

#ifndef BALLON_H
#define BALLON_H


class Ballon {
public:
    Ballon() {}
    Ballon(float x, float y);
    glm::vec3 position;
    float rotation;
    float xvel;
    float yvel;
    void gravity();
    float speed;
    bounding_box_t bb;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    bool will_draw;
private:
    VAO *object;
    VAO *object1;
};

#endif  //PLATFORM_H
