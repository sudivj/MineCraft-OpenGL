#define TEXTURE_LOADING

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

enum class glTYPE {
    GLRGB,
    GLRGBA
};

class texture
{
    private:
        int checkInitErrors(unsigned char *data, glTYPE renderType) // Check for texture errors
        {
            if (data)
            {
                switch (renderType)
                {
                    case glTYPE::GLRGB:
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                        break;
                    
                    case glTYPE::GLRGBA:
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                        break;
                }
                glGenerateMipmap(GL_TEXTURE_2D);
                return 0;
            }
            else
            {
                std::cout << "Failed to load texture" << std::endl;
                return -1;
            }
            stbi_image_free(data);
        }

    public:    

        int width, height, nrChannels;

        unsigned int genTexture(int size, const char *imagePath, glTYPE renderType) //Generate texture
        {
            unsigned int texture;

            glGenTextures(size, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);

            glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            
            checkInitErrors(stbi_load(imagePath, &width, &height, &nrChannels, 0), renderType); // Check for texture errors

            return texture;
        }
};
