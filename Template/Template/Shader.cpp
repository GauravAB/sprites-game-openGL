#include "shader.h"

void Shader::CompileShaderAndCreateProgram(const GLchar* vs, const GLchar* tc, const GLchar* te, const GLchar* gs, const GLchar* fs , const char* name)
{
	char str[256];

	
	sprintf(str, "%s shaderProgramlog.txt",name);
	m_cFile = fopen(str, "w");

	GLuint vertexShaderObject = 0;
	GLuint fragmentShaderObject = 0;
	GLuint geometryShaderObject = 0;
	GLuint tessControlShaderObject = 0;
	GLuint tessEvaluationShaderObject = 0;

	if (vs)
	{
		
			vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShaderObject, 1, (const GLchar**)&vs, NULL);
			glCompileShader(vertexShaderObject);
			if (checkCompileError(vertexShaderObject, "Vertex Shader"))
			{
				glDeleteShader(vertexShaderObject);
				vertexShaderObject = NULL;
			}
	}
	else
	{
		fprintf_s(m_cFile, "Vertex Shader Source not found\n");
	}
	
	if (fs)
	{
		
			fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fs, NULL);
			if (checkCompileError(fragmentShaderObject, "Fragment Shader"))
			{
				glDeleteShader(fragmentShaderObject);
				fragmentShaderObject = NULL;
			}
	}
	else
	{
		fprintf_s(m_cFile, "Fragment Shader Source not found\n");
	}

	if (vs && fs)
	{
		if (tc)
		{
			
			tessControlShaderObject = glCreateShader(GL_TESS_CONTROL_SHADER);
			glShaderSource(tessControlShaderObject, 1, (const GLchar**)&tc, NULL);

			if (checkCompileError(tessControlShaderObject, "TessControl Shader"))
			{
				glDeleteShader(tessControlShaderObject);
				tessControlShaderObject = NULL;
			}

		}
		

		if (te)
		{
			
			tessEvaluationShaderObject = glCreateShader(GL_TESS_EVALUATION_SHADER);
			glShaderSource(tessEvaluationShaderObject, 1, (const GLchar**)&te, NULL);
			if (checkCompileError(tessEvaluationShaderObject, "TessEvaluation Shader"))
			{
				glDeleteShader(tessEvaluationShaderObject);
				tessEvaluationShaderObject = NULL;
			}
		}
		

		if (gs)
		{
			
			geometryShaderObject = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometryShaderObject, 1, (const GLchar**)&gs, NULL);
			if (checkCompileError(geometryShaderObject, "Geometry Shader"))
			{
				glDeleteShader(geometryShaderObject);
				geometryShaderObject = NULL;
			}
		}
		
	}

	shaderProgramObjectID = glCreateProgram();
		
	glAttachShader(shaderProgramObjectID, vertexShaderObject);
	glAttachShader(shaderProgramObjectID, fragmentShaderObject);
		
	if (geometryShaderObject != NULL)
	{
		glAttachShader(shaderProgramObjectID, geometryShaderObject);
	}

	if (tessControlShaderObject != NULL)
	{
		glAttachShader(shaderProgramObjectID, tessControlShaderObject);
	}

	if (tessEvaluationShaderObject != NULL)
	{
		glAttachShader(shaderProgramObjectID, tessEvaluationShaderObject);
	}

	glLinkProgram(shaderProgramObjectID);

	bool res = checkLinkError(shaderProgramObjectID);

	//set the deletion flag for shader objects
	if (vs)
	{
		glDeleteShader(vertexShaderObject);
	}

	if (tc)
	{
		glDeleteShader(tessControlShaderObject);
	}
	
	if (te)
	{
		glDeleteShader(tessEvaluationShaderObject);
	}

	if (gs)
	{
		glDeleteShader(geometryShaderObject);
	}

	if (fs)
	{
		glDeleteShader(fragmentShaderObject);
	}
	
	if (res == true)
	{
		if (m_cFile)
		{
			fprintf_s(m_cFile, "failed to created shader\n");
		}
	}
	else
	{
		if (m_cFile)
		{
			fprintf_s(m_cFile, "created shader successfully...\n");
		}
	}
}



bool Shader::checkCompileError(GLuint shaderID, const char* type)
{

	GLint iCompileStatus;
	GLint iInfoLogLength;
	char *szInfoLog = NULL;

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &iCompileStatus);
	if (iCompileStatus == GL_FALSE)
	{
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char*)malloc(iInfoLogLength);
			GLsizei written;
			glGetShaderInfoLog(shaderID, iInfoLogLength, &written, szInfoLog);
			fprintf_s(m_cFile, "\n%s compile error Log: \n%s\n", type, szInfoLog);
			free(szInfoLog);
			return true;
		}
	}

	return false;
}


bool Shader::checkLinkError(GLuint program)
{

	GLint iShaderLinkStatus;
	int iInfoLogLength;
	char *szInfoLog;

	glGetProgramiv(program, GL_LINK_STATUS, &iShaderLinkStatus);

	if (iShaderLinkStatus == GL_FALSE)
	{
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			GLsizei written;
			szInfoLog = (char*)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				glGetProgramInfoLog(program, iInfoLogLength, &written, szInfoLog);
				fprintf_s(m_cFile, "\nLinking error Log: \n%s\n", szInfoLog);
				free(szInfoLog);
				return true;
			}
		}
	}
	
	return false;
}

Shader::~Shader()
{
	
}

void Shader::setBool(const char* name, bool value)
{
	glUniform1i(glGetUniformLocation(shaderProgramObjectID, name), (int)value);
}

void Shader::setInt(const char* name, int value)
{
	glUniform1i(glGetUniformLocation(shaderProgramObjectID, name), value);
}

void Shader::setFloat(const char* name, float value)
{
	glUniform1f(glGetUniformLocation(shaderProgramObjectID, name), value);

}

void Shader::setVec2(const char* name, vmath::vec2 value)const
{
	glUniform2fv(glGetUniformLocation(shaderProgramObjectID, name),1,value);
}

void Shader::setVec2(const char* name, float x, float y)const
{
	glUniform2f(glGetUniformLocation(shaderProgramObjectID, name), x , y);
}

void Shader::setVec3(const char* name, vmath::vec3 value)const
{
	glUniform3fv(glGetUniformLocation(shaderProgramObjectID, name), 1, value);
}

void Shader::setVec3(const char* name, float x, float y, float z)const
{
	glUniform3f(glGetUniformLocation(shaderProgramObjectID, name), x , y , z);
}

void Shader::setVec4(const char* name, vmath::vec4 value)const
{
	glUniform4fv(glGetUniformLocation(shaderProgramObjectID, name),1,value);
}

void Shader::setVec4(const char* name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(shaderProgramObjectID, name), x, y, z , w);
	 
}

void Shader :: setMat2(const char* name, vmath::mat2 matrix , GLboolean B)
{
	glUniformMatrix2fv(glGetUniformLocation(shaderProgramObjectID, name), 1 , B ,matrix);
}

void Shader::setMat3(const char* name, vmath::mat3 matrix , GLboolean B)
{
	glUniformMatrix3fv(glGetUniformLocation(shaderProgramObjectID, name), 1, B, matrix);

}
void Shader::setMat4(const char* name, vmath::mat4 matrix, GLboolean B)
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramObjectID, name), 1, B, matrix);
}

void Shader::use()
{
	glUseProgram(shaderProgramObjectID);
}

void Shader::unuse()
{
	glUseProgram(0);
}

GLuint Shader::getProgramObject()
{
	return shaderProgramObjectID;
}


