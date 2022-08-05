final String vertexShader =
    "uniform mat4 u_MVPMatrix;      \n"     // A constant representing the combined model/view/projection matrix.
  + "uniform mat4 u_MVMatrix;       \n"     // A constant representing the combined model/view matrix.
  + "uniform vec3 u_LightPos;       \n"     // The position of the light in eye space.
 
  + "attribute vec4 a_Position;     \n"     // Per-vertex position information we will pass in.
  + "attribute vec4 a_Color;        \n"     // Per-vertex color information we will pass in.
  + "attribute vec3 a_Normal;       \n"     // Per-vertex normal information we will pass in.
 
  + "varying vec4 v_Color;          \n"     // This will be passed into the fragment shader.
 
  + "void main()                    \n"     // The entry point for our vertex shader.
  + "{                              \n"
// Transform the vertex into eye space.
  + "   vec3 modelViewVertex = vec3(u_MVMatrix * a_Position);              \n"
// Transform the normal's orientation into eye space.
  + "   vec3 modelViewNormal = vec3(u_MVMatrix * vec4(a_Normal, 0.0));     \n"
// Will be used for attenuation.
  + "   float distance = length(u_LightPos - modelViewVertex);             \n"
// Get a lighting direction vector from the light to the vertex.
  + "   vec3 lightVector = normalize(u_LightPos - modelViewVertex);        \n"
// Calculate the dot product of the light vector and vertex normal. If the normal and light vector are
// pointing in the same direction then it will get max illumination.
  + "   float diffuse = max(dot(modelViewNormal, lightVector), 0.1);       \n"
// Attenuate the light based on distance.
  + "   diffuse = diffuse * (1.0 / (1.0 + (0.25 * distance * distance)));  \n"
// Multiply the color by the illumination level. It will be interpolated across the triangle.
  + "   v_Color = a_Color * diffuse;                                       \n"
// gl_Position is a special variable used to store the final position.
// Multiply the vertex by the matrix to get the final point in normalized screen coordinates.
  + "   gl_Position = u_MVPMatrix * a_Position;                            \n"
  + "}                                            