#version 120
#extension GL_ARB_texture_rectangle: enable

#define PI (3.14159265358979323846)
#define TWO_PI (2*PI)

uniform int ksectors = 10;
uniform float kangleRad = 0.0;
uniform vec2 kcenter = vec2( 1280.0, 720.0 ) / 2.0;
uniform vec2 screenCenter = vec2( 1280.0, 720.0 ) / 2.0;
uniform sampler2DRect inputTexture;

void main(){
    vec2 texCoord = gl_TexCoord[0].xy;
    vec2 v = texCoord - screenCenter;
    float r = length( v );
    float a = atan( v.y, v.x );
    
    float A = TWO_PI / float(ksectors);
    a = mod( a, A );
    if ( a > A/2.0 ) a = A - a;
    a -= kangleRad;
    
    vec2 u = vec2( cos(a), sin(a) ) * r;
    u += kcenter;
    gl_FragColor = texture2DRect( inputTexture, u );
}



/*
#version 120
//enables working with textures of arbitrary size(not only powers of two).
#extension GL_ARB_texture_rectangle: enable

#define PI (3.14159265358979323846)
#define TWO_PI (2*PI)

//input parameters of the shader.
uniform int ksectors = 10;
uniform float kangleRad = 0.0;
uniform vec2 kcenter = vec2(1280.0,720.0) /2.0;
uniform vec2 screenCenter = vec2( 1280.0, 720.0 ) / 2.0;
uniform sampler2Drect inputTexture;

void main(){

    //get the default texture coordinates from gl_texcoord[0] and the xy/st components and assign to texCoord.
    vec2 texCoord = gl_TexCoord[0].st;
    //distance between current pixel coord and screen center.
    vec2 v = texCoord - screenCenter;
    float r = length(v);
    //returns angle between the X-axis and v.
    float a = atan(v.y, v.x);

    //angular size of A in radians.
    float A = TWO_PI / float(ksectors);
    //wraps the angle a to a range from 0 to A.
    a = mod(a,A);

    //if angle go over half circle repeat make symetrical pattern.
    if ( a > A/2.0) a = A - a;

    //shifts a by a number of radians to rotate the sector grabbed from the input image.
    a -= kangleRad;

 
    vec2 u = vec2(cos(a), sin(a)) *r;
    u+=kcenter;
    gl_FragColor = texture2Drect(inputTexture,u);
}
*/