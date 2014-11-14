#version 330 

in vec3 vert;

out vec3 LightIntensity;

uniform mat4 mvpMatrix;
uniform mat3 mvpNormalMatrix;
uniform mat4 eyeMatrix;

uniform vec3 Kd;
uniform vec3 Ks;
uniform float Shininess;


void main() {
	vec4 lightPosition = vec4(0,50,-50,1);

	vec3 La = vec3(2, 2, 2);
	vec3 Ld = vec3(0.5, 0.5, 0.5);
	vec3 Ls = vec3(5, 5, 5);

	vec3 Ka = vec3(0.1, 0.1, 0.1);

	vec3 tnorm = normalize(mvpNormalMatrix * vert);

	vec4 eyeCoords = eyeMatrix * vec4(vert, 1.0);

	// PHONG GARBAGE
	vec3 s = normalize(vec3(lightPosition - eyeCoords));
	vec3 v = normalize(-eyeCoords.xyz);
	vec3 r = reflect(-s, tnorm);

	vec3 ambient = La * Ka;
	float sDotN = max(dot(s, tnorm), 0.0);

	vec3 diffuse = Ld * Kd * sDotN;
	vec3 spec = vec3(0.0);

	if (sDotN > 0.0)
		spec = Ls * Ks * pow(max(dot(r,v), 0.0), Shininess);
		LightIntensity = ambient + diffuse + spec;

	gl_Position = mvpMatrix * vec4(vert, 1.0);
}