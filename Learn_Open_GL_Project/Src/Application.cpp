#include <GLAD/include/glad/glad.h>
#include <GLFW/include/GLFW/glfw3.h>
#include <iostream>
#include "Application.h"

/// Macro to apply breakpoint at runtime if soln is on Debug
#define ASSERT(x) if( !(x) _debugBreak();

//Macro to wrap GlClear, Glogcall and Assert forparticular func x
#define GL_GET_LOG(x) GLClearError();\
        x;\
        ASSERT(GL_GET_LOGS(#x, __FILE__, __LINE__));

/// <summary>
/// function to map error code to repective messages
/// </summary>
/// <param name="error"></param>
/// <returns></returns>
static std::string getGLErrorString(GLenum error)
{
	switch (error)
	{
	case GL_NO_ERROR:
		return "No error";

	case GL_INVALID_ENUM:
		return "Invalid enum";

	case GL_INVALID_VALUE:
		return "Invalid value";

	case GL_INVALID_OPERATION:
		return "Invalid operation";

	case GL_OUT_OF_MEMORY:
		return "Out of memory";

	default:
		return "Unknown error";
	}
}

/// <summary>
/// Method to clear error log
/// </summary>
static void GLClearError()
{
	//glgetError basically return current error codes and set error flag to GL_NO_ERROR
	while (glGetError() != GL_NO_ERROR);
}

/// <summary>
/// Method to collect get log errors
/// </summary>
/// <param name="functionName"></param>
/// <param name="filePath"></param>
/// <param name="line"></param>
/// <returns></returns>
static bool GL_GET_LOGS(const char* functionName, const char* filePath, long line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[Open GL]" << "v" << glGetString(GL_VERSION) << std::endl
			<< "[File] : " << filePath << std::endl
			<< "[ERROR] :- " << getGLErrorString(error) <<
			"\n[FUNCTION] " << functionName << " on line " << line << std::endl;
		return false;
	}

	return true;
}



/// <summary>
/// Function to Intialize and Configure GLFW
/// </summary>
void Intialize_And_Configure_GLFW()
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

/// <summary>
/// Function to Create Window Using GLFW Lib
/// </summary>
/// <returns></returns>
GLFWwindow* CreateWindow()
{
	GLFWwindow* window = glfwCreateWindow(800, 800, "Graphics Engine", NULL, NULL);
	if (window == nullptr)
	{
		std::cout << "Failed to Create Window " << std::endl;
		glfwTerminate();
		return nullptr;
	}

	// Make the OpenGL context current
	glfwMakeContextCurrent(window);

	return window;
}

/// <summary>
/// Intialize GLAD Loader
/// </summary>
/// <returns></returns>
int Intialize_GLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to Intialize GLAD" << std::endl;
		return -1;
	}

	return 1;
}

/// <summary>
/// Function To Process Inputs
/// </summary>
/// <param name="window"></param>
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

/// <summary>
/// OpenGL Callback function which will be invoked whenever window is resized 
/// </summary>
/// <param name="window"></param>
/// <param name="width"></param>
/// <param name="height"></param>
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

/// <summary>
/// Process of Creating VBO to send vertices information to Graphic Memory (Vertex Shader)
/// </summary>
unsigned int& SendingDataToVertexShader()
{
	//Vertex data
	float vertices[] =
	{
		-0.5f, -0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f,
		-0.5f,  0.5f,  0.0f,
		 0.5f,  0.5f,  0.0f,
	};

	//declaring Vertex BufferObject
	unsigned int vbo;

	//Generate Buffer
	glGenBuffers(1, &vbo);

	//Bind Buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Allocate vertex data to bounded buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	return vbo;
}

/// <summary>
/// Function to Gen Vertex Arrays Objs & bind them to currenlty used Buffer
/// After this we can generate our VBO and give specification to them then after using them we can unbind VAO
/// </summary>
/// <returns></returns>
unsigned int& Gen_And_Bind_VAO()
{
	unsigned int VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	return VAO;
}

/// <summary>
/// Function to Generate element buffer object used for using exisisting vertices many team to save memory
/// </summary>
/// <returns></returns>
unsigned int& Gen_And_Bind_IndexBuffer()
{
	unsigned int indices[] =
	{  // note that we start from 0!
		0, 1, 2,   // first triangle
		1, 2, 3    // second triangle
	};

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	return EBO;
}

/// <summary>
/// Function to give specification to buffer object. Regarding how it should use Binded Data.
/// </summary>
void GiveSpecification_To_BufferObj()
{
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

unsigned int& Compile_And_UseShaders()
{
	const char* vertexShaderSource = "#version 330 core\n"
									 "layout (location = 0) in vec3 aPos;\n"
									 "void main()\n"
									 "{\n"
									 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
									 "}\0";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	Shader_Compile_Logs(vertexShader);

	const char* fragShaderSource = "#version 330 core\n"
								   "out vec4 FragColor;\n"
								   "void main()\n"
								   "{\n"
								   "	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
								   "}\n";

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
	glCompileShader(fragmentShader);
	Shader_Compile_Logs(fragmentShader);


	//Shader Program Code
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glUseProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void Shader_Compile_Logs(unsigned int vertexShader)
{
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

int main()
{
	Intialize_And_Configure_GLFW();

	GLFWwindow* window = CreateWindow();
	if (window == nullptr)
	{
		return -1;
	}

	if (Intialize_GLAD() == -1)
	{
		return -1;
	}

	//OpenGL viewport Size
	glViewport(0, 0, 800, 800);


	unsigned int VAO = Gen_And_Bind_VAO();
	unsigned int VBO = SendingDataToVertexShader();
	unsigned int EBO = Gen_And_Bind_IndexBuffer();



	unsigned int shaderProgram = Compile_And_UseShaders();
	GiveSpecification_To_BufferObj();


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);

	//Set Framebuffer callback function
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glUseProgram(shaderProgram);

		//GiveSpecification_To_BufferObj();

		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
