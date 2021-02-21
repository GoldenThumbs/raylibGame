#version 330

in vec3 fragTexCoord;
in vec3 fragPosition;
in vec4 fragColor;
out vec4 finalColor;

uniform sampler2D colorbuffer;
uniform sampler2D normalbuffer;
uniform sampler2D positionbuffer;
//uniform sampler2D depthbuffer;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec4 lightCol;
uniform float lightRadius;

void main()
{
    vec2 uv = (fragTexCoord.xy / fragTexCoord.z) * 0.5 + 0.5;
    
    vec3 color = texture(colorbuffer, uv).rgb;
    vec3 normal = texture(normalbuffer, uv).rgb * 2.0 - 1.0;
    vec3 position = texture(positionbuffer, uv).rgb;

    if (normal == vec3(0.0, 0.0, 0.0))
    { discard; }

    vec3 viewDir = normalize(viewPos - position);

    vec3 lRaw = lightPos - position;
    vec3 lightDir = normalize(lRaw);

    float NdL = max(dot(normal, lightDir), 0.0);

    float dist = length(lRaw);
    float atten = clamp(1.0 - dist*dist/(lightRadius*lightRadius), 0, 1);
    atten *= atten;
    NdL *= atten;

    vec3 h = normalize(lightDir + viewDir);
    float NdH = max(dot(normal, h), 0.0);
    float spec = pow(NdH, 64.0) * NdL;

    vec3 diff = color * NdL;

    finalColor.rgb = (diff + spec) * lightCol.rgb;
    //#ifdef BASE_PASS
    //    finalColor.rgb += color * 0.01 * fragColor.rgb;
    //#endif
    finalColor.a = 1.0;
    //finalColor = pow(finalColor, vec4(1.0/2.2));
}