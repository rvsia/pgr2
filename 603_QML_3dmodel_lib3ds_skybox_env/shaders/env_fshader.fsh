#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

varying vec3 reflected;

uniform samplerCube primaryTexture;
uniform mat4 modelViewMatrix;

void main()
{
    gl_FragColor = textureCube(primaryTexture, reflected);
}
