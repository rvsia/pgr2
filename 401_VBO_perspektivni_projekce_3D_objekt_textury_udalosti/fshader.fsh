#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
varying vec2 v_texcoord;

void main()
{
    // vyber barvu pixelu z textury
    vec4 pom = texture2D(texture, v_texcoord);
    pom.g = 0.0;
    pom.b = 0.0;
    gl_FragColor = pom;
}
