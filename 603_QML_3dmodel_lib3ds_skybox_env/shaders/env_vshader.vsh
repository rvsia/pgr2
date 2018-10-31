#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;

attribute vec4 a_Vertex;
attribute vec4 a_Normal;

varying vec3 reflected;

void main()
{
    vec4 eyePosition = a_Vertex;
    vec4 eyeNormal = normalize(modelview_matrix * a_Normal);

    reflected = reflect(eyePosition.xyz, eyeNormal.xyz);

    mat4 matrix = projection_matrix * modelview_matrix;
    gl_Position = matrix * a_Vertex;
}

