#include "coin.h"
#include "main.h"
#include <cmath>

Coin::Coin(float x, float y, int type) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->will_draw = true;
    this->bb.x=x;
    this->bb.y=y;
    this->bb.width=this->bb.height=0.2;
    this->type=type;
    this->xvel = -0.07;
    this->yvel = 0.07;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    
    float angle = 0;
    float rot_angle = (2*3.14159265359)/100;

    GLfloat vertex_buffer_data[100*9];
    for(int i=0;i<100;i++){
        vertex_buffer_data[9*i] = 0.0;
        vertex_buffer_data[9*i+1] = 0.0;
        vertex_buffer_data[9*i+2] = 0;

        vertex_buffer_data[9*i+3] = 0.1*cos(angle);
        vertex_buffer_data[9*i+4] = 0.1*sin(angle);
        vertex_buffer_data[9*i+5] = 0;

        vertex_buffer_data[9*i+6] = 0.1*cos(angle+rot_angle);
        vertex_buffer_data[9*i+7] = 0.1*sin(angle+rot_angle);
        vertex_buffer_data[9*i+8] = 0;

        angle+=rot_angle;
    }

    if(type==1)
    this->object = create3DObject(GL_TRIANGLES, 100*3, vertex_buffer_data, COLOR_COIN_OUT, GL_FILL);
    else if(type==2)
    this->object = create3DObject(GL_TRIANGLES, 100*3, vertex_buffer_data, COLOR_COIN_IN, GL_FILL);
    else if(type==4)
    this->object = create3DObject(GL_TRIANGLES, 100*3, vertex_buffer_data, COLOR_GREEN, GL_FILL);
    else if(type==3)
    this->object = create3DObject(GL_TRIANGLES, 100*3, vertex_buffer_data, COLOR_BALLON, GL_FILL);

}

void Coin::draw(glm::mat4 VP) {
    if(this->type==1 || this->type==2)
    this->rotation+=1;
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    //rotate = rotate * glm::translate(glm::vec3(0, 0, -0.1));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Coin::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}
