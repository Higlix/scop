#include "Shader/Shader.h"

Shader::Shader()
{

}

Shader::~Shader()
{

}

const char *Shader::getSource()
{
    return ((const char *) shaderSource.c_str());
}

static std::string readFileFromPath(const char *path)
{
    std::string text = "";
    std::string output = "";
    std::ifstream file(path);

    while (getline(file, text))
    {
        output += text + "\n";
    }
    
    file.close();
    return (output);
}

void Shader::readSource(const char *path)
{
    if (path)
    {
        shaderSource = readFileFromPath(path);
    }
}

void Shader::readSource(const std::string path)
{
    if (path.length() != 0)
    {
        shaderSource = readFileFromPath(path.c_str());
    }
}

void Shader::setSource(const char *source)
{
    shaderSource = source;
}

void Shader::setSource(const std::string source)
{
    shaderSource = source;
}