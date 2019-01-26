#include "life.h"
#include "main.h"
#include <cmath>

Life::Life(float x, float y, int health) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->will_draw = true;
    this->bb.x=x;
    this->bb.y=y;
    this->bb.width=this->bb.height=0.2;
    this->type=type;
    


    GLfloat vertex_buffer_data[]={
        -1, -0.05,0,
        -1, 0.05, 0,
        -1+0.2*health, 0.05, 0,

        -1+0.2*health, 0.05, 0,
        -1+0.2*health, -0.05, 0,
        -1, -0.05,0,
    };
    GLfloat vertex_buffer_data1[]={
        -1.16,0.05,0,
        -1.14,0.05,0,
        -1.14,-0.05,0,

        -1.14,-0.05,0,
        -1.16,-0.05,0,
        -1.16,0.05,0,

        -1.1,0.01,0,
        -1.1,-0.01,0,
        -1.2,-0.01,0,

        -1.2,-0.01,0,
        -1.2,0.01,0,
        -1.1,0.01,0,

    };


    this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, COLOR_BALLON, GL_FILL);
    this->object1 = create3DObject(GL_TRIANGLES, 4*3, vertex_buffer_data1, COLOR_GREEN, GL_FILL);

    

}

void Life::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    //rotate = rotate * glm::translate(glm::vec3(0, 0, -0.1));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->object1);
}

void Life::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}
