#version 330

in vec3 vertexPosition;

// Input uniform values
uniform mat4 mvp;
uniform mat4 matModel;

out vec3 fragTexCoord;

void main()
{
    // Calculate final vertex position
    gl_Position = mvp*vec4(vertexPosition, 1.0);

    fragTexCoord = gl_Position.xyw;
}