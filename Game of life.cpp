
// #include <OpenGL/gl3.h>
#include <glad/gl.h>
#include <glad.c>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GL_SILENCE_DEPRECATION
#include <math.h>
#include <iostream>
#include <ctime>
#include <OpenGL/glu.h>
#include <unistd.h>

int WIDTH = 1080;
int HEIGHT = 720;

double xpos, ypos;
double GLxpos, GLypos;

bool StartGame = false;

float di = 20;
float dj = 20;

const int indexOX = 40;
const int indexOY = 40;

int map[indexOX][indexOY];
int tmp_map[indexOX][indexOY];

float OX = indexOX * di / 2;
float OY = indexOY * dj / 2;

void square(float x, float y, float side){
    glBegin(GL_TRIANGLE_STRIP);
        glVertex2f(x, y);
        glVertex2f(x + side, y);
        glVertex2f(x + side, y - side);
        glVertex2f(x + side, y - side);
        glVertex2f(x, y - side);
        glVertex2f(x, y);
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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double x, y, z = 0;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &xpos, &ypos);
        ClientToOpenGL(&xpos, &ypos, &x, &y, &z);
    }
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        StartGame = !(StartGame);
}

int main(void)
{
    for (int i = 0; i < indexOX; i++)
        for (int j = 0; j < indexOY; j++) {
            map[i][j] = 0;
            tmp_map[i][j] = 0;
        }

    GLFWwindow* window;

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Game of life", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);
    // glfwSetCursorPosCallback(window, cursor_position_callback);
    gladLoadGL(glfwGetProcAddress);

    glOrtho(-OX,OX, -OY,OY, -1,1);

    int summ = 0;
    int count = 0;

    bool flag = true;

    while (!glfwWindowShouldClose(window))
    {
        glfwGetFramebufferSize(window, &WIDTH, &HEIGHT);
        glViewport(0, 0, WIDTH, HEIGHT);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        float ratio = WIDTH / (float) HEIGHT;
        glPushMatrix();

        if (GLxpos != 0 || GLypos != 0) {
            int i = int((GLxpos + OX) * 2 / di);
            int j = int((OY - GLypos) * 2 / dj);
            if (map[i][j] == 0)
                map[i][j] = 1;
            else
                map[i][j] = 0;
            GLxpos = 0;
            GLypos = 0;
        }

        for (int i = 0; i < indexOX; i++)
            for (int j = 0; j < indexOY; j++) {
                if (map[i][j] == 0)
                    glColor3f(0.0, 0.0, 1.0);
                else
                    glColor3f(1.0, 1.0, 1.0);
                square(float(i) * di - OX, (OY - float(j) * dj), 15);
            }
        
        if (StartGame) {
            for (int i = 0; i < indexOX; i++){
                for (int j = 0; j < indexOY; j++) {

                    int a = i - 1;
                    int b = j + 1;
                    int c = i + 1;
                    int d = j - 1;

                    if (i == 0) a = indexOX - 1;
                    if (j == indexOY - 1) b = 0;
                    if (i == indexOX - 1) c = 0;
                    if (j == 0) d = indexOY - 1;

                    // summ = map[a][b] + map[i][b] + map[c][b] + map[a][j] + map[c][j] + map[a][d] + map[i][d] + map[c][d];
                    
                    summ = map[i - 1][j + 1] + map[i][j + 1] + map[i + 1][j + 1] +
                           map[i - 1][j] + map[i + 1][j] + map[i - 1][j - 1] + map[i][j - 1] + map[i + 1][j - 1];

                    if(map[i][j] == 1) {
                        if (summ == 2)
                            tmp_map[i][j] = 1;
                        else if (summ == 3)
                            tmp_map[i][j] = 1;
                        else
                            tmp_map[i][j] = 0;
                    } else {
                        if (summ == 3)
                            tmp_map[i][j] = 1;
                        else
                            tmp_map[i][j] = 0;
                    }
                }
            }
            for (int i = 0; i < indexOX; i++)
                for (int j = 0; j < indexOY; j++)
                    map[i][j] = tmp_map[i][j];
        }

        glPopMatrix();
        glfwSwapBuffers(window);
        glfwPollEvents();
        // usleep(1e5);
    }
    glfwTerminate();
    exit(EXIT_SUCCESS);
}