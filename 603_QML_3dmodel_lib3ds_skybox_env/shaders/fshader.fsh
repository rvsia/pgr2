#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform samplerCube primaryTexture;
varying vec3 texcoords;

void main()
{
    gl_FragColor = textureCube(primaryTexture, texcoords);
}
