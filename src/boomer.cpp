#include "boomer.h"
#include "main.h"
#include <cmath>

Boomer::Boomer(float x, float y) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->will_draw = true;
    this->bb.x=x;
    this->bb.y=y;
    this->bb.width=0.5;
    this->bb.height=0.5;
    this->acc = 0.001;
    this->xvel = -0.1;
    this->yvel = -0.03;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    float rot_angle = (2*M_PI)/100;
    GLfloat vertex_buffer_data[]={
        0,0.1,0,
        0,-0.1,0,
        0.2,0,0,

        0.2,0,0,
        0,0.1,0,
        -0.2,0.5,0,

        0.2,0,0,
        0,-0.1,0,
        -0.2,-0.5,0,
    };

    this->object = create3DObject(GL_TRIANGLES, 3*3, vertex_buffer_data, COLOR_FIRE, GL_FILL);
    
}

void Boomer::draw(glm::mat4 VP) {
    this->rotation+=2;
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);

}

void Boomer::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}
