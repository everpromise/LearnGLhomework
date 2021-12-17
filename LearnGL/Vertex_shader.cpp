#include <iostream>
#include <string>
#include <fstream>
#include <vector>

//#include <gl/glew.h>
//#include <gl/freeglut.h>
//#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Structs.h"

void processInput(GLFWwindow *window);
void frambuffer_size_callback(GLFWwindow* window, int width, int height);


std::string readShaderFile(std::string filename)	// 读取着色器
{
	std::string ret, tmp;
	std::ifstream in(filename);
	if (!in.is_open()) {
		std::cout << "文件：" << filename << " 打开失败" << std::endl;
		exit(-1);
	}

	while (std::getline(in, tmp)) {
		ret += tmp + '\n';
	}
	in.close();
	return ret;
}

void TestCompileShader(GLuint Shader) {
	int success;
	char infolog[512];
	glGetShaderiv(Shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(Shader, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << std::endl;
	}
}
void TestLinkProgram(GLuint Program) {
	int success;
	char infolog[512];
	glGetProgramiv(Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(Program, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << std::endl;
	}
}

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main() {
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpengl", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frambuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Fail to initialize GLAD" << std::endl;
		return -2;
	}

	/* 创建用于保存绘制数据的顶点缓冲对象VBO */
	// 创建VBO对象 --顶点缓冲对象（Vertex Buffer Objects, VBO)
	GLuint VBO;
	glGenBuffers(1, &VBO);  // VBO 缓冲type GL_ARRAY_BUFFER
	// 把VBO绑定到GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//// 将自己的顶点数据传输到GL_ARRAY_BUFFER中
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/* 1. 创建用于保存绘制数据的顶点数组对象VAO */
	// 创建VAO对象 --顶点数组对象（Vertex Array Objects, VAO）
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	// 把VAO对象绑定到内存缓冲中供GL使用
	glBindVertexArray(VAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//// 设置顶点属性指针 激活location=0的位置属性
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	/* 创建索引缓冲对象EBO */
	GLuint EBO;
	glGenBuffers(1, &EBO);
	// 绑定到索引缓冲中
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



	/* 2. 创建着色器对象，并连接到着色器程序中 */
	// 创建顶点着色器对象
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vshaderSource = readShaderFile("shaders\\vshader.vs").c_str();

	glShaderSource(vertexShader, 1, (&vertexShaderSource), NULL); // 将着色器源码附加到着色器对象中
	glCompileShader(vertexShader);
	TestCompileShader(vertexShader);
	// 创建片元着色器对象
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fshaderSource = readShaderFile("shaders\\fshader.fs").c_str();
	glShaderSource(fragmentShader, 1, (&fragmentShaderSource), NULL);
	glCompileShader(fragmentShader);
	TestCompileShader(fragmentShader);
	// 创建着色器程序, 加入着色器对象并链接
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	TestLinkProgram(shaderProgram);
	// 激活program对象 并 删除着色器对象
	glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	// 解析顶点数据方式
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//  .. :: 绘制代码(循环绘制中) :: ..
	// 3. 绘制物体
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// -----
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		////draw our first triangle
		//glUseProgram(shaderProgram);
		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		// draw rect
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();

	return 0;

}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void frambuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}