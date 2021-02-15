#version 330

in vec2 fragTexCoord;
in vec3 fragColor;

out vec4 finalColor;

uniform sampler2D colorbuffer;
uniform sampler2D normalbuffer;

void main()
{
    vec3 normal = texture(normalbuffer, fragTexCoord).rgb;
    //vec3 position = texture(positionbuffer, fragTexCoord).rgb;
    vec3 color = texture(colorbuffer, fragTexCoord).rgb;

    finalColor = vec4(color, 1.0);
}