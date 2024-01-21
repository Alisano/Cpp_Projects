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
#include <unordered_map>
#include <vector>

int WIDTH = 720;
int HEIGHT = 720;

double xpos, ypos;
float GLxpos, GLypos;

static const char* fragment_shader_text = R"(
#version 410 core
precision highp float;
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
)";

static const char* vertex_shader_text = R"(
#version 410 core
layout(location = 0) in vec2 aPos;

void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

const int row = 25;
const int column = 25;

const float x_0 = -1.0;
const float y_0 = -1.0;
const float x_end = 1.0;
const float y_end = 1.0;
const float x_bias = 0.1;
const float y_bias = 0.2;
const float y_0_bias = 0.15;

const int NUMBER_OF_VERTICES = row * column;
float deltaTime = 0.0002f;
float MASS_OF_VERTEX = 0.0001f;
float K_HOOKE_COEF = 50.0f;
float K_HOOKE_COEF_MULT_FOR_VERT_SPRING = 10;
float GRAVITY_ACCEL[2] = {0.0f, -10.0f};
float next_verticle_positions[2];
float DAMP_COEF = -0.01;

typedef struct {
    float accel[2];
    float cur_pos[2];
    float prev_pos[2];
    float islocked = false;
} verticle;

typedef struct {
    float springDistance[2];
    float springDistanceWithoutForce[2];
    float force[2];
    float springDistanceMod;
    float springDistanceWithoutForceMod;
    float bias[2];
    verticle* vert[2];
} spring;

verticle vertices_arr[NUMBER_OF_VERTICES];
spring springs_arr[6 * NUMBER_OF_VERTICES];

float springLength(float length_x, float length_y) {
   return sqrt(length_x * length_x + length_y * length_y);
}

std::vector<float> structToVector() {
    std::vector<float> result;
    for (int i = 0; i < NUMBER_OF_VERTICES; i++) {
        result.push_back(vertices_arr[i].cur_pos[0]);
        result.push_back(vertices_arr[i].cur_pos[1]);
    }
    return result;
}

void calculateNewPositionFromSprings() {
    for (int i = 0; i < NUMBER_OF_VERTICES; i++) {
        if(!vertices_arr[i].islocked) {
            next_verticle_positions[0] = 2 * vertices_arr[i].cur_pos[0] - vertices_arr[i].prev_pos[0] + vertices_arr[i].accel[0] * deltaTime * deltaTime;
            next_verticle_positions[1] = 2 * vertices_arr[i].cur_pos[1] - vertices_arr[i].prev_pos[1] + vertices_arr[i].accel[1] * deltaTime * deltaTime;
            vertices_arr[i].prev_pos[0] = vertices_arr[i].cur_pos[0];
            vertices_arr[i].prev_pos[1] = vertices_arr[i].cur_pos[1];
            vertices_arr[i].cur_pos[0] = next_verticle_positions[0];
            vertices_arr[i].cur_pos[1] = next_verticle_positions[1];
        }
    }
}

void calculateAccelerationFromSprings() {
    int j = 0;
    for (int i = 0; i < NUMBER_OF_VERTICES; i++) {
        float force_v1[2] = {0.0f, 0.0f};
        float force_v2[2] = {0.0f, 0.0f};
        float force_v3[2] = {0.0f, 0.0f};
        float force_v4[2] = {0.0f, 0.0f};
        float force_v5[2] = {0.0f, 0.0f};
        float force_v6[2] = {0.0f, 0.0f};
        float correction_x = 0.0;
        float correction_y = 0.0;
        float k_linear_coef = 0.0;

        if ((i + 1) % column != 0) {
            springs_arr[j].springDistance[0] = (*(springs_arr[j].vert[1])).cur_pos[0] - (*(springs_arr[j].vert[0])).cur_pos[0];
            springs_arr[j].springDistance[1] = (*(springs_arr[j].vert[1])).cur_pos[1] - (*(springs_arr[j].vert[0])).cur_pos[1];
            springs_arr[j].springDistanceMod = springLength(springs_arr[j].springDistance[0], springs_arr[j].springDistance[1]);
            springs_arr[j].force[0] = K_HOOKE_COEF_MULT_FOR_VERT_SPRING * K_HOOKE_COEF * (springs_arr[j].springDistance[0] - springs_arr[j].springDistanceWithoutForce[0]); 
            springs_arr[j].force[1] = K_HOOKE_COEF_MULT_FOR_VERT_SPRING * K_HOOKE_COEF * (springs_arr[j].springDistance[1] - springs_arr[j].springDistanceWithoutForce[1]); 
            springs_arr[j].springDistanceMod = springLength(springs_arr[j].springDistance[0], springs_arr[j].springDistance[1]);            
            force_v1[0] = springs_arr[j].force[0];
            force_v1[1] = springs_arr[j].force[1];
        }

        if ((i >= (column + 1)) && (i < (NUMBER_OF_VERTICES + column + 1))) {
            springs_arr[j + 1].springDistance[0] = (*(springs_arr[j + 1].vert[1])).cur_pos[0] - (*(springs_arr[j + 1].vert[0])).cur_pos[0];
            springs_arr[j + 1].springDistance[1] = (*(springs_arr[j + 1].vert[1])).cur_pos[1] - (*(springs_arr[j + 1].vert[0])).cur_pos[1];
            springs_arr[j + 1].springDistanceMod = springLength(springs_arr[j + 1].springDistance[0], springs_arr[j + 1].springDistance[1]);
            springs_arr[j + 1].force[0] = K_HOOKE_COEF * (springs_arr[j + 1].springDistance[0] - springs_arr[j + 1].springDistanceWithoutForce[0]); 
            springs_arr[j + 1].force[1] = K_HOOKE_COEF * (springs_arr[j + 1].springDistance[1] - springs_arr[j + 1].springDistanceWithoutForce[1]); 
            springs_arr[j + 1].springDistanceMod = springLength(springs_arr[j + 1].springDistance[0], springs_arr[j + 1].springDistance[1]);
            force_v2[0] = springs_arr[j + 1].force[0];
            force_v2[1] = springs_arr[j + 1].force[1];
        }

        if (i % column != 0) {
            springs_arr[j + 2].springDistance[0] = (*(springs_arr[j + 2].vert[1])).cur_pos[0] - (*(springs_arr[j + 2].vert[0])).cur_pos[0];
            springs_arr[j + 2].springDistance[1] = (*(springs_arr[j + 2].vert[1])).cur_pos[1] - (*(springs_arr[j + 2].vert[0])).cur_pos[1];
            springs_arr[j + 2].springDistanceMod = springLength(springs_arr[j + 2].springDistance[0], springs_arr[j + 2].springDistance[1]);
            springs_arr[j + 2].force[0] = K_HOOKE_COEF_MULT_FOR_VERT_SPRING * K_HOOKE_COEF * (springs_arr[j + 2].springDistance[0] - springs_arr[j + 2].springDistanceWithoutForce[0]); 
            springs_arr[j + 2].force[1] = K_HOOKE_COEF_MULT_FOR_VERT_SPRING * K_HOOKE_COEF * (springs_arr[j + 2].springDistance[1] - springs_arr[j + 2].springDistanceWithoutForce[1]); 
            springs_arr[j + 2].springDistanceMod = springLength(springs_arr[j + 2].springDistance[0], springs_arr[j + 2].springDistance[1]);
            force_v3[0] = springs_arr[j + 2].force[0];
            force_v3[1] = springs_arr[j + 2].force[1];
        }

        if (i < (NUMBER_OF_VERTICES - column - 1)) {
            springs_arr[j + 3].springDistance[0] = (*(springs_arr[j + 3].vert[1])).cur_pos[0] - (*(springs_arr[j + 3].vert[0])).cur_pos[0];
            springs_arr[j + 3].springDistance[1] = (*(springs_arr[j + 3].vert[1])).cur_pos[1] - (*(springs_arr[j + 3].vert[0])).cur_pos[1];
            springs_arr[j + 3].springDistanceMod = springLength(springs_arr[j + 3].springDistance[0], springs_arr[j + 3].springDistance[1]);
            springs_arr[j + 3].force[0] = K_HOOKE_COEF * (springs_arr[j + 3].springDistance[0] - springs_arr[j + 3].springDistanceWithoutForce[0]); 
            springs_arr[j + 3].force[1] = K_HOOKE_COEF * (springs_arr[j + 3].springDistance[1] - springs_arr[j + 3].springDistanceWithoutForce[1]); 
            springs_arr[j + 3].springDistanceMod = springLength(springs_arr[j + 3].springDistance[0], springs_arr[j + 3].springDistance[1]);
            force_v4[0] = springs_arr[j + 3].force[0];
            force_v4[1] = springs_arr[j + 3].force[1];
        }

        if (i < (NUMBER_OF_VERTICES - column)) {
            springs_arr[j + 4].springDistance[0] = (*(springs_arr[j + 4].vert[1])).cur_pos[0] - (*(springs_arr[j + 4].vert[0])).cur_pos[0];
            springs_arr[j + 4].springDistance[1] = (*(springs_arr[j + 4].vert[1])).cur_pos[1] - (*(springs_arr[j + 4].vert[0])).cur_pos[1];
            springs_arr[j + 4].springDistanceMod = springLength(springs_arr[j + 4].springDistance[0], springs_arr[j + 4].springDistance[1]);
            springs_arr[j + 4].force[0] = K_HOOKE_COEF * (springs_arr[j + 4].springDistance[0] - springs_arr[j + 4].springDistanceWithoutForce[0]); 
            springs_arr[j + 4].force[1] = K_HOOKE_COEF * (springs_arr[j + 4].springDistance[1] - springs_arr[j + 4].springDistanceWithoutForce[1]); 
            springs_arr[j + 4].springDistanceMod = springLength(springs_arr[j + 4].springDistance[0], springs_arr[j + 4].springDistance[1]);
            force_v5[0] = springs_arr[j + 4].force[0];
            force_v5[1] = springs_arr[j + 4].force[1];
        }

        if (i >= column) {
            springs_arr[j + 5].springDistance[0] = (*(springs_arr[j + 5].vert[1])).cur_pos[0] - (*(springs_arr[j + 5].vert[0])).cur_pos[0];
            springs_arr[j + 5].springDistance[1] = (*(springs_arr[j + 5].vert[1])).cur_pos[1] - (*(springs_arr[j + 5].vert[0])).cur_pos[1];
            springs_arr[j + 5].springDistanceMod = springLength(springs_arr[j + 5].springDistance[0], springs_arr[j + 5].springDistance[1]);
            springs_arr[j + 5].force[0] = K_HOOKE_COEF * (springs_arr[j + 5].springDistance[0] - springs_arr[j + 5].springDistanceWithoutForce[0]);
            springs_arr[j + 5].force[1] = K_HOOKE_COEF * (springs_arr[j + 5].springDistance[1] - springs_arr[j + 5].springDistanceWithoutForce[1]); 
            springs_arr[j + 5].springDistanceMod = springLength(springs_arr[j + 5].springDistance[0], springs_arr[j + 5].springDistance[1]);
            force_v6[0] = springs_arr[j + 5].force[0];
            force_v6[1] = springs_arr[j + 5].force[1];
        }

        float total_force[2];
        total_force[0] = force_v1[0] + force_v2[0] + force_v3[0] + force_v4[0] + force_v5[0] + force_v6[0] + GRAVITY_ACCEL[0] * MASS_OF_VERTEX;
        total_force[1] = force_v1[1] + force_v2[1] + force_v3[1] + force_v4[1] + force_v5[1] + force_v6[1] + GRAVITY_ACCEL[1] * MASS_OF_VERTEX;

        if(!vertices_arr[i].islocked) {
            total_force[0] += DAMP_COEF * (vertices_arr[i].cur_pos[0] - vertices_arr[i].prev_pos[0]) / deltaTime;
            total_force[1] += DAMP_COEF * (vertices_arr[i].cur_pos[1] - vertices_arr[i].prev_pos[1]) / deltaTime;
            vertices_arr[i].accel[0] = total_force[0] / MASS_OF_VERTEX;
            vertices_arr[i].accel[1] = total_force[1] / MASS_OF_VERTEX;            
        } else {
            vertices_arr[i].accel[0] = 0.0f;
            vertices_arr[i].accel[1] = 0.0f; 
        }
        j += 6;
    }
}

void ClientToOpenGL(double* xpos, double* ypos, double * x0, double * y0, double * z0) {
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
bool buttonIsPressed = false;
bool pointIsDrag = false;
float roundingPoint[4];

void cursor_position_callback(GLFWwindow* window) {
    if (buttonIsPressed) {
        glfwGetCursorPos(window, &xpos, &ypos);
        roundingPoint[2] = 1000;
        float searchRadius = 1.0;
        float tmp_length = 0.0;
        double x, y, z = 0;
        x = (x_end - x_0) * (xpos / WIDTH) + x_0;
        y = (y_0 - y_end) * (ypos / HEIGHT) - y_0;

        if (!pointIsDrag) {
            for (int i = 0; i < NUMBER_OF_VERTICES; i++) {
                tmp_length = sqrt(pow((vertices_arr[i].cur_pos[0] - x), 2.0) + pow((vertices_arr[i].cur_pos[1] - y), 2.0));
                if ((tmp_length <= searchRadius) && (tmp_length <= roundingPoint[2])) {
                    roundingPoint[0] = x;
                    roundingPoint[1] = y;
                    roundingPoint[2] = tmp_length;
                    roundingPoint[3] = float(i);
                    pointIsDrag = true;
                }
            }
        }

        if (pointIsDrag) {
            vertices_arr[int(roundingPoint[3])].cur_pos[0] = x;
            vertices_arr[int(roundingPoint[3])].cur_pos[1] = y;
            vertices_arr[int(roundingPoint[3])].prev_pos[0] = x;
            vertices_arr[int(roundingPoint[3])].prev_pos[1] = y;
            vertices_arr[int(roundingPoint[3])].accel[0] = 0.0;
            vertices_arr[int(roundingPoint[3])].accel[1] = 0.0;
        }
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double x, y, z = 0;
    if ((button == GLFW_MOUSE_BUTTON_LEFT) && (action == GLFW_PRESS)) {
        buttonIsPressed = true;
    } else {
        buttonIsPressed = false;
        pointIsDrag = false;
    }
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Cloths simulation", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // glfwSetCursorPosCallback(window, cursor_position_callback);
    gladLoadGL(glfwGetProcAddress);

    std::vector<float> vertices;
    std::vector<int> indices;

    int index = 0;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            vertices.push_back(x_0 + x_bias + (x_end - x_0 - 2 * x_bias) / float(column - 1) * float(j));
            vertices.push_back(y_0_bias + y_end - y_bias - (y_end - y_0 - 2 * y_bias) / float(row - 1) * float(i));

            index = i * column + j;
            if ((i != row - 1) && (j != column - 1)) {
                indices.push_back(index);
                indices.push_back(index + 1);
                indices.push_back(index + column);
                
                indices.push_back(index + 1);
                indices.push_back(index + column + 1);
                indices.push_back(index + column);
            }
        }
    }

    GLuint vertexVBO;
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * NUMBER_OF_VERTICES * 2, vertices.data(), GL_STATIC_DRAW);

    GLuint vertexEBO;
    glGenBuffers(1, &vertexEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

    GLuint vertexVAO;
    glGenVertexArrays(1, &vertexVAO);
    glBindVertexArray(vertexVAO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexEBO);
    glBindVertexArray(0);

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

    for (int i = 0; i < NUMBER_OF_VERTICES; i++) {
        vertices_arr[i].cur_pos[0] = vertices[2 * i];
        vertices_arr[i].cur_pos[1] = vertices[2 * i + 1];
        vertices_arr[i].prev_pos[0] = vertices[2 * i];
        vertices_arr[i].prev_pos[1] = vertices[2 * i + 1];
        vertices_arr[i].accel[0] = 0.0f;
        vertices_arr[i].accel[1] = 0.0f;
    }

    int j = 0;
    for (int i = 0; i < NUMBER_OF_VERTICES; i++) {

        springs_arr[j].vert[0] = &vertices_arr[i];
        springs_arr[j + 1].vert[0] = &vertices_arr[i];
        springs_arr[j + 2].vert[0] = &vertices_arr[i];
        springs_arr[j + 3].vert[0] = &vertices_arr[i];
        springs_arr[j + 4].vert[0] = &vertices_arr[i];
        springs_arr[j + 5].vert[0] = &vertices_arr[i];

        springs_arr[j].vert[1] = &vertices_arr[i];
        springs_arr[j + 1].vert[1] = &vertices_arr[i];
        springs_arr[j + 2].vert[1] = &vertices_arr[i];
        springs_arr[j + 3].vert[1] = &vertices_arr[i];
        springs_arr[j + 4].vert[1] = &vertices_arr[i];
        springs_arr[j + 5].vert[1] = &vertices_arr[i];

        springs_arr[j].springDistanceWithoutForce[0] = 0.0f;
        springs_arr[j].springDistanceWithoutForce[1] = 0.0f;
        springs_arr[j + 1].springDistanceWithoutForce[0] = 0.0f;
        springs_arr[j + 1].springDistanceWithoutForce[1] = 0.0f;
        springs_arr[j + 2].springDistanceWithoutForce[0] = 0.0f;
        springs_arr[j + 2].springDistanceWithoutForce[1] = 0.0f;
        springs_arr[j + 3].springDistanceWithoutForce[0] = 0.0f;
        springs_arr[j + 3].springDistanceWithoutForce[1] = 0.0f;
        springs_arr[j + 4].springDistanceWithoutForce[0] = 0.0f;
        springs_arr[i + 4].springDistanceWithoutForce[1] = 0.0f;
        springs_arr[j + 5].springDistanceWithoutForce[0] = 0.0f;
        springs_arr[j + 5].springDistanceWithoutForce[1] = 0.0f;

        springs_arr[j].springDistance[0] = 0.0f;
        springs_arr[j].springDistance[1] = 0.0f;
        springs_arr[j + 1].springDistance[0] = 0.0f;
        springs_arr[j + 1].springDistance[1] = 0.0f;
        springs_arr[j + 2].springDistance[0] = 0.0f;
        springs_arr[j + 2].springDistance[1] = 0.0f;
        springs_arr[j + 3].springDistance[0] = 0.0f;
        springs_arr[j + 3].springDistance[1] = 0.0f;
        springs_arr[j + 4].springDistance[0] = 0.0f;
        springs_arr[j + 4].springDistance[1] = 0.0f;
        springs_arr[j + 5].springDistance[0] = 0.0f;
        springs_arr[j + 5].springDistance[1] = 0.0f;

        if ((i + 1) % column != 0) {
            springs_arr[j].vert[1] = &vertices_arr[i + 1];
            springs_arr[j].springDistance[0] = (*(springs_arr[j].vert[1])).cur_pos[0] - (*(springs_arr[j].vert[0])).cur_pos[0];
            springs_arr[j].springDistance[1] = (*(springs_arr[j].vert[1])).cur_pos[1] - (*(springs_arr[j].vert[0])).cur_pos[1];
            springs_arr[j].springDistanceWithoutForce[0] = springs_arr[j].springDistance[0];
            springs_arr[j].springDistanceWithoutForce[1] = springs_arr[j].springDistance[1];
            springs_arr[j].springDistanceWithoutForceMod = springLength(springs_arr[j].springDistanceWithoutForce[0], springs_arr[j].springDistanceWithoutForce[1]);
            springs_arr[j].springDistanceMod = springs_arr[j].springDistanceWithoutForceMod;
        }

        if ((i >= (column + 1)) && (i < (NUMBER_OF_VERTICES + column + 1))) {
            springs_arr[j + 1].vert[1] = &vertices_arr[i - column - 1];
            springs_arr[j + 1].springDistance[0] = (*(springs_arr[j + 1].vert[1])).cur_pos[0] - (*(springs_arr[j + 1].vert[0])).cur_pos[0];
            springs_arr[j + 1].springDistance[1] = (*(springs_arr[j + 1].vert[1])).cur_pos[1] - (*(springs_arr[j + 1].vert[0])).cur_pos[1];
            springs_arr[j + 1].springDistanceWithoutForce[0] = springs_arr[j + 1].springDistance[0];
            springs_arr[j + 1].springDistanceWithoutForce[1] = springs_arr[j + 1].springDistance[1];
            springs_arr[j + 1].springDistanceWithoutForceMod = springLength(springs_arr[j + 1].springDistanceWithoutForce[0], springs_arr[j + 1].springDistanceWithoutForce[1]);
            springs_arr[j + 1].springDistanceMod = springs_arr[j + 1].springDistanceWithoutForceMod;
        }

        if (i % column != 0) {
            springs_arr[j + 2].vert[1] = &vertices_arr[i - 1];
            springs_arr[j + 2].springDistance[0] = (*(springs_arr[j + 2].vert[1])).cur_pos[0] - (*(springs_arr[j + 2].vert[0])).cur_pos[0];
            springs_arr[j + 2].springDistance[1] = (*(springs_arr[j + 2].vert[1])).cur_pos[1] - (*(springs_arr[j + 2].vert[0])).cur_pos[1];
            springs_arr[j + 2].springDistanceWithoutForce[0] = springs_arr[j + 2].springDistance[0];
            springs_arr[j + 2].springDistanceWithoutForce[1] = springs_arr[j + 2].springDistance[1];
            springs_arr[j + 2].springDistanceWithoutForceMod = springLength(springs_arr[j + 2].springDistanceWithoutForce[0], springs_arr[j + 2].springDistanceWithoutForce[1]);
            springs_arr[j + 2].springDistanceMod = springs_arr[j + 2].springDistanceWithoutForceMod;
        }

        if (i < (NUMBER_OF_VERTICES - column - 1)) {
            springs_arr[j + 3].vert[1] = &vertices_arr[i + column + 1];
            springs_arr[j + 3].springDistance[0] = (*(springs_arr[j + 3].vert[1])).cur_pos[0] - (*(springs_arr[j + 3].vert[0])).cur_pos[0];
            springs_arr[j + 3].springDistance[1] = (*(springs_arr[j + 3].vert[1])).cur_pos[1] - (*(springs_arr[j + 3].vert[0])).cur_pos[1];
            springs_arr[j + 3].springDistanceWithoutForce[0] = springs_arr[j + 3].springDistance[0];
            springs_arr[j + 3].springDistanceWithoutForce[1] = springs_arr[j + 3].springDistance[1];
            springs_arr[j + 3].springDistanceWithoutForceMod = springLength(springs_arr[j + 3].springDistanceWithoutForce[0], springs_arr[j + 3].springDistanceWithoutForce[1]);
            springs_arr[j + 3].springDistanceMod = springs_arr[j + 3].springDistanceWithoutForceMod;
        }

        if (i < (NUMBER_OF_VERTICES - column)) {
            springs_arr[j + 4].vert[1] = &vertices_arr[i + column];
            springs_arr[j + 4].springDistance[0] = (*(springs_arr[j + 4].vert[1])).cur_pos[0] - (*(springs_arr[j + 4].vert[0])).cur_pos[0];
            springs_arr[j + 4].springDistance[1] = (*(springs_arr[j + 4].vert[1])).cur_pos[1] - (*(springs_arr[j + 4].vert[0])).cur_pos[1];
            springs_arr[j + 4].springDistanceWithoutForce[0] = springs_arr[j + 4].springDistance[0];
            springs_arr[j + 4].springDistanceWithoutForce[1] = springs_arr[j + 4].springDistance[1];
            springs_arr[j + 4].springDistanceWithoutForceMod = springLength(springs_arr[j + 4].springDistanceWithoutForce[0], springs_arr[j + 4].springDistanceWithoutForce[1]);
            springs_arr[j + 4].springDistanceMod = springs_arr[j + 4].springDistanceWithoutForceMod;
        }

        if (i >= column) {
            springs_arr[j + 5].vert[1] = &vertices_arr[i - column];
            springs_arr[j + 5].springDistance[0] = (*(springs_arr[j + 5].vert[1])).cur_pos[0] - (*(springs_arr[j + 5].vert[0])).cur_pos[0];
            springs_arr[j + 5].springDistance[1] = (*(springs_arr[j + 5].vert[1])).cur_pos[1] - (*(springs_arr[j + 5].vert[0])).cur_pos[1];
            springs_arr[j + 5].springDistanceWithoutForce[0] = springs_arr[j + 5].springDistance[0];
            springs_arr[j + 5].springDistanceWithoutForce[1] = springs_arr[j + 5].springDistance[1];
            springs_arr[j + 5].springDistanceWithoutForceMod = springLength(springs_arr[j + 5].springDistanceWithoutForce[0], springs_arr[j + 5].springDistanceWithoutForce[1]);
            springs_arr[j + 5].springDistanceMod = springs_arr[j + 5].springDistanceWithoutForceMod;
        }
        j += 6;
    }

    vertices_arr[0].islocked = true;
    vertices_arr[column - 1].islocked = true;
 
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "OpenGL error: " << error << std::endl;
        }

        glBindVertexArray(vertexVAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexEBO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        calculateAccelerationFromSprings();
        calculateNewPositionFromSprings();
        cursor_position_callback(window);

        vertices = structToVector();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * NUMBER_OF_VERTICES * 2, vertices.data());

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &vertexVAO);
    glDeleteBuffers(1, &vertexVBO);
    glDeleteBuffers(1, &vertexEBO);
    glDeleteProgram(program);
    glfwTerminate();
    exit(EXIT_SUCCESS);
    return 0;
}