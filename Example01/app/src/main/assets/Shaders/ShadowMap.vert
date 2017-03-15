#version 300 es
// Input vertex data
layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec3 i_Normal;

// Output
out vec3 f_PosWS;
out vec3 f_NormalWS;
out vec4 f_ShadowUV;

// Uniforms
uniform mat4 u_M;
uniform mat4 u_MVP;
uniform mat3 u_Normal;
uniform mat4 u_BiasedLightMVP;

void main() {
	gl_Position = u_MVP * vec4(i_Pos, 1.0);
	f_PosWS     = (u_M * vec4(i_Pos, 1.0)).xyz;
	f_NormalWS  = u_Normal * i_Normal;
	f_ShadowUV  = u_BiasedLightMVP * vec4(i_Pos, 1.0);
}
