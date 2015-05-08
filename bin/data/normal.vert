#version 110

varying vec3 light;
varying vec3 halfvector;
varying vec4 diffuse;
varying vec4 ambient;
varying vec4 specular;
varying vec4 globalambient;
varying vec4 vPosition;
varying float distance;

void main() {

    // get vertex vs light
    vec4 vertex = gl_ModelViewMatrix * gl_Vertex;
    vPosition = vertex;
    vec3 aux = vec3(gl_LightSource[0].position - vertex);
    // normalize light direction
    light = normalize(aux);
    // get distance to vertex
    distance = length(aux);
    // normalize the halfvector
    // halfvector = normalize(gl_LightSource[0].halfVector.xyz);
    halfvector = normalize(normalize(gl_LightSource[0].halfVector.xyz) + vec3(0, 0, 1));
    // get diffuse, ambient & global ambient component
    specular = gl_FrontMaterial.specular * gl_LightSource[0].specular;
    diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
    ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
    globalambient = gl_LightModel.ambient * gl_FrontMaterial.ambient;
    // texture coordinates
    gl_TexCoord[0] = gl_MultiTexCoord0;
    // vertex color
    gl_FrontColor = gl_Color;
    // transform matrix
    gl_Position = ftransform();

}
