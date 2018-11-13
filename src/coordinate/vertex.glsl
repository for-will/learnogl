#version 330 core
layout (location=0) in vec3 ps;
layout (location=1) in vec2 inCoord;

out vec2 coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * model * vec4(ps, 1);
    coord = inCoord;
}