// gl_render_fsr.cpp
//

#include "gl_render.h"
#include "image.h"
#include "glew/glew.h"

#define A_CPU
#include "../../build/mod/shaders/ffx_a.h"
#include "../../build/mod/shaders/ffx_fsr1.h"

bool CheckShader(GLuint handle, const char* desc);

GLint uniform_texture_input;
GLint uniform_const0;
GLint uniform_const1;
GLint uniform_const2;
GLint uniform_const3;
GLint uniform_proj_matrix;

namespace devilution
{
	template <typename T = unsigned char>
	std::unique_ptr<T[]> LoadFileInMem(const char* path, size_t* elements = nullptr);

	void app_fatal(const char* pszFmt, ...);
}

bool str_replace(std::string& str, const std::string& from, const std::string& to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	int len = from.length();
	std::string hack = to.c_str();
	str.replace(start_pos, len, hack);
	return true;
}

GLuint fsr_program = 0;

void GL_InitFSR(void)
{
	size_t ffx_a_len, ffx_fsr1_len, fsr_upscale_len, fsr_vertex_len;
	std::string ffx_a = (char *)devilution::LoadFileInMem<unsigned char>("shaders/ffx_a.h", &ffx_a_len).get();
	std::string ffx_fsr1 = (char*)devilution::LoadFileInMem<unsigned char>("shaders/ffx_fsr1.h", &ffx_fsr1_len).get();
	std::string fsr_upscale_shader_buffer = (char*)devilution::LoadFileInMem<unsigned char>("shaders/fsrupscale.pixelshader", &fsr_upscale_len).get();
	std::string fsr_upscale_shader_buffer_vertex = (char*)devilution::LoadFileInMem<unsigned char>("shaders/render.vertexshader", &fsr_vertex_len).get();

	ffx_a[ffx_a_len] = 0;
	ffx_fsr1[ffx_fsr1_len] = 0;
	fsr_upscale_shader_buffer[fsr_upscale_len] = 0;
	fsr_upscale_shader_buffer_vertex[fsr_vertex_len] = 0;

	str_replace(fsr_upscale_shader_buffer, "<include_fsr_ffx_a>", ffx_a);
	str_replace(fsr_upscale_shader_buffer, "<input_fsr1>", ffx_fsr1);

	const char* fsr_raw_shader_buffer = fsr_upscale_shader_buffer.c_str();
	const char* fsr_raw_vertex_shader_buffer = fsr_upscale_shader_buffer_vertex.c_str();

	GLuint fsr_shader_vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(fsr_shader_vertex, 1, &fsr_raw_vertex_shader_buffer, NULL);
	glCompileShader(fsr_shader_vertex);

	GLuint fsr_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsr_shader, 1, &fsr_raw_shader_buffer, NULL);
	glCompileShader(fsr_shader);

	CheckShader(fsr_shader, "fsr");

	fsr_program = glCreateProgram();
	glAttachShader(fsr_program, fsr_shader);
	glAttachShader(fsr_program, fsr_shader_vertex);
	glLinkProgram(fsr_program);

	GLint isLinked = 0;
	glGetProgramiv(fsr_program, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(fsr_program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(fsr_program, maxLength, &maxLength, &infoLog[0]);

		// The program is useless now. So delete it.
		glDeleteProgram(fsr_program);

		devilution::app_fatal(&infoLog[0]);
	}

	uniform_texture_input = glGetUniformLocation(fsr_program, "InputTexture");
	uniform_const0 = glGetUniformLocation(fsr_program, "Const0");
	uniform_const1 = glGetUniformLocation(fsr_program, "Const1");
	uniform_const2 = glGetUniformLocation(fsr_program, "Const2");
	uniform_const3 = glGetUniformLocation(fsr_program, "Const3");
	uniform_proj_matrix = glGetUniformLocation(fsr_program, "ProjMtx");
}

struct XMUINT4 {
	uint32_t  x;
	uint32_t  y;
	uint32_t  z;
	uint32_t  w;
};

void GL_Compute_SetRCAS(int displayWidth, int displayHeight)
{
	glUseProgram(fsr_program);

	XMUINT4 Const0, Const1, Const2, Const3;
	FsrEasuCon(reinterpret_cast<AU1*>(&Const0), reinterpret_cast<AU1*>(&Const1), reinterpret_cast<AU1*>(&Const2), reinterpret_cast<AU1*>(&Const3), static_cast<AF1>(displayWidth), static_cast<AF1>(displayHeight), static_cast<AF1>(displayWidth), static_cast<AF1>(displayHeight), (AF1)displayWidth, (AF1)displayHeight);

	glUniform4ui(uniform_const0, Const0.x, Const0.y, Const0.z, Const0.w);
	glUniform4ui(uniform_const1, Const1.x, Const1.y, Const1.z, Const1.w);
	glUniform4ui(uniform_const2, Const2.x, Const2.y, Const2.z, Const2.w);
	glUniform4ui(uniform_const3, Const3.x, Const3.y, Const3.z, Const3.w);

	glUniformMatrix4fv(uniform_proj_matrix, 1, GL_FALSE, &global_ortho_projection[0][0]);
}
