#include "player.h"
#include "main.h"

Player::Player(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->speed = 0.05;
    this->velocity = 0;
    this->bb.x=x;
    this->bb.y=y;
    this->bb.height=1.1;
    this->bb.width=0.3;
    this->health = 10;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        0.1f, 0.3f, 0.0f,
        0.1f, -0.3f, 0.0f,
        -0.1f, -0.3f, 0.0f,

        -0.1f, -0.3f, 0.0f,
        0.1f, 0.3f, 0.0f,
        -0.1f, 0.3f, 0.0f,

        0.0f, -0.4f, 0.0f,
        0.0f, -0.3f, 0.0f,
        0.15f, -0.4f, 0.0f,

        0.15, -0.3f, 0.0f,
        0.15, -0.4f, 0.0f,
        0.0f, -0.3f, 0.0f,

        0.15f, -0.3f, 0.0f,
        0.15f, -0.4f, 0.0f,
        0.2f, -0.4f, 0.0f,

        -0.1f, 0.3f, 0.0f,
        0.0f, 0.3f, 0.0f,
        0.0f, 0.4f, 0.0f,

        0.1f, 0.3f, 0.0f,
        0.0f, 0.3f, 0.0f,
        0.0f, 0.4f, 0.0f,

        0.0f, 0.4f, 0.0f,
        0.1f, 0.4f, 0.0f,
        0.1f, 0.3f, 0.0f,

        0.0f, 0.4f,0.0f,
        0.15f, 0.4f, 0.0f,
        0.15f, 0.6f, 0.0f,

        0.0f, 0.4f,0.0f,
        0.0f, 0.6f, 0.0f,
        0.15f, 0.6f, 0.0f,

        0.0f, 0.6f, 0.0f,
        0.15f, 0.6f, 0.0f,
        0.15f, 0.65f, 0.0f,

        0.15f, 0.65f, 0.0f,
        0.0f, 0.65f, 0.0f,
        0.0f, 0.6f, 0.0f,

        0.15f, 0.6f, 0.0f,
        0.2f, 0.6f, 0.0f,
        0.15f, 0.65f, 0.0f,

        0.15f, 0.65f, 0.0f,
        0.2f, 0.65f, 0.0f,
        0.15f, 0.6f, 0.0f,

        0.15f, 0.65f, 0.0f,
        0.2f, 0.65f, 0.0f,
        0.2f, 0.7f, 0.0f,

        -0.07f,-0.325f,0.0f,
        -0.07f,-0.4f,0.0f,
        -0.09f,-0.325f,0.0f,

        -0.09f,-0.325f,0.0f,
        -0.09f,-0.4f,0.0f,
        -0.07f,-0.4f,0.0f,

        -0.04f,-0.325f,0.0f,
        -0.04f,-0.4f,0.0f,
        -0.06f,-0.325f,0.0f,

        -0.06f,-0.325f,0.0f,
        -0.06f,-0.4f,0.0f,
        -0.04f,-0.4f,0.0f,

    };

    this->object = create3DObject(GL_TRIANGLES, 19*3, vertex_buffer_data, color, GL_FILL);
}

void Player::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Player::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Player::tick_left() {
    //this->rotation += this->speed;
     this->position.x -= this->speed;
     this->bb.x=this->position.x;
}

void Player::tick_right() {
    //this->rotation += this->speed;
     this->position.x += this->speed/2;
     this->bb.x=this->position.x;
}

void Player::jet() {
    this->velocity = this->speed;
    if(this->position.y<3)
    this->position.y += this->speed;
    else
    this->velocity=0;
    this->bb.x=this->position.x;
    this->bb.y=this->position.y+0.45;
}
void Player::gravity() {
    if(this->position.y>3)this->velocity = 0;
    this->velocity+= -this->speed/25;
    if(this->position.y-0.4<-2.9)this->velocity=0;
    this->position.y += this->velocity;
    this->bb.x=this->position.x;
    this->bb.y=this->position.y+0.45;
}
