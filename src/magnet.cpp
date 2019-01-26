#include "magnet.h"
#include "main.h"
#include <cmath>

Magnet::Magnet(float x, float y) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->will_draw = true;
    this->bb.x=x;
    this->bb.y=y;
    this->bb.width=this->bb.height=0.2;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    float angle = -M_PI/2;
    float rot_angle = (M_PI)/100;
    GLfloat vertex_buffer_data[100*9];
    for(int i=0;i<100;i++){
        vertex_buffer_data[9*i] = 0.0;
        vertex_buffer_data[9*i+1] = 0.0;
        vertex_buffer_data[9*i+2] = 0.0;

        vertex_buffer_data[9*i+3] = 0.5*cos(angle);
        vertex_buffer_data[9*i+4] = 0.5*sin(angle);
        vertex_buffer_data[9*i+5] = 0.0;

        vertex_buffer_data[9*i+6] = 0.5*cos(angle+rot_angle);
        vertex_buffer_data[9*i+7] = 0.5*sin(angle+rot_angle);
        vertex_buffer_data[9*i+8] = 0.0;

        angle+=rot_angle;
    }
    GLfloat vertex_buffer_data1[]={
        0, 0.5, 0,
        0, 0.25, 0,
        -0.5,0.25, 0,

        -0.5,0.25, 0,
        -0.5,0.5, 0,
        0, 0.5, 0,

        0, -0.5, 0,
        0, -0.25, 0,
        -0.5,-0.25, 0,

        -0.5,-0.25, 0,
        -0.5,-0.5, 0,
        0, -0.5, 0,

    };
    
    GLfloat vertex_buffer_data2[]={
        -0.5, 0.5, 0,
        -0.5, 0.25, 0,
        -1,0.25, 0,

        -1,0.25, 0,
        -1,0.5, 0,
        -0.5, 0.5, 0,

        -0.5, -0.5, 0,
        -0.5, -0.25, 0,
        -1,-0.25, 0,

        -1,-0.25, 0,
        -1,-0.5, 0,
        -0.5, -0.5, 0,

    };

    this->object = create3DObject(GL_TRIANGLES, 100*3, vertex_buffer_data, COLOR_CRIMSON, GL_FILL);
    this->object1 = create3DObject(GL_TRIANGLES, 4*3, vertex_buffer_data1, COLOR_CRIMSON, GL_FILL);
    this->object2 = create3DObject(GL_TRIANGLES, 4*3, vertex_buffer_data2, COLOR_BLACK, GL_FILL);
    
}

void Magnet::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->object1);
    draw3DObject(this->object2);

}

void Magnet::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}
