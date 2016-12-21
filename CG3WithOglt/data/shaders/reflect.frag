#version 330

smooth in vec2 vTexCoord;
smooth in vec3 vNormal;
smooth in vec4 vEyeSpacePos;
smooth in vec3 vWorldPos;
in vec3 WorldNormal;

out vec4 outputColor;

uniform sampler2D gSampler;
uniform vec4 vColor;

#include "dirLight.frag"

uniform DirectionalLight sunLight;


uniform samplerCube envirMap;//Cube Map #akira
uniform vec3 CameraPos;//Camera position #akira

void main()
{
	//###Region akira
	vec3 ViewVector=normalize (vWorldPos-CameraPos);
	vec3 unitNormal=normalize(vNormal);

	//vec3 Reflect_Vector= reflect(ViewVector,unitNormal);
	//vec4 Reflect_Color=texture(envirMap, Reflect_Vector);
	vec3 Refract_Vector= refract(ViewVector,WorldNormal,1.0/1.33);
	vec4 Refract_Color=texture(envirMap,Refract_Vector);
	//####Region akira


	vec3 vNormalized = normalize(vNormal);
	
	vec4 vTexColor = texture2D(gSampler, vTexCoord);

	vec4 vMixedColor = vTexColor;
  	vec4 vDirLightColor = GetDirectionalLightColor(sunLight, vNormalized);
	outputColor = mix(vMixedColor,Refract_Color, 0.2);//mix color by 1:1 You can change 
													//transparency by change the float value 
}