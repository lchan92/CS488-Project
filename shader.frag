#version 330


in vec3 normal;
in vec3 position;
in vec2 texCoord;


const int NUM_LIGHTS = 2;
uniform vec4 lightPositions[NUM_LIGHTS];
uniform vec3 lightColours[NUM_LIGHTS];
uniform vec3 lightFalloffs[NUM_LIGHTS];


uniform float Shininess;


uniform sampler2D tex0;
uniform sampler2D tex1;


out vec4 fragColor;


void phongModel(const in vec3 norm, const in vec4 texColour) {
	fragColor = vec4(0.2, 0.2, 0.2, 1) * texColour; // ambient

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

			fragColor = fragColor + vec4(colour, 1.0);
			fragColor = clamp(fragColor, vec4(0,0,0,1), vec4(1,1,1,1));
	    }
	}
	

}

void main() {
	// TEXTURES
	vec4 texColour1 = texture(tex0, texCoord);
	vec4 texColour2 = texture(tex1, texCoord);
	vec4 texColour = mix(texColour1, texColour2, 0.2);

	phongModel(normalize(normal), texColour);
}
