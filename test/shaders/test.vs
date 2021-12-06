#version 400 core

uniform mat4	mesh_model;
uniform mat4	obj_model;
uniform mat4	view;
uniform mat4	projection;

layout (location = 0) in vec3	aPos;
layout (location = 1) in vec3	aNorm;
layout (location = 2) in vec2	aTex;

out vec2	TextCoord;

void main()
{
	gl_Position = projection * view * obj_model * vec4(aPos, 1.0);
	TextCoord = aTex;
}
