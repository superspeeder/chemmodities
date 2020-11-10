#shader vertex
#version 400

layout (location = 0) in vec2 position;
layout (location = 1) in vec4 color;

/*
out VS_OUT {
    vec4 color;
} vs_out;
*/

out vec4 fColor;

void main() {
    gl_Position = vec4(position.x,position.y,0.0,1.0);
//    vs_out.color = color;
    fColor = color;
}