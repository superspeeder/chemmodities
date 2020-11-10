#include "Shader.hpp"
#include <sstream>
#include <iostream>

chengine::Shader::Shader(const char* path) {
	// first line should be a directive
	std::string source = readFile(path);

	std::stringstream stream(source);

	std::string line1;

	std::getline(stream, line1);
	if (!chengine::lineContainsDirective(line1.c_str())) {
		throw std::runtime_error("Shader '" + std::string(path) + "' does not contain a directive on the first line");
	}
	
	chengine::Directive type_directive = chengine::parseDirective(line1.c_str());

	if (type_directive.type != "shader") {
		throw std::runtime_error("Shader '" + std::string(path) + "' must contain a #shader <type> directive on the first line");
	}

	std::string type = type_directive.value;
	
	GLenum stype;

	if (type == "vertex") {
		stype = GL_VERTEX_SHADER;
	}
	else if (type == "fragment") {
		stype = GL_FRAGMENT_SHADER;
	} else if (type == "geometry") {
        stype = GL_GEOMETRY_SHADER;
    }
	else {
		throw std::runtime_error("Shader type '" + type + "' is not recognized");
	}

	m_Handle = glCreateShader(stype);

	source.erase(0, source.find("\n") + 1);

	char const* srcptr = source.c_str();

	glShaderSource(m_Handle, 1, &srcptr, nullptr);
	glCompileShader(m_Handle);
	
	GLint compileStatus = GL_FALSE;
	GLint infoLogLength = 0;

	glGetShaderiv(m_Handle, GL_COMPILE_STATUS, &compileStatus);
	glGetShaderiv(m_Handle, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 0) {

		char infoLog[512];

		glGetShaderInfoLog(m_Handle, 512, nullptr, infoLog);
		std::cerr << infoLog << std::endl;
		throw std::runtime_error("Shader '" + std::string(path) + "' did not compile properly");
	}

}


chengine::ShaderProgram::ShaderProgram() {
	m_Handle = glCreateProgram();
}

void chengine::ShaderProgram::pushShader(chengine::Shader* shader) {
	glAttachShader(m_Handle, shader->getHandle());
}
void chengine::ShaderProgram::linkProgram() {
	glLinkProgram(m_Handle);

	GLint linkStatus = GL_FALSE;

	glGetProgramiv(m_Handle, GL_LINK_STATUS, &linkStatus);

	if (linkStatus == GL_FALSE) {
		char infoLog[512];
		
		glGetProgramInfoLog(m_Handle, 512, nullptr, infoLog);
		std::cerr << infoLog << std::endl;
		throw std::runtime_error("Program " + std::to_string(m_Handle) + " wasn't able to link properly.");
	}
}

void chengine::ShaderProgram::use() {
	glUseProgram(m_Handle);
}

unsigned int chengine::ShaderProgram::getUniformLocation(const char* name) {
	return glGetUniformLocation(m_Handle, name);
}

void chengine::ShaderProgram::uniform1f(const char* name, float x) {
	glUniform1f(getUniformLocation(name), x);
}
void chengine::ShaderProgram::uniform2f(const char* name, float x, float y) {
	glUniform2f(getUniformLocation(name), x, y);
}
void chengine::ShaderProgram::uniform3f(const char* name, float x, float y, float z) {
	glUniform3f(getUniformLocation(name), x, y, z);
}
void chengine::ShaderProgram::uniform4f(const char* name, float x, float y, float z, float w) {
	glUniform4f(getUniformLocation(name), x, y, z, w);
}

void chengine::ShaderProgram::uniform1i(const char* name, int x) {
	glUniform1i(getUniformLocation(name), x);
}
void chengine::ShaderProgram::uniform2i(const char* name, int x, int y) {
	glUniform2i(getUniformLocation(name), x, y);
}
void chengine::ShaderProgram::uniform3i(const char* name, int x, int y, int z) {
	glUniform3i(getUniformLocation(name), x, y, z);
}
void chengine::ShaderProgram::uniform4i(const char* name, int x, int y, int z, int w) {
	glUniform4i(getUniformLocation(name), x, y, z, w);
}

void chengine::ShaderProgram::uniform1d(const char* name, double x) {
	glUniform1d(getUniformLocation(name), x);
}
void chengine::ShaderProgram::uniform2d(const char* name, double x, double y) {
	glUniform2d(getUniformLocation(name), x, y);
}
void chengine::ShaderProgram::uniform3d(const char* name, double x, double y, double z) {
	glUniform3d(getUniformLocation(name), x, y, z);
}
void chengine::ShaderProgram::uniform4d(const char* name, double x, double y, double z, double w) {
	glUniform4d(getUniformLocation(name), x, y, z, w);
}


chengine::ShaderProgram& chengine::operator<<(chengine::ShaderProgram& sp, const char* path) {
    sp.pushShader(new chengine::Shader(path));
	std::cout << "Added shader " << path << "\n";
    return sp;
}
