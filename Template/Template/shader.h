#pragma once

#include <cstdlib>
#include <fstream>
#include "glFiles.h"
#include "vmath.h"


class Shader
{
public:
	GLuint shaderProgramObjectID;
	
	//methods
	Shader():m_cFile(0) {};
	~Shader();
	
	void use();
	void unuse();
	GLuint getProgramObject();
	void CompileShaderAndCreateProgram(const GLchar* vs, const GLchar* tc, const GLchar* te, const GLchar* gs, const GLchar* fs , const char* name);

	void setBool(const char* name, bool value);
	void setInt(const char* name, int value);
	void setFloat(const char* name, float value);
	void setVec2(const char* name, vmath::vec2 value)const;
	void setVec2(const char* name, float x, float y)const;
	void setVec3(const char* name, vmath::vec3 value)const;
	void setVec3(const char* name, float x, float y, float z)const;
	void setVec4(const char* name, vmath::vec4 value)const;
	void setVec4(const char* name, float x, float y, float z, float w);
	void setMat2(const char* name, vmath::mat2 matrix,GLboolean B);
	void setMat3(const char* name, vmath::mat3 matrix,GLboolean B);
	void setMat4(const char* name, vmath::mat4 matrix,GLboolean B);


private:
	bool checkCompileError(GLuint shaderID, const char* type);
	bool checkLinkError(GLuint program);
	FILE *m_cFile;
};



