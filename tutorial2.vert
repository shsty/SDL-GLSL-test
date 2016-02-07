#version 330 core
// in_Position was bound to attribute index 0 and in_Color was bound to attribute index 1
in  vec2 in_Position;
in  vec3 in_Color;

// We output the ex_Color variable to the next shader in the chain
out vec2 ex_UV;

void main(void) {

    gl_Position = vec4(in_Position.x, in_Position.y, 0.0, 1.0);
    ex_UV = in_Position;

}
