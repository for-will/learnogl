#version 330 core
layout (location=0) in vec3 position;
layout (location=1) in vec2 coord;
out vec2 texCoord;
uniform mat4 trans;
void main(){
    gl_Position = trans * vec4(position, 1.0);
    texCoord = coord;
}