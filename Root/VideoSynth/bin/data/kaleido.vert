#version 120
void main() {
//Get the vertex and multiply by the modelviewProjection matrix to get the position that goes to the frag shader.
gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
gl_TexCoord[0] = gl_MultiTexCoord0;
gl_FrontColor = gl_Color;
}