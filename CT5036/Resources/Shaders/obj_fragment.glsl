#version 460

smooth in vec4 vertexPosition;
smooth in vec4 vertexNormal;
smooth in vec2 vertexUV;

out vec4 outputColour;

uniform vec4 cameraPosition;
uniform vec4 kA;
uniform vec4 kD;
uniform vec4 kS;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

vec3 iA = vec3(0.25f, 0.25f, 0.25f);
vec3 iD = vec3(1.f, 1.f, 1.f);
vec3 iS = vec3(1.f, 1.f, 1.f);

vec4 lightDirection = normalize(vec4(0.f) - vec4(10.f, 8.f, 10.f, 0.f));

void main()
{
	// Get texture data from UV coordinates.
	vec4 textureData = texture(normalTexture, vertexUV);
	vec4 diffuseData = texture(diffuseTexture, vertexUV);
	vec3 ambientLight = kA.xyz * iA;
	
	float negativeLightDirection = max(0.f, dot(normalize(vertexNormal), -lightDirection));
	vec3 diffuse = kD.xyz * iD * negativeLightDirection * textureData.rgb * diffuseData.rgb;

	// Reflected light vector.
	vec3 R = reflect(lightDirection, normalize(vertexNormal)).xyz;
	// Surface to eye vector.
	vec3 E = normalize(cameraPosition - vertexPosition).xyz;

	float specularTerm = pow(max(0.f, dot(E, R)), kS.a);
	vec3 specular = kS.xyz * iS * specularTerm;

	outputColour = vec4(ambientLight + diffuse + specular, 1.f);
}