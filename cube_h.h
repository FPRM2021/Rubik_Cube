#ifndef GLFW_GLAD_GLUT_GLEW_CMAKE_PROJECT_CUBE_H_H
#define GLFW_GLAD_GLUT_GLEW_CMAKE_PROJECT_CUBE_H_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Cube{
public:

    unsigned int VBO, VAO, EBO, VBO1, VAO1, EBO1;
    float vertices[24];
    GLint uniColor;
    int id;

    unsigned int indices[36] = {
            0,1,2,
            3,1,2,

            0,1,4,
            1,4,5,

            1,3,5,
            3,5,7,

            3,2,7,
            2,7,6,

            2,0,6,
            0,6,4,

            4,5,6,
            5,6,7,
    };

    unsigned int indices1[36] = {
            0,1,
            1,3,
            3,2,
            2,0,
            4,5,
            5,7,
            7,6,
            6,4,
            0,4,
            1,5,
            2,6,
            3,7,
    };

    Cube(){
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glGenVertexArrays(1, &VAO1);
        glGenBuffers(1, &VBO1);
        glGenBuffers(1, &EBO1);
    }

    void setGLint (GLint uniC){
        uniColor=uniC;
    }

    void setID (int id_){
        id=id_;
    }

    void setVertex(std::vector<float> vert){
        for(int i=0;i<24;i++){
            vertices[i]=vert[i];
        }
    }

    void createbindbuffers(){


        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);

        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO1);

        glBindBuffer(GL_ARRAY_BUFFER, VBO1);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
    }

    void drawCube(){

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        glUniform3f(uniColor, 1.0f, 1.0f, 1.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glUniform3f(uniColor, 0.0f, 0.5f, 0.0f);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

        glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

        glUniform3f(uniColor, 0.0f, 0.0f, 1.0f);
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

        glUniform3f(uniColor, 1.0f, 0.7f, 0.0f);
        glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

        glUniform3f(uniColor, 1.0f, 1.0f, 0.0f);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);



        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices,GL_STATIC_DRAW);

        glBindVertexArray(VAO1);

        glUniform3f(uniColor, 0.0f, 0.0f, 0.0f);
        glDrawElements(GL_LINES, 36, GL_UNSIGNED_INT, 0);//crear un nuevo indices1

        glBindBuffer(GL_ARRAY_BUFFER, VBO1);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices,GL_STATIC_DRAW);

    }
    void deleteBuffer(){
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO1);
        glDeleteBuffers(1, &VBO1);
        glDeleteBuffers(1, &EBO1);

    }
};

class Camada{
public:
    Cube* camadas[9];
    int indice;

    int ccv[9][9]={
            {18,9,0,
             21,12,3,
             24,15,6}, //izquierda_vertical

            {19,10,1,
             22,13,4,
             25,16,7}, //centro_vertical

            {20,11,2,
             23,14,5,
             26,17,8}, //derecha_vertical

            {18,19,20,
             9,10,11,
             0,1,2,}, //top

            {21,22,23,
             12,13,14,
             3,4,5,}, //centro_horizontal

            {24,25,26,
             15,16,17,
             6,7,8,}, //bottom

            {0,1,2,
             3,4,5,
             6,7,8,}, //front

            {9,10,11,
             12,13,14,
             15,16,17,}, //cara_central

            {18,19,20,
             21,22,23,
             24,25,26}, //cara_trasera
    };

    int ejerotacion[9]={0,0,0,1,1,1,2,2,2,};


    Camada(){}

    void setIndex(int indice_, Cube* cubos_[27]){
        indice=indice_;
        for(int i=0;i<9;i++){
            camadas[i]=cubos_[ccv[indice][i]];
        }
    }

    // timing
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;


    void movimiento(int sentido, Cube* cubesp_[27], GLFWwindow* window) {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //utilizar glm para las rotaciones, no necesariamente los shaders
        float signo;
        if (sentido)
            signo = 1.0;
        else
            signo = -1.0;

//        for (int i = 0; i < 9; i++) {
//                for (int j = 0; j < 24; j = j + 3) {
//                    glm::vec4 vec(camadas[i]->vertices[j], camadas[i]->vertices[j + 1], camadas[i]->vertices[j + 2],1.0f);
//                    glm::mat4 trans = glm::mat4(1.0f);
//                    glm::vec3 eje(0.0f, 0.0f, 0.0f);
//                    eje[ejerotacion[indice]] = 1.0f;
//                    trans = glm::rotate(trans, glm::radians(signo * 90), eje);
//                    vec = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f) + trans * vec;
//                    camadas[i]->vertices[j] = vec[0];
//                    camadas[i]->vertices[j + 1] = vec[1];
//                    camadas[i]->vertices[j + 2] = vec[2];
//                }
//        }

        float vel = 9.0;
        glfwSetTime(0.0);
        float angle = 90.0 / vel;
        int count = int(vel);
        while (count <= 90.0f) {
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 24; j = j + 3) {
                    glm::vec4 vec(camadas[i]->vertices[j], camadas[i]->vertices[j + 1], camadas[i]->vertices[j + 2],
                                  1.0f);
                    glm::mat4 trans = glm::mat4(1.0f);
                    glm::vec3 eje(0.0f, 0.0f, 0.0f);
                    eje[ejerotacion[indice]] = 1.0f;

                    trans = glm::rotate(trans, glm::radians(signo * angle), eje);

                    vec = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f) + trans * vec;
                    camadas[i]->vertices[j] = vec[0];
                    camadas[i]->vertices[j + 1] = vec[1];
                    camadas[i]->vertices[j + 2] = vec[2];
                }
                camadas[i]->drawCube();
            }
            glfwSwapBuffers(window);
            count = count + angle;
        }


//        x+: horario
//        x-: anti-horario

//        y+: anti-horario
//        y-: horario

//        z+: anti-horario
//        z-: horario

        int modIndices[2][9] = {
                {6, 3, 0, 7, 4, 1, 8, 5, 2,},//horario
                {2, 5, 8, 1, 4, 7, 0, 3, 6,}}; //antihorario

        if (indice > 2) {
            for (int i = 0; i < 9; i++) {
                cubesp_[ccv[indice][i]] = camadas[modIndices[sentido][i]];
            }
        } else {
            if (sentido == 1)
                sentido = 0;
            else
                sentido = 1;
            for (int i = 0; i < 9; i++) {
                cubesp_[ccv[indice][i]] = camadas[modIndices[sentido][i]];
            }
        }
    }

};


#endif //GLFW_GLAD_GLUT_GLEW_CMAKE_PROJECT_CUBE_H_H
