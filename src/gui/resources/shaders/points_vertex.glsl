#version 460
layout(location = 0) in vec3 aPos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform float u_size;
uniform int u_projection_type;

void main() {
    gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);
    gl_PointSize = u_size;
}