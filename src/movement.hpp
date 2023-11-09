#define MOVEMENT

#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <math.h>

#include "../include/shader.h"

#include <iostream>

// A class which controls the movement of vectors in OpenGL
class movement
{
    private:
        unsigned int shaderID;
        glm::mat4 trans;

        void setTrans(const glm::mat4 &mat)
        {
            glUniformMatrix4fv(glGetUniformLocation(shaderID, "transform"), 1, GL_FALSE, &mat[0][0]);
        }
    public:
        movement(Shader ourShader)
        {
            shaderID = ourShader.ID;
        }
        void reset()
        {
            trans = glm::mat4(1.0f);
        }
        void move(float x, float y, float z);
        void scale(float x, float y, float z);
        void rotate(float angle, float x, float y, float z);
};

void movement::move(float x, float y, float z)
{
    // glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3((float)x, (float)y, (float)z));
    setTrans(trans);
}

void movement::scale(float x, float y, float z)
{
    // glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::scale(trans, glm::vec3((float)x, (float)y, (float)z));
    setTrans(trans);
}

void movement::rotate(float angle, float x, float y, float z)
{
    // glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, (float)angle, glm::vec3((float)x, (float)y, (float)z));
    setTrans(trans);
}
