#ifndef SHADER_H
# define SHADER_H

# include <iostream>
# include <fstream>

class Shader {
private:
    std::string shaderSource;

public:
    virtual const char *getSource();
    virtual void readSource(const char *path);
    virtual void readSource(const std::string path);
    virtual void setSource(const char *source);
    virtual void setSource(const std::string source);

    Shader();
    virtual ~Shader();
};

class FragmentShader : public Shader {
    
};

class VertexShader : public Shader {
    
};


#endif