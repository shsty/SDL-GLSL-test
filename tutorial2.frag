#version 330
// It was expressed that some drivers required this next line to function properly
precision highp float;

in  vec2 ex_UV;

out vec4 gl_FragColor;

uniform vec4 tr;
uniform sampler2D tex;

float rad2(vec2 a){
    return a.x*a.x+a.y*a.y;
}

vec2 mul(vec2 a, vec2 b){
    return vec2( a.x*b.x-a.y*b.y , a.x*b.y+a.y*b.x );
}

vec2 div(vec2 a, vec2 b){
    return vec2( a.x*b.x+a.y*b.y , -a.x*b.y+a.y*b.x ) / (b.x*b.x+b.y*b.y);
}

vec2 conj(vec2 a){
    return vec2(a.x, -a.y);
}

vec2 transform(vec2 uv, vec4 ab){
    return div( mul(uv, ab.xy) + ab.zw, mul(uv, conj(ab.zw)) + conj(ab.xy) );
}

void main(void) {
    float r = rad2(ex_UV);
    if (r > 1.0) {
        r = exp(8 * (1-r));
        gl_FragColor = vec4(1.0, 1.0, 1.0, r);
        return;
    }

    vec2 UV = transform(ex_UV, tr) * 4;
    vec4 texcolor = texture(tex, vec2( (UV.s+1)/2, (1-UV.t)/2 ) );
    gl_FragColor = texcolor;
}
