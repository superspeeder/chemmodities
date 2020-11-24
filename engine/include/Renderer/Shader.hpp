#pragma once

#include "Utils/Utils.hpp"

#include <glad/glad.h>
#include <vector>
#include <string>

namespace che {
	enum ShaderType {
		Vertex, Fragment, Geometry
	};

	class Shader : public che::Disposable {
	private:
		unsigned int m_Handle;
		std::string m_Path;
	public:

		Shader(const char* path);
		~Shader() override;

		inline unsigned int getHandle() {
			return m_Handle;
		};

		inline std::string getPath() {
			return m_Path;
		};
	};


	class ShaderProgram : public che::Disposable {
	private:
		unsigned int m_Handle;

		std::vector<Shader*> m_Shaders;
	public:

		ShaderProgram();
		~ShaderProgram() override;

		void pushShader(Shader* shader);
		void linkProgram();

		void use();

		unsigned int getUniformLocation(const char* name);

		void uniform1f(const char* name, float x);
		void uniform2f(const char* name, float x, float y);
		void uniform3f(const char* name, float x, float y, float z);
		void uniform4f(const char* name, float x, float y, float z, float w);

		void uniform1i(const char* name, int x);
		void uniform2i(const char* name, int x, int y);
		void uniform3i(const char* name, int x, int y, int z);
		void uniform4i(const char* name, int x, int y, int z, int w);

		void uniform1d(const char* name, double x);
		void uniform2d(const char* name, double x, double y);
		void uniform3d(const char* name, double x, double y, double z);
		void uniform4d(const char* name, double x, double y, double z, double w);

		inline unsigned int getHandle() { return m_Handle; };
		
		inline static void unbind() { glUseProgram(0); };
	};
}


che::ShaderProgram& operator<<(che::ShaderProgram& sp, const char* path);
