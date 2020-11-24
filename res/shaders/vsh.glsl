#shader vertex
#version 400

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 uv;


out vec2 uvs;

void main() {
    gl_Position = vec4(position.x,position.y,position.z,position.w);
    uvs = uv;
}