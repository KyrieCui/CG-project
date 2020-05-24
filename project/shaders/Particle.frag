#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 startColor;

void main()
{
   
    gl_FragColor = texture(image, gl_PointCoord) * vec4(startColor,1.0);
	if(gl_FragColor.r <0.5)
	   discard;
}