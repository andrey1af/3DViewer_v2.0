#version 460

uniform int u_type;
uniform vec3 u_color;

out vec4 fragColor;

void main() {
    vec2 coord = gl_PointCoord * 2.0 - 1.0;
    if (u_type == 2) {
        if (dot(coord, coord) > 1.0) discard;
    }
    fragColor = vec4(u_color, 1.0);
}