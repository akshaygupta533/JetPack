#include "fireline.h"
#include "main.h"
#include <cmath>

Fireline::Fireline(float x, float y, float angle) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->will_draw = true;
    this->bb.x=x;
    this->bb.y=y;
    this->bb.width=this->bb.height=0.2;
    this->angle = angle;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    float rot_angle = (2*M_PI)/100;

    GLfloat vertex_buffer_data[] = {

        1.5*cos(angle+M_PI/100),1.5*sin(angle+M_PI/100),0,
        1.5*cos(angle-M_PI/100),1.5*sin(angle-M_PI/100),0,
        -1.5*cos(angle+M_PI/100),-1.5*sin(angle+M_PI/100),0,

        1.5*cos(angle+M_PI/100),1.5*sin(angle+M_PI/100),0,
        -1.5*cos(angle+M_PI/100),-1.5*sin(angle+M_PI/100),0,
        -1.5*cos(angle-M_PI/100),-1.5*sin(angle-M_PI/100),0,
        };


    GLfloat vertex_buffer_data1[100*9];
    GLfloat vertex_buffer_data2[100*9];
    GLfloat circlecentres[2][3]={
        {1.5*cos(angle),1.5*sin(angle),0},
        {-1.5*cos(angle),-1.5*sin(angle),0},
    };

    angle = 0;

    for(int i=0;i<100;i++){
        vertex_buffer_data1[9*i] = circlecentres[0][0];
        vertex_buffer_data1[9*i+1] = circlecentres[0][1];
        vertex_buffer_data1[9*i+2] = circlecentres[0][2];

        vertex_buffer_data1[9*i+3] = 0.1*cos(angle) + circlecentres[0][0];
        vertex_buffer_data1[9*i+4] = 0.1*sin(angle) + circlecentres[0][1];
        vertex_buffer_data1[9*i+5] = 0.0;

        vertex_buffer_data1[9*i+6] = 0.1*cos(angle+rot_angle) + circlecentres[0][0];
        vertex_buffer_data1[9*i+7] = 0.1*sin(angle+rot_angle) + circlecentres[0][1];
        vertex_buffer_data1[9*i+8] = 0.0;

        angle+=rot_angle;
    }
    
    angle = 0;

    for(int i=0;i<100;i++){
        vertex_buffer_data2[9*i] = circlecentres[1][0];
        vertex_buffer_data2[9*i+1] = circlecentres[1][1];
        vertex_buffer_data2[9*i+2] = circlecentres[1][2];

        vertex_buffer_data2[9*i+3] = 0.1*cos(angle) + circlecentres[1][0];
        vertex_buffer_data2[9*i+4] = 0.1*sin(angle) + circlecentres[1][1];
        vertex_buffer_data2[9*i+5] = 0.0;

        vertex_buffer_data2[9*i+6] = 0.1*cos(angle+rot_angle) + circlecentres[1][0];
        vertex_buffer_data2[9*i+7] = 0.1*sin(angle+rot_angle) + circlecentres[1][1];
        vertex_buffer_data2[9*i+8] = 0.0;

        angle+=rot_angle;
    }

    this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, COLOR_FIRE, GL_FILL);
    this->object1 = create3DObject(GL_TRIANGLES, 100*3, vertex_buffer_data1, COLOR_COIN_OUT, GL_FILL);
    this->object2 = create3DObject(GL_TRIANGLES, 100*3, vertex_buffer_data2, COLOR_COIN_OUT, GL_FILL);

}

void Fireline::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->object1);
    draw3DObject(this->object2);
}

void Fireline::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}
