#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture;

void main()
{
//    color = texture(ourTexture, TexCoord); //纹理采样器,纹理坐标
    color = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0f);
}
