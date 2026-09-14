#version 330 core
out vec4 color;

uniform vec3 colorFigura; 

void main()
{
    color = vec4(colorFigura, 1.0f);
}