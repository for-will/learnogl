#version 330 core
in vec2 texCoord;
out vec4 color;
uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;

void main(){
//    color = vec4(0.5, 0.2, 0, 1.0);
    color = mix(texture( ourTexture0, texCoord), texture(ourTexture1, vec2(texCoord.x, 1-texCoord.y)), 0.2);
//    color = vec4(texCoord, 0, 1);
}