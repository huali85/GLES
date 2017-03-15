#version 300 es
// Input
in vec3 f_PosWS;
in vec3 f_NormalWS;
in vec4 f_ShadowUV;

// Output
out vec4 o_Color;

// Uniforms
uniform sampler2DShadow u_ShadowMap;
uniform vec3 u_Ambient;
uniform vec3 u_Diffuse;
uniform vec4 u_Specular;
uniform vec3 u_EyePosWS;
uniform vec3 u_LightPosWS;

//#define EPSILON 0.00001

float CalcShadowFactor() {
    // 2x2 PCF
    float sum = 0.0;
    sum += textureProjOffset(u_ShadowMap, f_ShadowUV, ivec2(-1, -1));
    sum += textureProjOffset(u_ShadowMap, f_ShadowUV, ivec2(-1,  1));
    sum += textureProjOffset(u_ShadowMap, f_ShadowUV, ivec2( 1,  1));
    sum += textureProjOffset(u_ShadowMap, f_ShadowUV, ivec2( 1, -1));
    sum *= 0.25;
    if (f_ShadowUV.z / f_ShadowUV.w > 1.0)
        sum = 0.0f;
    return sum;
}

void main() {
    vec3 normalWS   = normalize(f_NormalWS);
    vec3 lightDirWS = normalize(u_LightPosWS - f_PosWS);
    vec3 viewDirWS  = normalize(u_EyePosWS - f_PosWS);
    vec3 halfWS     = normalize(lightDirWS + viewDirWS);
    // Compute the lighting in eye-space
    float diffuse  = max(0.0, dot(normalWS, lightDirWS));
    float specular = pow(max(0.0, dot(normalWS, halfWS)), u_Specular.a);
    // Combine lighting with material properties
	vec3 diffuseColor = u_Diffuse * diffuse;
	vec3 specularColor = u_Specular.rgb * specular;
	vec3 finalColor = u_Ambient + (diffuseColor + specularColor) * CalcShadowFactor();
	//vec3 finalColor = u_Ambient + diffuseColor + specularColor;
    o_Color = vec4(finalColor, 1.0);
}
