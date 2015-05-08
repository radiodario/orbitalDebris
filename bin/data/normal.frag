#version 110

varying vec3 light;
varying vec3 halfvector;
varying vec4 diffuse;
varying vec4 ambient;
varying vec4 specular;
varying vec4 globalambient;
varying vec4 vPosition;
varying float distance;

uniform sampler2D textureMap;
uniform sampler2D normalMap;
uniform sampler2D glossMap;

uniform float alpha;

void main() {

    // texture lookup
    float x = gl_TexCoord[0].s;
    float y = gl_TexCoord[0].t;
    // extract the normal from the normal map
    vec3 norm = normalize(texture2D(normalMap, vec2(x, y)).rgb * 2. - 1.);
    // ambient component is always present
    vec4 col = globalambient;
    // compute dot product between normal and normalized light
    float normDotLight = max(dot(norm, normalize(light)), 0.);

    // if dot product > 0
    if(normDotLight > 0.) {

        // calculate attenuation
        float attenuation = 1. / (gl_LightSource[0].constantAttenuation + gl_LightSource[0].linearAttenuation * distance + gl_LightSource[0].quadraticAttenuation * distance * distance);

        // add diffuse component
        col += attenuation * (diffuse * normDotLight + ambient);
        // normalize half vector
        vec3 HV = halfvector;

        // vec3 eyeDirection = normalize(vPosition.xyz);
        // // compute dot product
        vec3 reflectionDirection = normalize(reflect(-light, norm));

        float normDotHV = max(dot(reflectionDirection, HV), 0.);
        // float normDotHV = max(dot(norm, HV), 0.);
        // float normDotHV = max(dot(reflectionDirection, HV), 0.);
        // // add specular component

        float shininess = gl_FrontMaterial.shininess;

        col += attenuation * specular * pow(normDotHV, shininess) * texture2D(glossMap, vec2(x, y));


    }

    // set color
    //vec4 bump = col * vec4(.5, .5, .5, 1.0);
    vec4 bump = col * vec4(texture2D(textureMap, vec2(x, y)).rgb, 1.0);
   //vec4(texture2D(diffuseMap, vec2(x, y)).rgb, 1.0);
    //vec4 diff = vec4(0., 0., 0., 0.);

    // normal
    gl_FragColor = bump;


    // gl_FragColor = vec4(texture2D(colorMap, vec2(x, y)).rgb, 1.0);

    // additive blending
    // gl_FragColor = mix(  vec4((gl_Color).rgb, 1.0), vec4((bump + diff).rgb, 1.0), 0.8);

    // screen blending
    //gl_FragColor = mix(gl_Color, vec4((bump + diff) - (bump * diff), alpha), 0.8);

}
