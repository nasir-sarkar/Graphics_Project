#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <stdlib.h>
using namespace std;


///Variables
bool fullscreen = false;
int windowWidth = 1300, windowHeight = 700;
int windowPosX = 100, windowPosY = 100;
bool isDark = false;
bool isNight = false;
bool isLighting = false;



///circle
void circle(float rad, float a, float b)
{
    glBegin(GL_POLYGON);
    for (int i = 0; i < 200; i++)
    {
        float pi = 3.1416;
        float A = (i * 2 * pi) / 200;
        float r = rad;
        float x = r * cos(A);
        float y = r * sin(A);
        glVertex2d(x + a, y + b);
    }
    glEnd();
}


void circleBorder(float rad, float a, float b)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 200; i++)
    {
        glColor3ub(0, 0, 0);
        float pi = 3.1416;
        float A = (i * 2 * pi) / 200;
        float r = rad;
        float x = r * cos(A);
        float y = r * sin(A);
        glVertex2d(x + a, y + b);
    }
    glEnd();
}





//......................................................................................//
///Rain

bool isRaining = false;


struct RainDrop {
    float x, y;
    float speed;
};
vector<RainDrop> rainDrops;


void initRain() {
    rainDrops.clear();
    for (int i = 0; i < 1000; i++)
    {
        RainDrop drop;
        drop.x = -5.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(25.0f)));
        drop.y = 8.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(5.0f)));
        drop.speed = 0.1f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(0.15f)));
        rainDrops.push_back(drop);
    }
}

void drawRain() {                 ///9001
    if (!isRaining) return;

    glColor3ub(255, 255, 255);
    glLineWidth(1.0);
    glBegin(GL_LINES);
    for (const auto& drop : rainDrops)
    {
        glVertex2f(drop.x, drop.y);
        glVertex2f(drop.x - 0.07f, drop.y - 0.2f);
    }
    glEnd();
}


void updateRain() {
    if (!isRaining) return;

    for (auto& drop : rainDrops)
    {
        drop.y -= drop.speed;
        if (drop.y < 0)
        {
            drop.y = 8.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(5.0f)));
            drop.x = -5.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(25.0f)));
        }
    }
}

void updateScene(int value) {        ///8001
    updateRain();

    glutPostRedisplay();
    glutTimerFunc(5, updateScene, 0);
}




//......................................................................................//
///Sky
void sky(){            ///9002

    if(isDark) {
        glColor3ub(10, 24, 55);
    }
    else if(isNight) {
        glColor3ub(5, 10, 30);  // Very dark blue for night
    }
    else {
        glColor3ub(148, 232, 255);
    }


    glBegin(GL_QUADS);
    glVertex2f(-5, 5.5);
    glVertex2f(-5, 8);
    glVertex2f(20, 8);
    glVertex2f(20, 5.5);
    glEnd();

}




//......................................................................................//
///Cloud


float cloudPositions[6] = {-3.0f, 1.0f, 5.0f, 9.0f, 14.0f, 18.0f};
const int NUM_CLOUDS = 6;
int cloudUpdateInterval = 80; // Default interval
bool cloudsMoving = true;


void drawCloud(float xPos) {

    if(isRaining || isNight) {
        glColor3ub(96, 123, 136);
    }
    else {
        glColor3ub(255, 255, 255);
    }

    // Middle circle
    glBegin(GL_POLYGON);
    for(int i=0; i<200; i++) {
        float pi = 3.1416;
        float A = (i*2*pi)/200;
        float r = 0.2646;
        float x = r * cos(A) + xPos + 0.25;
        float y = r * sin(A) + 7.6;
        glVertex2f(x,y);
    }
    glEnd();

    // Left circle
    glBegin(GL_POLYGON);
    for(int i=0; i<200; i++) {
        float pi = 3.1416;
        float A = (i*2*pi)/200;
        float r = 0.2646;
        float x = r * cos(A) + xPos;
        float y = r * sin(A) + 7.4;
        glVertex2f(x,y);
    }
    glEnd();

    // Right circle
    glBegin(GL_POLYGON);
    for(int i=0; i<200; i++) {
        float pi = 3.1416;
        float A = (i*2*pi)/200;
        float r = 0.2646;
        float x = r * cos(A) + xPos + 0.5;
        float y = r * sin(A) + 7.4;
        glVertex2f(x,y);
    }
    glEnd();
}

//coulds
void clouds() {                ///9003

    for(int i = 0; i < NUM_CLOUDS; i++)
    {
        drawCloud(cloudPositions[i]);
    }

}

void updateClouds(int value) {           ///8002
    if (cloudsMoving) {
        for (int i = 0; i < NUM_CLOUDS; i++) {
            cloudPositions[i] += 0.01f;

            if (cloudPositions[i] > 20.0f) {
                cloudPositions[i] = -5.0f - (rand() % 5);
            }
        }

        glutPostRedisplay();
    }

    glutTimerFunc(cloudUpdateInterval, updateClouds, 0);
}





//......................................................................................//
///Sun
void sun(){              ///9004

    if(isNight) {
        glColor3ub(240, 240, 240);
    }
    else {
        glColor3ub(255, 195, 0);
    }

	circle(0.4, 16, 7.2);
}



//......................................................................................//
///Hill
void hills() {                 ///9005

    glColor3ub(157, 105, 19 );

    // Hill 1
    glBegin(GL_TRIANGLES);
    glVertex2f(-5, 5.5);
    glVertex2f(0, 5.5);
    glVertex2f(-2.5, 7.0);
    glEnd();

    // Hill 2
    glBegin(GL_TRIANGLES);
    glVertex2f(0, 5.5);
    glVertex2f(5, 5.5);
    glVertex2f(2.5, 7.0);
    glEnd();

    // Hill 3
    glBegin(GL_TRIANGLES);
    glVertex2f(5, 5.5);
    glVertex2f(10, 5.5);
    glVertex2f(7.5, 7.0);
    glEnd();

    // Hill 4
    glBegin(GL_TRIANGLES);
    glVertex2f(10, 5.5);
    glVertex2f(15, 5.5);
    glVertex2f(12.5, 7.0);
    glEnd();

    // Hill 5
    glBegin(GL_TRIANGLES);
    glVertex2f(15, 5.5);
    glVertex2f(20, 5.5);
    glVertex2f(17.5, 7.0);
    glEnd();
}

void hillsBorder() {

    glColor3ub(0, 0, 0);
    glLineWidth(3);

    // Hill 1 Border
    glBegin(GL_LINE_LOOP);
    glVertex2f(-5, 5.5);
    glVertex2f(0, 5.5);
    glVertex2f(-2.5, 7.0);
    glEnd();

    // Hill 2 Border
    glBegin(GL_LINE_LOOP);
    glVertex2f(0, 5.5);
    glVertex2f(5, 5.5);
    glVertex2f(2.5, 7.0);
    glEnd();

    // Hill 3 Border
    glBegin(GL_LINE_LOOP);
    glVertex2f(5, 5.5);
    glVertex2f(10, 5.5);
    glVertex2f(7.5, 7.0);
    glEnd();

    // Hill 4 Border
    glBegin(GL_LINE_LOOP);
    glVertex2f(10, 5.5);
    glVertex2f(15, 5.5);
    glVertex2f(12.5, 7.0);
    glEnd();

    // Hill 5 Border
    glBegin(GL_LINE_LOOP);
    glVertex2f(15, 5.5);
    glVertex2f(20, 5.5);
    glVertex2f(17.5, 7.0);
    glEnd();
}




//......................................................................................//
///Ground
void ground(){              ///9006

    if(isDark) {
        glColor3ub(5, 46, 2);  // Darker green for night
    }
    else if(isNight) {
        glColor3ub(2, 20, 1);  // Darker green for night
    }
    else {
        glColor3ub(24, 159, 6); // Original green for day
    }



    //below hills
    glBegin(GL_QUADS);
    glVertex2f(-5, 5.5);
    glVertex2f(-5, 3.5);
    glVertex2f(20, 3.5);
    glVertex2f(20, 5.5);
    glEnd();


    //below road
    glBegin(GL_QUADS);
    glVertex2f(-5, 2.1);
    glVertex2f(-5, 2);
    glVertex2f(20, 2);
    glVertex2f(20, 2.1);
    glEnd();


     //below river
    glBegin(GL_QUADS);
    glVertex2f(-5, 0.6);
    glVertex2f(-5, 0);
    glVertex2f(20, 0);
    glVertex2f(20, 0.6);
    glEnd();


    //below Palace
    glColor3ub(255, 219, 123);

    glBegin(GL_QUADS);
    glVertex2f(6.7, 4);
    glVertex2f(6.3, 3.5);
    glVertex2f(8.7, 3.5);
    glVertex2f(8.3, 4);
    glEnd();

}




//......................................................................................//
///Tree

bool treesMoving = false;
float angle = 0.0f;

//tree1
void tree1() {                     ///9007
    //Trunk
    glColor3ub(125, 71, 48);
    glBegin(GL_QUADS);
    glVertex2f(-2.5, 5.8);
    glVertex2f(-1.9, 5.8);
    glVertex2f(-1.9, 3.7);
    glVertex2f(-2.5, 3.7);
    glEnd();



    if (treesMoving){

        glPushMatrix();
        float trunkTopX = (-2.5f + -1.9f) / 2.0f;
        float trunkTopY = 5.8f;
        glTranslatef(trunkTopX, trunkTopY, 0);
        glRotatef(5 * sin(angle), 0, 0, 1);
        glTranslatef(-trunkTopX, -trunkTopY, 0);
    }


    //Leaves
    glColor3ub(6, 80, 15);
    circle(0.8, -2.2, 6.7); //middle
    circle(0.8, -2.83, 6.2); //left
    circle(0.8, -1.57, 6.2); //right

    glPopMatrix();
}

void treeBorder1() {
    //Trunk
    glColor3ub(0, 0, 0);
    glLineWidth(4);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-2.5, 5.8);
    glVertex2f(-1.9, 5.8);
    glVertex2f(-1.9, 3.7);
    glVertex2f(-2.5, 3.7);
    glEnd();


    if (treesMoving){

        glPushMatrix();
        float trunkTopX = (-2.5f + -1.9f) / 2.0f;
        float trunkTopY = 5.8f;
        glTranslatef(trunkTopX, trunkTopY, 0);
        glRotatef(5 * sin(angle), 0, 0, 1);
        glTranslatef(-trunkTopX, -trunkTopY, 0);
    }


    circleBorder(0.8, -2.2, 6.7); //middle
    circleBorder(0.8, -2.83, 6.2); //left
    circleBorder(0.8, -1.57, 6.2); //right

    glPopMatrix();
}

//tree2
void tree2() {                    ///9008
    //Trunk
    glColor3ub(125, 71, 48);
    glBegin(GL_QUADS);
    glVertex2f(0.7, 5.8);
    glVertex2f(1.3, 5.8);
    glVertex2f(1.3, 3.7);
    glVertex2f(0.7, 3.7);
    glEnd();


    if (treesMoving){

        glPushMatrix();
        float trunkTopX = (0.7f + 1.3f) / 2.0f;
        float trunkTopY = 5.8f;
        glTranslatef(trunkTopX, trunkTopY, 0);
        glRotatef(5 * sin(angle), 0, 0, 1);
        glTranslatef(-trunkTopX, -trunkTopY, 0);
    }


    //Leaves
    glColor3ub(6, 80, 15);
    circle(0.8, 1.0, 6.7);   //middle
    circle(0.8, 0.37, 6.2);  //left
    circle(0.8, 1.63, 6.2);  //right

    glPopMatrix();
}


void treeBorder2() {
    //Trunk
    glColor3ub(0, 0, 0);
    glLineWidth(5);
    glBegin(GL_LINE_LOOP);
    glVertex2f(0.7, 5.8);
    glVertex2f(1.3, 5.8);
    glVertex2f(1.3, 3.7);
    glVertex2f(0.7, 3.7);
    glEnd();


    if (treesMoving){

        glPushMatrix();
        float trunkTopX = (0.7f + 1.3f) / 2.0f;
        float trunkTopY = 5.8f;
        glTranslatef(trunkTopX, trunkTopY, 0);
        glRotatef(5 * sin(angle), 0, 0, 1);
        glTranslatef(-trunkTopX, -trunkTopY, 0);
    }


    //Leaves
    circleBorder(0.8, 1.0, 6.7);   //middle
    circleBorder(0.8, 0.37, 6.2);  //left
    circleBorder(0.8, 1.63, 6.2);  //right

    glPopMatrix();
}

//tree3
void tree3() {                     ///9009
    //Trunk
    glColor3ub(125, 71, 48);
    glBegin(GL_QUADS);
    glVertex2f(13.33, 6);
    glVertex2f(13.71, 6);
    glVertex2f(13.71, 4);
    glVertex2f(13.33, 4);
    glEnd();


    if (treesMoving){

        glPushMatrix();
        float trunkTopX = (13.33f + 13.71f) / 2.0f;
        float trunkTopY = 6.0f;
        glTranslatef(trunkTopX, trunkTopY, 0);
        glRotatef(5 * sin(angle), 0, 0, 1);
        glTranslatef(-trunkTopX, -trunkTopY, 0);
    }


    //Leaves
    glColor3ub(6, 80, 15);
    circle(0.6, 13.52, 6.69);  // right
    circle(0.6, 13.0, 6.27);   // middle
    circle(0.6, 14.0, 6.31);    // left

    glPopMatrix();
}


void treeBorder3() {
    //Trunk
    glColor3ub(0, 0, 0);
    glLineWidth(5);
    glBegin(GL_LINE_LOOP);
    glVertex2f(13.33, 6);
    glVertex2f(13.71, 6);
    glVertex2f(13.71, 4);
    glVertex2f(13.33, 4);
    glEnd();


    if (treesMoving){

        glPushMatrix();
        float trunkTopX = (13.33f + 13.71f) / 2.0f;
        float trunkTopY = 6.0f;
        glTranslatef(trunkTopX, trunkTopY, 0);
        glRotatef(5 * sin(angle), 0, 0, 1);
        glTranslatef(-trunkTopX, -trunkTopY, 0);
    }


    //Leaves
    circleBorder(0.6, 13.52, 6.69);  // right
    circleBorder(0.6, 13.0, 6.27);   // middle
    circleBorder(0.6, 14.0, 6.31);    // left

    glPopMatrix();
}



void updateTree(int value) {      ///8010

    angle += 0.1f;

    if (angle > 2 * 3.1416f)
        angle -= 2 * 3.1416f;

    glutPostRedisplay();
    glutTimerFunc(40, updateTree, 0);
}







//......................................................................................//
///Road
void road() {                         ///9010

    glColor3ub(71, 65, 65);

    glBegin(GL_QUADS);
    glVertex2f(-5, 3.5);
    glVertex2f(-5, 2.1);
    glVertex2f(20, 2.1);
    glVertex2f(20, 3.5);
    glEnd();


    glColor3ub(255, 255, 255);
    glBegin(GL_QUADS);
    glVertex2f(-4.7, 2.9);
    glVertex2f(-4.7, 2.8);
    glVertex2f(-3.8, 2.8);
    glVertex2f(-3.8, 2.9);
    glEnd();


    glBegin(GL_QUADS);
    glVertex2f(-2.9, 2.9);
    glVertex2f(-2.9, 2.8);
    glVertex2f(-2.0, 2.8);
    glVertex2f(-2.0, 2.9);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(-1.1, 2.9);
    glVertex2f(-1.1, 2.8);
    glVertex2f(-0.2, 2.8);
    glVertex2f(-0.2, 2.9);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(0.7, 2.9);
    glVertex2f(0.7, 2.8);
    glVertex2f(1.6, 2.8);
    glVertex2f(1.6, 2.9);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(2.5, 2.9);
    glVertex2f(2.5, 2.8);
    glVertex2f(3.4, 2.8);
    glVertex2f(3.4, 2.9);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(4.3, 2.9);
    glVertex2f(4.3, 2.8);
    glVertex2f(5.2, 2.8);
    glVertex2f(5.2, 2.9);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(6.1, 2.9);
    glVertex2f(6.1, 2.8);
    glVertex2f(7.0, 2.8);
    glVertex2f(7.0, 2.9);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(7.9, 2.9);
    glVertex2f(7.9, 2.8);
    glVertex2f(8.8, 2.8);
    glVertex2f(8.8, 2.9);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(9.7, 2.9);
    glVertex2f(9.7, 2.8);
    glVertex2f(10.6, 2.8);
    glVertex2f(10.6, 2.9);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(11.5, 2.9);
    glVertex2f(11.5, 2.8);
    glVertex2f(12.4, 2.8);
    glVertex2f(12.4, 2.9);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(13.3, 2.9);
    glVertex2f(13.3, 2.8);
    glVertex2f(14.2, 2.8);
    glVertex2f(14.2, 2.9);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(15.1, 2.9);
    glVertex2f(15.1, 2.8);
    glVertex2f(16.0, 2.8);
    glVertex2f(16.0, 2.9);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(16.9, 2.9);
    glVertex2f(16.9, 2.8);
    glVertex2f(17.8, 2.8);
    glVertex2f(17.8, 2.9);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(18.7, 2.9);
    glVertex2f(18.7, 2.8);
    glVertex2f(19.6, 2.8);
    glVertex2f(19.6, 2.9);
    glEnd();
}


//roadBorder01
void roadBorder() {

    glColor3ub(0, 0, 0);
    glLineWidth(2);

    glBegin(GL_LINE_LOOP);
    glVertex2f(-8, 3.5);
    glVertex2f(-8, 2.1);
    glVertex2f(23, 2.1);
    glVertex2f(23, 3.5);
    glEnd();
}




//......................................................................................//
///Palace
void palace() {                       ///9011

    //palace base
    glColor3ub(52, 6, 50);
    glBegin(GL_QUADS);
    glVertex2f(2.85, 4);
    glVertex2f(2.85, 3.9);
    glVertex2f(12.15, 3.9);
    glVertex2f(12.15, 4);
    glEnd();


    // Palace left
    glColor3ub(181, 29, 27);
    glBegin(GL_QUADS);
    glVertex2f(3, 6);
    glVertex2f(3, 4);
    glVertex2f(6.5, 4);
    glVertex2f(6.5, 6);
    glEnd();

    // Palace right
    glColor3ub(181, 29, 27);
    glBegin(GL_QUADS);
    glVertex2f(6.5, 6);
    glVertex2f(6.5, 4);
    glVertex2f(12, 4);
    glVertex2f(12, 6);
    glEnd();


    //Head Triangle
    glColor3ub(0, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(7.41, 7.31);
    glVertex2f(7.49, 7.44);
    glVertex2f(7.56, 7.31);
    glEnd();



    //Head Circle
	glColor3ub(181, 29, 27);
	circle(0.911, 7.47, 6.4);



	//Head base
    glColor3ub(66, 166, 155);
    glBegin(GL_QUADS);
    glVertex2f(6.5, 6.2);
    glVertex2f(6.5, 6);
    glVertex2f(8.5, 6);
    glVertex2f(8.5, 6.2);
    glEnd();



    //Palace middle
    glColor3ub(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(6.5, 6);
    glVertex2f(6.5, 4);
    glVertex2f(8.5, 4);
    glVertex2f(8.5, 6);
    glEnd();

    //Inner Quads
    glColor3ub(66, 166, 155);
    glBegin(GL_QUADS);
    glVertex2f(6.7, 5.5);
    glVertex2f(6.7, 4);
    glVertex2f(8.3, 4);
    glVertex2f(8.3, 5.5);
    glEnd();



    //Upper triangle of window

    //left1
    glColor3ub(0, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(3.7, 5.4);
    glVertex2f(3.4, 5);
    glVertex2f(4, 5);
    glEnd();

    //left2
    glColor3ub(0, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(4.7, 5.4);
    glVertex2f(4.4, 5);
    glVertex2f(5, 5);
    glEnd();

    //left3
    glColor3ub(0, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(5.7, 5.39);
    glVertex2f(5.4, 5);
    glVertex2f(6, 5);
    glEnd();


    //right1
    glColor3ub(0, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(9.32, 5.4);
    glVertex2f(9, 5);
    glVertex2f(9.6, 5);
    glEnd();


    //right2
    glColor3ub(0, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(10.29, 5.41);
    glVertex2f(10, 5);
    glVertex2f(10.6, 5);
    glEnd();

    //right3
    glColor3ub(0, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(11.29, 5.41);
    glVertex2f(11, 5);
    glVertex2f(11.6, 5);
    glEnd();





    //Gombhuj

    //Left-1
    glColor3ub(181, 29, 27);
    glBegin(GL_TRIANGLES);
    glVertex2f(3.27, 6.8);
    glVertex2f(3, 6.5);
    glVertex2f(3.5, 6.5);
    glEnd();

    glColor3ub(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(3, 6.5);
    glVertex2f(3, 6);
    glVertex2f(3.5, 6);
    glVertex2f(3.5, 6.5);
    glEnd();


    //Left-2
    glColor3ub(181, 29, 27);
    glBegin(GL_TRIANGLES);
    glVertex2f(4.76, 6.8);
    glVertex2f(4.5, 6.5);
    glVertex2f(5, 6.5);
    glEnd();

    glColor3ub(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(4.5, 6.5);
    glVertex2f(4.5, 6);
    glVertex2f(5, 6);
    glVertex2f(5, 6.5);
    glEnd();




    //Right-1
    glColor3ub(181, 29, 27);
    glBegin(GL_TRIANGLES);
    glVertex2f(10.25, 6.85);
    glVertex2f(10, 6.5);
    glVertex2f(10.5, 6.5);
    glEnd();

    glColor3ub(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(10, 6.5);
    glVertex2f(10, 6);
    glVertex2f(10.5, 6);
    glVertex2f(10.5, 6.5);
    glEnd();


    //Right-2
    glColor3ub(181, 29, 27);
    glBegin(GL_TRIANGLES);
    glVertex2f(11.76, 6.85);
    glVertex2f(11.5, 6.5);
    glVertex2f(12, 6.5);
    glEnd();

    glColor3ub(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(11.5, 6.5);
    glVertex2f(11.5, 6);
    glVertex2f(12, 6);
    glVertex2f(12, 6.5);
    glEnd();
}

void palaceBorder() {

    glColor3ub(0, 0, 0);
    glLineWidth(4);


    //palace base
    glBegin(GL_LINE_LOOP);
    glVertex2f(2.85, 4);
    glVertex2f(2.85, 3.9);
    glVertex2f(12.15, 3.9);
    glVertex2f(12.15, 4);
    glEnd();


    // Palace left
    glBegin(GL_LINE_LOOP);
    glVertex2f(3, 6);
    glVertex2f(3, 4);
    glVertex2f(6.5, 4);
    glVertex2f(6.5, 6);
    glEnd();

    // Palace right
    glBegin(GL_LINE_LOOP);
    glVertex2f(6.5, 6);
    glVertex2f(6.5, 4);
    glVertex2f(12, 4);
    glVertex2f(12, 6);
    glEnd();

    // Head Triangle
    glBegin(GL_LINE_LOOP);
    glVertex2f(7.41, 7.31);
    glVertex2f(7.49, 7.44);
    glVertex2f(7.56, 7.31);
    glEnd();

    // Head Circle
    circleBorder(0.911, 7.47, 6.4);

    // Head base
    glBegin(GL_LINE_LOOP);
    glVertex2f(6.5, 6.2);
    glVertex2f(6.5, 6);
    glVertex2f(8.5, 6);
    glVertex2f(8.5, 6.2);
    glEnd();

    // Palace middle
    glBegin(GL_LINE_LOOP);
    glVertex2f(6.5, 6);
    glVertex2f(6.5, 4);
    glVertex2f(8.5, 4);
    glVertex2f(8.5, 6);
    glEnd();

    // Inner Quads
    glBegin(GL_LINE_LOOP);
    glVertex2f(6.7, 5.5);
    glVertex2f(6.7, 4);
    glVertex2f(8.3, 4);
    glVertex2f(8.3, 5.5);
    glEnd();





    // Gombhuj

    // Left-1
    glBegin(GL_LINE_LOOP);
    glVertex2f(3.27, 6.8);
    glVertex2f(3, 6.5);
    glVertex2f(3.5, 6.5);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(3, 6.5);
    glVertex2f(3, 6);
    glVertex2f(3.5, 6);
    glVertex2f(3.5, 6.5);
    glEnd();

    // Left-2
    glBegin(GL_LINE_LOOP);
    glVertex2f(4.76, 6.8);
    glVertex2f(4.5, 6.5);
    glVertex2f(5, 6.5);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(4.5, 6.5);
    glVertex2f(4.5, 6);
    glVertex2f(5, 6);
    glVertex2f(5, 6.5);
    glEnd();

    // Right-1
    glBegin(GL_LINE_LOOP);
    glVertex2f(10.25, 6.85);
    glVertex2f(10, 6.5);
    glVertex2f(10.5, 6.5);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(10, 6.5);
    glVertex2f(10, 6);
    glVertex2f(10.5, 6);
    glVertex2f(10.5, 6.5);
    glEnd();

    // Right-2
    glBegin(GL_LINE_LOOP);
    glVertex2f(11.76, 6.85);
    glVertex2f(11.5, 6.5);
    glVertex2f(12, 6.5);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(11.5, 6.5);
    glVertex2f(11.5, 6);
    glVertex2f(12, 6);
    glVertex2f(12, 6.5);
    glEnd();
}


//windowPalace
void windowPalace(){         ///9012


    if(isDark || isNight || isLighting){
        glColor3ub(255, 255, 100);
    }
    else {
        glColor3ub(255, 255, 255);
    }


    //Inner Triangles
    glBegin(GL_TRIANGLES);
    glVertex2f(6.7, 5.5);
    glVertex2f(8.3, 5.5);
    glVertex2f(7.5, 6);
    glEnd();



    //Square (upper of windows)

    //left
    glBegin(GL_QUADS);
    glVertex2f(3.4, 5.8);
    glVertex2f(3.4, 5.7);
    glVertex2f(4, 5.7);
    glVertex2f(4, 5.8);
    glEnd();


    glBegin(GL_QUADS);
    glVertex2f(4.4, 5.8);
    glVertex2f(4.4, 5.7);
    glVertex2f(5, 5.7);
    glVertex2f(5, 5.8);
    glEnd();


    glBegin(GL_QUADS);
    glVertex2f(5.4, 5.8);
    glVertex2f(5.4, 5.7);
    glVertex2f(6, 5.7);
    glVertex2f(6, 5.8);
    glEnd();



    //right
    glBegin(GL_QUADS);
    glVertex2f(9, 5.8);
    glVertex2f(9, 5.7);
    glVertex2f(9.6, 5.7);
    glVertex2f(9.6, 5.8);
    glEnd();


    glBegin(GL_QUADS);
    glVertex2f(10, 5.8);
    glVertex2f(10, 5.7);
    glVertex2f(10.6, 5.7);
    glVertex2f(10.6, 5.8);
    glEnd();


    glBegin(GL_QUADS);
    glVertex2f(11, 5.8);
    glVertex2f(11, 5.7);
    glVertex2f(11.6, 5.7);
    glVertex2f(11.6, 5.8);
    glEnd();





    //Window Left

    //Window left-1
    glBegin(GL_QUADS);
    glVertex2f(3.4, 5);
    glVertex2f(3.4, 4.4);
    glVertex2f(4, 4.4);
    glVertex2f(4, 5);
    glEnd();


    //Window left-2
    glBegin(GL_QUADS);
    glVertex2f(4.4, 5);
    glVertex2f(4.4, 4.4);
    glVertex2f(5, 4.4);
    glVertex2f(5, 5);
    glEnd();


    //Window left-3
    glBegin(GL_QUADS);
    glVertex2f(5.4, 5);
    glVertex2f(5.4, 4.4);
    glVertex2f(6, 4.4);
    glVertex2f(6, 5);
    glEnd();




    //Window Right

    //Window right-1
    glBegin(GL_QUADS);
    glVertex2f(9, 5);
    glVertex2f(9, 4.4);
    glVertex2f(9.6, 4.4);
    glVertex2f(9.6, 5);
    glEnd();


     //Window right-2
    glBegin(GL_QUADS);
    glVertex2f(10, 5);
    glVertex2f(10, 4.4);
    glVertex2f(10.6, 4.4);
    glVertex2f(10.6, 5);
    glEnd();


     //Window right-3
    glBegin(GL_QUADS);
    glVertex2f(11, 5);
    glVertex2f(11, 4.4);
    glVertex2f(11.6, 4.4);
    glVertex2f(11.6, 5);
    glEnd();

}


void windowPalaceBorder(){

    glColor3ub(0, 0, 0);
    glLineWidth(2);

    // Window Left

    // Window left-1
    glBegin(GL_LINE_LOOP);
    glVertex2f(3.7, 5.4);
    glVertex2f(3.4, 5);
    glVertex2f(4, 5);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(3.4, 5);
    glVertex2f(3.4, 4.4);
    glVertex2f(4, 4.4);
    glVertex2f(4, 5);
    glEnd();

    // Window left-2
    glBegin(GL_LINE_LOOP);
    glVertex2f(4.7, 5.4);
    glVertex2f(4.4, 5);
    glVertex2f(5, 5);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(4.4, 5);
    glVertex2f(4.4, 4.4);
    glVertex2f(5, 4.4);
    glVertex2f(5, 5);
    glEnd();

    // Window left-3
    glBegin(GL_LINE_LOOP);
    glVertex2f(5.7, 5.39);
    glVertex2f(5.4, 5);
    glVertex2f(6, 5);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(5.4, 5);
    glVertex2f(5.4, 4.4);
    glVertex2f(6, 4.4);
    glVertex2f(6, 5);
    glEnd();

    // Window Right

    // Window right-1
    glBegin(GL_LINE_LOOP);
    glVertex2f(9.32, 5.4);
    glVertex2f(9, 5);
    glVertex2f(9.6, 5);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(9, 5);
    glVertex2f(9, 4.4);
    glVertex2f(9.6, 4.4);
    glVertex2f(9.6, 5);
    glEnd();

    // Window right-2
    glBegin(GL_LINE_LOOP);
    glVertex2f(10.29, 5.41);
    glVertex2f(10, 5);
    glVertex2f(10.6, 5);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(10, 5);
    glVertex2f(10, 4.4);
    glVertex2f(10.6, 4.4);
    glVertex2f(10.6, 5);
    glEnd();

    // Window right-3
    glBegin(GL_LINE_LOOP);
    glVertex2f(11.29, 5.41);
    glVertex2f(11, 5);
    glVertex2f(11.6, 5);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(11, 5);
    glVertex2f(11, 4.4);
    glVertex2f(11.6, 4.4);
    glVertex2f(11.6, 5);
    glEnd();

    // Square

    // left
    glBegin(GL_LINE_LOOP);
    glVertex2f(3.4, 5.8);
    glVertex2f(3.4, 5.7);
    glVertex2f(4, 5.7);
    glVertex2f(4, 5.8);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(4.4, 5.8);
    glVertex2f(4.4, 5.7);
    glVertex2f(5, 5.7);
    glVertex2f(5, 5.8);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(5.4, 5.8);
    glVertex2f(5.4, 5.7);
    glVertex2f(6, 5.7);
    glVertex2f(6, 5.8);
    glEnd();

    // right
    glBegin(GL_LINE_LOOP);
    glVertex2f(9, 5.8);
    glVertex2f(9, 5.7);
    glVertex2f(9.6, 5.7);
    glVertex2f(9.6, 5.8);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(10, 5.8);
    glVertex2f(10, 5.7);
    glVertex2f(10.6, 5.7);
    glVertex2f(10.6, 5.8);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(11, 5.8);
    glVertex2f(11, 5.7);
    glVertex2f(11.6, 5.7);
    glVertex2f(11.6, 5.8);
    glEnd();


    // Inner Triangles
    glBegin(GL_LINE_LOOP);
    glVertex2f(6.7, 5.5);
    glVertex2f(8.3, 5.5);
    glVertex2f(7.5, 6);
    glEnd();


}



//......................................................................................//
///flower


bool flowersMoving = false;
float flowerAngle = 0.0f;

//flower1
void flower1(){                 ///9013
    glPushMatrix();

    // Apply swing rotation around the base of the flower
    if(flowersMoving) {
        float swingAngle = 10.0f * sinf(flowerAngle); // Smoother natural swing (-5° to +5°)

        // Move to pivot point
        glTranslatef(16.5f, 0.1f, 0.0f);
        glRotatef(swingAngle, 0.0f, 0.0f, 1.0f); // Swing left and right around base
        glTranslatef(-16.5f, -0.1f, 0.0f); // Return to original position
    }

    // Body
    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(16.5, 0.9);
    glVertex2f(16.5, 0.1);
    glEnd();

    // Flower
    glColor3ub(255, 0, 0);
    circle(0.1, 16.5, 0.9);

    glColor3ub(255, 209, 53);
    circle(0.1, 16.5, 1.1);
    circle(0.1, 16.67, 1);
    circle(0.1, 16.67, 0.8);
    circle(0.1, 16.5, 0.7);
    circle(0.1, 16.32, 0.8);
    circle(0.1, 16.33, 1);

    // Left leaf
    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(16.28, 0.41);
    glVertex2f(16.5, 0.23);
    glEnd();

    glColor3ub(7, 128, 27);
    glBegin(GL_QUADS);
    glVertex2f(16.28, 0.41);
    glVertex2f(16.22, 0.59);
    glVertex2f(16.01, 0.68);
    glVertex2f(16.09, 0.46);
    glEnd();

    // Right leaf
    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(16.6, 0.3);
    glVertex2f(16.5, 0.2);
    glEnd();

    glColor3ub(7, 128, 27);
    glBegin(GL_QUADS);
    glVertex2f(16.6, 0.3);
    glVertex2f(16.67, 0.54);
    glVertex2f(16.89, 0.65);
    glVertex2f(16.81, 0.42);
    glEnd();

    glPopMatrix();
}


//flower2
void flower2(){                  ///9014

    glPushMatrix();

    if(flowersMoving) {
        float swingAngle = 10.0f * sinf(flowerAngle); // Smoother natural swing (-5° to +5°)

        // Move to pivot point
        glTranslatef(17.5f, 0.2f, 0.0f);
        glRotatef(swingAngle, 0.0f, 0.0f, 1.0f); // Swing left and right around base
        glTranslatef(-17.5f, -0.2f, 0.0f); // Return to original position
    }


    //Body
    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(17.5, 1);
    glVertex2f(17.5, 0.2);
    glEnd();


    //Flower
    glColor3ub(255, 0, 0);
	circle(0.1, 17.5, 1); //red middle

    glColor3ub(255, 209, 53);
	circle(0.1, 17.5, 1.2);
	circle(0.1, 17.67, 1.1);
	circle(0.1, 17.67, 0.9);
	circle(0.1, 17.5, 0.8);
	circle(0.1, 17.32, 0.9);
	circle(0.1, 17.33, 1.1);


	//Left leaf
	glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(17.36, 0.46);
    glVertex2f(17.5, 0.36);
    glEnd();

    glColor3ub(7, 128, 27);
    glBegin(GL_QUADS);
    glVertex2f(17.17, 0.52);
    glVertex2f(17.1, 0.72);
    glVertex2f(17.3, 0.66);
    glVertex2f(17.36, 0.46);
    glEnd();


    //Right leaf
	glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(17.5, 0.42);
    glVertex2f(17.6, 0.5);
    glEnd();

    glColor3ub(7, 128, 27);
    glBegin(GL_QUADS);
    glVertex2f(17.6, 0.5);
    glVertex2f(17.7, 0.7);
    glVertex2f(17.9, 0.75);
    glVertex2f(17.84, 0.52);
    glEnd();

    glPopMatrix();
}


//flower3
void flower3(){                  ///9015

    glPushMatrix();

    if(flowersMoving) {
        float swingAngle = 10.0f * sinf(flowerAngle); // Smoother natural swing (-5° to +5°)

        // Move to pivot point
        glTranslatef(18.5f, 0.1f, 0.0f);
        glRotatef(swingAngle, 0.0f, 0.0f, 1.0f); // Swing left and right around base
        glTranslatef(-18.5f, -0.1f, 0.0f); // Return to original position
    }


    //Body
	glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(18.5, 0.9);
    glVertex2f(18.5, 0.1);
    glEnd();


    //Flower
    glColor3ub(255, 0, 0);
	circle(0.1, 18.5, 0.9); // red middle

    glColor3ub(255, 209, 53);
	circle(0.1, 18.33, 1);
	circle(0.1, 18.5, 0.7);
	circle(0.1, 18.33, 0.8);
	circle(0.1, 18.67, 0.8);
	circle(0.1, 18.5, 1.1);
	circle(0.1, 18.68, 1);


	//Left leaf
	glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(18.5, 0.35);
    glVertex2f(18.4, 0.4);
    glEnd();

    glColor3ub(7, 128, 27);
    glBegin(GL_QUADS);
    glVertex2f(18.1, 0.63);
    glVertex2f(18.19, 0.45);
    glVertex2f(18.4, 0.4);
    glVertex2f(18.3, 0.57);
    glEnd();


    //Right leaf
	glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(18.6, 0.4);
    glVertex2f(18.5, 0.3);
    glEnd();

    glColor3ub(7, 128, 27);
    glBegin(GL_QUADS);
    glVertex2f(18.6, 0.4);
    glVertex2f(18.66, 0.57);
    glVertex2f(18.84, 0.66);
    glVertex2f(18.77, 0.49);
    glEnd();

    glPopMatrix();
}



//flower4
void flower4(){                ///9016

    glPushMatrix();

    if(flowersMoving) {
        float swingAngle = 10.0f * sinf(flowerAngle); // Smoother natural swing (-5° to +5°)

        // Move to pivot point
        glTranslatef(4.0f, 0.1f, 0.0f);
        glRotatef(swingAngle, 0.0f, 0.0f, 1.0f); // Swing left and right around base
        glTranslatef(-4.0f, -0.1f, 0.0f); // Return to original position
    }



    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(16.5 - 12.5, 0.9);
    glVertex2f(16.5 - 12.5, 0.1);
    glEnd();

    glColor3ub(255, 0, 0);
    circle(0.1, 16.5 - 12.5, 0.9);

    glColor3ub(255, 209, 53);
    circle(0.1, 16.5 - 12.5, 1.1);
    circle(0.1, 16.67 - 12.5, 1);
    circle(0.1, 16.67 - 12.5, 0.8);
    circle(0.1, 16.5 - 12.5, 0.7);
    circle(0.1, 16.32 - 12.5, 0.8);
    circle(0.1, 16.33 - 12.5, 1);

    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(16.28 - 12.5, 0.41);
    glVertex2f(16.5 - 12.5, 0.23);
    glEnd();

    glColor3ub(7, 128, 27);
    glBegin(GL_QUADS);
    glVertex2f(16.28 - 12.5, 0.41);
    glVertex2f(16.22 - 12.5, 0.59);
    glVertex2f(16.01 - 12.5, 0.68);
    glVertex2f(16.09 - 12.5, 0.46);
    glEnd();

    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(16.6 - 12.5, 0.3);
    glVertex2f(16.5 - 12.5, 0.2);
    glEnd();

    glColor3ub(7, 128, 27);
    glBegin(GL_QUADS);
    glVertex2f(16.6 - 12.5, 0.3);
    glVertex2f(16.67 - 12.5, 0.54);
    glVertex2f(16.89 - 12.5, 0.65);
    glVertex2f(16.81 - 12.5, 0.42);
    glEnd();

    glPopMatrix();
}


//flower501
void flower5(){                ///9017

    glPushMatrix();

    if(flowersMoving) {
        float swingAngle = 10.0f * sinf(flowerAngle); // Smoother natural swing (-5° to +5°)

        // Move to pivot point
        glTranslatef(5.0f, 0.2f, 0.0f);
        glRotatef(swingAngle, 0.0f, 0.0f, 1.0f); // Swing left and right around base
        glTranslatef(-5.0f, -0.2f, 0.0f); // Return to original position
    }



    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(17.5 - 12.5, 1);
    glVertex2f(17.5 - 12.5, 0.2);
    glEnd();

    glColor3ub(255, 0, 0);
    circle(0.1, 17.5 - 12.5, 1);

    glColor3ub(255, 209, 53);
    circle(0.1, 17.5 - 12.5, 1.2);
    circle(0.1, 17.67 - 12.5, 1.1);
    circle(0.1, 17.67 - 12.5, 0.9);
    circle(0.1, 17.5 - 12.5, 0.8);
    circle(0.1, 17.32 - 12.5, 0.9);
    circle(0.1, 17.33 - 12.5, 1.1);

    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(17.36 - 12.5, 0.46);
    glVertex2f(17.5 - 12.5, 0.36);
    glEnd();

    glColor3ub(7, 128, 27);
    glBegin(GL_QUADS);
    glVertex2f(17.17 - 12.5, 0.52);
    glVertex2f(17.1 - 12.5, 0.72);
    glVertex2f(17.3 - 12.5, 0.66);
    glVertex2f(17.36 - 12.5, 0.46);
    glEnd();

    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(17.5 - 12.5, 0.42);
    glVertex2f(17.6 - 12.5, 0.5);
    glEnd();

    glColor3ub(7, 128, 27);
    glBegin(GL_QUADS);
    glVertex2f(17.6 - 12.5, 0.5);
    glVertex2f(17.7 - 12.5, 0.7);
    glVertex2f(17.9 - 12.5, 0.75);
    glVertex2f(17.84 - 12.5, 0.52);
    glEnd();

    glPopMatrix();
}


//flower6
void flower6(){               ///9018

    glPushMatrix();

    if(flowersMoving) {
        float swingAngle = 10.0f * sinf(flowerAngle); // Smoother natural swing (-5° to +5°)

        // Move to pivot point
        glTranslatef(6.0f, 0.1f, 0.0f);
        glRotatef(swingAngle, 0.0f, 0.0f, 1.0f); // Swing left and right around base
        glTranslatef(-6.0f, -0.1f, 0.0f); // Return to original position
    }


    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(18.5 - 12.5, 0.9);
    glVertex2f(18.5 - 12.5, 0.1);
    glEnd();

    glColor3ub(255, 0, 0);
    circle(0.1, 18.5 - 12.5, 0.9);

    glColor3ub(255, 209, 53);
    circle(0.1, 18.33 - 12.5, 1);
    circle(0.1, 18.5 - 12.5, 0.7);
    circle(0.1, 18.33 - 12.5, 0.8);
    circle(0.1, 18.67 - 12.5, 0.8);
    circle(0.1, 18.5 - 12.5, 1.1);
    circle(0.1, 18.68 - 12.5, 1);

    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(18.5 - 12.5, 0.35);
    glVertex2f(18.4 - 12.5, 0.4);
    glEnd();

    glColor3ub(7, 128, 27);
    glBegin(GL_QUADS);
    glVertex2f(18.1 - 12.5, 0.63);
    glVertex2f(18.19 - 12.5, 0.45);
    glVertex2f(18.4 - 12.5, 0.4);
    glVertex2f(18.3 - 12.5, 0.57);
    glEnd();

    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(18.6 - 12.5, 0.4);
    glVertex2f(18.5 - 12.5, 0.3);
    glEnd();

    glColor3ub(7, 128, 27);
    glBegin(GL_QUADS);
    glVertex2f(18.6 - 12.5, 0.4);
    glVertex2f(18.66 - 12.5, 0.57);
    glVertex2f(18.84 - 12.5, 0.66);
    glVertex2f(18.77 - 12.5, 0.49);
    glEnd();

    glPopMatrix();
}



void updateFlowers(int value) {          ///8003
    if (flowersMoving) {
        flowerAngle += 0.05f;

        if (flowerAngle > 2 * 3.1416f) {
            flowerAngle -= 2 * 3.1416f;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(8, updateFlowers, 0);
}





//......................................................................................//
///fish

void fish(){                     ///9019

    //Fish
    glColor3ub(255, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(16.8, 5.8);
    glVertex2f(17, 4.8);
    glVertex2f(18, 4.4);
    glVertex2f(17.8, 5.4);
    glEnd();

    //Upper-Wing
    glColor3ub(52, 6, 50);
    glBegin(GL_TRIANGLES);
    glVertex2f(17.8, 5.4);
    glVertex2f(17.86, 5.09);
    glVertex2f(18.25, 5.12);
    glEnd();

    //Lower-Wing
    glBegin(GL_TRIANGLES);
    glVertex2f(17, 4.8);
    glVertex2f(17.09, 4.4);
    glVertex2f(17.3, 4.7);
    glEnd();


    //Tail
    glColor3ub(52, 6, 50);
    glBegin(GL_TRIANGLES);
    glVertex2f(18, 4.4);
    glVertex2f(18, 4);
    glVertex2f(18.4, 4.4);
    glEnd();


    //Base
    glColor3ub(83, 76, 10);
    glBegin(GL_QUADS);
    glVertex2f(17.2, 4);
    glVertex2f(17.2, 3.8);
    glVertex2f(19, 3.8);
    glVertex2f(19, 4);
    glEnd();


    //Eye
    glColor3ub(52, 6, 50);
	circle(0.1, 17.09, 5.47);
}



void fishBorder(){

    glColor3ub(0, 0, 0);
    glLineWidth(2);

    //Fish
    glBegin(GL_LINE_LOOP);
    glVertex2f(16.8, 5.8);
    glVertex2f(17, 4.8);
    glVertex2f(18, 4.4);
    glVertex2f(17.8, 5.4);
    glEnd();

    //Upper-Wing
    glBegin(GL_LINE_LOOP);
    glVertex2f(17.8, 5.4);
    glVertex2f(17.86, 5.09);
    glVertex2f(18.25, 5.12);
    glEnd();

    //Lower-Wing
    glBegin(GL_LINE_LOOP);
    glVertex2f(17, 4.8);
    glVertex2f(17.09, 4.4);
    glVertex2f(17.3, 4.7);
    glEnd();

    //Tail
    glBegin(GL_LINE_LOOP);
    glVertex2f(18, 4.4);
    glVertex2f(18, 4);
    glVertex2f(18.4, 4.4);
    glEnd();


    //Base
    glBegin(GL_LINE_LOOP);
    glVertex2f(17.2, 4);
    glVertex2f(17.2, 3.8);
    glVertex2f(19, 3.8);
    glVertex2f(19, 4);
    glEnd();


    //Eye
    circleBorder(0.1, 17.09, 5.47);
}




//......................................................................................//
///Waterfall


bool isWaterFalling = true;
const int NUM_WATERDROPS = 200;


struct WaterDrop {
    float x;        // x position
    float y;        // y position
    float speed;    // falling speed
    float curve;    // how much it curves left
    bool active;    // whether this drop is currently falling
};
WaterDrop waterDrops[NUM_WATERDROPS];


void initWaterfall() {
    for (int i = 0; i < NUM_WATERDROPS; i++) {
        waterDrops[i].x = 16.8f + (rand() % 5) * 0.01f;
        waterDrops[i].y = 5.8f - (rand() % 4) * 0.1f;
        waterDrops[i].speed = 0.04f + (rand() % 10) * 0.003f;
        waterDrops[i].curve = 0.03f + (rand() % 6) * 0.005f; // STRONGER CURVE
        waterDrops[i].active = (rand() % 2) == 0;
    }
}

void waterfall() {                    ///9020
    if (!isWaterFalling) return;

    glColor3ub(173, 216, 230);
    glLineWidth(1.5);

    glBegin(GL_LINES);
    for (int i = 0; i < NUM_WATERDROPS; i++) {
        if (waterDrops[i].active) {
            glVertex2f(waterDrops[i].x, waterDrops[i].y);
            glVertex2f(waterDrops[i].x - waterDrops[i].curve,
                      waterDrops[i].y - 0.2f);
        }
    }
    glEnd();

    // Splash follows sharper curve
    glColor3ub(200, 230, 255);
    for (int i = 0; i < NUM_WATERDROPS; i++) {
        if (waterDrops[i].y <= 3.96f && waterDrops[i].y > 3.8f) {
            circle(0.03f, waterDrops[i].x - waterDrops[i].curve * 3, 3.95f);
        }
    }
}

void updateWaterfall(int value) {               ///8004
    if (isWaterFalling) {
        for (int i = 0; i < NUM_WATERDROPS; i++) {
            if (waterDrops[i].active) {
                waterDrops[i].x -= waterDrops[i].curve * 0.8f; // STRONGER DRIFT
                waterDrops[i].y -= waterDrops[i].speed;

                if (waterDrops[i].y <= 3.96f) {
                    waterDrops[i].x = 16.8f + (rand() % 5) * 0.01f;
                    waterDrops[i].y = 5.8f - (rand() % 4) * 0.1f;
                    waterDrops[i].active = true;
                }
            } else {
                if (rand() % 100 < 5) {
                    waterDrops[i].active = true;
                }
            }
        }
    }
    glutPostRedisplay();
    glutTimerFunc(20, updateWaterfall, 0);
}





//......................................................................................//
///Stone
void stone(){                ///9021

    glColor3ub(231, 216, 36);
    glBegin(GL_POLYGON);
    glVertex2f(14.95, 3.8);
    glVertex2f(14.95, 3.88);
    glVertex2f(15.04, 3.93);
    glVertex2f(15.14, 3.96);
    glVertex2f(15.2, 3.86);
    glVertex2f(15.2, 3.8);
    glEnd();


    glColor3ub(231, 216, 36);
    glBegin(GL_POLYGON);
    glVertex2f(15.27, 3.8);
    glVertex2f(15.48, 3.8);
    glVertex2f(15.55, 3.88);
    glVertex2f(15.52, 3.96);
    glVertex2f(15.36, 3.96);
    glVertex2f(15.23, 3.88);
    glEnd();


    glColor3ub(231, 216, 36);
    glBegin(GL_POLYGON);
    glVertex2f(15.72, 3.96);
    glVertex2f(15.83, 3.93);
    glVertex2f(15.91, 3.89);
    glVertex2f(15.9, 3.8);
    glVertex2f(15.6, 3.8);
    glVertex2f(15.58, 3.92);
    glEnd();


    glColor3ub(231, 216, 36);
    glBegin(GL_POLYGON);
    glVertex2f(15.95, 3.9);
    glVertex2f(16.08, 3.96);
    glVertex2f(16.23, 3.96);
    glVertex2f(16.33, 3.93);
    glVertex2f(16.4, 3.88);
    glVertex2f(16.4, 3.8);
    glVertex2f(16, 3.8);
    glEnd();
}



void stoneBorder(){

    glColor3ub(0, 0, 0);
    glLineWidth(2);


    glBegin(GL_LINE_LOOP);
    glVertex2f(14.95, 3.8);
    glVertex2f(14.95, 3.88);
    glVertex2f(15.04, 3.93);
    glVertex2f(15.14, 3.96);
    glVertex2f(15.2, 3.86);
    glVertex2f(15.2, 3.8);
    glEnd();


    glBegin(GL_LINE_LOOP);
    glVertex2f(15.27, 3.8);
    glVertex2f(15.48, 3.8);
    glVertex2f(15.55, 3.88);
    glVertex2f(15.52, 3.96);
    glVertex2f(15.36, 3.96);
    glVertex2f(15.23, 3.88);
    glEnd();


    glBegin(GL_LINE_LOOP);
    glVertex2f(15.72, 3.96);
    glVertex2f(15.83, 3.93);
    glVertex2f(15.91, 3.89);
    glVertex2f(15.9, 3.8);
    glVertex2f(15.6, 3.8);
    glVertex2f(15.58, 3.92);
    glEnd();


    glBegin(GL_LINE_LOOP);
    glVertex2f(15.95, 3.9);
    glVertex2f(16.08, 3.96);
    glVertex2f(16.23, 3.96);
    glVertex2f(16.33, 3.93);
    glVertex2f(16.4, 3.88);
    glVertex2f(16.4, 3.8);
    glVertex2f(16, 3.8);
    glEnd();
}





//......................................................................................//
///RiverFlowLine

bool isRiverFlowing = false;

const int numRiverLines = 250;               // Number of flowing segments
float riverLineX[numRiverLines];            // X position
float riverLineY[numRiverLines];            // Y position
float riverLineSpeed[numRiverLines];



void initRiverFlow() {
    for (int i = 0; i < numRiverLines; i++) {
        riverLineX[i] = -5.0f + (rand() % 250) / 10.0f;   // random X between -5 to 20
        riverLineY[i] = 0.7f + (rand() % 130) / 100.0f;   // random Y between 0.7 to ~2.0
        riverLineSpeed[i] = 0.01f + ((rand() % 10) / 1000.0f); // random speed 0.01 to 0.02
    }
}


void riverFlowLines() {           ///9022

    if(isNight){
        glColor3ub(0, 0, 0);
    }
    else {
        glColor3ub(255, 255, 255);  // white water lines
    }

    glLineWidth(1.2);

    glBegin(GL_LINES);
    for (int i = 0; i < numRiverLines; i++) {
        float x = riverLineX[i];
        float y = riverLineY[i];
        glVertex2f(x, y);
        glVertex2f(x + 1.0f, y);
    }
    glEnd();
}

void updateRiverFlow(int value) {        ///8005
    if (isRiverFlowing) {
        for (int i = 0; i < numRiverLines; i++) {
            riverLineX[i] += riverLineSpeed[i];
            if (riverLineX[i] > 20.0f) {
                riverLineX[i] = -5.0f;
                riverLineY[i] = 0.7f + (rand() % 130) / 100.0f;
            }
        }
    }
    glutPostRedisplay();
    glutTimerFunc(5, updateRiverFlow, 0);
}





//......................................................................................//
///River

void river(){                   ///9023

    if(isNight){
        glColor3ub(0, 77, 115);
    }
    else {
        glColor3ub(92, 140, 210);
    }


     //River
    glBegin(GL_QUADS);
    glVertex2f(-5, 2);
    glVertex2f(-5, 0.6);
    glVertex2f(20, 0.6);
    glVertex2f(20, 2);
    glEnd();

    riverFlowLines();
}


//riverBorder01
void riverBorder(){
    glColor3ub(0, 0, 0);
    glLineWidth(2);

     //River
    glBegin(GL_LINE_LOOP);
    glVertex2f(-5, 2);
    glVertex2f(-5, 0.6);
    glVertex2f(20, 0.6);
    glVertex2f(20, 2);
    glEnd();
}





//......................................................................................//
///Bridge

void bridge(){                ///9024

    //Lines
    glColor3ub(0, 0, 0);

    glBegin(GL_LINES);
    glVertex2f(6.64, 1.4);
    glVertex2f(6.64, 1.84);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(7.08, 1.24);
    glVertex2f(7.08, 0.76);
    glEnd();


    //Bridge
    glColor3ub(157, 105, 19 );
    glBegin(GL_QUADS);
    glVertex2f(6.5, 2);
    glVertex2f(7.48, 0.6);
    glVertex2f(10, 0.6);
    glVertex2f(8.5, 2);
    glEnd();
}


void bridgeBorder(){

    glColor3ub(0, 0, 0);
    glLineWidth(2);

    //Bridge
    glBegin(GL_LINE_LOOP);
    glVertex2f(6.5, 2);
    glVertex2f(7.48, 0.6);
    glVertex2f(10, 0.6);
    glVertex2f(8.5, 2);
    glEnd();
}





//......................................................................................//
///Boat

bool isBoatMoving = false;
float boatAngle = 0.0f;
float boatX = 13.6f;


void boat(){                      ///9025

    glPushMatrix();
    glTranslatef(0.0f, 0.1f * sin(boatAngle), 0.0f);


    //Left
    glColor3ub(0, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(11.4, 1.4);
    glVertex2f(12.4, 0.8);
    glVertex2f(12.4, 1.2);
    glEnd();

    //Middle
    glColor3ub(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(12.4, 1.2);
    glVertex2f(12.4, 0.8);
    glVertex2f(14.8, 0.8);
    glVertex2f(14.8, 1.2);
    glEnd();

    //Right
    glColor3ub(0, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(14.8, 0.8);
    glVertex2f(14.8, 1.2);
    glVertex2f(15.8, 1.4);
    glEnd();



    //Head
    glColor3ub(83, 35, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(13, 1.8);
    glVertex2f(12.8, 1.2);
    glVertex2f(13.2, 1.2);
    glEnd();


    glColor3ub(62, 0, 83);
    glBegin(GL_QUADS);
    glVertex2f(13, 1.8);
    glVertex2f(13.2, 1.2);
    glVertex2f(14.6, 1.2);
    glVertex2f(14.4, 1.8);
    glEnd();



    //HeadBorder
    glColor3ub(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(13, 1.8);
    glVertex2f(12.8, 1.2);
    glVertex2f(13.2, 1.2);
    glEnd();


    glColor3ub(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(13, 1.8);
    glVertex2f(13.2, 1.2);
    glVertex2f(14.6, 1.2);
    glVertex2f(14.4, 1.8);
    glEnd();


    glPopMatrix();
}

void updateBoat(int value){        ///8006
    if (isBoatMoving) {
        boatAngle += 0.05f;
        if (boatAngle > 2 * 3.1416f) {
            boatAngle -= 2 * 3.1416f;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(10, updateBoat, 0);
}





//......................................................................................//
///House

//house1
void house1(){                       ///9026

    //Head
    glColor3ub(52, 6, 50);
    glBegin(GL_QUADS);
    glVertex2f(-4.2, 2.4);
    glVertex2f(-4.8, 1.6);
    glVertex2f(-0.6, 1.6);
    glVertex2f(-1.2, 2.4);
    glEnd();


    //Body
    glColor3ub(184, 135, 100);
    glBegin(GL_QUADS);
    glVertex2f(-4.2, 1.6);
    glVertex2f(-4.2, 0.2);
    glVertex2f(-1.2, 0.2);
    glVertex2f(-1.2, 1.6);
    glEnd();


    //Base
    glColor3ub(52, 6, 50);
    glBegin(GL_QUADS);
    glVertex2f(-4.2, 0.28);
    glVertex2f(-4.2, 0.2);
    glVertex2f(-1.2, 0.2);
    glVertex2f(-1.2, 0.28);
    glEnd();


    //Door
    glColor3ub(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(-3.2, 1.2);
    glVertex2f(-2.3, 1.2);
    glVertex2f(-2.3, 0.28);
    glVertex2f(-3.2, 0.28);
    glEnd();
}


void house1Border(){

    glColor3ub(0, 0, 0);
    glLineWidth(2);

    //Head
    glBegin(GL_LINE_LOOP);
    glVertex2f(-4.2, 2.4);
    glVertex2f(-4.8, 1.6);
    glVertex2f(-0.6, 1.6);
    glVertex2f(-1.2, 2.4);
    glEnd();


    //Body
    glBegin(GL_LINE_LOOP);
    glVertex2f(-4.2, 1.6);
    glVertex2f(-4.2, 0.2);
    glVertex2f(-1.2, 0.2);
    glVertex2f(-1.2, 1.6);
    glEnd();


    //Base
    glBegin(GL_LINE_LOOP);
    glVertex2f(-4.2, 0.28);
    glVertex2f(-4.2, 0.2);
    glVertex2f(-1.2, 0.2);
    glVertex2f(-1.2, 0.28);
    glEnd();


    //Door
    glBegin(GL_LINE_LOOP);
    glVertex2f(-3.2, 1.2);
    glVertex2f(-2.3, 1.2);
    glVertex2f(-2.3, 0.28);
    glVertex2f(-3.2, 0.28);
    glEnd();
}



//windowHouse1
void windowHouse1(){                      ///9027

    if(isDark || isNight || isLighting){
        glColor3ub(255, 255, 100);
    }
    else {
        glColor3ub(255, 255, 255);
    }


     //Left window
    glBegin(GL_QUADS);
    glVertex2f(-4, 1.2);
    glVertex2f(-3.4, 1.2);
    glVertex2f(-3.4, 0.6);
    glVertex2f(-4, 0.6);
    glEnd();


    //Right window
    glBegin(GL_QUADS);
    glVertex2f(-2, 1.2);
    glVertex2f(-1.4, 1.2);
    glVertex2f(-1.4, 0.6);
    glVertex2f(-2, 0.6);
    glEnd();
}


void windowHouse1Border(){

    glColor3ub(0, 0, 0);
    glLineWidth(2);

    //Left window
    glBegin(GL_LINE_LOOP);
    glVertex2f(-4, 1.2);
    glVertex2f(-3.4, 1.2);
    glVertex2f(-3.4, 0.6);
    glVertex2f(-4, 0.6);
    glEnd();


    //Right window
    glBegin(GL_LINE_LOOP);
    glVertex2f(-2, 1.2);
    glVertex2f(-1.4, 1.2);
    glVertex2f(-1.4, 0.6);
    glVertex2f(-2, 0.6);
    glEnd();
}





//house2
void house2(){                       ///9028

    //Head
    glColor3ub(52, 6, 50);
    glBegin(GL_QUADS);
    glVertex2f(14.4, 6);
    glVertex2f(14, 5.4);
    glVertex2f(16.2, 5.4);
    glVertex2f(15.6, 6);
    glEnd();


    //Body
    glColor3ub(184, 135, 100);
    glBegin(GL_QUADS);
    glVertex2f(14.2, 5.4);
    glVertex2f(14.2, 4.4);
    glVertex2f(16, 4.4);
    glVertex2f(16, 5.4);
    glEnd();


    //Base
    glColor3ub(52, 6, 50);
    glBegin(GL_QUADS);
    glVertex2f(14.2, 4.4);
    glVertex2f(14.2, 4.34);
    glVertex2f(16, 4.34);
    glVertex2f(16, 4.4);
    glEnd();


    //Door
    glColor3ub(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(14.9, 5.1);
    glVertex2f(14.9, 4.4);
    glVertex2f(15.4, 4.4);
    glVertex2f(15.4, 5.1);
    glEnd();
}


void house2Border(){

    glColor3ub(0, 0, 0);
    glLineWidth(2);


     //Head
    glBegin(GL_LINE_LOOP);
    glVertex2f(14.4, 6);
    glVertex2f(14, 5.4);
    glVertex2f(16.2, 5.4);
    glVertex2f(15.6, 6);
    glEnd();


    //Body
    glBegin(GL_LINE_LOOP);
    glVertex2f(14.2, 5.4);
    glVertex2f(14.2, 4.4);
    glVertex2f(16, 4.4);
    glVertex2f(16, 5.4);
    glEnd();


    //Base
    glBegin(GL_LINE_LOOP);
    glVertex2f(14.2, 4.4);
    glVertex2f(14.2, 4.34);
    glVertex2f(16, 4.34);
    glVertex2f(16, 4.4);
    glEnd();


    //Door
    glBegin(GL_LINE_LOOP);
    glVertex2f(14.9, 5.1);
    glVertex2f(14.9, 4.4);
    glVertex2f(15.4, 4.4);
    glVertex2f(15.4, 5.1);
    glEnd();
}





//windowHouse2
void windowHouse2(){                ///9029

    if(isDark || isNight || isLighting){
        glColor3ub(255, 255, 100);
    }
    else {
        glColor3ub(255, 255, 255);
    }


     //Left window
    glBegin(GL_QUADS);
    glVertex2f(14.4, 5.1);
    glVertex2f(14.4, 4.7);
    glVertex2f(14.7, 4.7);
    glVertex2f(14.7, 5.1);
    glEnd();


    //Right window
    glBegin(GL_QUADS);
    glVertex2f(15.5, 5.1);
    glVertex2f(15.5, 4.7);
    glVertex2f(15.8, 4.7);
    glVertex2f(15.8, 5.1);
    glEnd();
}


void windowHouse2Border(){

    glColor3ub(0, 0, 0);
    glLineWidth(2);

    //Left window
    glBegin(GL_LINE_LOOP);
    glVertex2f(14.4, 5.1);
    glVertex2f(14.4, 4.7);
    glVertex2f(14.7, 4.7);
    glVertex2f(14.7, 5.1);
    glEnd();


    //Right window
    glBegin(GL_LINE_LOOP);
    glVertex2f(15.5, 5.1);
    glVertex2f(15.5, 4.7);
    glVertex2f(15.8, 4.7);
    glVertex2f(15.8, 5.1);
    glEnd();
}





//......................................................................................//
///Blockage

//blockage1
void blockage1(){                     ///9030

    glColor3ub(0, 0, 0);
    glLineWidth(5);

    for (float x = -4.9f; x <= 7.0f; x += 0.3f) {
        glBegin(GL_LINES);
        glVertex2f(x, 1.1);
        glVertex2f(x, 0.6);
        glEnd();
    }

    glBegin(GL_LINES);
    glVertex2f(7.0, 0.85);
    glVertex2f(-5, 0.85);
    glEnd();
}

//blockage2
void blockage2(){                   ///9031

    glColor3ub(0, 0, 0);
    glLineWidth(5);




    for (float x = -4.9f; x <= 6.4f; x += 0.3f) {
        glBegin(GL_LINES);
        glVertex2f(x, 2.5);
        glVertex2f(x, 2.0);
        glEnd();
    }


    glBegin(GL_LINES);
    glVertex2f(6.4, 2.25);
    glVertex2f(-5, 2.25);
    glEnd();
}


//blockage3
void blockage3(){                 ///9032

    glColor3ub(0, 0, 0);
    glLineWidth(5);




    for (float x = 8.8f; x <= 20.0f; x += 0.3f) {
        glBegin(GL_LINES);
        glVertex2f(x, 2.5);
        glVertex2f(x, 2.0);
        glEnd();
    }


    glBegin(GL_LINES);
    glVertex2f(20.0, 2.25);
    glVertex2f(8.6, 2.25);
    glEnd();
}






//......................................................................................//
///Wheel

float _move = -5.0f;
float _angle1 = 0.0f;


void wheels() {                      ///9033

    glPushMatrix();
    glTranslatef(_move, 0.0f, 0.0f); //Same movement transformation as the car


    //Back wheel
    glPushMatrix();
    glTranslatef(-0.37, 2.96, 0.0f);
    glRotatef(_angle1, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.37, -2.96, 0.0f);

    glColor3ub(160, 112, 10);
	circle(0.7550, -0.37, 2.96);//main

    glColor3ub(100, 40, 0);//middle
	circle(0.08, -0.37, 2.96);
    glPopMatrix();





    //Front wheel
    glPushMatrix();
    glTranslatef(3.06, 2.75, 0.0f);
    glRotatef(_angle1, 0.0f, 0.0f, 1.0f);
    glTranslatef(-3.06, -2.75, 0.0f);

    glColor3ub(160, 112, 10);
	circle(0.5477, 3.06, 2.75);//main

	glColor3ub(100, 40, 0);//middle
	circle(0.07, 3.06, 2.75);
    glPopMatrix();


    glPopMatrix();
}






//......................................................................................//
///Car

bool headlightOn = false;

void car() {                          ///9034

    glPushMatrix();
    glTranslatef(_move, 0.0f, 0.0f);


    //* Main car body
    glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
    glVertex2f(2.6, 5);
    glVertex2f(-0.9, 5);
    glVertex2f(-1.5, 4.2);
    glVertex2f(-1.3, 3.2);
    glVertex2f(-1.4, 3.2);
    glVertex2f(-1.4, 2.5);
    glVertex2f(2.6, 2.5);
    glEnd();

    //Head
    glColor3ub(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(-1.3, 5);
    glVertex2f(-1.3, 4.9);
    glVertex2f(3.1, 4.9);
    glVertex2f(3.1, 5);
    glEnd();


    //front
    glBegin(GL_QUADS);
    glVertex2f(2.6, 3.5);
    glVertex2f(3.8, 3.5);
    glVertex2f(3.8, 2.5);
    glVertex2f(2.6, 2.5);
    glEnd();

    //Back
    glColor3ub(100, 40, 0);
    glBegin(GL_POLYGON);
    glVertex2f(-0.1, 4.9);
    glVertex2f(-0.88, 4.9);
    glVertex2f(-1.4, 4.2);
    glVertex2f(-1.2, 3.2);
    glVertex2f(-0.1, 3.2);
    glEnd();


    //Window Big
    glColor3ub(200, 230, 240);
    glBegin(GL_QUADS);
    glVertex2f(0, 4.9);
    glVertex2f(0, 3.2);
    glVertex2f(1.2, 3.2);
    glVertex2f(1.2, 4.9);
    glEnd();

    //Window Small
    glColor3ub(200, 230, 240);
    glBegin(GL_QUADS);
    glVertex2f(1.4, 4.9);
    glVertex2f(1.4, 3.4);
    glVertex2f(2.4, 3.4);
    glVertex2f(2.4, 4.9);
    glEnd();


    //Headlight back
    glColor3ub(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(3.5, 3.58);
    glVertex2f(3.5, 3.5);
    glVertex2f(3.8, 3.5);
    glVertex2f(3.8, 3.58);
    glEnd();

    //Headlight Front
    glColor3ub(200, 230, 240);
    glBegin(GL_QUADS);
    glVertex2f(3.8, 3.58);
    glVertex2f(3.8, 3.5);
    glVertex2f(3.85, 3.5);
    glVertex2f(3.85, 3.58);
    glEnd();


    //Handle
    glColor3ub(100, 40, 0);
    glBegin(GL_QUADS);
    glVertex2f(1.8, 3.3);
    glVertex2f(1.8, 3.2);
    glVertex2f(2.4, 3.2);
    glVertex2f(2.4, 3.3);
    glEnd();


    glPopMatrix();
}


void updateCar(int value) {      ///8007
    _move += 0.02;
    _angle1 += 2.0f;

    if(_move > 22.0) {
        _move = -5.0;
    }

    if(_angle1 > 360.0) {
        _angle1 -= 360;
    }

    glutPostRedisplay();
    glutTimerFunc(1, updateCar, 0);
}





//......................................................................................//
///Headlight
void headlight() {         ///9035

    if (!headlightOn) return;

    glPushMatrix();
    glTranslatef(_move, 0.0f, 0.0f);

    glColor3ub(255, 209, 0);
    glBegin(GL_TRIANGLES);
        glVertex2f(3.85, 3.54); // starting point
        glVertex2f(7.5, 4);   // top-right
        glVertex2f(7.5, 3.1);  // bottom-right
    glEnd();


    glPopMatrix();
}








//......................................................................................//
///Bush

//bush1
void bush1(){              ///9036
    glColor3ub(6, 80, 15);

	circle(0.5, -2.7, 3.8);
	circle(0.5, -1.9, 3.8);
	circle(0.5, -2.4, 4.2);
	circle(0.5, -3.2, 4.1);
}

void bush1Border(){
    glLineWidth(5);

    circleBorder(0.5, -2.7, 3.8);
	circleBorder(0.5, -1.9, 3.8);
	circleBorder(0.5, -2.4, 4.2);
	circleBorder(0.5, -3.2, 4.1);
}

//bush2
void bush2(){                  ///9037
    glColor3ub(6, 80, 0);

	circle(0.5, -4.3, 3.8);
	circle(0.5, -3.5, 3.8);
	circle(0.5, -4, 4.2);
    circle(0.5, -4.8, 4);
}

void bush2Border(){
    glLineWidth(5);

    circleBorder(0.5, -4.3, 3.8);
	circleBorder(0.5, -3.5, 3.8);
	circleBorder(0.5, -4, 4.2);
    circleBorder(0.5, -4.8, 4);
}

//bush3
void bush3(){                  ///9038
    glColor3ub(6, 80, 0);

	circle(0.5, -1.2, 3.8);
	circle(0.5, -0.4, 3.8);
	circle(0.5, -1, 4.1);
	circle(0.5, 0.3, 3.8);
	circle(0.5, 1, 3.8);
	circle(0.5, 0.6, 4.1);
	circle(0.5, -0.2, 4.2);
}

void bush3Border(){
    glLineWidth(5);

    circleBorder(0.5, -1.2, 3.8);
	circleBorder(0.5, -0.4, 3.8);
	circleBorder(0.5, -1, 4.1);
	circleBorder(0.5, 0.3, 3.8);
	circleBorder(0.5, 1, 3.8);
	circleBorder(0.5, 0.6, 4.1);
	circleBorder(0.5, -0.2, 4.2);
}

//bush4
void bush4(){                 ///9039
    glColor3ub(6, 80, 0);

	circle(0.5, 1.9, 3.8);
	circle(0.5, 1.7, 4.1);
}

void bush4Border(){
    glLineWidth(5);

    circleBorder(0.5, 1.9, 3.8);
	circleBorder(0.5, 1.7, 4.1);
}


//bush5
void bush5(){               ///9040

    glColor3ub(6, 80, 0);


    circle(0.42, 12.72, 4.2);
    circle(0.42, 13.08, 4.33);
    circle(0.42, 13.4, 4.2);
    circle(0.42, 13.7, 4.2);


    glBegin(GL_QUADS);
    glVertex2f(12.65, 3.95);
    glVertex2f(12.65, 3.77);
    glVertex2f(13.75, 3.77);
    glVertex2f(13.75, 3.95);
    glEnd();

}

void bush5Border(){

    glLineWidth(3);

    circleBorder(0.42, 12.72, 4.2);
    circleBorder(0.42, 13.08, 4.33);
    circleBorder(0.42, 13.4, 4.2);
    circleBorder(0.42, 13.7, 4.2);

    glBegin(GL_LINE_LOOP);
    glVertex2f(12.65, 3.95);
    glVertex2f(12.65, 3.77);
    glVertex2f(13.75, 3.77);
    glVertex2f(13.75, 3.95);
    glEnd();

}






//......................................................................................//
///Post

//post1
void post1(){                ///9041

    //uper base
    glColor3ub(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(1.7, 4.4);
    glVertex2f(1.7, 4.2);
    glVertex2f(2.7, 4.2);
    glVertex2f(2.7, 4.4);
    glEnd();

    //body
    glColor3ub(181, 29, 27);
    glBegin(GL_QUADS);
    glVertex2f(1.8, 4.2);
    glVertex2f(1.8, 3.7);
    glVertex2f(2.6, 3.7);
    glVertex2f(2.6, 4.2);
    glEnd();

    //lower base
    glBegin(GL_QUADS);
    glColor3ub(0, 0, 0);
    glVertex2f(1.7, 3.7);
    glVertex2f(1.7, 3.5);
    glVertex2f(2.7, 3.5);
    glVertex2f(2.7, 3.7);
    glEnd();
}

void post1Border(){

    //body
    glColor3ub(0, 0, 0);
    glLineWidth(2);

    glBegin(GL_LINE_LOOP);
    glVertex2f(1.8, 4.2);
    glVertex2f(1.8, 3.7);
    glVertex2f(2.6, 3.7);
    glVertex2f(2.6, 4.2);
    glEnd();
}


//post1Light
void post1Light(){             ///9042

    if(isNight || isLighting){
        glColor3ub(255, 255, 100);
    }
    else {
        glColor3ub(255, 255, 255);
    }


    glBegin(GL_POLYGON);
	for(int i=0;i<200;i++)
        {
            float pi=3.1416;
            float A=(i*2*pi)/400;
            float r=0.5;
            float x = r * cos(A) + 2.2;
            float y = r * sin(A) + 4.4;
            glVertex2f(x,y );
        }
	glEnd();
}



//post2
void post2(){          ///9043

    //upper base
    glColor3ub(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(12.3, 4.4);
    glVertex2f(12.3, 4.2);
    glVertex2f(13.3, 4.2);
    glVertex2f(13.3, 4.4);
    glEnd();

    //body
    glColor3ub(181, 29, 27);
    glBegin(GL_QUADS);
    glVertex2f(12.4, 4.2);
    glVertex2f(12.4, 3.7);
    glVertex2f(13.2, 3.7);
    glVertex2f(13.2, 4.2);
    glEnd();


    //lower base
    glBegin(GL_QUADS);
    glColor3ub(0, 0, 0);
    glVertex2f(12.3, 3.7);
    glVertex2f(12.3, 3.5);
    glVertex2f(13.3, 3.5);
    glVertex2f(13.3, 3.7);
    glEnd();
}

void post2Border(){

    //body
    glColor3ub(0, 0, 0);
    glLineWidth(2);

    glBegin(GL_LINE_LOOP);
    glVertex2f(12.4, 4.2);
    glVertex2f(12.4, 3.7);
    glVertex2f(13.2, 3.7);
    glVertex2f(13.2, 4.2);
    glEnd();
}


//post2Light
void post2Light(){            ///9044

    if(isNight || isLighting){
        glColor3ub(255, 255, 100);
    }
    else {
        glColor3ub(255, 255, 255);
    }

    glBegin(GL_POLYGON);
	for(int i=0;i<200;i++)
        {
            float pi=3.1416;
            float A=(i*2*pi)/400;
            float r=0.5;
            float x = r * cos(A) + 12.8;
            float y = r * sin(A) + 4.4;
            glVertex2f(x,y );
        }
	glEnd();
}




//......................................................................................//
///Lamppost

//lampPost1
void lampPost1(){               ///9045

    glColor3ub(52, 6, 50);
    glLineWidth(10);

    //horizontal
    glBegin(GL_LINES);
    glVertex2f(-4, 5.4);
    glVertex2f(-4, 3.5);
    glEnd();

    //vertical
    glBegin(GL_LINES);
    glVertex2f(-4.6, 5.4);
    glVertex2f(-3.4, 5.4);
    glEnd();
}


//lampPost1Light
void lampPost1Light(){            ///9046

    if(isNight || isLighting){
        glColor3ub(255, 255, 100);
    }
    else {
        glColor3ub(255, 255, 255);
    }

    glBegin(GL_POLYGON);
	for(int i=0;i<200;i++)
        {
            float pi=3.1416;
            float A=(i*2*pi)/400;
            float r=0.55;
            float x = r * cos(A) - 4;
            float y = r * sin(A) + 5.4;
            glVertex2f(x,y );
        }
	glEnd();
}



//lampPost2
void lampPost2(){              ///9047

    glColor3ub(52, 6, 50);
    glLineWidth(10);

    //horizontal
    glBegin(GL_LINES);
    glVertex2f(19.2, 5.4);
    glVertex2f(19.2, 3.5);
    glEnd();

    //vertical
    glBegin(GL_LINES);
    glVertex2f(18.6, 5.4);
    glVertex2f(19.8, 5.4);
    glEnd();
}


//lampPost2Light
void lampPost2Light(){            ///9048

    if(isNight || isLighting){
        glColor3ub(255, 255, 100);
    }
    else {
        glColor3ub(255, 255, 255);
    }

    glBegin(GL_POLYGON);
	for(int i=0;i<200;i++)
        {
            float pi=3.1416;
            float A=(i*2*pi)/400;
            float r=0.55;
            float x = r * cos(A) + 19.2;
            float y = r * sin(A) + 5.4;
            glVertex2f(x,y );
        }
	glEnd();
}






//......................................................................................//
///Stand

//stand
void stand(){                    ///9049

    glColor3ub(83, 35, 0);
    glLineWidth(5);

    //base
    glBegin(GL_LINES);
    glVertex2f(0.2, 0.2);
    glVertex2f(0.6, 0.2);
    glEnd();

    //body
    glBegin(GL_LINES);
    glVertex2f(0.4, 1.4);
    glVertex2f(0.4, 0.2);
    glEnd();

    //head
    glBegin(GL_LINES);
    glVertex2f(-0.2, 1.3);
    glVertex2f(0.6, 1.3);
    glEnd();

    //hook
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(0.1, 1.3);
    glVertex2f(0.1, 1.2);
    glEnd();

    //upper-base
    glColor3ub(66, 166, 155);
    glBegin(GL_QUADS);
    glVertex2f(-0.1, 1.2);
    glVertex2f(-0.1, 1.1);
    glVertex2f(0.3, 1.1);
    glVertex2f(0.3, 1.2);
    glEnd();

    //lowerbase
    glBegin(GL_QUADS);
    glVertex2f(-0.1, 0.7);
    glVertex2f(-0.1, 0.6);
    glVertex2f(0.3, 0.6);
    glVertex2f(0.3, 0.7);
    glEnd();


    //Lamp
    if(isNight || isLighting){
        glColor3ub(255, 255, 100);
    }
    else {
        glColor3ub(255, 255, 255);
    }

    glBegin(GL_QUADS);
    glVertex2f(-0.1, 1.1);
    glVertex2f(-0.1, 0.7);
    glVertex2f(0.3, 0.7);
    glVertex2f(0.3, 1.1);
    glEnd();
}

void standBorder(){

    glColor3ub(0, 0, 0);
    glLineWidth(1);

     //upper-base
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.1, 1.2);
    glVertex2f(-0.1, 1.1);
    glVertex2f(0.3, 1.1);
    glVertex2f(0.3, 1.2);
    glEnd();

    //lowerbase
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.1, 0.7);
    glVertex2f(-0.1, 0.6);
    glVertex2f(0.3, 0.6);
    glVertex2f(0.3, 0.7);
    glEnd();


    //lamp
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.1, 1.1);
    glVertex2f(-0.1, 0.7);
    glVertex2f(0.3, 0.7);
    glVertex2f(0.3, 1.1);
    glEnd();
}






//......................................................................................//
///Plant

void plant() {               ///9050

    float startX = -4.95;
    float endX = 2.9;
    float step = 0.35; // Horizontal spacing between trees


    auto drawTree = [](float x, float baseY) {
        //Trunk
        glColor3ub(139, 69, 19); // Brown
        glBegin(GL_LINES);
        glVertex2f(x + 0.1, baseY - 0.1); // bottom of trunk
        glVertex2f(x + 0.1, baseY);       // top of trunk
        glEnd();

        //Main branches (V shape)
        glColor3ub(236, 190, 21); // Yellow branches
        glBegin(GL_LINES);
        glVertex2f(x, baseY + 0.15);             // left up
        glVertex2f(x + 0.1, baseY);              // center
        glEnd();

        glBegin(GL_LINES);
        glVertex2f(x + 0.1, baseY);              // center
        glVertex2f(x + 0.2, baseY + 0.15);        // right up
        glEnd();

        //Extra side branches
        glBegin(GL_LINES);
        glVertex2f(x + 0.1, baseY);              // center
        glVertex2f(x, baseY + 0.1);              // left diagonal
        glEnd();

        glBegin(GL_LINES);
        glVertex2f(x + 0.1, baseY);              // center
        glVertex2f(x + 0.2, baseY + 0.1);         // right diagonal
        glEnd();

        //Leafy top (green circle)
        glColor3ub(6, 80, 15); // Forest green
        glBegin(GL_POLYGON);
        for (int i = 0; i < 100; i++) {
            float angle = 2 * 3.1416 * i / 100;
            float r = 0.07;
            float cx = x + 0.1 + r * cos(angle);
            float cy = baseY + 0.2 + r * sin(angle);
            glVertex2f(cx, cy);
        }
        glEnd();
    };



    //Left side

    // Line 1 (Top)
    for (float x = startX; x <= endX; x += step) {
        drawTree(x, 5.25);
    }

    // Line 2 (slightly lower and staggered)
    for (float x = startX + 0.15; x <= endX; x += step) {
        drawTree(x, 5.0);
    }

    // Line 3
    for (float x = startX; x <= endX; x += step) {
        drawTree(x, 4.75);
    }

    // Line 4 (bottom line, staggered)
    for (float x = startX + 0.15; x <= endX; x += step) {
        drawTree(x, 4.5);
    }




    //Right side
        // Line 1 (Top Right)
    for (float x = 12.0; x <= 20.0; x += step) {
        drawTree(x, 5.25);
    }

    // Line 2 (slightly lower and staggered Right)
    for (float x = 12.0 + 0.15; x <= 20.0; x += step) {
        drawTree(x, 5.0);
    }

    // Line 3 (lower Right)
    for (float x = 12.0; x <= 20.0; x += step) {
        drawTree(x, 4.75);
    }

    // Line 4 (lower Right)
    for (float x = 12.0 + 0.15; x <= 20.0; x += step) {
        drawTree(x, 4.5);
    }
}



///display
void display() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(7.5);

	sky();
	sun();
	clouds();
	ground();


	hills();
	hillsBorder();


	plant();


	house2();
    windowHouse2();
    house2Border();
    windowHouse2Border();


	treeBorder1();
	treeBorder2();
	treeBorder3();
	tree1();
	tree2();
	tree3();

    river();
    riverBorder();
	riverFlowLines();

	bush1Border();
	bush1();

	bush2Border();
	bush2();

	bush4Border();
	bush4();

	bush3Border();
	bush3();

	bush5Border();
	bush5();



	road();
	roadBorder();

	palaceBorder();
	palace();
    windowPalace();
    windowPalaceBorder();

    bridge();
    bridgeBorder();



    fish();
    fishBorder();

    blockage1();


    waterfall();
    stone();
    stoneBorder();


    flower1();
    flower2();
    flower3();
    flower4();
    flower5();
    flower6();



    post1Light();
    post1();
    post1Border();

    post2Light();
    post2();
    post2Border();

    lampPost1Light();
    lampPost1();

    lampPost2Light();
    lampPost2();

    car();
    wheels();
    headlight();

    blockage2();
    blockage3();

    house1();
    windowHouse1();
    house1Border();
    windowHouse1Border();

    boat();

    drawRain();

    stand();
    standBorder();


	glFlush();
}



///////////////Keyboard////////////////////
void keyboard(unsigned char key, int x, int y) {       ///8008

    if(key == 'n' || key == 'N') {
        isNight = true;
        isDark = false;
    }
    else if(key == 'd' || key == 'D') {

        if(isRaining){
            isNight = false;
            isDark = true;
        }
        else{
            isNight = false;
            isDark = false;
            isRaining = false;
        }
    }
    else if(key == 'r' || key == 'R') {

        if (!isNight) {
            isDark = true;
            isRaining = true;
            initRain();
        }
        else{
            isRaining = true;
            initRain();
        }
    }
    else if(key == 's' || key == 'S') {
        isRaining = false;
        isDark = false;
    }
    else if(key == 'f' || key == 'F') {
        isBoatMoving = true;
        isRiverFlowing = true;
    }
    else if(key == 'g' || key == 'G') {
        isBoatMoving = false;
        isRiverFlowing = false;
    }
    else if(key == 'l' || key == 'L') {
        isLighting = true;
    }
    else if(key == 'o' || key == 'O') {
        isLighting = false;
    }
    else if(key == 'w' || key == 'W') {
        isWaterFalling = true;
        initWaterfall(); // Reset all drops
    }
    else if(key == 'q' || key == 'Q') {
        isWaterFalling = false;
    }
    else if (key == 'c' || key == 'C') {
        if (!cloudsMoving) {
            cloudUpdateInterval = 80;
            cloudsMoving = true;
            glutTimerFunc(cloudUpdateInterval, updateClouds, 0); // Start animation
        }
    }
    else if (key == 'v' || key == 'V') {
        cloudsMoving = false; // Stop cloud movement
    }


    glutPostRedisplay();
}



///////////////Mouse////////////////////
bool waitForDoubleClick = false;

void mouse(int button, int state, int x, int y) {        ///8009
    static int lastClickTime = 0;
    static int clickDelay = 300; //Milliseconds

    if (state == GLUT_DOWN) {
        int currentTime = glutGet(GLUT_ELAPSED_TIME);

        if (button == GLUT_LEFT_BUTTON)
        {
            if (currentTime - lastClickTime < clickDelay)
            {
                // Double-click detected
                headlightOn = true;
                waitForDoubleClick = false; // cancel single-click
            }
            else
            {
                // Start a timer or set a flag for single-click
                waitForDoubleClick = true;
                glutTimerFunc(clickDelay, [](int) {
                    if (waitForDoubleClick)
                    {
                        flowersMoving = true;
                        treesMoving = true;
                        cloudUpdateInterval = 10;
                    }
                    waitForDoubleClick = false;
                }, 0);
            }
            lastClickTime = currentTime;

        }

        else if (button == GLUT_RIGHT_BUTTON)
        {
            if (currentTime - lastClickTime < clickDelay)
            {
                headlightOn = false;
                waitForDoubleClick = false;
            }
            else
            {
                waitForDoubleClick = true;
                glutTimerFunc(clickDelay, [](int) {
                    if (waitForDoubleClick)
                    {
                        flowersMoving = false;
                        treesMoving = false;
                        cloudUpdateInterval = 80;
                    }
                    waitForDoubleClick = false;
                }, 0);
            }
            lastClickTime = currentTime;

        }
        else if (button == GLUT_MIDDLE_BUTTON)
        {
            fullscreen = !fullscreen;

            if (fullscreen)
            {
                glutFullScreen();
            }
            else
            {
                glutReshapeWindow(windowWidth, windowHeight);
                glutPositionWindow(windowPosX, windowPosY);
            }
        }
    }
}



///Main
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(1300, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Middle-Age Scene");
    gluOrtho2D(-5, 20, 0, 8);
    glutDisplayFunc(display);

    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    glutTimerFunc(1, updateCar, 0);
    glutTimerFunc(cloudUpdateInterval, updateClouds, 0);
    glutTimerFunc(5, updateScene, 0);
    glutTimerFunc(10, updateBoat, 0);
    glutTimerFunc(8, updateFlowers, 0);
    glutTimerFunc(40, updateTree, 0);

    initRiverFlow();
    glutTimerFunc(5, updateRiverFlow, 0);

    initWaterfall();
    glutTimerFunc(30, updateWaterfall, 0);

    glutMainLoop();
    return 0;
}
