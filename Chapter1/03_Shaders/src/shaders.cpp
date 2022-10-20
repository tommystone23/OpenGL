 #include "shaders.h"

 Shader::Shader(const char* vertexPath, const char* fragmentPath)
 {
    // 1. Retrieve the vertex and fragment source code from filePath
    string vertexCode;
    string fragmentCode;
    ifstream vShaderFile;
    ifstream fShaderFile;
    // Ensure ifstream can throw exceptions
    vShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
    fShaderFile.exceptions (ifstream::failbit | ifstream::badbit);

    try 
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        stringstream vShaderStream, fShaderStream;
        // read files buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // Convert stream into strings
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

    }
    catch(ifstream::failure &e)
    {
        printf("ERROR::SHADER::FILE_READ_FAILURE\n");
        return;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // Compile Shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];
    
    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::FAILED_COMPILATION%s\n", infoLog);
        return;
    }
    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::FAILED_COMPILATION%s\n", infoLog);
        return;
    }

    // Shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // Print linking errors 
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        printf("ERROR::PROGRAM::FAILED_LINKING%s\n", infoLog);
        return;
    }
 }

 void Shader::use()
 {
    glUseProgram(ID);
 }

 void Shader::setBool(const string &name, bool value) const
 {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
 }

 void Shader::setInt(const string &name, int value) const
 {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
 }

 void Shader::setFloat(const string &name, float value) const
 {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
 }