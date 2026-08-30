// Práctica 2
// Ortiz Valles Joaquin Rafael
// Archivo: practica2_Rafael_Ortiz.cpp
// Fecha de entrega: 30/08/2026
// Número de cuenta: 319071616

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);
const GLint WIDTH = 800, HEIGHT = 600;

// Matriz de 14x14 representando el fantasma
// 0: Vacío, 1: Rojo, 2: Blanco, 3: Azul
int mapa[14][14] = {
    {0,0,0,0,0,1,1,1,1,0,0,0,0,0},
    {0,0,0,1,1,1,1,1,1,1,1,0,0,0},
    {0,0,1,1,1,1,1,1,1,1,1,1,0,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,1,2,2,1,1,1,2,2,1,1,1,0},
    {0,1,2,2,2,2,1,2,2,2,2,1,1,0},
    {1,1,3,3,2,2,1,3,3,2,2,1,1,1},
    {1,1,3,3,2,2,1,3,3,2,2,1,1,1},
    {1,1,2,2,2,2,1,2,2,2,2,1,1,1},
    {1,1,1,2,2,1,1,1,2,2,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,0,1,1,1,1,1,1,0,1,1,1},
    {1,1,0,0,0,1,1,1,1,0,0,0,1,1}
};

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Práctica 2 - Ortiz Valles Joaquin Rafael", NULL, NULL);
    glfwSetFramebufferSizeCallback(window, resize);

    if (window == NULL) {
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

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    // Generación dinámica de vértices
    std::vector<float> vertices;
    float pixelSize = 0.1f; // Tamaño de cada "píxel" en coordenadas OpenGL
    float startX = -0.7f;   // Posición inicial X (esquina superior izquierda)
    float startY = 0.7f;    // Posición inicial Y

    for (int row = 0; row < 14; ++row) {
        for (int col = 0; col < 14; ++col) {
            int colorCode = mapa[row][col];

            // Ignorar los espacios vacíos
            if (colorCode == 0) continue;

            // Determinar color (R, G, B)
            float r = 0.0f, g = 0.0f, b = 0.0f;
            if (colorCode == 1) { r = 0.86f; g = 0.15f; b = 0.15f; } // Rojo
            else if (colorCode == 2) { r = 1.0f; g = 1.0f; b = 1.0f; } // Blanco
            else if (colorCode == 3) { r = 0.25f; g = 0.41f; b = 0.75f; } // Azul

            // Calcular coordenadas (X, Y) del píxel actual
            float x = startX + (col * pixelSize);
            float y = startY - (row * pixelSize); // Se resta porque Y crece hacia arriba en OpenGL

            // Generar los 6 vértices para hacer un cuadrado (2 triángulos)
            // Triángulo 1
            vertices.insert(vertices.end(), { x + pixelSize, y, 0.0f, r, g, b });             // Arriba Derecha
            vertices.insert(vertices.end(), { x + pixelSize, y - pixelSize, 0.0f, r, g, b }); // Abajo Derecha
            vertices.insert(vertices.end(), { x, y - pixelSize, 0.0f, r, g, b });             // Abajo Izquierda
            // Triángulo 2
            vertices.insert(vertices.end(), { x, y - pixelSize, 0.0f, r, g, b });             // Abajo Izquierda
            vertices.insert(vertices.end(), { x, y, 0.0f, r, g, b });                         // Arriba Izquierda
            vertices.insert(vertices.end(), { x + pixelSize, y, 0.0f, r, g, b });             // Arriba Derecha
        }
    }

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Usamos vector.size() * sizeof(float) para obtener el tamaño en bytes
    // vector.data() devuelve un puntero al bloque de memoria interno
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Atributo 0: Posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Atributo 1: Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f); // Fondo oscuro
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.Use();
        glBindVertexArray(VAO);

        // Dibujar triángulos. El número total de vértices es el tamaño del vector dividido entre 6 atributos (X,Y,Z,R,G,B)
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 6);

        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}