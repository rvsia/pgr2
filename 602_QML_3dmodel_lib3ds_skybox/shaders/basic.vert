#version 120

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;

struct material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 emissive;
    float shininess;
};

struct light {
    vec4 position;
    vec4 diffuse;
    vec4 specular;
    vec4 ambient;

    float constant_attenuation;
    float linear_attenuation;
    float quadratic_attenuation;

    float spot_cut_off;
    vec3 spot_direction;
    float spot_exponent;
};

uniform light light0;
uniform material material0;

attribute vec3 a_Vertex;
attribute vec3 a_Normal;

varying vec3 normal;
varying vec3 lightDir;
varying vec3 halfVector;

varying vec4 vertDiffuse;
varying vec4 vertAmbient;
varying float dist;

void main(void)
{
    // normal matrix je model_view bez translace
    mat3x3 normal_matrix = mat3x3(modelview_matrix);

    // otocime normalu podle modelview matice
    normal = normalize(normal_matrix * a_Normal);
    // otocime vrchol podle modelview matice
    vec4 pos = modelview_matrix * vec4(a_Vertex, 1.0);

    // otocime svetlo ?
    //vec3 lightPos = (modelview_matrix * light0.position).xyz;
    vec3 lightPos = (light0.position).xyz;

    // spocitame pozici pozorovatele
    vec3 observer = -(pos.xyz);

    // spocitame vektor mezi pozici svetla a vrcholem
    lightDir = lightPos - pos.xyz;
    // spocteme vzdalenost mezi svetlem a vrcholem
    dist = length(lightDir);
    // a normalizujeme ho (pozor provest az po vypocteni vzdalenosti)
    lightDir = normalize(lightDir);

    // vektor pulici uhel mezi svetlem a pozorovatelem
    halfVector = normalize(lightPos + observer);

    // vynasobene komponenety ambientni a difuzni slozky svetla
    vertDiffuse = material0.diffuse * light0.diffuse;
    vertAmbient = material0.ambient * light0.ambient;

    // provedeme projekci vrcholu
    gl_Position = projection_matrix * pos;
}
