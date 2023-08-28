
// #include <OpenGL/gl3.h>
#include <glad/gl.h>
#include <glad.c>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GL_SILENCE_DEPRECATION
#include <math.h>
#include <iostream>
#include <ctime>

int WIDTH = 1080;
int HEIGHT = 720;

float g = 9.8;
float m1 = 41;
float m2 = 40;

float theta1 = 3.1415926 / 2;
float theta2 = 3.1415926 / 3;

float R1 = 200;
float R2 = 200;

float circle_radius = 15;
float cnt_of_circle_pol = 100;
float angle_circle = 2 * 3.1415926 / cnt_of_circle_pol;

float accel_1(float theta1, float theta2, float vel_1, float vel_2) {
    return (-g * (2 * m1 + m2) * sin(theta1) - m2*g*sin(theta1 - 2 * theta2) - 
        2 * sin(theta1 - theta2) * m2 * (vel_2 * vel_2 * R2 + 
        vel_1 * vel_1 * R1 * cos(theta1 - theta2))) / (R1 * (2 * m1 + m2 - m2 * cos(2 * theta1 - 2 * theta2)));
}

float accel_2(float theta1, float theta2, float vel_1, float vel_2) {
    return (2 * sin(theta1 - theta2) * (vel_1 * vel_1 * R1 * (m1 + m2) + 
    g * (m1 + m2) * cos(theta1) + vel_2 * vel_2 * R2 * m2 * cos(theta1 - theta2))) / (R2 * (2 * m1 + m2 - m2 * cos(2 * theta1 - 2 * theta2)));
}

void circle(float x0, float y0) {
    glBegin(GL_TRIANGLE_FAN);
        glColor3f(1.0, 0.0, 0.0);
        float x, y;
        for (int i = 0; i < cnt_of_circle_pol; i ++){
            x = cos(angle_circle * i) * circle_radius;
            y = sin(angle_circle * i) * circle_radius;
            glVertex2f(x + x0, y + y0);
        }
    glEnd();
}

const int points_size = 100000;

float arr_points[2][points_size];

void points_draw(int counter){
    glPointSize(3);
    glBegin(GL_POINTS);
        glColor3f(1.0, 1.0, 1.0);
        for ( int i = 0; i < counter; i ++) {
            glVertex2f(arr_points[0][i], arr_points[1][i]);
        }
    glEnd();  
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Double pendulum", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);

    float vel_1 = 0;
    float vel_2 = 0;

    float dt = 0.1;

    glOrtho(-500,500, -400,400, -1,1);

    int counter = 0;

    while (!glfwWindowShouldClose(window))
    {
        theta1 += vel_1 * dt;
        theta2 += vel_2 * dt;

        vel_1 += accel_1(theta1, theta2, vel_1, vel_2) * dt;
        vel_2 += accel_2(theta1, theta2, vel_1, vel_2) * dt;

        glfwGetFramebufferSize(window, &WIDTH, &HEIGHT);
        glViewport(0, 0, WIDTH, HEIGHT);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glPushMatrix();
        glTranslatef(0.0, 200, 0.0);
   
        glLineWidth(5);
        glBegin(GL_LINE_STRIP);
            glColor3f(1.0, 1.0, 1.0);
            glVertex2f(0.0, 0.0);
            glVertex2f(R1 * cos(theta1 - 3.1415926 / 2), R1 * sin(theta1 - 3.1415926 / 2));
            glVertex2f(R1 * cos(theta1 - 3.1415926 / 2), R1 * sin(theta1 - 3.1415926 / 2));
            glVertex2f(R1 * cos(theta1 - 3.1415926 / 2) + R2 * cos(theta2 - 3.1415926 / 2), 
                R1 * sin(theta1 - 3.1415926 / 2) + R2 * sin(theta2 - 3.1415926 / 2));
        glEnd();

        glPointSize(15);
        glBegin(GL_POINTS);
            glColor3f(0.0, 0.0, 1.0);
            glVertex2f(0, 0.0);
            glColor3f(1.0, 0.0, 0.0);
        glEnd();

        circle(R1 * cos(theta1 - 3.1415926 / 2), R1 * sin(theta1 - 3.1415926 / 2));
        circle(R1 * cos(theta1 - 3.1415926 / 2) + R2 * cos(theta2 - 3.1415926 / 2), 
            R1 * sin(theta1 - 3.1415926 / 2) + R2 * sin(theta2 - 3.1415926 / 2));
        
        arr_points [0][counter] = R1 * cos(theta1 - 3.1415926 / 2) + R2 * cos(theta2 - 3.1415926 / 2);
        arr_points [1][counter] = R1 * sin(theta1 - 3.1415926 / 2) + R2 * sin(theta2 - 3.1415926 / 2);

        if (counter < points_size)
            points_draw(counter);  
        else 
            counter = -1;

        glPopMatrix();
        glfwSwapBuffers(window);
        glfwPollEvents();
        counter ++;
    }

    glfwTerminate();
    exit(EXIT_SUCCESS);
}