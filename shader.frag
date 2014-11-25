#version 330


in vec3 normal;
in vec3 position;
in vec2 texCoord;


uniform vec3 Kd;
uniform vec3 Ks;
uniform float Shininess;


uniform sampler2D tex0;
uniform sampler2D tex1;


out vec4 fragColor;



void adsModel(const in vec3 norm,
				out vec3 ambAndDiff, out vec3 spec) {
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

	//return lightIntensity * (Ka + Kd * diffuseIntensity + Ks * specularIntensity);
	//return La*Ka + Ld*Kd*diffuseIntensity + Ls*Ks*specularIntensity;
	ambAndDiff = La*Ka + Ld*Kd*diffuseIntensity;
	spec = Ls*Ks*specularIntensity;
}


void main() {
	// PHONG
	vec3 ambAndDiff, spec;
	adsModel(normalize(normal), ambAndDiff, spec);

	// TEXTURES
	vec4 texColour1 = texture(tex0, texCoord);
	vec4 texColour2 = texture(tex1, texCoord);
	//vec4 texColour = mix(texColour1, texColour2, 0.5);

	fragColor = vec4(ambAndDiff, 1.0) * texColour1 + vec4(spec, 1.0);
}
