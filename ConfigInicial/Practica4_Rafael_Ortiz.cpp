//// Práctica 4
// Ortiz Valles Joaquin Rafael
// Fecha de entrega: 13/09/2026
// Número de cuenta: 319071616

#include<iostream>
#include <cmath>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
#include "Shader.h"

void Inputs(GLFWwindow* window);

const GLint WIDTH = 800, HEIGHT = 600;
float movX = 0.0f;
float movY = 0.0f;
float movZ = -5.0f;
float rot = 0.0f;

// use with Perspective Projection
float vertices[] = {
	-0.5f, -0.5f,  0.5f, 0.5f, 0.8f, 1.0f, // Front
	 0.5f, -0.5f,  0.5f, 0.5f, 0.8f, 1.0f,
	 0.5f,  0.5f,  0.5f, 0.5f, 0.8f, 1.0f,
	 0.5f,  0.5f,  0.5f, 0.5f, 0.8f, 1.0f,
	-0.5f,  0.5f,  0.5f, 0.5f, 0.8f, 1.0f,
	-0.5f, -0.5f,  0.5f, 0.5f, 0.8f, 1.0f,

	-0.5f, -0.5f, -0.5f, 0.5f, 0.8f, 1.0f, // Back
	 0.5f, -0.5f, -0.5f, 0.5f, 0.8f, 1.0f,
	 0.5f,  0.5f, -0.5f, 0.5f, 0.8f, 1.0f,
	 0.5f,  0.5f, -0.5f, 0.5f, 0.8f, 1.0f,
	-0.5f,  0.5f, -0.5f, 0.5f, 0.8f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.5f, 0.8f, 1.0f,

	 0.5f, -0.5f,  0.5f, 0.5f, 0.8f, 1.0f, // Right
	 0.5f, -0.5f, -0.5f, 0.5f, 0.8f, 1.0f,
	 0.5f,  0.5f, -0.5f, 0.5f, 0.8f, 1.0f,
	 0.5f,  0.5f, -0.5f, 0.5f, 0.8f, 1.0f,
	 0.5f,  0.5f,  0.5f, 0.5f, 0.8f, 1.0f,
	 0.5f, -0.5f,  0.5f, 0.5f, 0.8f, 1.0f,

	-0.5f,  0.5f,  0.5f, 0.5f, 0.8f, 1.0f, // Left
	-0.5f,  0.5f, -0.5f, 0.5f, 0.8f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.5f, 0.8f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.5f, 0.8f, 1.0f,
	-0.5f, -0.5f,  0.5f, 0.5f, 0.8f, 1.0f,
	-0.5f,  0.5f,  0.5f, 0.5f, 0.8f, 1.0f,

	-0.5f, -0.5f, -0.5f, 0.5f, 0.8f, 1.0f, // Bottom
	 0.5f, -0.5f, -0.5f, 0.5f, 0.8f, 1.0f,
	 0.5f, -0.5f,  0.5f, 0.5f, 0.8f, 1.0f,
	 0.5f, -0.5f,  0.5f, 0.5f, 0.8f, 1.0f,
	-0.5f, -0.5f,  0.5f, 0.5f, 0.8f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.5f, 0.8f, 1.0f,

	-0.5f,  0.5f, -0.5f, 0.5f, 0.8f, 1.0f, // Top
	 0.5f,  0.5f, -0.5f, 0.5f, 0.8f, 1.0f,
	 0.5f,  0.5f,  0.5f, 0.5f, 0.8f, 1.0f,
	 0.5f,  0.5f,  0.5f, 0.5f, 0.8f, 1.0f,
	-0.5f,  0.5f,  0.5f, 0.5f, 0.8f, 1.0f,
	-0.5f,  0.5f, -0.5f, 0.5f, 0.8f, 1.0f,
};


// Vértices para el Triángulo
float verticesTriangulo[] = {
	 0.0f,  0.5f, 0.0f,    1.0f, 1.0f, 1.0f, // Arriba
	-0.5f, -0.5f, 0.0f,    1.0f, 1.0f, 1.0f, // Izquierda
	 0.5f, -0.5f, 0.0f,    1.0f, 1.0f, 1.0f  // Derecha
};

// Vértices y para el Círculo
const int numSegmentos = 36;
float verticesCirculo[(numSegmentos + 2) * 6];

void generarCirculo() {
	verticesCirculo[0] = 0.0f; verticesCirculo[1] = 0.0f; verticesCirculo[2] = 0.0f;
	verticesCirculo[3] = 1.0f; verticesCirculo[4] = 1.0f; verticesCirculo[5] = 1.0f;
	for (int i = 0; i <= numSegmentos; i++) {
		float angulo = i * 2.0f * 3.14159f / numSegmentos;
		int idx = (i + 1) * 6;
		verticesCirculo[idx] = cos(angulo) * 0.5f;     // X
		verticesCirculo[idx + 1] = sin(angulo) * 0.5f; // Y
		verticesCirculo[idx + 2] = 0.0f;               // Z
		verticesCirculo[idx + 3] = 1.0f;               // R
		verticesCirculo[idx + 4] = 1.0f;               // G
		verticesCirculo[idx + 5] = 1.0f;               // B
	}
}

int main() {
	glfwInit();

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 4 - Ortiz Valles Joaquin Rafael", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;


	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}


	//  Dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);


	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");


	generarCirculo(); 

	GLuint VBO, VAO;          // Cubos y rectángulos
	GLuint VBO_Tri, VAO_Tri;  // Triángulos
	GLuint VBO_Cir, VAO_Cir;  // Círculos

	// 1. VAO del Cubo original
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// 2. VAO del Triángulo
	glGenVertexArrays(1, &VAO_Tri);
	glGenBuffers(1, &VBO_Tri);
	glBindVertexArray(VAO_Tri);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Tri);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTriangulo), verticesTriangulo, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// 3. VAO del Círculo
	glGenVertexArrays(1, &VAO_Cir);
	glGenBuffers(1, &VBO_Cir);
	glBindVertexArray(VAO_Cir);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Cir);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCirculo), verticesCirculo, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	glm::mat4 projection = glm::mat4(1);
	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(window))
	{
		Inputs(window);
		glfwPollEvents();

		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.Use();
		glm::mat4 model = glm::mat4(1);
		glm::mat4 view = glm::mat4(1);

		view = glm::translate(view, glm::vec3(movX, movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint colorLoc = glGetUniformLocation(ourShader.Program, "colorFigura"); // Nuevo Uniform para color

		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glBindVertexArray(VAO);
		glUniform3f(colorLoc, 0.5f, 0.8f, 1.0f); // Le aplicamos su color azulito original 

		// Cuerpo
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 0.4f));
		model = glm::translate(model, glm::vec3(0.0f, 0.6f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pierna Izquierda
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.5f, 0.2f));
		model = glm::translate(model, glm::vec3(1.0f, -0.4f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pierna Derecha
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.5f, 0.2f));
		model = glm::translate(model, glm::vec3(-1.0f, -0.4f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Entrepierna
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.2f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Brazo izquierdo
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.1f, 0.6f, 0.2f));
		model = glm::translate(model, glm::vec3(-6.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Brazo derecho
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.1f, 0.6f, 0.2f));
		model = glm::translate(model, glm::vec3(6.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pie derecho
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.1f, 0.3f));
		model = glm::translate(model, glm::vec3(-1.0f, -5.0f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pie izquierdo
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.1f, 0.3f));
		model = glm::translate(model, glm::vec3(1.0f, -5.0f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// 1. Cuadro Gris 1
		glUniform3f(colorLoc, 0.5f, 0.5f, 0.5f); // Gris
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.2f, 0.9f, 0.3f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// 2. Cuadro Gris 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.2f, 0.9f, 0.3f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// 3. Rectángulo Gris 1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.2f, 0.27f, 0.21f));
		model = glm::scale(model, glm::vec3(0.4f, 0.1f, 0.1f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// 4. Cuadro más azul claro
		glUniform3f(colorLoc, 0.6f, 0.9f, 1.0f); // Azul muy claro
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.8f, 0.2f));
		model = glm::scale(model, glm::vec3(0.7f, 0.8f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// 5. Otro Rectángulo Gris
		glUniform3f(colorLoc, 0.5f, 0.5f, 0.5f); // Gris
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.6f, 0.3f));
		model = glm::scale(model, glm::vec3(0.6f, 0.1f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// 6. Rectángulo Amarillo 1
		glUniform3f(colorLoc, 1.0f, 1.0f, 0.0f); // Amarillo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.2f, 0.16f, 0.2f));
		model = glm::scale(model, glm::vec3(0.18f, 0.03f, 0.03f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// 7. Rectángulo Amarillo 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.2f, 0.16f, 0.2f));
		model = glm::scale(model, glm::vec3(0.03f, 0.1f, 0.03f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// 8. Triángulo Azul
		glBindVertexArray(VAO_Tri); 
		glUniform3f(colorLoc, 0.0f, 0.0f, 1.0f); // Azul fuerte
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.1f, 0.3f, 0.21f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 3); // solo son 3 vértices

		// 9. Círculo Verde 1
		glBindVertexArray(VAO_Cir);
		glUniform3f(colorLoc, 0.0f, 1.0f, 0.0f); // Verde
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.25f, 0.24f, 0.21f));
		model = glm::scale(model, glm::vec3(0.19f, 0.19f, 0.19f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLE_FAN, 0, numSegmentos + 2);

		// 10. Círculo Verde 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.4f, 0.3f, 0.21f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLE_FAN, 0, numSegmentos + 2);


		glBindVertexArray(0);
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO_Tri);
	glDeleteBuffers(1, &VBO_Tri);
	glDeleteVertexArrays(1, &VAO_Cir);
	glDeleteBuffers(1, &VBO_Cir);

	glfwTerminate();
	return EXIT_SUCCESS;
}

void Inputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		movY += 0.001f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		movY -= 0.001f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rot += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rot -= 0.1f;
}