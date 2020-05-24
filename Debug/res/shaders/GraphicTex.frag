#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform sampler2D image1;
uniform sampler2D image2;
uniform vec3 spriteColor;

void main()
{
   // color = vec4(spriteColor, 1.0) * texture(image, TexCoords);
   vec4 color1 = texture(image, vec2(TexCoords));
    vec4 color3 = texture(image2, vec2(TexCoords));
    vec4 color2 = texture(image1, vec2(TexCoords.x  ,TexCoords.y+spriteColor.x));

        color = color1 * spriteColor.x + color3 * ( 1.0 - spriteColor.x) +  ( color2 * 0.5 );
}