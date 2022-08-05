export const fragShader = 
`
#ifdef GL_OES_standard_derivatives
    #extension GL_OES_standard_derivatives : enable
#endif

precision mediump float;
varying vec3 interpBary;

void main(void)
{
    //change this from true to false and compare the differences
    bool antiAliasing = true;
    //bool antiAliasing = false;
    vec4 blue = vec4(0.0,0.5,1.0,1.0);
    vec4 white = vec4(1.0,1.0,1.0,1.0);

    //  Advanced version
    //shader which draws anti-aliased edge lines on the mesh
    //uses fwidth and passed in barycentric coordinates
    if(antiAliasing)
    {
        vec3 dF = fwidth(interpBary);
        vec3 smoothdF = smoothstep(vec3(0.0),dF, interpBary);
        float g = min(min(smoothdF.x,smoothdF.y),smoothdF.z);

        gl_FragColor = mix(blue,white,g);
        //gl_FragColor = mix(blue,blue,g);
    }
    
    //  Simple Version
    //This checks if the fragments are near the edge and colors blue if true
    //      and white if false
    //This shader exhibits aliasing which is why the edge lines appear jagged
    else 
    {
        if(any(lessThan(interpBary,vec3(0.02))))
        {
            gl_FragColor = blue;
        }
        else 
        {
            gl_FragColor = white;
        }
    }
}

`; 
 