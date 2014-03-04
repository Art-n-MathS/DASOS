#version 150

// this is a pointer to the current 2D texture object
uniform sampler2D tex;
in vec2 vertUV;
in vec3 normal;
out vec4 outColour;
void main ()
{
 // set the fragment colour to the current texture
 outColour = texture(tex,vertUV);
}
