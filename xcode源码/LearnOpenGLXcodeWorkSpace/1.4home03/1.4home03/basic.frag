#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

// Texture samplers
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

uniform float mixValue;

void main()
{
    // Linearly interpolate between both textures (second texture is only slightly combined)
    color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), mixValue);
//    color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2,vec2(1-TexCoord.x, TexCoord.y)), 0.2);

}
