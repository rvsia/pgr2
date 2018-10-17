#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 matrix;

attribute vec4 a_position;
attribute vec2 a_texcoord;
varying vec2 v_texcoord;

void main()
{
    // otoceni pozic vrcholu pomoci transformacni matice
    gl_Position = matrix * a_position;

    // posleme texturovaci souradnice do fragment shaderu,
    // hodnoty se automaticky interpoluji
    v_texcoord = a_texcoord;
}
