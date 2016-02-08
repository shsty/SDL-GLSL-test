#version 330 core
// It was expressed that some drivers required this next line to function properly
precision highp float;

in  vec2 ex_UV;
out vec4 gl_FragColor;

uniform sampler2D tex;

void main(void) {
    vec2 UV = ex_UV * 4;
    vec4 texcolor = texture(tex, vec2( (UV.s+1)/2, (1-UV.t)/2 ) );
    gl_FragColor = texcolor;
//    gl_FragColor = vec4(texcolor.a, 0.0, 0.0, 1.0);
}
