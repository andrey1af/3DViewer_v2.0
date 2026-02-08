#version 460
in float v_dash_coord;
out vec4 fragColor;

uniform float u_dash_length;
uniform float u_gap_length;
uniform int u_dotted;
uniform vec3 u_color;

void main() {
    if (u_dotted == 1) {
        float total = u_dash_length + u_gap_length;
        float local = mod(v_dash_coord, total);
        if (local > u_dash_length) {
            discard;
        }
    }

    fragColor = vec4(u_color, 1.0);
}
