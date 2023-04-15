#include "EngineCore/ResourceManager.hpp"
#include "EngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "EngineCore/Rendering/OpenGL/Texture2D.hpp"
#include "EngineCore/Log.h"

#include <sstream>
#include <fstream>
#include <iostream>


#define STB_IMAGE_IMPLEMENTATION
#include "EngineCore/stb_image.h"

namespace Engine {
    ResourceManager::ShaderProgramsMap ResourceManager::m_shaderPrograms;
    ResourceManager::TexturesMap ResourceManager::m_textures;

    std::string ResourceManager::m_path;



    void ResourceManager::unloadAllResources() {
        m_shaderPrograms.clear();
        m_textures.clear();
    }

    void ResourceManager::setExecutablePath(const std::string& executablePath) {
        size_t found = executablePath.find_last_of("/\\");
        m_path = executablePath.substr(0, found);
    }

    std::string ResourceManager::getFileString(const std::string& relativeFilePath)
    {
        std::ifstream f;
        f.open(m_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
        if (!f.is_open())
        {
            LOG_CRITICAL("Failed to open file: {0}", relativeFilePath);
            return std::string{};
        }

        std::stringstream buffer;
        buffer << f.rdbuf();
        return buffer.str();
    }

    std::shared_ptr<Engine::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName, 
        const std::string& vertexPath, const std::string& fragmentPath)
    {
        std::string vertexString = getFileString(vertexPath);
        if (vertexString.empty())
        {
            LOG_CRITICAL("No vertex shader!");
            return nullptr;
        }

        std::string fragmentxString = getFileString(fragmentPath);
        if (fragmentxString.empty())
        {
            LOG_CRITICAL("No fragment shader!");
            return nullptr;
        }

        std::shared_ptr<Engine::ShaderProgram>& newShader = m_shaderPrograms.emplace(shaderName, 
                std::make_shared<Engine::ShaderProgram>(vertexString.c_str(), fragmentxString.c_str())).first->second;
        if (newShader->isCompiled())
        {
            return newShader;
        }

        LOG_CRITICAL("Can't load shader program:\nVertex: {0}\nFragment: {1}", vertexPath, fragmentPath);

        return nullptr;
    }


    std::shared_ptr<Engine::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderName)
    {
        ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
        if (it != m_shaderPrograms.end())
        {
            return it->second;
        }
        LOG_CRITICAL("Can't find the shader program: {0}", shaderName);
        return nullptr;
    }


    std::shared_ptr<Engine::Texture2D> ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath)
    {
        int channels = 0;
        int width = 0;
        int height = 0;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* pixels = stbi_load(std::string(m_path + "/" + texturePath).c_str(), &width, &height, &channels, 0);

        if (!pixels)
        {
            LOG_CRITICAL("Can't load image: {0}", texturePath);
            return nullptr;
        }

        std::shared_ptr<Engine::Texture2D> newTexture = m_textures.emplace(textureName, std::make_shared<Engine::Texture2D>(pixels, 
            width,
            height)).first->second;
        stbi_image_free(pixels);
        return newTexture;
    }

    std::shared_ptr<Engine::Texture2D> ResourceManager::getTexture(const std::string& textureName)
    {
        TexturesMap::const_iterator it = m_textures.find(textureName);
        if (it != m_textures.end())
        {
            return it->second;
        }
        LOG_CRITICAL("Can't find the texture: {0}", textureName);
        return nullptr;
    }

}