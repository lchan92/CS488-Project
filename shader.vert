#version 330 


in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTexCoord;


uniform mat4 M;
uniform mat4 MV;
uniform mat3 N;
uniform mat4 MVP;

uniform bool drawSkyBox;
uniform vec3 worldCameraPos;


out vec3 normal;
out vec3 position;
out vec2 texCoord;
out vec3 reflectDir;



void main()
{
    normal = normalize(N * vertexNormal);
    position = vec3(MV * vec4(vertexPosition, 1.0));
    texCoord = vertexTexCoord;

    if (drawSkyBox) {
    	reflectDir = vertexPosition;
    } else {
    	vec3 worldPos = vec3(M * vec4(vertexPosition, 1.0));
    	vec3 worldNorm = vec3(M * vec4(vertexNormal, 0.0));
        //vec3 worldView = normalize(worldCameraPos - worldPos);
    	vec3 worldView = normalize(vec3(0.0) - worldPos);

    	reflectDir = reflect(-worldView, worldNorm);
    }

    gl_Position = MVP * vec4(vertexPosition, 1.0);
}