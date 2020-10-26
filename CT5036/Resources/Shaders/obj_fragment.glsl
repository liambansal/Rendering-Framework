#version 460
smooth in vec4 vertexColour;
smooth in vec4 vertexNormal;
out vec4 outputColour;

void main()
{
	vec4 lightDirection = normalize(vec4(0.f) - vec4(10.f, 8.f, 10.f, 0.f));
	float negativeLightDirection = max(0.f, dot(normalize(vertexNormal), -lightDirection));
	vec4 litColour = vec4(vertexColour.xyz * negativeLightDirection, 1.0);
	outputColour = vertexColour + litColour;
}