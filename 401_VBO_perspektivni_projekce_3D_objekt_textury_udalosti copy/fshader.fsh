#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
varying vec2 v_texcoord;

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

varying vec3 normal;
varying vec3 lightDir;
varying vec3 halfVector;

varying vec4 vertDiffuse;
varying vec4 vertAmbient;
varying float dist;

void main(void) {
    // znovu normalizujeme normalu
    vec3 N = normalize(normal);

    // spocitame uhel mezi normalou a paprskem svetla
    float NdotL = max(dot(N, normalize(lightDir)), 0.0);

    // vytvorime pomocnou cernou barvu
    vec4 color = vec4(0.0);

    // pokud normala ukazuje ke svetlu..
    if (NdotL > 0.0)
    {
        // pokud je vrchol v ramci svetelneho kuzelu...
        // tj. jestli uhel mezi smerem svetla a otocenim kuzelu svetla je mensi, nez cut_off uhel...
        // cos na intervalu 0..180 klesa, proto <
        float spotEffect = dot(normalize(light0.spot_direction), normalize(-lightDir));
        if (spotEffect > cos(light0.spot_cut_off)){

            // pridame k barve difuzni slozku
            color += vertDiffuse * NdotL;

            // znovu normalizujeme half vector
            vec3 HV = normalize(halfVector);
            // pouzijeme Blinn-Phonguv model
            // spocitame uhel mezi normalou a vektorem pulici uhel mezi svetlem a pozorovatelem
            float NdotHV = max(dot(N, HV), 0.0);
            // spocitame odleskovou slozku
            color += material0.specular * light0.specular * pow(NdotHV, material0.shininess);

            // umocnime spot effect, jedna se o cislo mensi, nez 0, takze se umozneni zmensuje
            spotEffect = pow(spotEffect, light0.spot_exponent);

            // spocitame utlum
            float attenuation = spotEffect / (light0.constant_attenuation +
                                              light0.linear_attenuation * dist +
                                              light0.quadratic_attenuation * dist * dist);
            color = color * attenuation;
        }
    }

    // vyslednou barvu vynasobime utlumem a pricteme ambientni slozku
    gl_FragColor = (color + vertAmbient);
}



/*void main()
{
    // vyber barvu pixelu z textury
    vec4 pom = texture2D(texture, v_texcoord);
    gl_FragColor = pom;
}*/
