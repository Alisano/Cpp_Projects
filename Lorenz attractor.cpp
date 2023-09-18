
// #include <OpenGL/gl3.h>
#include <glad/gl.h>
#include <glad.c>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GL_SILENCE_DEPRECATION
#include <math.h>
#include <ctime>
#include <OpenGL/glu.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int WIDTH = 720;
int HEIGHT = 720;

float OX = 60;
float OY = 60;
float OZ = 60;

double xpos, ypos;
float GLxpos, GLypos;

float circle_radius = 1;
float cnt_of_circle_pol = 20;
float angle_circle = 2 * 3.1415926 / cnt_of_circle_pol;


float dt = 0.01;

void circle(float x0, float y0, float z0) {
    float radius = circle_radius;

    glBegin(GL_TRIANGLE_FAN);
        float x, y;
        for (int i = 0; i < cnt_of_circle_pol; i ++){
            x = cos(angle_circle * i) * radius;
            y = sin(angle_circle * i) * radius;
            glVertex3f(x + x0, y + y0, z0);
        }
    glEnd();
}

void ClientToOpenGL(double* xpos, double* ypos, double * x0, double * y0, double * z0){
    int vp[4];
    double mMatrix[16], pMatrix[16];
    float z;
    int x = int(*xpos);
    int y = int(*ypos);
    glGetIntegerv(GL_VIEWPORT, vp);
    y = vp[3] - y - 1;
    glGetDoublev(GL_MODELVIEW_MATRIX, mMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, pMatrix);
    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
    gluUnProject(x, y, z, mMatrix, pMatrix, vp, x0, y0, z0);
    GLxpos = *x0;
    GLypos = *y0;
}

int counter = 0;
const int points_size = 1000000;
float arr_points[3][points_size];

void points_draw(int counter){
    glPointSize(3);
    glBegin(GL_POINTS);
        glColor3f(1.0, 1.0, 1.0);
        for ( int i = 0; i < counter; i ++)
            glVertex3f(arr_points[0][i], arr_points[1][i], arr_points[2][i]);
    glEnd();  
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Lorenz attractor", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);

    glOrtho(-OX,OX, -OY,OY, -OZ,OZ);

    float pos[3] = {0.1, 0.1, 0.1};
    float vel[3] = {0.0, 0.0, 0.0};

    float a = 10;
    float b = 28;
    float c = 8 / 3;

    while (!glfwWindowShouldClose(window))
    {
        glfwGetFramebufferSize(window, &WIDTH, &HEIGHT);
        glViewport(0, 0, WIDTH, HEIGHT);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(1.0, 1.0, 1.0);

        glPushMatrix();

        circle(pos[0], pos[1], pos[2]);

        vel[0] = a * (pos[1] - pos[0]);
        vel[1] = pos[0] * (b - pos[2]) - pos[1];
        vel[2] = pos[0] * pos[1] - c * pos[2];

        pos[0] += vel[0] * dt;
        pos[1] += vel[1] * dt;
        pos[2] += vel[2] * dt;

        arr_points[0][counter] = pos[0];
        arr_points[1][counter] = pos[1];
        arr_points[2][counter] = pos[2];

        if (counter < points_size)
            points_draw(counter);  
        else 
            counter = -1;

        glPopMatrix();

        glRotatef(0.02, 1, 1, 0);
    
        glfwSwapBuffers(window);
        glfwPollEvents();
        counter ++;
    }
    glfwTerminate();
    exit(EXIT_SUCCESS);
}