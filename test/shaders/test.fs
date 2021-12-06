#version 400 core

in vec2	TextCoord;

uniform	sampler2D	texture_diffuse0;

out vec4 FragColor;

void main()
{
	//FragColor = vec4(1.0f, 0.7f, 0.0f, 0.0f);
	FragColor = texture(texture_diffuse0, TextCoord);
}
