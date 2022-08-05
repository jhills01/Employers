async function selectShaders(objName)
{
    //NOTE: I believe textures/UV stuff should be apart of ALL shaders
    switch ( objName )
    {
        //skybox shader-----------------------------------------------------------
        case "skybox":
            console.log("skybox shaders selected");

            vert_src = `
            precision highp float;
    
            attribute vec3 vert_pos;
            attribute vec3 vert_norm;
            attribute vec2 vert_uv;
    
            uniform mat4 model_mtx;
            uniform mat4 norm_mtx;
            uniform mat4 proj_mtx;
    
            varying vec3 v_vert_pos;
            varying vec3 v_vert_norm;
            varying vec2 v_vert_uv;
    
            void main(void)
            {
                v_vert_pos = vert_pos;
                v_vert_norm = vert_norm;
                v_vert_uv = vert_uv;
                gl_Position = proj_mtx * vec4(-vert_pos, 1.0); //NOTE: This made texture to be drawn on inside of cubemap
                //gl_Position = proj_mtx * vec4(vert_pos, 1.0);
            }
            `;

            frag_src = `
            precision highp float;
    
            varying vec3 v_vert_pos;
            varying vec3 v_vert_norm;
            varying vec2 v_vert_uv;

            uniform samplerCube u_texture;
            //uniform sampler2D tex;

            vec3 light_pos = vec3(1.0, 1.0, -1.0); // Light position
            float shininessVal = 80.0; // Shininess
    
            ///*
            void main(void)
            {
                //gl_FragColor = textureCube(u_texture, v_vert_pos);
                //gl_FragColor = textureCube(u_texture, v_vert_norm);
                
                gl_FragColor = textureCube(u_texture, v_vert_pos);

                //gl_FragColor = texture2D(tex, v_vert_uv);
            }
            //*/

            //NOT BEING USED RIGHT NOW!!!!!!!!!!!
            /*
            void main(void)
            {
                vec3 L = normalize(light_pos - v_vert_pos); //used for light attenuation
                vec3 N = normalize(v_vert_norm);

                //Lambert's Cosine Law
                float lambertian = max(dot(N, L), 0.0);
                float specular = 0.0;
                if(lambertian > 0.0) 
                {
                    vec3 R = reflect(-L, N);      // Reflected light vector
                    vec3 V = normalize(-v_vert_pos); // Vector to viewer
                    
                    // Compute the specular term
                    float specAngle = max(dot(R, V), 0.0);
                    specular = pow(specAngle, shininessVal);
                }

                gl_FragColor = textureCube(u_texture, reflect(-L, N)); //"R" is out of scope here
            }
            //*/
            `;
            
            break;
        
        //sun shader--------------------------------------------------------------
        case "sun":
            console.log("sun shaders selected");
            
            vert_src = `
            precision highp float;

            mat3 transpose(in mat3 inMatrix)
            {
                vec3 i0 = inMatrix[0];
                vec3 i1 = inMatrix[1];
                vec3 i2 = inMatrix[2];
    
                mat3 outMatrix = mat3(
                    vec3(i0.x, i1.x, i2.x),
                    vec3(i0.y, i1.y, i2.y),
                    vec3(i0.z, i1.z, i2.z)
                );
    
                return outMatrix;
            }
    
            attribute vec3 vert_pos;
            attribute vec3 vert_norm; 
            attribute vec2 vert_uv;

            uniform mat4 model_mtx;
            uniform mat4 norm_mtx;
            uniform mat4 proj_mtx;

            varying vec3 v_vert_pos;
            varying vec3 v_vert_norm; 
            varying vec2 v_vert_uv;

            void main(void)
            {
                //pass uv coordinate to frag shader
                v_vert_uv = vert_uv;   
                
                //NOTE: seems like both vert_pos and vert_norm...
                //      ...both get multiplied by model_mtx b4 going to frag
                vec4 vert_pos4 = model_mtx * vec4(vert_pos, 1.0);
                v_vert_pos = vec3(vert_pos4) / vert_pos4.w;	
                v_vert_norm = vec3( model_mtx * vec4(vert_norm, 0.0)); 
                
                //gl_Position = proj_mtx * vert_pos4; //OOOOOOPS!!
                gl_Position = proj_mtx * vec4(vert_pos, 1.0);
     
            /*---UNUSED-----------------------------------------
                ts_frag_pos = vec3(model_mtx * vec4(vert_pos, 1.0));

                //--- vec3 vert_norm = cross(vert_bitang, vert_tang);
    
                vec3 t = normalize(mat3(norm_mtx) * vert_tang);
                vec3 b = normalize(mat3(norm_mtx) * vert_bitang);
                vec3 n = normalize(mat3(norm_mtx) * vert_norm);
                mat3 tbn = transpose(mat3(t, b, n));
    
                vec3 light_pos = vec3(1, 2, 0);
                ts_light_pos = tbn * light_pos;
                // Our camera is always at the origin
                ts_view_pos = tbn * vec3(0, 0, 0);
                ts_frag_pos = tbn * ts_frag_pos;
            //*/
            }
            `;

            frag_src = `
            precision highp float;

            float Ka = 0.8;   // Ambient reflection coefficient
            float Kd = 1.0;   // Diffuse reflection coefficient
            float Ks = 0.0;   // Specular reflection coefficient
            float shininessVal = 80.0; // Shininess
    
            // Material color
            //vec3 ambientColor = vec3(0.2, 0.1, 0.0);
            vec3 ambientColor = vec3(0.9, 0.8, 0.5);
            vec3 diffuseColor = vec3(0.8, 0.4, 0.0);
            vec3 specularColor = vec3(1.0, 1.0, 1.0);
            //vec3 light_pos = vec3(1.0, 1.0, -1.0); // Light position
            vec3 light_pos = vec3(1, 2, 0);
    
            uniform sampler2D tex;

            varying vec3 v_vert_pos;
            varying vec3 v_vert_norm; 
            varying vec2 v_vert_uv;
    
            void main(void)
            {
				vec3 L = normalize(light_pos - v_vert_pos); //used for light attenuation
				vec3 N = normalize(v_vert_norm);

				//Lambert's Cosine Law
				float lambertian = max(dot(N, L), 0.0);
  				float specular = 0.0;
  				if(lambertian > 0.0) 
				{
					vec3 R = reflect(-L, N);      // Reflected light vector
					vec3 V = normalize(-v_vert_pos); // Vector to viewer
					
					// Compute the specular term
					float specAngle = max(dot(R, V), 0.0);
					specular = pow(specAngle, shininessVal);
  				}
				
				vec4 rgba = texture2D(tex, v_vert_uv);
				if (rgba.a >= 0.1) 
				{
					gl_FragColor = vec4(Ka * ambientColor +
										Kd * lambertian * vec3(rgba.r, rgba.g, rgba.b) +
										Ks * specular * specularColor, 1.0);

					/*
					gl_FragColor = vec4(Ka * ambientColor +
										Kd * lambertian * diffuseColor +
                                        Ks * specular * specularColor, 1.0);
                    //*/
				}
				else discard;

				//gl_FragColor = texture2D(uSampler, vTexCoord);

            /*--UNUSED------------------------------------------------------
                vec3 light_dir = normalize(ts_light_pos - ts_frag_pos);
                vec3 view_dir = normalize(ts_view_pos - ts_frag_pos);
                vec2 uv = frag_uv; //NOTE: was parallex step. I skipped that b/c I'm just doing normal
    
                vec3 albedo = texture2D(tex_diffuse, uv).rgb;
                vec3 ambient = 0.3 * albedo;
    
                // Normal mapping
                vec3 norm = normalize(texture2D(tex_norm, uv).rgb * 2.0 - 1.0);
                float diffuse = max(dot(light_dir, norm), 0.0);
                //gl_FragColor = vec4(diffuse * albedo + ambient, 1.0);

                gl_FragColor = vec4(Ka * ambientColor +
                                    Kd * diffuseColor +
                                    Ks * specularColor, 1.0);
            //*/
            }
            `;
            
            break;
       
        //earth shader-----------------------------------------------------------
        case "earth":
            console.log("earth shaders selected");

            vert_src = `
            precision highp float;
    
            attribute vec3 vert_pos;
            //attribute vec3 vert_norm; //NOTE: added this
            attribute vec3 vert_tang;
            attribute vec3 vert_bitang;
            attribute vec2 vert_uv;
    
            uniform mat4 model_mtx;
            uniform mat4 norm_mtx;
            uniform mat4 proj_mtx;
    
            varying vec2 frag_uv;
            //varying vec3 vert_norm; //NOTE: added this
            varying vec3 ts_light_pos; // Tangent space values
            varying vec3 ts_view_pos;  //
            varying vec3 ts_frag_pos;  //
    
            mat3 transpose(in mat3 inMatrix)
            {
                vec3 i0 = inMatrix[0];
                vec3 i1 = inMatrix[1];
                vec3 i2 = inMatrix[2];
    
                mat3 outMatrix = mat3(
                    vec3(i0.x, i1.x, i2.x),
                    vec3(i0.y, i1.y, i2.y),
                    vec3(i0.z, i1.z, i2.z)
                );
    
                return outMatrix;
            }
    
            void main(void)
            {
                gl_Position = proj_mtx * vec4(vert_pos, 1.0);
                ts_frag_pos = vec3(model_mtx * vec4(vert_pos, 1.0));
                vec3 vert_norm = cross(vert_bitang, vert_tang);
    
                vec3 t = normalize(mat3(norm_mtx) * vert_tang);
                vec3 b = normalize(mat3(norm_mtx) * vert_bitang);
                vec3 n = normalize(mat3(norm_mtx) * vert_norm);
                mat3 tbn = transpose(mat3(t, b, n));
    
                vec3 light_pos = vec3(1, 2, 0);
                ts_light_pos = tbn * light_pos;
                // Our camera is always at the origin
                ts_view_pos = tbn * vec3(0, 0, 0);
                ts_frag_pos = tbn * ts_frag_pos;
            
                frag_uv = vert_uv;
            }
            `;

            frag_src = `
            precision highp float;
    
            float Ka = 1.0;   // Ambient reflection coefficient
            float Kd = 1.0;   // Diffuse reflection coefficient
            float Ks = 1.0;   // Specular reflection coefficient
            float shininessVal = 80.0; // Shininess
    
            // Material color
            vec3 ambientColor = vec3(0.2, 0.1, 0.0);
            vec3 diffuseColor = vec3(0.8, 0.4, 0.0);
            vec3 specularColor = vec3(1.0, 1.0, 1.0);
            vec3 lightPos = vec3(1.0, 1.0, -1.0); // Light position
    
            uniform sampler2D tex_norm;
            uniform sampler2D tex_diffuse;
            uniform sampler2D tex_depth;
            uniform float sun_light_z;
    
            varying vec2 frag_uv;
            //varying vec3 vert_norm; //NOTE: added this
            varying vec3 ts_light_pos;
            varying vec3 ts_view_pos;
            varying vec3 ts_frag_pos;
    
            void main(void)
            {
                vec3 light_dir = normalize(ts_light_pos - ts_frag_pos);
                //vec3 light_dir = normalize(vec3(0.0, 0.0, sun_light_z) - ts_frag_pos);
                vec3 view_dir = normalize(ts_view_pos - ts_frag_pos);
                vec2 uv = frag_uv; //NOTE: was parallex step. I skipped that b/c I'm just doing normal
    
                vec3 albedo = texture2D(tex_diffuse, uv).rgb;
                vec3 ambient = 0.3 * albedo;
    
                // Normal mapping
                vec3 norm = normalize(texture2D(tex_norm, uv).rgb * 2.0 - 1.0);
                float diffuse = max(dot(light_dir, norm), 0.0);
                gl_FragColor = vec4(diffuse * albedo + ambient, 1.0);
            }
            `;
            
            break;
        
        //planet shader-----------------------------------------------------------
        case "planet":
            console.log("planet shaders selected");
                        
            vert_src = `
            precision highp float;

            mat3 transpose(in mat3 inMatrix)
            {
                vec3 i0 = inMatrix[0];
                vec3 i1 = inMatrix[1];
                vec3 i2 = inMatrix[2];
    
                mat3 outMatrix = mat3(
                    vec3(i0.x, i1.x, i2.x),
                    vec3(i0.y, i1.y, i2.y),
                    vec3(i0.z, i1.z, i2.z)
                );
    
                return outMatrix;
            }
    
            attribute vec3 vert_pos;
            attribute vec3 vert_norm; 
            attribute vec2 vert_uv;

            uniform mat4 model_mtx;
            uniform mat4 norm_mtx;
            uniform mat4 proj_mtx;

            varying vec3 v_vert_pos;
            varying vec3 v_vert_norm; 
            varying vec2 v_vert_uv;

            void main(void)
            {
                //pass uv coordinate to frag shader
                v_vert_uv = vert_uv;   
                
                //NOTE: seems like both vert_pos and vert_norm...
                //      ...both get multiplied by model_mtx b4 going to frag
                vec4 vert_pos4 = model_mtx * vec4(vert_pos, 1.0);
                v_vert_pos = vec3(vert_pos4) / vert_pos4.w;	
                v_vert_norm = vec3( model_mtx * vec4(vert_norm, 0.0)); 
                
                //gl_Position = proj_mtx * vert_pos4; //OOOOOOPS!!
                gl_Position = proj_mtx * vec4(vert_pos, 1.0);
     
            /*---UNUSED-----------------------------------------
                ts_frag_pos = vec3(model_mtx * vec4(vert_pos, 1.0));

                //--- vec3 vert_norm = cross(vert_bitang, vert_tang);
    
                vec3 t = normalize(mat3(norm_mtx) * vert_tang);
                vec3 b = normalize(mat3(norm_mtx) * vert_bitang);
                vec3 n = normalize(mat3(norm_mtx) * vert_norm);
                mat3 tbn = transpose(mat3(t, b, n));
    
                vec3 light_pos = vec3(1, 2, 0);
                ts_light_pos = tbn * light_pos;
                // Our camera is always at the origin
                ts_view_pos = tbn * vec3(0, 0, 0);
                ts_frag_pos = tbn * ts_frag_pos;
            //*/
            }
            `;

            frag_src = `
            precision highp float;

            float Ka = 0.25;   // Ambient reflection coefficient
            float Kd = 1.0;   // Diffuse reflection coefficient
            float Ks = 1.0;   // Specular reflection coefficient
            float shininessVal = 80.0; // Shininess
    
            // Material color
            vec3 ambientColor = vec3(0.2, 0.1, 0.0);
            vec3 diffuseColor = vec3(0.8, 0.4, 0.0);
            vec3 specularColor = vec3(1.0, 1.0, 1.0);
            //vec3 light_pos = vec3(1.0, 1.0, -1.0); // Light position
            //vec3 light_pos = vec3(1, 2, 0);
    
            uniform sampler2D tex;
            uniform float sun_light_Z;

            varying vec3 v_vert_pos;
            varying vec3 v_vert_norm; 
            varying vec2 v_vert_uv;
    
            void main(void)
            {
				//vec3 L = normalize(light_pos - v_vert_pos); //used for light attenuation
				vec3 L = normalize( vec3(0.0, 0.0, sun_light_Z) - v_vert_pos); //used for light attenuation
				vec3 N = normalize(v_vert_norm);

				//Lambert's Cosine Law
				float lambertian = max(dot(N, L), 0.0);
  				float specular = 0.0;
  				if(lambertian > 0.0) 
				{
					vec3 R = reflect(-L, N);      // Reflected light vector
					vec3 V = normalize(-v_vert_pos); // Vector to viewer
					
					// Compute the specular term
					float specAngle = max(dot(R, V), 0.0);
					specular = pow(specAngle, shininessVal);
  				}
				
				vec4 rgba = texture2D(tex, v_vert_uv);
				if (rgba.a >= 0.1) 
				{
					gl_FragColor = vec4(Ka * ambientColor +
										Kd * lambertian * vec3(rgba.r, rgba.g, rgba.b) +
										Ks * specular * specularColor, 1.0);

					/*
					gl_FragColor = vec4(Ka * ambientColor +
										Kd * lambertian * diffuseColor +
                                        Ks * specular * specularColor, 1.0);
                    //*/
				}
				else discard;

				//gl_FragColor = texture2D(uSampler, vTexCoord);

            /*--UNUSED------------------------------------------------------
                vec3 light_dir = normalize(ts_light_pos - ts_frag_pos);
                vec3 view_dir = normalize(ts_view_pos - ts_frag_pos);
                vec2 uv = frag_uv; //NOTE: was parallex step. I skipped that b/c I'm just doing normal
    
                vec3 albedo = texture2D(tex_diffuse, uv).rgb;
                vec3 ambient = 0.3 * albedo;
    
                // Normal mapping
                vec3 norm = normalize(texture2D(tex_norm, uv).rgb * 2.0 - 1.0);
                float diffuse = max(dot(light_dir, norm), 0.0);
                //gl_FragColor = vec4(diffuse * albedo + ambient, 1.0);

                gl_FragColor = vec4(Ka * ambientColor +
                                    Kd * diffuseColor +
                                    Ks * specularColor, 1.0);
            //*/
            }
            `;


            break;    
    }

//CODE STORAGE----------------------------------------------------------------------------------------------
    
//SKYBOX ORIGINAL SHADERS-------------------------------------------------------------------
{
/*
vert_src = `
precision highp float;

attribute vec3 vert_pos;
//attribute vec3 vert_norm; //NOTE: added this
attribute vec3 vert_tang;
attribute vec3 vert_bitang;
attribute vec2 vert_uv;

uniform mat4 model_mtx;
uniform mat4 norm_mtx;
uniform mat4 proj_mtx;

varying vec2 frag_uv;
//varying vec3 vert_norm; //NOTE: added this
varying vec3 ts_light_pos; // Tangent space values
varying vec3 ts_view_pos;  //
varying vec3 ts_frag_pos;  //

mat3 transpose(in mat3 inMatrix)
{
    vec3 i0 = inMatrix[0];
    vec3 i1 = inMatrix[1];
    vec3 i2 = inMatrix[2];

    mat3 outMatrix = mat3(
        vec3(i0.x, i1.x, i2.x),
        vec3(i0.y, i1.y, i2.y),
        vec3(i0.z, i1.z, i2.z)
    );

    return outMatrix;
}

void main(void)
{
    gl_Position = proj_mtx * vec4(vert_pos, 1.0);
    ts_frag_pos = vec3(model_mtx * vec4(vert_pos, 1.0));
    vec3 vert_norm = cross(vert_bitang, vert_tang);

    vec3 t = normalize(mat3(norm_mtx) * vert_tang);
    vec3 b = normalize(mat3(norm_mtx) * vert_bitang);
    vec3 n = normalize(mat3(norm_mtx) * vert_norm);
    mat3 tbn = transpose(mat3(t, b, n));

    vec3 light_pos = vec3(1, 2, 0);
    ts_light_pos = tbn * light_pos;
    // Our camera is always at the origin
    ts_view_pos = tbn * vec3(0, 0, 0);
    ts_frag_pos = tbn * ts_frag_pos;

    frag_uv = vert_uv;
}
`;

frag_src = `
precision highp float;

float Ka = 1.0;   // Ambient reflection coefficient
float Kd = 1.0;   // Diffuse reflection coefficient
float Ks = 1.0;   // Specular reflection coefficient
float shininessVal = 80.0; // Shininess

// Material color
vec3 ambientColor = vec3(0.2, 0.1, 0.0);
vec3 diffuseColor = vec3(0.8, 0.4, 0.0);
vec3 specularColor = vec3(1.0, 1.0, 1.0);
vec3 lightPos = vec3(1.0, 1.0, -1.0); // Light position

uniform sampler2D tex_norm;
uniform sampler2D tex_diffuse;
uniform sampler2D tex_depth;

varying vec2 frag_uv;
//varying vec3 vert_norm; //NOTE: added this
varying vec3 ts_light_pos;
varying vec3 ts_view_pos;
varying vec3 ts_frag_pos;

void main(void)
{
    vec3 light_dir = normalize(ts_light_pos - ts_frag_pos);
    vec3 view_dir = normalize(ts_view_pos - ts_frag_pos);
    vec2 uv = frag_uv; //NOTE: was parallex step. I skipped that b/c I'm just doing normal

    vec3 albedo = texture2D(tex_diffuse, uv).rgb;
    vec3 ambient = 0.3 * albedo;

    // Normal mapping
    vec3 norm = normalize(texture2D(tex_norm, uv).rgb * 2.0 - 1.0);
    float diffuse = max(dot(light_dir, norm), 0.0);
    gl_FragColor = vec4(diffuse * albedo + ambient, 1.0);
}
`;
//*/
}
//SKYBOX ORIGINAL SHADERS-------------------------------------------------------------------


    // if (input == "df1")
    // {    
    //     console.log("phong shader selected");

    //     vert_src = `
    //     precision highp float;

    //     attribute vec3 vert_pos;
    //     //attribute vec3 vert_norm; //NOTE: added this
    //     attribute vec3 vert_tang;
    //     attribute vec3 vert_bitang;
    //     attribute vec2 vert_uv;

    //     uniform mat4 model_mtx;
    //     uniform mat4 norm_mtx;
    //     uniform mat4 proj_mtx;

    //     varying vec2 frag_uv;
    //     //varying vec3 vert_norm; //NOTE: added this
    //     varying vec3 ts_light_pos; // Tangent space values
    //     varying vec3 ts_view_pos;  //
    //     varying vec3 ts_frag_pos;  //

    //     mat3 transpose(in mat3 inMatrix)
    //     {
    //         vec3 i0 = inMatrix[0];
    //         vec3 i1 = inMatrix[1];
    //         vec3 i2 = inMatrix[2];

    //         mat3 outMatrix = mat3(
    //             vec3(i0.x, i1.x, i2.x),
    //             vec3(i0.y, i1.y, i2.y),
    //             vec3(i0.z, i1.z, i2.z)
    //         );

    //         return outMatrix;
    //     }

    //     void main(void)
    //     {
    //         gl_Position = proj_mtx * vec4(vert_pos, 1.0);
    //         ts_frag_pos = vec3(model_mtx * vec4(vert_pos, 1.0));
    //         vec3 vert_norm = cross(vert_bitang, vert_tang);

    //         vec3 t = normalize(mat3(norm_mtx) * vert_tang);
    //         vec3 b = normalize(mat3(norm_mtx) * vert_bitang);
    //         vec3 n = normalize(mat3(norm_mtx) * vert_norm);
    //         mat3 tbn = transpose(mat3(t, b, n));

    //         vec3 light_pos = vec3(1, 2, 0);
    //         ts_light_pos = tbn * light_pos;
    //         // Our camera is always at the origin
    //         ts_view_pos = tbn * vec3(0, 0, 0);
    //         ts_frag_pos = tbn * ts_frag_pos;
        
    //         frag_uv = vert_uv;
    //     }
    //     `;

    //     frag_src = `
    //     precision highp float;

    //     float Ka = 1.0;   // Ambient reflection coefficient
    //     float Kd = 1.0;   // Diffuse reflection coefficient
    //     float Ks = 1.0;   // Specular reflection coefficient
    //     float shininessVal = 80.0; // Shininess

    //     // Material color
    //     vec3 ambientColor = vec3(0.2, 0.1, 0.0);
    //     vec3 diffuseColor = vec3(0.8, 0.4, 0.0);
    //     vec3 specularColor = vec3(1.0, 1.0, 1.0);
    //     vec3 lightPos = vec3(1.0, 1.0, -1.0); // Light position

    //     uniform sampler2D tex_norm;
    //     uniform sampler2D tex_diffuse;
    //     uniform sampler2D tex_depth;

    //     varying vec2 frag_uv;
    //     //varying vec3 vert_norm; //NOTE: added this
    //     varying vec3 ts_light_pos;
    //     varying vec3 ts_view_pos;
    //     varying vec3 ts_frag_pos;

    //     void main(void)
    //     {
    //         vec3 light_dir = normalize(ts_light_pos - ts_frag_pos);
    //         vec3 view_dir = normalize(ts_view_pos - ts_frag_pos);
    //         vec2 uv = frag_uv; //NOTE: was parallex step. I skipped that b/c I'm just doing normal

    //         vec3 albedo = texture2D(tex_diffuse, uv).rgb;
    //         vec3 ambient = 0.3 * albedo;

    //         // Normal mapping
    //         vec3 norm = normalize(texture2D(tex_norm, uv).rgb * 2.0 - 1.0);
    //         float diffuse = max(dot(light_dir, norm), 0.0);
    //         gl_FragColor = vec4(diffuse * albedo + ambient, 1.0);
    //     }
    //     `;
    // }
    // else if (input == "df2") 
    // {
    //     console.log("gouraud shader selected");


    // }
    // else if (input == "???") 
    // {
    //     console.log("??? shader selected");

    // }

    //return [vert_src, frag_src];
}