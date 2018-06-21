#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include "Shader.h"
#include "stb_imags.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// stores how much we're seeing of either texture
float mixValue = 0.2f;

float rotate_angle = 0.0f;


// camera
Camera camera(glm::vec3(0.0f, 2.0f, 3.0f));
float lastX = 400.0f;
float lastY = 300.0f;
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
bool firstMouse = true;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


	GLFWwindow* window = glfwCreateWindow(800, 600, "HelloWorld", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader ourShader("shader.ves", "shader.frs");//自定义的Shader类实例化
	Shader ourShader1("shader1.ves", "shader1.frs");//自定义的Shader类实例化

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	//float vertices[] = {
	//	0.5f,  0.5f, 0.0f,  // top right
	//	0.5f, -0.5f, 0.0f,  // bottom right
	//	-0.5f, -0.5f, 0.0f,  // bottom left
	//	-0.5f,  0.5f, 0.0f   // top left 
	//};
	//float vertices[] = {
	//	// first triangle
	//	-0.9f, -0.5f, 0.0f,  // left 
	//	-0.0f, -0.5f, 0.0f,  // right
	//	-0.45f, 0.5f, 0.0f,  // top 
	//						 // second triangle
	//						 0.0f, -0.5f, 0.0f,  // left
	//						 0.9f, -0.5f, 0.0f,  // right
	//						 0.45f, 0.5f, 0.0f   // top 
	//};

	//float vertices[] = {
	//	// 位置              // 颜色
	//	0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
	//	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
	//	0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
	//};

	//float vertices[] = {
	//	//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
	//	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
	//	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
	//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
	//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
	//};

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f

	};


	float vertices1[] = {
		0.5f,  0.0f, 0.5f,  15.0f, 15.0f, // top right
		0.5f, 0.0f, -0.5f, 15.0f,0.0f, // bottom right
		-0.5f, 0.0f,-0.5f, 0.0f, 0.0f, // bottom left
		-0.5f,  0.0f, 0.5f ,0.0f,15.0f // top left 
	};

	// world space positions of our cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(14.5f,  0.5f,  14.5f),
		glm::vec3(14.5f,  0.5f, -14.5f),
		glm::vec3(-14.5f,  0.5f, -14.5f),
		glm::vec3(-14.5f, 0.5f, 14.5f),

		glm::vec3(-10.5f, 0.5f, -2.5f),
		glm::vec3(-5.0f,  0.5f, 5.5f),
		glm::vec3(3.8f, 0.5f, -12.3f),
		glm::vec3(12.4f, 0.5f, -3.5f),
		glm::vec3(-8.7f,  0.5f, 7.5f),
		glm::vec3(4.5f,  0.5f, 10.5f)
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	unsigned int VBO[2], VAO[2];
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 颜色属性
	/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);*/

	//纹理属性
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//glBindVertexArray(0);



	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//纹理属性
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);



	unsigned int texture1, texture2, texture3;
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
											// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// texture 2
	// ---------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("floor.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	//glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	//glm::mat4 trans;//4×4单位矩阵
					//trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));//构造平移变换矩阵(调试看到的每行代表一列)
					//vec = trans * vec;
					//std::cout << vec.x << vec.y << vec.z << std::endl;

	//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));//旋转角度和旋转轴
	//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));




	ourShader.use();
	glUniform1i(glGetUniformLocation(ourShader.ID, "ourTexture1"), 0);
	glUniform1i(glGetUniformLocation(ourShader.ID, "ourTexture2"), 1);

	ourShader1.use();
	glUniform1i(glGetUniformLocation(ourShader1.ID, "ourTexture3"), 0);

	glEnable(GL_DEPTH_TEST);
	//glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.0f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw our first triangle
		/*glUseProgram(shaderProgram);*/

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);


		ourShader.use();
		//ourShader.setFloat("xOffset", offset);

		glUniform1f(glGetUniformLocation(ourShader.ID, "mixValue"), mixValue);


		//glm::mat4 model;//模型矩阵
		//model = glm::rotate(model, rotate_angle, glm::vec3(0.5f, 1.0f, 0.0f));

		glm::mat4 view;//观察矩阵
		/*float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;*/
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		//view = glm::lookAt(glm::vec3(camX,0.0f,camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);

		/*glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"),1, GL_FALSE, glm::value_ptr(model));*/
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		//glm::mat4 trans;
		//trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		//glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "transform"), 1, GL_FALSE, glm::value_ptr(model));

		/*float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, 0.0f, greenValue, 1.0f);*/

		glBindVertexArray(VAO[0]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			/*if (i % 2 == 0) {
				model = glm::rotate(model, rotate_angle, glm::vec3(1.0f, 0.3f, 0.5f));
			}
			else {
				float angle = 20.0f * i;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			}*/
			glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// glBindVertexArray(0); // no need to unbind it every time 



		//trans = glm::mat4(); // reset it to an identity matrix
		//trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
		//float scaleAmount = sin(glfwGetTime());
		//trans = glm::scale(trans, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
		//glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "transform"), 1, GL_FALSE, &trans[0][0]); // this time take the matrix value array's first element as its memory pointer value


		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		////建议您在组合矩阵时，先进行缩放操作，然后是旋转，最后才是位移.
		//trans = glm::mat4(); // reset it to an identity matrix
		//trans = glm::translate(trans, glm::vec3(-0.5f, -0.5f, 0.0f));
		//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.5f, 0.5f, 0.0f));
		//glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "transform"), 1, GL_FALSE, &trans[0][0]); // this time take the matrix value array's first element as its memory pointer value


		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture3);

		ourShader1.use();
		glBindVertexArray(VAO[1]);
		glm::mat4 model;
		model = glm::scale(model, glm::vec3(30, 30, 30));
		glUniformMatrix4fv(glGetUniformLocation(ourShader1.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(ourShader1.ID, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(ourShader1.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	//glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixValue += 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (mixValue >= 1.0f)
			mixValue = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixValue -= 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (mixValue <= 0.0f)
			mixValue = 0.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		rotate_angle += 0.01;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		rotate_angle -= 0.01;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime, glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime, glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime, glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime, glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);

}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}