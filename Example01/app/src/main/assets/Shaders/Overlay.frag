#version 300 es
// Input data
in vec2 f_UV;
// Output
out vec4 o_Color;
// Uniforms
uniform sampler2D u_Texture;
void main() {
	float depth = texture(u_Texture, f_UV).r;
	o_Color = vec4(depth, depth, depth, 1.0f);
}
