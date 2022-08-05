export const vertShader = 
`
attribute vec4 vPos; //vertex 3d position with extra omega dimension
//attribute vec4(vPos3, 1.0) vPos4; //vertex 4d position with omega == 1
attribute vec3 bary; //barycentric
varying vec3 interpBary;

uniform mat4 uMVMatrix;//modelviewmatrix
uniform mat4 uPMatrix;//projectionmatrix

void main(void) 
{
    interpBary = bary;
    gl_Position = uPMatrix * uMVMatrix * vPos;
    //gl_Position = uPMatrix * uMVMatrix * vPos4;
}
`; 
 