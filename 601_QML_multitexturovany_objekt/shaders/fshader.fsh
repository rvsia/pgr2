#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D primaryTexture;
uniform sampler2D secondaryTexture;

varying vec2 v_texcoord;
varying vec2 v_secondTexcoord;

void main()
{

    /*vec4 primaryColor = texture2D(primaryTexture, v_texcoord);
    vec4 secondaryColor = texture2D(secondaryTexture, v_secondTexcoord);

    // vyber barvu pixelu z textury
    //gl_FragColor = vec4(mix(primaryColor.rgb,secondaryColor.rgb, 0.5), 1.0) ;
    gl_FragColor = vec4((secondaryColor.rgb*primaryColor.rgb), 1.0);
    */

    vec4 primaryColor = texture2D(primaryTexture, v_texcoord);
    vec4 secondaryColor = texture2D(secondaryTexture, v_secondTexcoord);

    if (secondaryColor.a == 0.0){
        gl_FragColor = vec4(primaryColor.rgb, 1.0);
    } else {
        gl_FragColor = vec4(secondaryColor.rgb, 1.0);
    }
}
