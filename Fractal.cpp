// #include <glad/gl.h>
// #include <glad.c>
#include <OpenGL/gl3.h>
// #define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GL_SILENCE_DEPRECATION
#include <math.h>
#include <vector>
#include <array>
#include <iostream>
#include <ctime>

int WIDTH = 1080;
int HEIGHT = 720;
float zoom = 1.0;
float move[2] = {0.0, 0.0};

// "  vec2 c = vec2((gl_FragCoord.x / u_resolution.x * 1.0) - 2.1, (gl_FragCoord.y / u_resolution.y * 0.5 - 0.5) * 2.0);\n"
// "  c = c / pow(zoom, 2.0 * zoom) - vec2(move.x, move.y);\n"
// "  c = c / pow(time, 4.0) - vec2(-0.32999999995,0.0507);"
static const char* fragment_shader_text =
"#version 410 core\n"
"#ifdef GL_ES\n"
"precision mediump float;\n"
"#endif\n"
"uniform vec2 u_resolution;\n"
"const float ITERATIONS = 5000;\n"
"vec3 color;\n"
"uniform float time;\n"
"out vec4 FragColor;\n"
"uniform vec2 move;\n"
"uniform float zoom;\n"
"vec3 mandelbrot() {\n"
"  int iter = 0;\n"
"  vec2 c = vec2(gl_FragCoord.x / u_resolution.x - 2.1, gl_FragCoord.y / u_resolution.y);\n"
"  c = c / pow(zoom, 2.0 * zoom) - vec2(move.x, move.y);\n"
"  vec2 z = vec2(0.0);\n"
"  for (int i = 0; i < ITERATIONS * pow(zoom, 1.2); i++) {\n"
"    z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;\n"
"    i = i + 1;"
"    if ((z.x * z.x + z.y * z.y) > 4.0) {\n"
"       if (i > 200) {\n"
"           return vec3(0.0, 0.0, 0.5);\n"
"       }\n"
"       if ((i < 100) && (i > 40)) {\n"
"           return vec3(1.0, 0.0, 1.0);\n"
"       }\n"
"       if ((i < 40) && (i > 30)) {\n"
"           return vec3(1.0, 0.0, 0.0);\n"
"       }\n"
"       if ((i < 30) && (i > 10)) {\n"
"           return vec3(1.0, 1.0, 0.0);\n"
"       }\n"
"       else {\n"
"           return vec3(1.0, 1.0, 1.0);\n"
"       }\n"
"    }\n"
"  }\n"
"  return vec3(0.0, 0.0, 0.0);\n"
"}\n"
"void main() {\n"
"  FragColor = vec4(mandelbrot(), 1.0);\n"
"}\n";

static const char* vertex_shader_text =
"#version 410 core\n"
"layout(location = 0) in vec2 pos;\n"
"void main() {\n"
"   gl_Position = vec4(pos,0.0,1.0);\n"
"}\n";

class Complex_number{
    float re, im;
    public:
    Complex_number (float a, float b): re(a), im(b) {}
    Complex_number operator + (Complex_number& b) { 
        return Complex_number(this->re + b.re, this->im + b.im); 
    }
    Complex_number operator * (Complex_number& b) { 
        return Complex_number(this->re * b.re - this->im * b.im, this->re * b.im + this->im * b.re);
    }
    float module(){
        return sqrt(this->re * this->re + this->im * this->im);
    }
    float get_re() {
        return this->re;
    }
    float get_im() {
        return this->im;
    }
};

std::vector<std::vector<float>> array_coordinates(){

    std::vector<float> x_vec, y_vec;
    float di = 1e-3;
    float dj = 1e-3;
    int max_iteration = 100;
    bool convergence = true;

    for (float i = -2; i <= 1; i += di) {
        for (float j = -1; j <= 1; j += dj) {
            Complex_number z(0, 0);
            Complex_number c(i, j);
            for (int k = 0; k < max_iteration; k++) {
                if (z.module() > 2) {
                    convergence = false;
                    break;
                }
                z = (z * z) + c;
            }
            if(convergence) {
                x_vec.push_back(i);
                y_vec.push_back(j);
            }
            else
                convergence = true;
        }
    }
    std::vector<std::vector<float>> result = {x_vec, y_vec};

    return result;
}

// void Show(std::vector<float> x_coordinates, std::vector<float> y_coordinates){
//     glLoadIdentity();
//     glScalef(0.7, 1.0, 1.0);
//     glTranslatef(0.3, 0.0, 0.0);
//     glBegin(GL_POINTS);
//         glColor3f(0,0,0);
//         for (float i = 0; i < x_coordinates.size(); i ++){
//             glVertex2f(x_coordinates[i], y_coordinates[i]);
//         }
//     glEnd();
// }

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    char state = glfwGetKey(window, key);

    switch (key)
    {
    case GLFW_KEY_W:
        if (state == GLFW_PRESS) {
            move[1] -= 0.1 / pow(zoom, 2.0 * zoom);
        } 
        else {
            break;
        }
        
        break;

    case GLFW_KEY_A:
        if (state == GLFW_PRESS) {
            move[0] += 0.1 / pow(zoom, 2.0 * zoom);
        } 
        else {
            break;
        }
        break;

    case GLFW_KEY_S:
        if (state == GLFW_PRESS) {
            move[1] += 0.1 / pow(zoom, 2.0 * zoom);
        } 
        else {
            break;
        }
        break;

    case GLFW_KEY_D:
        if (state == GLFW_PRESS) {
            move[0] -= 0.1 / pow(zoom, 2.0 * zoom);
        } 
        else {
            break;
        }
        break;

    case GLFW_KEY_Z:
        if (state == GLFW_PRESS) {
            zoom += 0.1;
        } 
        else {
            break;
        }
        break;

    case GLFW_KEY_X:
        if (state == GLFW_PRESS) {
            zoom -= 0.1;
        } 
        else {
            break;
        }
        break;

    default:
        break;
    }
}

int main(void)
{
    clock_t time_req = clock();

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    // gladLoadGL(glfwGetProcAddress);

    // std::vector<std::vector<float>> coordinates = array_coordinates();
    // float* coordinates_arr = new float[coordinates_vec.size()];
    // std::copy(coordinates_vec.begin(), coordinates_vec.end(), coordinates_arr);

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

    float vertices[] = {
    -1.0f, -1.0f,
    1.0f, -1.0f,
    1.0f,  1.0f,

    1.0f,  1.0f,
    -1.0f,  1.0f,
    -1.0f, -1.0f
    };

    // GLuint vertexVBO; 
    // GLuint vertexVAO;
    GLuint vertexVAO, vertexVBO;

    glGenVertexArrays(1, &vertexVAO);
    glBindVertexArray(vertexVAO);
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // glGenBuffers(1, &vertexVBO);
    // glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // ,glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    // glEnableVertexAttribArray(0);

    // glBindAttribLocation(program, 0, "pos");
    // GLint posAttrib = glGetAttribLocation(program, "pos");
    // glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    // glEnableVertexAttribArray(posAttrib);
    // glGenVertexArrays(1, &vertexVAO);

    // glBindVertexArray(vertexVAO);
    //     glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    //     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    // glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO
    // glBindVertexArray(0); // unbind VAO

    // glUseProgram(program);
    // glUniform2f(glGetUniformLocation(program, "u_resolution"), (float)WIDTH, (float)HEIGHT);
    

    /* Loop until the user closes the window */
    // GLint posAttrib = glGetAttribLocation(program, "pos");
    // glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    // glEnableVertexAttribArray(posAttrib);

    // std::cout << glGetString(GL_VERSION);

    // glUniform2f(glGetUniformLocation(program, "u_resolution"), (float)WIDTH, (float)HEIGHT);
    // glBindAttribLocation(program, 0, "pos");
    glUseProgram(program);

    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        // glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

        // glViewport(0, 0, WIDTH, HEIGHT);
        glfwGetWindowSize(window,&WIDTH, &HEIGHT);
        
        glClear(GL_COLOR_BUFFER_BIT);
        glUniform2f(glGetUniformLocation(program, "u_resolution"), (float)WIDTH, (float)HEIGHT);
        glUniform1f(glGetUniformLocation(program, "time"), float(clock()- time_req)/CLOCKS_PER_SEC);
        glUniform1f(glGetUniformLocation(program, "zoom"), zoom);
        glUniform2f(glGetUniformLocation(program, "move"), move[0], move[1]);
        glBindAttribLocation(program, 0, "pos");

        glDrawArrays(GL_TRIANGLES, 0, 6);
       

        // glDrawArrays(GL_TRIANGLES, 0, 6);

        // Show(coordinates[0], coordinates[1]);
        // glLoadIdentity();

        // glUseProgram(program);

        // glBegin(GL_QUADS);
        //         glVertex3f(-1.0f, 1.0f, 0.0f);
        //         glVertex3f( 1.0f, 1.0f, 0.0f);
        //         glVertex3f( 1.0f,-1.0f, 0.0f);
        //         glVertex3f(-1.0f,-1.0f, 0.0f);
        // glEnd();
        // glPushMatrix();
            // glVertexPointer(2, GL_FLOAT, 0, &rect);
            // glEnableClientState(GL_VERTEX_ARRAY);
            //     glDrawArrays(GL_TRIANGLE_FAN, 0, rectCnt);
            // glDisableClientState(GL_VERTEX_ARRAY);
        // glPopMatrix();
        // glUseProgram(0);
        // glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
        //     glVertexPointer(2, GL_FLOAT, 0, NULL);
        // glBindBuffer(GL_ARRAY_BUFFER, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

    }
    glDeleteVertexArrays(1, &vertexVAO);
    glDeleteBuffers(1, &vertexVBO);
    glDeleteProgram(program);

    glfwTerminate();
    return 0;
}