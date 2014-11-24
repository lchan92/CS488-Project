#version 330


in vec3 normal;
in vec3 position;


uniform vec3 Kd;
uniform vec3 Ks;
uniform float Shininess;


out vec4 fragColor;



vec3 adsModel(const in vec3 norm) {
	vec4 lightPosition = vec4(0,50,-50,1);

	vec3 La = vec3(2, 2, 2);
	vec3 Ld = vec3(0.5, 0.5, 0.5);
	vec3 Ls = vec3(5, 5, 5);

	vec3 Ka = vec3(0.1, 0.1, 0.1);
	vec3 lightIntensity = vec3(2, 0.5, 5);



	vec3 s = normalize(lightPosition.xyz - position);
	vec3 v = normalize(-position.xyz);
	vec3 r = reflect(-s, norm);

	vec3 diffuseIntensity = vec3(max(dot(s, norm), 0.0));

	vec3 specularIntensity = vec3(0.0);
    if (dot(s, norm) > 0.0)
        specularIntensity = vec3(pow(max(dot(r, v), 0.0), Shininess));

	return lightIntensity * (Ka + Kd * diffuseIntensity + Ks * specularIntensity);
	//return La*Ka + Ld*Kd*diffuseIntensity + Ls*Ks*specularIntensity;
}


void main() {
	fragColor = vec4(adsModel(normalize(normal)), 1.0);
	//fragColor = vec4(normal, 1.0);
}
