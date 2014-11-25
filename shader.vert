#version 330 


in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTexCoord;



uniform mat4 MV;
uniform mat3 N;
uniform mat4 MVP;



out vec3 normal;
out vec3 position;
out vec2 texCoord;



void main()
{
    normal = normalize(N * vertexNormal);
    position = vec3(MV * vec4(vertexPosition, 1.0));
    texCoord = vertexTexCoord;

    gl_Position = MVP * vec4(vertexPosition, 1.0);
}