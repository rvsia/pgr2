#version 100

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

attribute mediump vec4 a_position;
attribute mediump vec2 a_texcoord;
uniform mediump mat4 matrix;
varying mediump vec2 v_texcoord;

void main(void)
{
    gl_Position = matrix * a_position;
    v_texcoord = a_texcoord;
}
