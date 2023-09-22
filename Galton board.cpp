
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

float OX = 400;
float OY = 400;

double xpos, ypos;
float GLxpos, GLypos;

float fall_circle_radius = 3;
float static_circle_radius = 10;
float line_width = 10;
float cnt_of_circle_pol = 10;
float angle_circle = 2 * 3.1415926 / cnt_of_circle_pol;
int index_of_obj = 0;
float g = 9.8;
float dt = 0.1;
const int map_x = 20;
const int map_y = 4;
const int count_of_falling_balls = 1000;
float fall_ball_mass = 1;
float MAX_SPEED = -10;
float elasticity = 0.65;
int obj_count = 0;
// const int THREAD_COUNT = 1;
// pthread_t thread[THREAD_COUNT];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef enum {
    Circle,
    Line
} geometry;

typedef struct
{
    geometry TypeOfObj;
    int obj_id;
    float pos_of_circle_or_orth_of_line[2];
    float vert_of_line[2][2];
    float vel_fall_ball[2];
    float mass;
    bool static_obj;
} geom_obj;

void circle(float x0, float y0, geom_obj** objects, bool static_object) {
    float radius = fall_circle_radius;
    if (static_object == true) 
        radius = static_circle_radius;

    glBegin(GL_TRIANGLE_FAN);
        float x, y;
        for (int i = 0; i < cnt_of_circle_pol; i ++){
            x = cos(angle_circle * i) * radius;
            y = sin(angle_circle * i) * radius;
            glVertex2f(x + x0, y + y0);
        }
    glEnd();

    (*objects)[index_of_obj].TypeOfObj = Circle;
    (*objects)[index_of_obj].obj_id = index_of_obj;
    (*objects)[index_of_obj].pos_of_circle_or_orth_of_line[0] = x0;
    (*objects)[index_of_obj].pos_of_circle_or_orth_of_line[1] = y0;
    (*objects)[index_of_obj].static_obj = static_object;
    (*objects)[index_of_obj].mass = fall_ball_mass;

    pthread_mutex_lock(&mutex);
    
    index_of_obj++;
    if (obj_count <= index_of_obj) {
        *objects = (geom_obj*)realloc(*objects, (index_of_obj + 1) * sizeof(geom_obj));
        obj_count = index_of_obj + 1;
    }

    pthread_mutex_unlock(&mutex);
}

void line(float pos_v1[2], float pos_v2[2], geom_obj** objects) {
    glLineWidth(line_width);
    glBegin(GL_LINES);
        glColor3f(0.0, 0.0, 1.0);
        glVertex2f(pos_v1[0], pos_v1[1]);
        glVertex2f(pos_v2[0], pos_v2[1]);
    glEnd();
    
    (*objects)[index_of_obj].TypeOfObj = Line;
    (*objects)[index_of_obj].obj_id = index_of_obj;
    (*objects)[index_of_obj].vert_of_line[0][0] = pos_v1[0];
    (*objects)[index_of_obj].vert_of_line[1][0] = pos_v2[0];
    (*objects)[index_of_obj].vert_of_line[0][1] = pos_v1[1];
    (*objects)[index_of_obj].vert_of_line[1][1] = pos_v2[1];
    (*objects)[index_of_obj].static_obj = true;  

    pthread_mutex_lock(&mutex);

    index_of_obj++;
    if (obj_count <= index_of_obj) {
        *objects = (geom_obj*)realloc(*objects, (index_of_obj + 1) * sizeof(geom_obj));
        obj_count = index_of_obj + 1;
    }

    pthread_mutex_unlock(&mutex);
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

int check_even(int num){
    if(num % 2 == 0)
        return 1;
    else
        return 0;
}

void momentum(geom_obj* object, geom_obj* objects, int i) {
    float dx = objects[i].pos_of_circle_or_orth_of_line[0] - object->pos_of_circle_or_orth_of_line[0];
    float dy = objects[i].pos_of_circle_or_orth_of_line[1] - object->pos_of_circle_or_orth_of_line[1];
    float norm_mod = sqrt(pow(dy, 2.0) + pow(dx, 2.0));
    float un[2] = {dx / norm_mod, dy / norm_mod};
    float ut[2] = {-un[1], un[0]};

    float old_obj1_vel_norm_mod = object->vel_fall_ball[0] * un[0] + object->vel_fall_ball[1] * un[1];
    float new_obj1_vel_tan_mod = object->vel_fall_ball[0] * ut[0] + object->vel_fall_ball[1] * ut[1];

    if (objects[i].static_obj == false) {
        float new_obj2_vel_tan_mod = objects[i].vel_fall_ball[0] * ut[0] + objects[i].vel_fall_ball[1] * ut[1];
        float old_obj2_vel_norm_mod = objects[i].vel_fall_ball[0] * un[0] + objects[i].vel_fall_ball[1] * un[1];
    
        float new_obj2_vel_norm_mod = old_obj1_vel_norm_mod;
        float new_obj1_vel_norm_mod = old_obj2_vel_norm_mod;
 
        objects[i].vel_fall_ball[0] = (new_obj2_vel_norm_mod * un[0] + new_obj2_vel_tan_mod * ut[0]) * elasticity;
        objects[i].vel_fall_ball[1] = (new_obj2_vel_norm_mod * un[1] + new_obj2_vel_tan_mod * ut[1]) * elasticity;

        object->vel_fall_ball[0] = (new_obj1_vel_norm_mod * un[0] + new_obj1_vel_tan_mod * ut[0]) * elasticity;
        object->vel_fall_ball[1] = (new_obj1_vel_norm_mod * un[1] + new_obj1_vel_tan_mod * ut[1]) * elasticity;
    } else {
        float new_obj1_vel_norm_mod = -old_obj1_vel_norm_mod;

        object->vel_fall_ball[0] = (new_obj1_vel_norm_mod * un[0] + new_obj1_vel_tan_mod * ut[0]) * elasticity;
        object->vel_fall_ball[1] = (new_obj1_vel_norm_mod * un[1] + new_obj1_vel_tan_mod * ut[1]) * elasticity;
    }
};

void collision_circle(geom_obj* object, geom_obj* objects, int i){
    float dx = object->pos_of_circle_or_orth_of_line[0] - objects[i].pos_of_circle_or_orth_of_line[0];
    float dy = object->pos_of_circle_or_orth_of_line[1] - objects[i].pos_of_circle_or_orth_of_line[1];
    float dist = sqrt(pow(dy, 2.0) + pow(dx, 2.0));

    float midpointx = (objects[i].pos_of_circle_or_orth_of_line[0] + object->pos_of_circle_or_orth_of_line[0]) / 2; 
    float midpointy = (objects[i].pos_of_circle_or_orth_of_line[1] + object->pos_of_circle_or_orth_of_line[1]) / 2;

    if (objects[i].static_obj == true) {
        if(dist < (fall_circle_radius + static_circle_radius)) {
            object->pos_of_circle_or_orth_of_line[0] = objects[i].pos_of_circle_or_orth_of_line[0] + (fall_circle_radius + static_circle_radius) * dx / dist; 
            object->pos_of_circle_or_orth_of_line[1] = objects[i].pos_of_circle_or_orth_of_line[1] + (fall_circle_radius + static_circle_radius) * dy / dist;
            momentum(object, objects, i);
        }
    } else {
        if(dist < (2 * fall_circle_radius)) {
            object->pos_of_circle_or_orth_of_line[0] = midpointx + fall_circle_radius * dx / dist; 
            object->pos_of_circle_or_orth_of_line[1] = midpointy + fall_circle_radius * dy / dist;

            objects[i].pos_of_circle_or_orth_of_line[0] = midpointx - fall_circle_radius * dx / dist; 
            objects[i].pos_of_circle_or_orth_of_line[1] = midpointy - fall_circle_radius * dy / dist;

            momentum(object, objects, i);
        }
    }
}

void collision_line(geom_obj* object, geom_obj * objects, int i) {

    float dx = object->pos_of_circle_or_orth_of_line[0] - objects[i].vert_of_line[0][0];
    float dy = object->pos_of_circle_or_orth_of_line[1] - objects[i].vert_of_line[0][1];

    float dx_line = objects[i].vert_of_line[1][0] - objects[i].vert_of_line[0][0];
    float dy_line = objects[i].vert_of_line[1][1] - objects[i].vert_of_line[0][1];

    float dist_line = sqrt(pow(dy_line, 2.0) + pow(dx_line, 2.0));

    float proj = dx * dx_line / dist_line + dy * dy_line / dist_line;

    float ort_coord[2] = {objects[i].vert_of_line[0][0] + proj * dx_line / dist_line, 
                          objects[i].vert_of_line[0][1] + proj * dy_line / dist_line};

    float dx_ort = object->pos_of_circle_or_orth_of_line[0] - ort_coord[0];
    float dy_ort = object->pos_of_circle_or_orth_of_line[1] - ort_coord[1];

    float dist = sqrt(pow(dx_ort, 2.0) + pow(dy_ort, 2.0));

    float dist_vert1 = sqrt(pow(objects[i].vert_of_line[0][0] - object->pos_of_circle_or_orth_of_line[0], 2.0) + 
                    pow(objects[i].vert_of_line[0][1] - object->pos_of_circle_or_orth_of_line[1], 2.0));
    float dist_vert2 = sqrt(pow(objects[i].vert_of_line[1][0] - object->pos_of_circle_or_orth_of_line[0], 2.0) +
                    pow(objects[i].vert_of_line[1][1] - object->pos_of_circle_or_orth_of_line[1], 2.0));
    float max_length = sqrt(pow(dist_line, 2.0) + pow(fall_circle_radius, 2.0));

    if((dist < (fall_circle_radius + line_width / 2))  && (max_length >= dist_vert1) && (max_length >= dist_vert2)) {
        objects[i].pos_of_circle_or_orth_of_line[0] = ort_coord[0];
        objects[i].pos_of_circle_or_orth_of_line[1] = ort_coord[1];
        object->pos_of_circle_or_orth_of_line[0] = ort_coord[0] + (fall_circle_radius + line_width / 2) * dx_ort / dist;
        object->pos_of_circle_or_orth_of_line[1] = ort_coord[1] + (fall_circle_radius + line_width / 2) * dy_ort / dist;
        momentum(object, objects, i);
    }
}

void collision(geom_obj* object, geom_obj * objects){
    for (int i = 0; i < index_of_obj; i++) {
        if (objects[i].obj_id != object->obj_id) {
            switch(objects[i].TypeOfObj){
                case Circle:
                    collision_circle(object, objects, i);
                    break;
                case Line:
                    collision_line(object, objects, i);
                    break;
            }
        }
    }
}

// typedef struct
// {
//     int current_index;
//     int step;
//     int j;
//     float pos_fall_ball[count_of_falling_balls][2];
//     float vel_fall_ball[count_of_falling_balls][2];
// } thread_struct;

// void* thread_func (void* vptr_args) {
//     thread_struct* param = (thread_struct*)vptr_args;
//     for (int i = param->current_index + param->step * param->j; i < param->current_index + param->step * (param->j + 1); i++) {
//         glColor3f(1.0, 1.0, 1.0);
//         circle(param->pos_fall_ball[i - param->current_index][0], param->pos_fall_ball[i - param->current_index][1], &(param->objects), false);
        
//         param->objects[i].mass = fall_ball_mass;
//         param->objects[i].pos_of_circle_or_orth_of_line[0] = param->pos_fall_ball[i - param->current_index][0];
//         param->objects[i].pos_of_circle_or_orth_of_line[1] = param->pos_fall_ball[i - param->current_index][1];
//         param->objects[i].vel_fall_ball[0] = param->vel_fall_ball[i - param->current_index][0];
//         param->objects[i].vel_fall_ball[1] = param->vel_fall_ball[i - param->current_index][1];

//         collision(&(param->objects[i]), param->objects);

//         if (param->objects[i].vel_fall_ball[1] > MAX_SPEED)
//             param->objects[i].vel_fall_ball[1] += - g * dt;

//         param->objects[i].pos_of_circle_or_orth_of_line[0] += param->objects[i].vel_fall_ball[0] * dt;
//         param->objects[i].pos_of_circle_or_orth_of_line[1] += param->objects[i].vel_fall_ball[1] * dt;

//         param->pos_fall_ball[i - param->current_index][0] = param->objects[i].pos_of_circle_or_orth_of_line[0];
//         param->pos_fall_ball[i - param->current_index][1] = param->objects[i].pos_of_circle_or_orth_of_line[1];
//         param->vel_fall_ball[i - param->current_index][0] = param->objects[i].vel_fall_ball[0];
//         param->vel_fall_ball[i - param->current_index][1] = param->objects[i].vel_fall_ball[1];
//     }
//     pthread_exit(0);
// }

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        exit(EXIT_FAILURE);

    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Galton board", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    gladLoadGL(glfwGetProcAddress);

    glOrtho(-OX,OX, -OY,OY, -1,1);
    
    float pos_fall_ball[count_of_falling_balls][2];
    float vel_fall_ball[count_of_falling_balls][2];
    float width_x_ball_generation = 6 * sqrt(count_of_falling_balls) * fall_circle_radius;
    float height_y_ball_generation = 6 * sqrt(count_of_falling_balls) * fall_circle_radius;
    float bias_y_ball_generation = -height_y_ball_generation;

    int j = -1;
    int k = 0;
    for (int i = 0; i <  count_of_falling_balls ; i++) {
        if (i % int(sqrt(count_of_falling_balls)) == 0) {
            j ++;
            k = 0;
        }
        pos_fall_ball[i][0] = -width_x_ball_generation / 2 + width_x_ball_generation / sqrt(count_of_falling_balls) * k;
        pos_fall_ball[i][1] = OY - bias_y_ball_generation - height_y_ball_generation / sqrt(count_of_falling_balls) * j;
        vel_fall_ball[i][0] = 0;
        vel_fall_ball[i][1] = 0;
        k++;
    }

    geom_obj* objects = (geom_obj*)malloc(sizeof(geom_obj));

    while (!glfwWindowShouldClose(window))
    {
        index_of_obj = 0;
        glfwGetFramebufferSize(window, &WIDTH, &HEIGHT);
        glViewport(0, 0, WIDTH, HEIGHT);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float gap = 200;
        float di = 6 * static_circle_radius;
        float dj = 6 * static_circle_radius;
        float x0 = -OX + static_circle_radius;
        float y0 = -OY + static_circle_radius + gap + map_y * 2 * static_circle_radius + dj * map_y - 50;


        for (int i = 0; i < map_x; i ++) {
            for (int j = 0; j > -map_y; j--) {
                glColor3f(0.0, 0.0, 1.0);
                circle(x0 + di * i + di * check_even(j) / 2, y0 + dj * j, &objects, true);
            }
        }

        for (int i = 0; i < map_x; i ++) {
            float new_gap = y0 - dj * map_y;
            float x = -OX + 2 * OX / map_x * i;
            float pos_v1[2] = {x, new_gap};
            float pos_v2[2] = {x, -OY};
            line(pos_v1, pos_v2, &objects);
        }

        float pixel_to_coord[2] = {2 * OX / WIDTH, 2 * OY / HEIGHT};

        float pos_v4[2] = {-OX, OY};
        float pos_v3[2] = {- di / 5, y0 + 100};
        line(pos_v3, pos_v4, &objects);

        float pos_v6[2] = {OX, OY};
        float pos_v5[2] = {di / 5, y0 + 100};
        line(pos_v5, pos_v6, &objects);

        float pos_v8[2] = {OX, OY};
        float pos_v7[2] = {OX, -OY};
        line(pos_v7, pos_v8, &objects);

        float pos_v10[2] = {-OX, OY};
        float pos_v9[2] = {-OX, -OY};
        line(pos_v9, pos_v10, &objects);

        float pos_v12[2] = {OX, -OY};
        float pos_v11[2] = {-OX, -OY};
        line(pos_v11, pos_v12, &objects);

        int current_index = index_of_obj;

        // float step = int(count_of_falling_balls / THREAD_COUNT);

        // thread_struct vptr_args[THREAD_COUNT];

        // for (int j = 0; j < THREAD_COUNT; j++) {
        //     vptr_args[j].current_index = current_index;
        //     vptr_args[j].step = step;
        //     vptr_args[j].j = j;
        //     for (int k = 0; k < count_of_falling_balls; k++) {
        //         vptr_args[j].pos_fall_ball[k][0] = pos_fall_ball[k][0];
        //         vptr_args[j].vel_fall_ball[k][0] = vel_fall_ball[k][0];
        //         vptr_args[j].pos_fall_ball[k][1] = pos_fall_ball[k][1];
        //         vptr_args[j].vel_fall_ball[k][1] = vel_fall_ball[k][1];
        //     }

        //     pthread_create(&thread[j], NULL, thread_func, (void*)vptr_args);
        // }

        // for (int j = 0; j < THREAD_COUNT; j++)
        //     pthread_join(thread[j], NULL);


    for (int i = current_index; i < (current_index + count_of_falling_balls); i++) {
        glColor3f(1.0, 1.0, 1.0);
        circle(pos_fall_ball[i - current_index][0], pos_fall_ball[i - current_index][1], &(objects), false);
        
        objects[i].mass = fall_ball_mass;
        objects[i].pos_of_circle_or_orth_of_line[0] = pos_fall_ball[i - current_index][0];
        objects[i].pos_of_circle_or_orth_of_line[1] = pos_fall_ball[i - current_index][1];
        objects[i].vel_fall_ball[0] = vel_fall_ball[i - current_index][0];
        objects[i].vel_fall_ball[1] = vel_fall_ball[i - current_index][1];

        collision(&(objects[i]), objects);

        if (objects[i].vel_fall_ball[1] > MAX_SPEED)
            objects[i].vel_fall_ball[1] += - g * dt;

        objects[i].pos_of_circle_or_orth_of_line[0] += objects[i].vel_fall_ball[0] * dt;
        objects[i].pos_of_circle_or_orth_of_line[1] += objects[i].vel_fall_ball[1] * dt;

        pos_fall_ball[i - current_index][0] = objects[i].pos_of_circle_or_orth_of_line[0];
        pos_fall_ball[i - current_index][1] = objects[i].pos_of_circle_or_orth_of_line[1];
        vel_fall_ball[i - current_index][0] = objects[i].vel_fall_ball[0];
        vel_fall_ball[i - current_index][1] = objects[i].vel_fall_ball[1];
    }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    free(objects);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}