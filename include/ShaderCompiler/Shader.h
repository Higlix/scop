#ifndef SHADER_COMPILER_H
# define SHADER_COMPILER_H

# include <iostream>

class AShader {
    private:
    char *shaderSource;

    public:
    virtual const char *getSource() = 0;
    virtual void readSource(const char *path);
    virtual void readSource(std::string *path);
};

#endif