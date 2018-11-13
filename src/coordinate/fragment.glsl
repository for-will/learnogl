#version 330 core
in vec2 coord;
out vec4 color;
uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;
void main(){
    color = mix(texture(ourTexture0, coord), texture(ourTexture1, vec2(coord.x, 1-coord.y)), 0.2);
}