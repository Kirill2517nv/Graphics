#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>

namespace Engine
{
    class ShaderProgram;
    class Texture2D;


    class ResourceManager {
    public:
        static void setExecutablePath(const std::string& executablePath);
        static void unloadAllResources();

        ~ResourceManager() = delete;
        ResourceManager() = delete;
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;
        ResourceManager& operator=(ResourceManager&&) = delete;
        ResourceManager(ResourceManager&&) = delete;

        static std::shared_ptr<Engine::ShaderProgram> loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
        static std::shared_ptr<Engine::ShaderProgram> getShaderProgram(const std::string& shaderName);
        static std::shared_ptr<Engine::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePath);
        static std::shared_ptr<Engine::Texture2D> getTexture(const std::string& textureName);


    private:
        static std::string getFileString(const std::string& relativeFilePath);

        typedef std::map<const std::string, std::shared_ptr<Engine::ShaderProgram>> ShaderProgramsMap;
        static ShaderProgramsMap m_shaderPrograms;

        typedef std::map<const std::string, std::shared_ptr<Engine::Texture2D>> TexturesMap;
        static TexturesMap m_textures;


        static std::string m_path;
    };
}