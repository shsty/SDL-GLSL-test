#version 400
// It was expressed that some drivers required this next line to function properly
precision highp float;

in  vec2 ex_UV;

out vec4 gl_FragColor;

uniform vec4 tr;
uniform sampler2D tex;

const float pi = 3.141592654;
const float ra = sqrt(sqrt(2.0)+1.0);

float rad2(vec2 a){
    return a.x*a.x+a.y*a.y;
}

float arg(vec2 a){
    return atan(a.y, a.x);
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

vec2 reduce(vec2 uv){
    vec2 c = uv;
    while ( (abs(arg(c)) > pi/4) || ( rad2(c) -2*ra*c.x + 1 < 0 ) ){
        while (abs(arg(c)) > pi/4) c = mul(c, vec2(0.0, 1.0));
        while ( rad2(c) -2*ra*c.x + 1 < 0 )
            c = mul(transform(conj(c), vec4(0.0, ra, 0.0, -1.0)), vec2(1.0,1.0)/sqrt(2.0));
    }
    if (c.y < 0.0) c = conj(c);
    if ( rad2(c) - 2 * mul(c, vec2(1.0, -1.0)/sqrt(2)*ra ).x + 1 < 0 ){
        vec2 t = vec2(ra, 1/ra) * (1+sqrt(2)-sqrt(3)) /2 /sqrt(2);
        c = transform(c, vec4(1.0, 0.0, -t));
        c = mul(c, vec2(-1, sqrt(3))/2);
        c = transform(c, vec4(1.0, 0.0, t));
    }
    return c;
}

void main(void) {
    float r = rad2(ex_UV);
    if (r > 1.0) {
        r = exp(8 * (1-r));
        gl_FragColor = vec4(1.0, 1.0, 1.0, r);
        return;
    }

    vec2 UV = transform(ex_UV, tr);
    UV = reduce(UV);
    //gl_FragColor = vec4(textureQueryLod(tex, vec2( (UV.s+1)/2, (1-UV.t)/2 ) )/4, 0.0, 1.0);return;
    vec4 texcolor = textureLod(tex, vec2( (UV.s+1)/2, (1-UV.t)/2 ) , -log2(1-rad2(ex_UV)));
    //vec4 texcolor = vec4(rad2(c) - 2 * mul(c, vec2(1.0, -1.0)/sqrt(2)*ra ).x + 1, 0.0, 0.0, 1.0);
    gl_FragColor = texcolor;
}
