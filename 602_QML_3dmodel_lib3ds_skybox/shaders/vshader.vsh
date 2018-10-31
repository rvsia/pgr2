#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif


uniform mat4 matrix;
attribute vec4 a_position;
varying vec3 texcoords;

void main()
{
    // posleme texturovaci souradnice do fragment shaderu,
    // hodnoty se automaticky interpoluji
    texcoords = a_position.xyz;

    // otoceni pozic vrcholu pomoci transformacni matice
    gl_Position = matrix * a_position;
}
