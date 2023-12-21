
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
#include <iostream>

int WIDTH = 720;
int HEIGHT = 720;

// float OX = 60;
// float OY = 60;
// float OZ = 60;

double xpos, ypos;
float GLxpos, GLypos;

static const char* fragment_shader_text = R"(
#version 410 core
precision highp float;
uniform vec2 u_resolution;
out vec4 FragColor;
float size = 50.0;

float interpolate(float a0, float a1, float w) {
    return (a1 - a0) * ((w * (w * 6.0 - 15.0) + 10.0) * w * w * w) + a0;
}

vec2 randomGradient(int ix, int iy) {
    uint w = 8 * 4;
    uint s = w / 2; // rotation width
    uint a = ix, b = iy;
    a *= 3284157443; b ^= a << s | a >> w-s;
    b *= 1911520717; a ^= b << s | b >> w-s;
    a *= 2048419325;
    float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]
    return vec2(cos(random), sin(random));
}

float dotGridGradient(int ix, int iy, float x, float y) {
    vec2 gradient = randomGradient(ix, iy);

    float dx = x - float(ix);
    float dy = y - float(iy);

    return (dx*gradient.x + dy*gradient.y);
}

float perlin(float x, float y) {
    int x0 = int(floor(x));
    int x1 = x0 + 1;
    int y0 = int(floor(y));
    int y1 = y0 + 1;

    float sx = x - float(x0);
    float sy = y - float(y0);

    float n0 = dotGridGradient(x0, y0, x, y);
    float n1 = dotGridGradient(x1, y0, x, y);
    float ix0 = interpolate(n0, n1, sx);

    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    float ix1 = interpolate(n0, n1, sx);

    float value = interpolate(ix0, ix1, sy);
    return value * 0.5 + 0.5; // Will return in range 0 to 1
}

void main() {
  float color = perlin(size * gl_FragCoord.x / u_resolution.x, size * gl_FragCoord.y / u_resolution.y);
  FragColor = vec4(color, color, color, 1.0);
}
)";

static const char* vertex_shader_text = R"(
#version 410 core
layout(location = 0) in vec2 pos;
void main() {
   gl_Position = vec4(pos,0.0,1.0);
}
)";

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

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Perlin noise", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);

    // glOrtho(-OX,OX, -OY,OY, -OZ,OZ);

    float vertices[] = {
    -1.0f, -1.0f,
    1.0f, -1.0f,
    1.0f,  1.0f,

    1.0f,  1.0f,
    -1.0f,  1.0f,
    -1.0f, -1.0f
    };

    GLuint vertexVAO, vertexVBO;

    glGenVertexArrays(1, &vertexVAO);
    glBindVertexArray(vertexVAO);
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);


    GLint ok;
    GLchar log[2000];
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(fragment_shader, 2000, NULL, log);
        std::cout << log;
    }

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(vertex_shader, 2000, NULL, log);
        std::cout << log;
    }


    GLuint program = glCreateProgram();
    glAttachShader(program, fragment_shader);
    glAttachShader(program, vertex_shader);
    glLinkProgram(program);

    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);

    glUseProgram(program);

    while (!glfwWindowShouldClose(window))
    {
        glfwGetFramebufferSize(window, &WIDTH, &HEIGHT);
        // glViewport(0, 0, WIDTH, HEIGHT);
        // glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        
        glClear(GL_COLOR_BUFFER_BIT);
        glUniform2f(glGetUniformLocation(program, "u_resolution"), (float)WIDTH, (float)HEIGHT);

        // glBindAttribLocation(program, 0, "pos");
        glDrawArrays(GL_TRIANGLES, 0, 6);
    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &vertexVAO);
    glDeleteBuffers(1, &vertexVBO);
    glDeleteProgram(program);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}