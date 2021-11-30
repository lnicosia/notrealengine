#version 400 core

uniform mat4					mesh_model;
uniform mat4					obj_model;

layout (location = 0) in vec3	aPos;

void main()
{
	gl_Position = vec4(aPos, 1.0) * mesh_model * obj_model;
}
