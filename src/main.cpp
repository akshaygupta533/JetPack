#include "main.h"
#include "timer.h"
#include "player.h"
#include "platform.h"
#include "wall.h"
#include "coin.h"
#include "fireline.h"
#include "boomer.h"
#include "magnet.h"
#include "ring.h"
#include "ballon.h"
#include "life.h"
#include "segmentdisplay.h"
#include "digit.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Player player1;
Platform plat;
vector<Wall> wall;
vector<Coin> coins;
vector<Fireline> flines;
vector<Fireline> fbeams;
vector<Boomer> bmrng;
Magnet mgnt;
vector<Ring> ring;
vector<Ballon> bln;
vector<Coin> lpowerup;
vector<Coin> spowerup;
double dist_travelled = 0;
long long score = 0;
Life lif;
Segmentdisplay sc;
int multiplier = 1;
long long ini_dis = 0;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye ( 5*sin(camera_rotation_angle*M_PI/180.0f), 0, 5*cos(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    plat.draw(VP);
    for(int i=0;i<wall.size();i++)
        wall[i].draw(VP);
    lif.draw(VP);

    for(int i=0;i<coins.size();i++){
        if(coins[i].will_draw)
        coins[i].draw(VP);
    }

    for(int i=0;i<flines.size();i++)
    {   if(flines[i].will_draw)
        flines[i].draw(VP);
    }

    for(int i=0;i<fbeams.size();i++)
    {
        if(fbeams[i].will_draw)
        fbeams[i].draw(VP);
    }

    for(int i=0;i<bmrng.size();i++){
        bmrng[i].draw(VP);
    }
    if(mgnt.will_draw)
        mgnt.draw(VP);

    for(int i=0;i<ring.size();i++){
        ring[i].draw(VP);
    }
    if(!bln.empty())
        bln[0].draw(VP);
    
    for(int i=0;i<lpowerup.size();i++){
        lpowerup[i].draw(VP);
    }
    for(int i=0;i<spowerup.size();i++){
        spowerup[i].draw(VP);
    }
    
    player1.draw(VP);
    sc.draw(VP);
    sc.update(int(score));
}

void tick_input(GLFWwindow *window) {

    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_SPACE);
    int ballon = glfwGetKey(window, GLFW_KEY_S);
    

    if( !( !ring.empty() && detect_ring_col(player1.bb, ring[0].position.x, ring[0].position.y, 1.5)) ){

        if (left) {
            if(player1.position.x>=-3.8)
            player1.tick_left();
        }
        else if(right) {
            if(player1.position.x<=3.8)
            player1.tick_right();
        }

        if(up) {
            player1.jet();
        }
        else{
            player1.gravity();
        }
        if(ballon){
            if(bln.empty()){
                bln.push_back(Ballon(player1.position.x+0.2,player1.position.y));
                bln[0].xvel=0.05;
                bln[0].yvel=0.05;
            }

        }
    }
    else if(!ring.empty()) 
    {
        player1.velocity=0;
        player1.position.y = ring[0].position.y + abs(sqrt(1.5*1.5-(player1.position.x-ring[0].position.x)*(player1.position.x-ring[0].position.x)));
    }
}

void tick_elements() { 
    if(player1.health<=0){
        cout<<"Final Score: "<<score<<endl;
        exit(0);
    }

    lif = Life(-2.5,3.5,player1.health);
    sc.set_position(-0.5,3.2);
    /* Collisions */
    if( !( !ring.empty() && detect_ring_col(player1.bb, ring[0].position.x, ring[0].position.y, 1.5)) ){
        for(int i=0;i<coins.size();i++){
            if( coins[i].will_draw && detect_collision(player1.bb, coins[i].bb )){
                coins[i].will_draw=false;
                if(coins[i].type==1)score+=100;
                else score+=200;
            }
        }

        if(mgnt.will_draw==true){
            if(mgnt.position.y-player1.position.y>0)player1.position.y+=0.025;
            else if(mgnt.position.y-player1.position.y<0)  player1.position.y-=0.025;

            if(mgnt.position.x-1-player1.position.x>0)player1.position.x+=0.025;
            else  if(mgnt.position.x-1-player1.position.x<0) player1.position.x-=0.025;

        }

        for(int i=0;i<bmrng.size();i++){
            if(detect_collision(bmrng[i].bb,player1.bb)){
                player1.health-=1;
                bmrng.clear();
            }
        }
        for(int i=0;i<lpowerup.size();i++){
            if(detect_collision(lpowerup[i].bb,player1.bb)){
                player1.health=10;
                lpowerup.clear();
            }
        }
        for(int i=0;i<spowerup.size();i++){
            if(detect_collision(spowerup[i].bb,player1.bb)){
                multiplier=10;
                ini_dis=dist_travelled;
                spowerup.clear();
            }
        }

        for(int i=0;i<flines.size();i++){
            if( flines[i].will_draw && detect_fireline_col(player1.bb,flines[i].position.x,flines[i].position.y,flines[i].angle)){
                player1.health-=2;
                flines[i].will_draw=false;
            }

            if( (!bln.empty()) && detect_fireline_col(bln[0].bb,flines[i].position.x,flines[i].position.y,flines[i].angle)){
                flines.clear();
                bln.clear();
            }

        }

        for(int i=0;i<fbeams.size();i++){
            if( fbeams[i].will_draw && detect_fireline_col(player1.bb,fbeams[i].position.x,fbeams[i].position.y,fbeams[i].angle))
            {    player1.health-=2;
                 fbeams[i].will_draw=false;
            }

            if( fbeams[i].will_draw && (!bln.empty()) && detect_fireline_col(bln[0].bb,fbeams[i].position.x,fbeams[i].position.y,fbeams[i].angle)){
                fbeams[i].will_draw=false;
                bln.clear();
            }
        }
    }
    /* ends here */ 
    
    if(dist_travelled-ini_dis>=100)multiplier=1;
    dist_travelled+=0.05;
    score += 1*multiplier;
   
    /* Random generation */
    float hght;
    float ang;
    if(coins.empty()){
        hght = rand()%6 - 2.5;
        for(int i=0;i<5;i++)
            coins.push_back(Coin(9+i*0.5,hght,1));
        for(int i=5;i<10;i++)
            coins.push_back(Coin(9+i*0.5,hght,2));
    }
    else{
        if(coins[9].position.x<=-4.1)
            coins.clear();
    }
    hght = rand()%4 - 1;
    ang = rand()%130 - 65;
    if(flines.empty()){
        flines.push_back(Fireline(10,hght,ang*M_PI/180));
        fbeams.push_back(Fireline(15,2,0));
        fbeams.push_back(Fireline(15,-2,0));
        fbeams[0].up=0;
        fbeams[1].up=1;
    }
    else{
        if(flines[0].position.x<=-10)
        {
            flines.clear();
            fbeams.clear();
        }
            
    }

    if(int(dist_travelled)%20==0 && bmrng.empty()){
        bmrng.push_back(Boomer(3,3));
        bmrng[0].xvel = -0.1;
        bmrng[0].yvel = -0.03;
    }
    else if(!bmrng.empty() && bmrng[0].position.y<=-3){
        bmrng.clear();
    }
    
    if( int(dist_travelled)%60==0 )
        mgnt.will_draw=true;
    else if( int(dist_travelled)%30==10 )
        mgnt.will_draw=false;

    if(int(dist_travelled)%40==0 && ring.empty())
        ring.push_back(Ring(6,0));
    else if(!ring.empty() && ring[0].position.x<=-6)
        ring.clear();
    
    if(lpowerup.empty() && int(dist_travelled)%50==0){
        lpowerup.push_back(Coin(2,0,3));
    }
    else if(!lpowerup.empty() && lpowerup[0].position.y<=-3){
        lpowerup.clear();
    }

    if(spowerup.empty() && int(dist_travelled)%90==0){
        spowerup.push_back(Coin(2,0,4));
    }
    else if(!spowerup.empty() && spowerup[0].position.y<=-3){
        spowerup.clear();
    } 

    /* ends here */
    
    if(!bln.empty()){
        bln[0].gravity();
        if(bln[0].position.y<=-2.6)
            bln.erase(bln.begin());
    }
    
    for(int i=0;i<wall.size();i++)
        wall[i].position.x-=0.05;
    for(int i=0;i<coins.size();i++){
        coins[i].position.x-=0.05;
        coins[i].bb.x-=0.05;
    }

    for(int i=0;i<bmrng.size();i++){
        bmrng[i].xvel+=0.001;
        bmrng[i].position.x+=bmrng[i].xvel;
        bmrng[i].position.y+=bmrng[i].yvel;
        bmrng[i].bb.x=bmrng[i].position.x;
        bmrng[i].bb.y=bmrng[i].position.y;
    }

    for(int i=0;i<lpowerup.size();i++){
        lpowerup[i].yvel-=0.002;
        lpowerup[i].position.x+=lpowerup[i].xvel;
        lpowerup[i].position.y+=lpowerup[i].yvel;
        lpowerup[i].bb.x=lpowerup[i].position.x;
        lpowerup[i].bb.y=lpowerup[i].position.y;
    }

    for(int i=0;i<spowerup.size();i++){
        spowerup[i].yvel-=0.002;
        spowerup[i].position.x+=spowerup[i].xvel;
        spowerup[i].position.y+=spowerup[i].yvel;
        spowerup[i].bb.x=spowerup[i].position.x;
        spowerup[i].bb.y=spowerup[i].position.y;
    }

    for(int i=0;i<flines.size();i++){
        flines[i].position.x-=0.05;
        }
    for(int i=0;i<fbeams.size();i++){
        fbeams[i].position.x-=0.05;
        }
    for(int i=0;i<ring.size();i++){
        ring[i].position.x-=0.05;
    }

    if(wall[0].position.x<=-8){
        wall.erase(wall.begin());
        wall.push_back(Wall(8,0,COLOR_WALL));
    }

    for(int i=0;i<fbeams.size();i++){
        if(fbeams[i].up==1)fbeams[i].position.y+=0.02;
        else fbeams[i].position.y-=0.02;
        if(fbeams[i].position.y>=2)fbeams[i].up=0;
        if(fbeams[i].position.y<=-2)fbeams[i].up=1;
    }
}
/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    player1 = Player(-3, 0, COLOR_PLAYER);
    lif = Life(-2.5,3.5,player1.health);
    plat = Platform(0,-3,COLOR_BLACK);
    wall.push_back(Wall(0,0,COLOR_WALL));
    wall.push_back(Wall(8,0,COLOR_WALL));
    // bmrng = Boomer(4,3);
    mgnt = Magnet(3,2);
    // ring = Ring(10,0);
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 1000;//ensure equal to the height component(to maintain 1:1 aspect ratio)
    int height = 1000;

    window = initGLFW(width, height);

    initGL (window, width, height);
    

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

bool detect_fireline_col(bounding_box_t a, float x, float y, float angle){

    if(a.x-a.width/2 >= x-1.5*cos(angle) && a.x+a.width/2 <=x+1.5*cos(angle))
    {
        if( dis_line(tan(angle),-1,y-x*tan(angle),a.x+a.width/2,a.y+a.height/2) <= 0.1 ||
            dis_line(tan(angle),-1,y-x*tan(angle),a.x+a.width/2,a.y-a.height/2) <= 0.1 ||
            dis_line(tan(angle),-1,y-x*tan(angle),a.x-a.width/2,a.y+a.height/2) <= 0.1 ||
            dis_line(tan(angle),-1,y-x*tan(angle),a.x-a.width/2,a.y-a.height/2) <= 0.1 ||
            dis_line(tan(angle),-1,y-x*tan(angle),a.x,a.y) <= 0.1
         )return true;
         else 
         return false;
    }
}
bool detect_ring_col(bounding_box_t a,float x,float y,float radius){
    if ( dis_two_points(a.x+a.width/2,a.y+a.height/2,x,y)<=radius ||
         dis_two_points(a.x-a.width/2,a.y+a.height/2,x,y)<=radius ||
         dis_two_points(a.x+a.width/2,a.y-a.height/2,x,y)<=radius ||
         dis_two_points(a.x-a.width/2,a.y-a.height/2,x,y)<=radius 
         )
         return true;
    else 
        return false;
}

float dis_line(float a,float b,float c, float x, float y){
    return abs( (a*x+b*y+c)/sqrt(a*a+b*b) );
}

float dis_two_points(float x1,float y1,float x2,float y2){
    return (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1);
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
