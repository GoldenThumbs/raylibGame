#version 330

in vec2 fragTexCoord;

out vec4 finalColor;

uniform sampler2D texture0;

void main()
{
    finalColor = texture(texture0, fragTexCoord);
    finalColor = pow(finalColor, vec4(1.0/2.2));
}