#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <cmath>
#include "shader_h.h"
#include "camera_h.h"
#include "cube_h.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <chrono>
#include <vector>
#include "AllCube/solve.h"
#include "AllCube/random.h"
#include <windows.h>

#define PI 3.14159265

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//camara
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

using namespace std;
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void setVerticesCubes(Cube ***cubos_, Cube *cubosp_[27], vector<float> pos_, GLint uniColor_);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void exeanimation(std::vector<std::string> str, GLFWwindow *window);
void updateCamadas_();

GLfloat sizes[2]; // Store supported point size range
GLfloat step;     // Store supported point size increments
GLfloat curSize;  // Store current point size

Camada camada_[9];
Cube *cubep[27];

bool keyblock = false;
//Para usar el solver:
//usar las funciones dentro del allcube
//randomize() para generar un cubo aleatorio
//get_solution() para resolver un cubo

//solver

std::vector<std::string> movreg;
std::vector<std::string> solvedCube;
unsigned int milliseconds = 500;


int main()
{

    auto t_start = std::chrono::high_resolution_clock::now();
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "RubikCube", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // build and compile our shader program
    // ------------------------------------
    // vertex shader

    Shader shaderProgram("C:/Users/fabri/Documents/Universidad/Computacion_Grafica/Flecha/GLFW_GLAD_GLUT_GLEW_cmake_project/src/rubick-jean-fabrizzio/vertex.vs",
                         "C:/Users/fabri/Documents/Universidad/Computacion_Grafica/Flecha/GLFW_GLAD_GLUT_GLEW_cmake_project/src/rubick-jean-fabrizzio/fragment.fs");

    /////////////////////////////////////////////////////////////////////
    curSize = sizes[0];

    // Get the location of the color uniform
    GLint uniColor = glGetUniformLocation(shaderProgram.ID, "triangleColor");

    //inicializacion de cubos
    Cube ***cubos = new Cube **[3];
    

    for (int i = 0; i < 3; i++)
    {
        cubos[i] = new Cube *[3];
        for (int j = 0; j < 3; j++)
        {
            cubos[i][j] = new Cube[3];
        }
    }

    vector<float> pos = {-0.45, 0.45, 0.45};

    setVerticesCubes(cubos, cubep, pos, uniColor);



    // render loop
    // -----------

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);
        keyblock = false;
        glfwSetKeyCallback(window, key_callback);

        updateCamadas_();

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glPointSize(curSize);

        shaderProgram.use();

        // transformation

        unsigned int transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
        glm::mat4 trans = glm::mat4(1.0f);
        //trans = glm::rotate(trans, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        trans = glm::rotate(trans, -(float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shaderProgram.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        shaderProgram.setMat4("view", view);

        glLineWidth(6);

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    ((*(*(cubos + i) + j) + k))->drawEffect();
                }
            }
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                ((*(*(cubos + i) + j) + k))->deleteBuffer();
            }
        }
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            delete[] cubos[i][j];
        }
        delete[] cubos[i];
    }
    delete[] cubos;

    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (keyblock == false && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (keyblock == false && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keyblock == false && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keyblock == false && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keyblock == false && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
//    if (keyblock == false && key == GLFW_KEY_C && action == GLFW_PRESS){
//        string tempo1= to_cube_not(movreg);
//        movreg.clear();
//        solvedCube=get_solution(tempo1);
//        for(int i=0;i<solvedCube.size();++i){
//            cout<<solvedCube[i]<<" ";
//        }
//        cout<<endl;
//    }

    if (keyblock == false && key == GLFW_KEY_Z && action == GLFW_PRESS){
        std::string temp = "";
        //std::vector<std::string> tempo;
        std::string aux = "";
        temp=randomize();
        for(std::string::size_type i = 0; i < temp.size(); ++i) {
            aux=temp[i];
            cout<<temp[i]<<" ";
            movreg.push_back(aux);
        }
        cout<<endl;
        string tempo1=to_cube_not(movreg);
        exeanimation(movreg, window);
//        solvedCube=get_solution(tempo1);
//        for(int i=0;i<solvedCube.size();++i){
//            cout<<solvedCube[i]<<" ";
//        }
//        cout<<endl;
    }

    if (keyblock == false && key == GLFW_KEY_X && action == GLFW_PRESS){
        string tempo1= to_cube_not(movreg);
        movreg.clear();
        solvedCube=get_solution(tempo1);
        for(int i=0;i<solvedCube.size();++i){
            cout<<solvedCube[i]<<" ";
        }
        cout<<endl;
        exeanimation(solvedCube,window);
    }

    if (keyblock == false && key == GLFW_KEY_T && action == GLFW_PRESS)
    {
        keyblock = true;
        movreg.push_back("L'");
        camada_[0].movimiento(0, cubep, window);
    }
    if (keyblock == false && key == GLFW_KEY_KP_7 && action == GLFW_PRESS)
    {
        keyblock = true;
        movreg.push_back("L");
        camada_[0].movimiento(1, cubep, window);
    }
//    if (keyblock == false && key == GLFW_KEY_Y && action == GLFW_PRESS)
//    {
//        keyblock = true;
//        movreg.push_back("M'");
//        camada_[1].movimiento(0, cubep, window);
//    }
//    if (keyblock == false && key == GLFW_KEY_KP_8 && action == GLFW_PRESS)
//    {
//        keyblock = true;
//        movreg.push_back("M");
//        camada_[1].movimiento(1, cubep, window);
//    }
    if (keyblock == false && key == GLFW_KEY_U && action == GLFW_PRESS)
    {
        keyblock = true;
        movreg.push_back("R");
        camada_[2].movimiento(0, cubep, window);
    }
    if (keyblock == false && key == GLFW_KEY_KP_9 && action == GLFW_PRESS)
    {
        keyblock = true;
        movreg.push_back("R'");
        camada_[2].movimiento(1, cubep, window);
    }
    if (keyblock == false && key == GLFW_KEY_G && action == GLFW_PRESS)
    {
        keyblock = true;
        movreg.push_back("U");
        camada_[3].movimiento(0, cubep, window);
    }
    if (keyblock == false && key == GLFW_KEY_KP_4 && action == GLFW_PRESS)
    {
        keyblock = true;
        movreg.push_back("U'");
        camada_[3].movimiento(1, cubep, window);
    }
//    if (keyblock == false && key == GLFW_KEY_H && action == GLFW_PRESS)
//    {
//        keyblock = true;
//        movreg.push_back("E'");
//        camada_[4].movimiento(0, cubep, window);
//    }
//    if (keyblock == false && key == GLFW_KEY_KP_5 && action == GLFW_PRESS)
//    {
//        keyblock = true;
//        movreg.push_back("E");
//        camada_[4].movimiento(1, cubep, window);
//    }
    if (keyblock == false && key == GLFW_KEY_J && action == GLFW_PRESS)
    {
        keyblock = true;
        movreg.push_back("D'");
        camada_[5].movimiento(0, cubep, window);
    }
    if (keyblock == false && key == GLFW_KEY_KP_6 && action == GLFW_PRESS)
    {
        keyblock = true;
        movreg.push_back("D");
        camada_[5].movimiento(1, cubep, window);
    }
    if (keyblock == false && key == GLFW_KEY_B && action == GLFW_PRESS)
    {
        keyblock = true;
        movreg.push_back("F");
        camada_[6].movimiento(0, cubep, window);
    }
    if (keyblock == false && key == GLFW_KEY_KP_1 && action == GLFW_PRESS)
    {
        keyblock = true;
        movreg.push_back("F'");
        camada_[6].movimiento(1, cubep, window);
    }
//    if (keyblock == false && key == GLFW_KEY_N && action == GLFW_PRESS)
//    {
//        keyblock = true;
//        movreg.push_back("S");
//        camada_[7].movimiento(0, cubep, window);
//    }
//    if (keyblock == false && key == GLFW_KEY_KP_2 && action == GLFW_PRESS)
//    {
//        keyblock = true;
//        movreg.push_back("S'");
//        camada_[7].movimiento(1, cubep, window);
//    }
    if (keyblock == false && key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        keyblock = true;
        movreg.push_back("B'");
        camada_[8].movimiento(0, cubep, window);
    }
    if (keyblock == false && key == GLFW_KEY_KP_3 && action == GLFW_PRESS)
    {
        keyblock = true;
        movreg.push_back("B");
        camada_[8].movimiento(1, cubep, window);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void setVerticesCubes(Cube ***cubos_, Cube *cubosp_[27], vector<float> pos_, GLint uniColor_)
{
    int count = 0;
    float separation = 0.01;
    pos_ = {pos_[0] - separation, pos_[1] + separation, pos_[2] + separation};
    vector<float> pos1_ = {pos_[0] + 0.3f, pos_[1] - 0.3f, pos_[2] - 0.3f};
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                vector<float> vertices_ = {pos_[0], pos_[1], pos_[2],
                                           pos1_[0], pos_[1], pos_[2],
                                           pos_[0], pos_[1], pos1_[2],
                                           pos1_[0], pos_[1], pos1_[2],
                                           pos_[0], pos1_[1], pos_[2],
                                           pos1_[0], pos1_[1], pos_[2],
                                           pos_[0], pos1_[1], pos1_[2],
                                           pos1_[0], pos1_[1], pos1_[2]};
                ((*(*(cubos_ + i) + j) + k))->setGLint(uniColor_);
                ((*(*(cubos_ + i) + j) + k))->setVertex(vertices_);
                ((*(*(cubos_ + i) + j) + k))->createbindbuffers();
                ((*(*(cubos_ + i) + j) + k))->setID(count);
                cubosp_[count] = ((*(*(cubos_ + i) + j) + k));
                pos_ = {pos_[0] + 0.3f + separation, pos_[1], pos_[2]};
                pos1_ = {pos_[0] + 0.3f, pos_[1] - 0.3f, pos_[2] - 0.3f};
                count++;
            }
            pos_ = {pos_[0] - 0.9f - (3 * separation), pos_[1] - 0.3f - separation, pos_[2]};
            pos1_ = {pos_[0] + 0.3f, pos_[1] - 0.3f, pos_[2] - 0.3f};
        }
        pos_ = {pos_[0], pos_[1] + 0.9f + (3 * separation), pos_[2] - 0.3f - (separation)};
        pos1_ = {pos_[0] + 0.3f, pos_[1] - 0.3f, pos_[2] - 0.3f};
    }
}

void exeanimation(std::vector<std::string> str, GLFWwindow *window){
    string mov="";

    while(!(str.empty())){
        mov=str[0];
        str.erase(str.begin());
        if(mov=="L'"){
            camada_[0].movimiento(0, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="L"){
            camada_[0].movimiento(1, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="L2"){
            camada_[0].movimiento(1, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
            camada_[0].movimiento(1, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="M'"){
            camada_[1].movimiento(0, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="M"){
            camada_[1].movimiento(1, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="M2"){
            camada_[1].movimiento(1, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
            camada_[1].movimiento(1, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="R"){
            camada_[2].movimiento(0, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="R'"){
            camada_[2].movimiento(1, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="R2"){
            camada_[2].movimiento(0, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
            camada_[2].movimiento(0, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="U"){
            camada_[3].movimiento(0, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="U2"){
            camada_[3].movimiento(0, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
            camada_[3].movimiento(0, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="U'"){
            camada_[3].movimiento(1, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="E'"){
            camada_[4].movimiento(0, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="E"){
            camada_[4].movimiento(1, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="E2"){
            camada_[4].movimiento(1, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
            camada_[4].movimiento(1, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="D'"){
            camada_[5].movimiento(0, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="D2"){
            camada_[5].movimiento(1, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
            camada_[5].movimiento(1, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="D"){
            camada_[5].movimiento(1, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="F"){
            camada_[6].movimiento(0, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="F2"){
            camada_[6].movimiento(0, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
            camada_[6].movimiento(0, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="F'"){
            camada_[6].movimiento(1, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="S"){
            camada_[7].movimiento(0, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="S2"){
            camada_[7].movimiento(0, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
            camada_[7].movimiento(0, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="S'"){
            camada_[7].movimiento(1, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="B'"){
            camada_[8].movimiento(0, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="B"){
            camada_[8].movimiento(1, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
        else if(mov=="B2"){
            camada_[8].movimiento(1, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
            camada_[8].movimiento(1, cubep, window);
            Sleep(milliseconds);
            updateCamadas_();
        }
    }
}

void updateCamadas_(){
    for (int i = 0; i < 9; i++)
    {
        camada_[i].setIndex(i, cubep);
    }
}