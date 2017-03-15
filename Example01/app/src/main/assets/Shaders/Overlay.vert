#version 300 es
// Input vertex data
layout(location = 0) in vec4 i_Pos;
// Output
out vec2 f_UV;
// Uniforms
uniform vec2 u_ScreenSize;
void main() {
    // Transform the position
    gl_Position.x = (2.0f * (i_Pos.x / u_ScreenSize.x) - 1.0f);
    gl_Position.y = -(2.0f * (i_Pos.y / u_ScreenSize.y) - 1.0f);
    gl_Position.z = 0.0f;
    gl_Position.w = 1.0f;
    f_UV = i_Pos.zw;
}
