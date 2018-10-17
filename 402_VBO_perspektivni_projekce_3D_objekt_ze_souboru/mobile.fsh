#version 100

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
varying mediump vec2 v_texcoord;

void main(void)
{
    gl_FragColor = texture2D(texture, v_texcoord);
}
