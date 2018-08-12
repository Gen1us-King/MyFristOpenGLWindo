#define GLEW_STATIC
//#include<GL\glew.h>
#include<glad\glad.h>
#include<iostream>
#include<GLFW\glfw3.h>

const unsigned int window_width = 800;
const unsigned int window_height = 600;
const unsigned int view_width = 700;
const unsigned int view_height = 500;
const char* vertexShaderSource = "#version 330 core\n"
"layout(location=0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0f);\n"
"}\n\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor; \n"
"void main()\n"
"{\n"
"FragColor =vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow * window = glfwCreateWindow(window_width, window_height, "MyFristOpenGLWindow",nullptr,nullptr);
	glfwMakeContextCurrent(window);//设置当前线程的主环境；
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();//终止窗口
		return -1;
	}
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	/*glewExperimental = GLFW_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to GLEWINIT" << std::endl;
		return 0;
	}*/
	glViewport(0, 0, view_width, view_height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//注册窗口大小改变回调函数
	
//vertexShader		
	unsigned int vertexShader, fragmentShader, shaderProgram;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	char inFolog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, inFolog);
		std::cout<<"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << inFolog << std::endl;
	}
//fragmentShader 
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, inFolog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << inFolog << std::endl;
	}

//shaderProgram
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderProgram, 512, NULL, inFolog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << inFolog << std::endl;
	}	
	glDeleteShader(vertexShader);
	glCompileShader(fragmentShader);
	//
	
	/*float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};*/
	float vertices[] = {
		0.5f, 0.5f, 0.0f,   // 右上角
		0.5f, -0.5f, 0.0f,  // 右下角
		-0.5f, -0.5f, 0.0f, // 左下角
		//-0.5f, 0.5f, 0.0f   // 左上角
	};
	int indices[] = {
		0,1,2,
	};
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);//当传递的参数是非0且是glGenVertexArray()第一次返回的，此时会将传递的参数名字绑定到新创建的顶点数组对象。

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER,0);//解绑
	glBindVertexArray(0);//解绑
	while (!glfwWindowShouldClose(window))
	{
		glfwSetKeyCallback(window, key_callback);
		glClearColor(0.5f, 1.0f, 0.4f, 0.3f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);//当传递的参数是已经被绑定过的对象名称，则此时的作用是激活顶点数组对象，后续的相关操作将作用到该顶点数组对象。
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}
void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window,true);
	}

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}
