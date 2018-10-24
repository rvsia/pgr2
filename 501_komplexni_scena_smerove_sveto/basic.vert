#version 120

uniform highp mat4 projection_matrix;
uniform highp mat4 modelview_matrix;

struct material{
    lowp vec4 ambient;
    lowp vec4 diffuse;
    lowp vec4 specular;
    lowp vec4 emissive;
    lowp float shininess;
};

struct light {
    lowp vec4 position;
    lowp vec4 diffuse;
    lowp vec4 specular;
    lowp vec4 ambient;
};

uniform material material0;
uniform light light0;

attribute highp vec3 a_Vertex;
attribute highp vec3 a_Normal;

varying highp vec4 color;

// Zakladni pojmy:
// Dot product - skalani soucin - vysledek je v zasade uhel mezi vektory viz dole
// A+B - soucet vektoru, vysledek je kolmice v doplnenem kosodelniku
// Vektorovy soucin - vysledkem je vektor, ktery je kolmici roviny ve ktere se vektory nachazi
// Ortogonalni matice - symetricka podel osy

void main(void)
{
    // -----------------------------------
    // tranformuji pozici vrcholu z "object space" do "eye space"
    vec4 pos = modelview_matrix * vec4(a_Vertex, 1.0);

    // -----------------------------------
    // musim prevest take normaly podle transformacni matice,
    // nicmene normaly potrebuji specialni matici 3x3, ne 4x4, proto ji musime odvodit
    // vypocet normalove matice
    mat3 normal_matrix = mat3(modelview_matrix);

    // otocim normalu a normalizuji
    vec3 N = normalize(normal_matrix * a_Normal);

    // -----------------------------------
    // spocitam ambientni slozku svetla
    vec4 finalColor = material0.ambient * light0.ambient;

    // -----------------------------------
    // otocime svetlo take do "eye space" - position je direction
    //vec3 L = normalize(modelview_matrix * light0.position).xyz;
    vec3 L = normalize(light0.position).xyz;

    // spocitam uhel mezi paprskem a normalou plochy
    // jedna se o skalarni soucin, kde n.l = |n|.|l|.cos(alfa), kde
    // alfa je uhel, ktery sviraji
    // cos ma maximum v 0 a 180
    float NdotL = max(dot(N, L), 0.0);

    // jestli je uhel kladny, je strana privracena
    if (NdotL > 0.0)
    {
        //----------------------------------
        // pridam difuzni slozku (Lambertian Reflection)
        finalColor += material0.diffuse * light0.diffuse * NdotL;

        // stanovim pozici pozorovatele (jsme v "eye space", proto je pozice kamery inverzni k atualnimu stredu)
        vec3 V = -pos.xyz;

        // spocitam odrazeny paprsek
        // v zasade bych mel paprsek odecitat, ale protoze ma opacnou
        // orientaci je nutne je pricist
        vec3 R = N+L;

        // spocitam uhel mezi okem a idealne odrazenym paprskem
        float VdotR = max(dot(V, R), 0.0);

        // odrazova slozka pomoci Blinn-Phong
        finalColor += material0.specular * light0.specular * pow(VdotR, material0.shininess);
    }

    color = finalColor;
    gl_Position = projection_matrix * pos;
}
