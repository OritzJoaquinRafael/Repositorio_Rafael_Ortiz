// Practica Final - Skybox
// Ortiz Valles Joaquin Rafael
// Fecha de entrega: 08/11/2026
// Número de cuenta: 319071616

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector> // --- SKYBOX ---
#include <string> // --- SKYBOX ---

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();
void saveAnimationToFile();
void loadAnimationFromFile();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

// --- SKYBOX VERTICES ---
GLfloat skyboxVertices[] = {
	// Positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

// --- SKYBOX FUNCTION ---
GLuint loadCubemap(std::vector<std::string> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, channels;
	for (GLuint i = 0; i < faces.size(); i++)
	{
		unsigned char* image = SOIL_load_image(faces[i].c_str(), &width, &height, &channels, SOIL_LOAD_RGB);
		if (image)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			SOIL_free_image_data(image);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

glm::vec3 Light1 = glm::vec3(0);

//Anim variables
float rotBall = 0.0f;
float rotDog = 0.0f;

// Extremidades Independientes
float tiltDog = 0.0f;
float head = 0.0f;
float tail = 0.0f;
float F_RightLeg = 0.0f;
float F_LeftLeg = 0.0f;
float B_RightLeg = 0.0f;
float B_LeftLeg = 0.0f;

//KeyFrames
float dogPosX, dogPosY, dogPosZ;

#define MAX_FRAMES 30 
int i_max_steps = 1000;
int i_curr_steps = 0;

typedef struct _frame {
	float rotDog;
	float rotDogInc;
	float tiltDog;
	float tiltDogInc;
	float dogPosX;
	float dogPosY;
	float dogPosZ;
	float incX;
	float incY;
	float incZ;

	float head;
	float headInc;
	float tail;
	float tailInc;
	float F_RightLeg;
	float F_RightLegInc;
	float F_LeftLeg;
	float F_LeftLegInc;
	float B_RightLeg;
	float B_RightLegInc;
	float B_LeftLeg;
	float B_LeftLegInc;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	printf("frameindex %d\n", FrameIndex);
	KeyFrame[FrameIndex].dogPosX = dogPosX;
	KeyFrame[FrameIndex].dogPosY = dogPosY;
	KeyFrame[FrameIndex].dogPosZ = dogPosZ;
	KeyFrame[FrameIndex].rotDog = rotDog;
	KeyFrame[FrameIndex].tiltDog = tiltDog;
	KeyFrame[FrameIndex].head = head;
	KeyFrame[FrameIndex].tail = tail;
	KeyFrame[FrameIndex].F_RightLeg = F_RightLeg;
	KeyFrame[FrameIndex].F_LeftLeg = F_LeftLeg;
	KeyFrame[FrameIndex].B_RightLeg = B_RightLeg;
	KeyFrame[FrameIndex].B_LeftLeg = B_LeftLeg;
	FrameIndex++;
}

void resetElements(void)
{
	dogPosX = KeyFrame[0].dogPosX;
	dogPosY = KeyFrame[0].dogPosY;
	dogPosZ = KeyFrame[0].dogPosZ;
	rotDog = KeyFrame[0].rotDog;
	tiltDog = KeyFrame[0].tiltDog;
	head = KeyFrame[0].head;
	tail = KeyFrame[0].tail;
	F_RightLeg = KeyFrame[0].F_RightLeg;
	F_LeftLeg = KeyFrame[0].F_LeftLeg;
	B_RightLeg = KeyFrame[0].B_RightLeg;
	B_LeftLeg = KeyFrame[0].B_LeftLeg;
}

void interpolation(void)
{
	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].dogPosX - KeyFrame[playIndex].dogPosX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].dogPosY - KeyFrame[playIndex].dogPosY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].dogPosZ - KeyFrame[playIndex].dogPosZ) / i_max_steps;
	KeyFrame[playIndex].rotDogInc = (KeyFrame[playIndex + 1].rotDog - KeyFrame[playIndex].rotDog) / i_max_steps;
	KeyFrame[playIndex].tiltDogInc = (KeyFrame[playIndex + 1].tiltDog - KeyFrame[playIndex].tiltDog) / i_max_steps;
	KeyFrame[playIndex].headInc = (KeyFrame[playIndex + 1].head - KeyFrame[playIndex].head) / i_max_steps;
	KeyFrame[playIndex].tailInc = (KeyFrame[playIndex + 1].tail - KeyFrame[playIndex].tail) / i_max_steps;
	KeyFrame[playIndex].F_RightLegInc = (KeyFrame[playIndex + 1].F_RightLeg - KeyFrame[playIndex].F_RightLeg) / i_max_steps;
	KeyFrame[playIndex].F_LeftLegInc = (KeyFrame[playIndex + 1].F_LeftLeg - KeyFrame[playIndex].F_LeftLeg) / i_max_steps;
	KeyFrame[playIndex].B_RightLegInc = (KeyFrame[playIndex + 1].B_RightLeg - KeyFrame[playIndex].B_RightLeg) / i_max_steps;
	KeyFrame[playIndex].B_LeftLegInc = (KeyFrame[playIndex + 1].B_LeftLeg - KeyFrame[playIndex].B_LeftLeg) / i_max_steps;
}

void saveAnimationToFile() {
	std::ofstream file("animacion_muertito.txt");
	if (file.is_open()) {
		file << FrameIndex << "\n";
		for (int i = 0; i < FrameIndex; i++) {
			file << KeyFrame[i].dogPosX << " " << KeyFrame[i].dogPosY << " " << KeyFrame[i].dogPosZ << " "
				<< KeyFrame[i].rotDog << " " << KeyFrame[i].tiltDog << " "
				<< KeyFrame[i].head << " " << KeyFrame[i].tail << " "
				<< KeyFrame[i].F_RightLeg << " " << KeyFrame[i].F_LeftLeg << " "
				<< KeyFrame[i].B_RightLeg << " " << KeyFrame[i].B_LeftLeg << "\n";
		}
		file.close();
		std::cout << "EXITO: Animacion exportada a 'animacion_muertito.txt'" << std::endl;
	}
}

void loadAnimationFromFile() {
	std::ifstream file("animacion_muertito.txt");
	if (file.is_open()) {
		file >> FrameIndex;
		for (int i = 0; i < FrameIndex; i++) {
			file >> KeyFrame[i].dogPosX >> KeyFrame[i].dogPosY >> KeyFrame[i].dogPosZ
				>> KeyFrame[i].rotDog >> KeyFrame[i].tiltDog
				>> KeyFrame[i].head >> KeyFrame[i].tail
				>> KeyFrame[i].F_RightLeg >> KeyFrame[i].F_LeftLeg
				>> KeyFrame[i].B_RightLeg >> KeyFrame[i].B_LeftLeg;
		}
		file.close();
		std::cout << "EXITO: Animacion cargada. Frames cargados: " << FrameIndex << std::endl;
		resetElements();
	}
}

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica Final Skybox - Ortiz Valles Joaquin Rafael", nullptr, nullptr);
	if (nullptr == window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) { return EXIT_FAILURE; }

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	Shader skyboxShader("Shader/skybox.vs", "Shader/skybox.frag"); // --- SKYBOX SHADER ---

	Model DogBody((char*)"Models/DogBody.obj");
	Model HeadDog((char*)"Models/HeadDog.obj");
	Model DogTail((char*)"Models/TailDog.obj");
	Model Model_F_RightLeg((char*)"Models/F_RightLegDog.obj");
	Model Model_F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
	Model Model_B_RightLeg((char*)"Models/B_RightLegDog.obj");
	Model Model_B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
	Model Piso((char*)"Models/piso.obj");
	Model Ball((char*)"Models/ball.obj");

	for (int i = 0; i < MAX_FRAMES; i++) {
		KeyFrame[i].dogPosX = 0; KeyFrame[i].dogPosY = 0; KeyFrame[i].dogPosZ = 0;
		KeyFrame[i].incX = 0; KeyFrame[i].incY = 0; KeyFrame[i].incZ = 0;
		KeyFrame[i].rotDog = 0; KeyFrame[i].rotDogInc = 0;
		KeyFrame[i].tiltDog = 0; KeyFrame[i].tiltDogInc = 0;
		KeyFrame[i].head = 0; KeyFrame[i].headInc = 0;
		KeyFrame[i].tail = 0; KeyFrame[i].tailInc = 0;
		KeyFrame[i].F_RightLeg = 0; KeyFrame[i].F_RightLegInc = 0;
		KeyFrame[i].F_LeftLeg = 0; KeyFrame[i].F_LeftLegInc = 0;
		KeyFrame[i].B_RightLeg = 0; KeyFrame[i].B_RightLegInc = 0;
		KeyFrame[i].B_LeftLeg = 0; KeyFrame[i].B_LeftLegInc = 0;
	}

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// --- SKYBOX VAO/VBO ---
	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	// --- CARGA DE TEXTURAS DEL SKYBOX (CUBEMAP) ---
	std::vector<std::string> faces;
	faces.push_back("Skybox/right.jpg");
	faces.push_back("Skybox/left.jpg");
	faces.push_back("Skybox/top.jpg");
	faces.push_back("Skybox/bottom.jpg");
	faces.push_back("Skybox/back.jpg");
	faces.push_back("Skybox/front.jpg");
	GLuint cubemapTexture = loadCubemap(faces);

	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		DoMovement();
		Animation();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glm::mat4 modelTemp = glm::mat4(1.0f);
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);

		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		glm::mat4 view = camera.GetViewMatrix();
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model(1);

		//Carga de modelo (Piso y Perro)
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		modelTemp = model = glm::translate(model, glm::vec3(dogPosX, dogPosY, dogPosZ));
		modelTemp = model = glm::rotate(model, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f));
		modelTemp = model = glm::rotate(model, glm::radians(tiltDog), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogBody.Draw(lightingShader);

		model = modelTemp; model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f)); model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); HeadDog.Draw(lightingShader);

		model = modelTemp; model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f)); model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); DogTail.Draw(lightingShader);

		model = modelTemp; model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f)); model = glm::rotate(model, glm::radians(F_LeftLeg), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); Model_F_LeftLeg.Draw(lightingShader);

		model = modelTemp; model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f)); model = glm::rotate(model, glm::radians(F_RightLeg), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); Model_F_RightLeg.Draw(lightingShader);

		model = modelTemp; model = glm::translate(model, glm::vec3(0.082f, -0.046, -0.218)); model = glm::rotate(model, glm::radians(B_LeftLeg), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); Model_B_LeftLeg.Draw(lightingShader);

		model = modelTemp; model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f)); model = glm::rotate(model, glm::radians(B_RightLeg), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); Model_B_RightLeg.Draw(lightingShader);

		// Ball
		model = glm::mat4(1);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ball.Draw(lightingShader);
		glDisable(GL_BLEND);
		glBindVertexArray(0);

		// Lamp object
		lampShader.Use();
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1); model = glm::translate(model, lightPos); model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::mat4(1); model = glm::translate(model, pointLightPositions[0]); model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// --- DIBUJAR SKYBOX AL FINAL ---
		glDepthFunc(GL_LEQUAL);
		skyboxShader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(skyboxShader.Program, "skybox"), 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}



void DoMovement()
{
	if (keys[GLFW_KEY_2]) rotDog += 1.0f;
	if (keys[GLFW_KEY_3]) rotDog -= 1.0f;
	if (keys[GLFW_KEY_H]) dogPosZ += 0.01;
	if (keys[GLFW_KEY_Y]) dogPosZ -= 0.01;
	if (keys[GLFW_KEY_G]) dogPosX -= 0.01;
	if (keys[GLFW_KEY_J]) dogPosX += 0.01;
	if (keys[GLFW_KEY_8]) dogPosY -= 0.001f;
	if (keys[GLFW_KEY_9]) dogPosY += 0.001f;
	if (keys[GLFW_KEY_R]) tiltDog += 0.1f;
	if (keys[GLFW_KEY_F]) tiltDog -= 0.1f;
	if (keys[GLFW_KEY_O]) head += 0.1f;
	if (keys[GLFW_KEY_P]) head -= 0.1f;
	if (keys[GLFW_KEY_Z]) tail += 0.1f;
	if (keys[GLFW_KEY_X]) tail -= 0.1f;
	if (keys[GLFW_KEY_C]) F_RightLeg += 0.1f;
	if (keys[GLFW_KEY_V]) F_RightLeg -= 0.1f;
	if (keys[GLFW_KEY_E]) F_LeftLeg += 0.1f;
	if (keys[GLFW_KEY_Q]) F_LeftLeg -= 0.1f;
	if (keys[GLFW_KEY_B]) B_RightLeg += 0.1f;
	if (keys[GLFW_KEY_N]) B_RightLeg -= 0.1f;
	if (keys[GLFW_KEY_M]) B_LeftLeg += 0.1f;
	if (keys[GLFW_KEY_COMMA]) B_LeftLeg -= 0.1f;

	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);

	if (keys[GLFW_KEY_T]) pointLightPositions[0].x += 0.01f;
	if (keys[GLFW_KEY_G]) pointLightPositions[0].x -= 0.01f;
	if (keys[GLFW_KEY_Y]) pointLightPositions[0].y += 0.01f;
	if (keys[GLFW_KEY_H]) pointLightPositions[0].y -= 0.01f;
	if (keys[GLFW_KEY_U]) pointLightPositions[0].z -= 0.1f;
	if (keys[GLFW_KEY_J]) pointLightPositions[0].z += 0.01f;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (keys[GLFW_KEY_L]) {
		if (play == false && (FrameIndex > 1)) {
			resetElements(); interpolation(); play = true; playIndex = 0; i_curr_steps = 0;
		}
		else { play = false; }
	}
	if (keys[GLFW_KEY_K]) { if (FrameIndex < MAX_FRAMES) saveFrame(); }
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) saveAnimationToFile();
	if (key == GLFW_KEY_4 && action == GLFW_PRESS) loadAnimationFromFile();
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) keys[key] = true;
		else if (action == GLFW_RELEASE) keys[key] = false;
	}
	if (keys[GLFW_KEY_SPACE]) {
		active = !active;
		if (active) Light1 = glm::vec3(0.2f, 0.8f, 1.0f);
		else Light1 = glm::vec3(0);
	}
	if (key == GLFW_KEY_M && action == GLFW_PRESS) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	if (key == GLFW_KEY_N && action == GLFW_PRESS) { glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); firstMouse = true; }
}

void Animation() {
	if (play) {
		if (i_curr_steps >= i_max_steps) {
			playIndex++;
			if (playIndex > FrameIndex - 2) {
				printf("termina anim\n"); playIndex = 0; play = false;
			}
			else { i_curr_steps = 0; interpolation(); }
		}
		else {
			dogPosX += KeyFrame[playIndex].incX; dogPosY += KeyFrame[playIndex].incY; dogPosZ += KeyFrame[playIndex].incZ;
			rotDog += KeyFrame[playIndex].rotDogInc; tiltDog += KeyFrame[playIndex].tiltDogInc;
			head += KeyFrame[playIndex].headInc; tail += KeyFrame[playIndex].tailInc;
			F_RightLeg += KeyFrame[playIndex].F_RightLegInc; F_LeftLeg += KeyFrame[playIndex].F_LeftLegInc;
			B_RightLeg += KeyFrame[playIndex].B_RightLegInc; B_LeftLeg += KeyFrame[playIndex].B_LeftLegInc;
			i_curr_steps++;
		}
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse) { lastX = xPos; lastY = yPos; firstMouse = false; }
	GLfloat xOffset = xPos - lastX; GLfloat yOffset = lastY - yPos;
	lastX = xPos; lastY = yPos;
	camera.ProcessMouseMovement(xOffset, yOffset);
}