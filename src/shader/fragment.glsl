#version 330 core
in vec3 ourColor;
in vec2 TextureCoord;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
//uniform vec4 ourColor;
void main(){
//    color = vec4(0.5, 0, 0, 1.0);
//    color = vec4(ourColor, 1.0);
    color = mix(texture(ourTexture1, TextureCoord), texture(ourTexture2, vec2(1-TextureCoord.x, 1-TextureCoord.y)), 0.2);//*vec4(ourColor, 1.0);
}