#version 330 


in vec3 vertexPosition;
in vec3 vertexNormal;



uniform mat4 MV;
uniform mat3 N;
uniform mat4 MVP;



out vec3 normal;
out vec3 position;



void main()
{
    // Transform to eye coordinates
    normal = normalize(N * vertexNormal);
    position = vec3(MV * vec4(vertexPosition, 1.0));
    //normal = normalize(vertexNormal);

    gl_Position = MVP * vec4(vertexPosition, 1.0);
}