#version 330


in vec3 normal;
in vec3 position;
in vec2 texCoord;
in vec3 reflectDir;


const int NUM_LIGHTS = 3;
uniform vec4 lightPositions[NUM_LIGHTS];
uniform vec3 lightColours[NUM_LIGHTS];
uniform vec3 lightFalloffs[NUM_LIGHTS];


//uniform float Shininess;

uniform samplerCube cubeMapTex;
uniform sampler2D tex1;
uniform sampler2D tex2;

uniform bool drawSkyBox;
uniform bool drawReflection;
uniform float reflectFactor;
uniform vec4 materialColour;

out vec4 fragColour;


void phongModel(const in vec3 norm, const in vec4 texColour) {
	fragColour = vec4(0.5, 0.5, 0.25, 1) * texColour; // ambient

	vec3 v = normalize(-position.xyz);

	for (int i = 0; i < NUM_LIGHTS; i++) {
		vec4 lightPosition = lightPositions[i];
		vec3 lightColour = lightColours[i];
		vec3 lightFalloff = lightFalloffs[i];

		vec3 s = normalize(lightPosition.xyz - position);
		vec3 r = reflect(-s, norm);

		vec3 specularIntensity = vec3(0.0);
		float lightDotNormal = dot(s, norm);

	    if (lightDotNormal > 0.005 || lightDotNormal < -0.005) {
	        specularIntensity = vec3(pow(max(dot(r, v), 0.0), 25));

		    vec3 specAndDiff = texColour.xyz + texColour.xyz * specularIntensity;
		    
		    // calculate attenuation
		    float sLength = length(s);
			float attenuation = 1/(lightFalloff.x + 
									lightFalloff.y * sLength + 
									lightFalloff.z * sLength * sLength);
			vec3 lightAttenuation = lightColour * attenuation;

			vec3 colour = specAndDiff * lightDotNormal * lightAttenuation;
			colour = clamp(colour, vec3(0,0,0), vec3(1,1,1));

			fragColour = fragColour + vec4(colour, 1.0);
			fragColour = clamp(fragColour, vec4(0,0,0,1), vec4(1,1,1,1));
	    }
	}
	

}

void main() {
	// CUBEMAP
	vec4 cubeMapColour = texture(cubeMapTex, reflectDir);

	if (drawSkyBox)
		//fragColour = vec4(reflectDir, 1);
		fragColour = cubeMapColour;
	else {
		// TEXTURES
		vec4 texColour1 = texture(tex1, texCoord);
		vec4 texColour2 = texture(tex2, texCoord);
		vec4 texColour = mix(texColour1, texColour2, 0.7);

		phongModel(normalize(normal), texColour);
		//fragColour = mix(fragColour, cubeMapColour, 0.2);

		if (drawReflection) {
			fragColour = vec4(fragColour.xyz, 0.3);
		}
	}
}
