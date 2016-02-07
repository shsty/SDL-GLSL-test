#version 330 core
// It was expressed that some drivers required this next line to function properly
precision highp float;

in  vec2 ex_UV;
out vec4 gl_FragColor;

uniform sampler2D tex;

void main(void) {
    vec2 UV = ex_UV * 4;
    gl_FragColor = texture(tex, vec2( (UV.s+1)/2, (1-UV.t)/2 ));
}
