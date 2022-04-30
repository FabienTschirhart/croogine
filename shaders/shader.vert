#version 450

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform Pushdata{ 
		mat2 transform;
		vec2 offset;
		vec3 color;
	} pushdata;


vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);


void main() {
	gl_Position = vec4(pushdata.transform * position + pushdata.offset, 0.0, 1.0);
	fragColor = pushdata.color + colors[gl_VertexIndex];
}