#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

//uniform vec3 viewPos;
uniform mat4 mvp;

// Output fragment color
layout (location = 0) out vec4 colorbuffer;
layout (location = 1) out vec4 normalbuffer;
layout (location = 2) out vec4 positionbuffer;

void main()
{
    vec4 texCol = texture(texture0, fragTexCoord);

    //vec3 vRaw = viewPos - fragPosition;

    vec3 n = normalize(fragNormal);
    //vec3 v = normalize(vRaw);

    //float NdV = max(0, dot(n, v));

    //float dist = length(vRaw);
    //float fog = clamp(1.0 / (dist * dist), 0, 1);

    //vec3 fCol = mix(colDiffuse.rgb, texCol.rgb * fragColor.rgb, fog);

    colorbuffer = vec4(texCol.rgb * fragColor.rgb, 1.0);
    normalbuffer = vec4(n * 0.5 + 0.5, 1.0);
    positionbuffer = vec4(fragPosition, 1.0);
    //finalColor.a = 1.0;
}