#version 460
layout(lines) in;
layout(triangle_strip, max_vertices = 4) out;

uniform vec2 u_resolution;
uniform float u_thickness;

out float v_dash_coord;

void main() {
    vec2 p0 = (gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w);
    vec2 p1 = (gl_in[1].gl_Position.xy / gl_in[1].gl_Position.w);

    p0 = (p0 * 0.5 + 0.5) * u_resolution;
    p1 = (p1 * 0.5 + 0.5) * u_resolution;

    vec2 dir = normalize(p1 - p0);
    vec2 normal = vec2(-dir.y, dir.x);
    vec2 offset = (u_thickness * 0.5) * normal;
    float len = length(p1 - p0);

    vec2 verts[4] = {
        p0 + offset,
        p0 - offset,
        p1 + offset,
        p1 - offset
    };

    float dash_coords[4] = float[4](0.0, 0.0, len, len);

    for (int i = 0; i < 4; ++i) {
        vec2 ndc = (verts[i] / u_resolution) * 2.0 - 1.0;
        gl_Position = vec4(ndc, 0.0, 1.0);
        v_dash_coord = dash_coords[i];
        EmitVertex();
    }
    EndPrimitive();
}