#version 300 es
// Input vertex data
layout(location = 0) in vec3 i_Pos;
// Uniforms
uniform mat4 u_LightMVP;
void main() {
    gl_Position = u_LightMVP * vec4(i_Pos, 1.0f);
}
