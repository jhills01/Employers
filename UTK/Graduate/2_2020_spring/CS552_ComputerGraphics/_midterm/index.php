<?php
?>
<html>
	<head>
		<meta charset="utf-8">
		<title>Model Viewer</title>
		<!-- include all javascript source files 
			scrips loaded first have contents accessible to later loaded scripts-->
		<script src="//ajax.googleapis.com/ajax/libs/jquery/1.10.2/jquery.min.js"></script>
		<script type="text/javascript" src="js/sylvester.js"></script>
		<script type="text/javascript" src="js/math.js"></script>
		<script type="text/javascript" src="js/glUtils.js"></script>
		<script type="text/javascript" src="js/meshLoader.js"></script>
		<script type="text/javascript" src="js/arcball.js"></script>

		<script type="text/javascript" src="js/bump_mapping_INTEGRATED.js"></script>
		<script type="text/javascript" src="js/textureManager.js"></script> 

		<script type="text/javascript" src="js/demo.js"></script>
		<script type="text/javascript" src="js/main.js"></script>
<!--		
		<script type="text/javascript" src="js/textureManager.js"></script> 
-->
	</head>

	<body>
		<canvas id="glcanvas">canvas not supported</canvas>

		<div id="meshSelect-wrapper">
			<span>Select object from this directory (.obj files only)</span>
			<select id="meshSelect">
			<?php //----- php code to create html selection with local files

				$files = glob("*.obj"); //find all .obj files in current directory
				$beginFile = "";
				foreach ($files as $filename) {
					if($filename == end($files)){
						$beginFile = $filename;
						echo "<option selected=\"selected\">$filename</option>";
					} else {
						echo "<option>$filename</option>";
					}
				}
			?>
			</select>
			<br />
			<span>Or upload a local file here:</span>
			<input type="file" id="files" name="files[]"/>
		</div>

<!--
		<br />
		<br />

		<div id="texSelect-wrapper">
			<span>Select image from imageFiles directory (.png or jpg files only)</span>
			<select id="texSelect">
			
			<//?php //----- php code to create html selection with local files
					//----- NOTE: to comment this out with the rest of the html, I added "//" WITHIN php tag: "<//?php" 

				$imageFiles = glob("imagefiles/*.{jpg,png}", GLOB_BRACE); //find all image files with... 
																   //given ext in current directory
				$beginImageFile = "";
				foreach ($imageFiles as $imageFilename) 
				{
					if($imageFilename == end($imageFiles))
					{
						$beginImageFile = $imageFilename;
						echo "<option selected=\"selected\">$imageFilename</option>";
					} else {
						echo "<option>$imageFilename</option>";
					}
				}
			?>
			</select>
		</div>
-->
		<!-- Fragment Shader -->
		<script id="FragmentShader1" type="x-shader/x-fragment">
			#ifdef GL_OES_standard_derivatives
				#extension GL_OES_standard_derivatives : enable
			#endif

			//precision mediump float;
			precision highp float;
			
			varying vec3 interpVNormal;
			varying vec3 vertPos;
			varying vec3 v_redYGradient;

			float Ka = 1.0;   // Ambient reflection coefficient
			float Kd = 1.0;   // Diffuse reflection coefficient
			float Ks = 1.0;   // Specular reflection coefficient
			float shininessVal = 80.0; // Shininess
			
			// Material color
			vec3 ambientColor = vec3(0.2, 0.1, 0.0);
			vec3 diffuseColor = vec3(0.8, 0.4, 0.0);
			vec3 specularColor = vec3(1.0, 1.0, 1.0);
			vec3 lightPos = vec3(1.0, 1.0, -1.0); // Light position

			//additional variables for textures
			varying highp vec2 vTexCoord; // a single VARYING texture coordinate FROM vertex shader
			uniform sampler2D uSampler;

			//---bump_mapping_INTEGRATED related items
			uniform sampler2D tex_norm;
			uniform sampler2D tex_diffuse;
			uniform sampler2D tex_depth;

			varying vec2 frag_uv;
			varying vec3 ts_light_pos;
			varying vec3 ts_view_pos;
			varying vec3 ts_frag_pos;

			void main(void) 
			{
				//ambientColor =  vec3(v_redYGradient[1], 0.1, 0.0);
				//diffuseColor =  vec3(v_redYGradient[1], 0.4, 0.0);
				//specularColor = vec3(v_redYGradient[1], 1.0, 1.0);
				
				vec3 L = normalize(lightPos - vertPos); //used for light attenuation
				vec3 N = normalize(interpVNormal);

				//Lambert's Cosine Law
				float lambertian = max(dot(N, L), 0.0);
  				float specular = 0.0;
  				if(lambertian > 0.0) 
				{
					vec3 R = reflect(-L, N);      // Reflected light vector
					vec3 V = normalize(-vertPos); // Vector to viewer
					
					// Compute the specular term
					float specAngle = max(dot(R, V), 0.0);
					specular = pow(specAngle, shininessVal);
  				}
				
				//gl_FragColor = vec4(v_redYGradient[1], 0, 0, 1.0);

				/*
				gl_FragColor = vec4(Ka * ambientColor +
               				        Kd * lambertian * diffuseColor +
               				        Ks * specular * specularColor, 1.0);
				//*/

				//NOTE: no light yet!
				//gl_FragColor = texture2D(uSampler, vTexCoord);

			//----BUMP MAP CODE BELOW----------------------------
			///*
				vec3 light_dir = normalize(ts_light_pos - ts_frag_pos);
    			vec3 view_dir = normalize(ts_view_pos - ts_frag_pos);

				vec3 albedo = texture2D(tex_diffuse, frag_uv).rgb;
				vec3 ambient = 0.3 * albedo;

				// Normal mapping
				vec3 norm = normalize(texture2D(tex_norm, frag_uv).rgb * 2.0 - 1.0);
				float diffuse = max(dot(light_dir, norm), 0.0);
				
				gl_FragColor = vec4(diffuse * albedo + ambient, 1.0);
			//*/
			}

		</script>

		<!-- Vertex Shader -->
		<script id="VertexShader1" type="x-shader/x-vertex">
			precision highp float;
			
			attribute vec3 vPos; //vertex position
			attribute vec3 vNormal; //vertex normal
			attribute vec3 redYGradient; //values for linear relationship between y-component of vertices and red of RGB
			varying vec3 interpVNormal;
			varying vec3 vertPos;
			varying vec3 v_redYGradient;

			//additional variables for textures
			attribute vec2 aTexCoord; // a single attribute texture coordinate
			varying highp vec2 vTexCoord; // a single VARYING texture coordinate TO frag shader
			
			//NOTE: even though these are calculated using .obj cube from dropdown, they should...
			//		...still work for hardcoded geomtry b/c both are cubes centered on origin
			uniform mat4 uMVMatrix;//modelviewmatrix
			uniform mat4 uPMatrix;//projectionmatrix

			//---bump_mapping_INTEGRATED related items
			attribute vec3 vert_pos;
			attribute vec3 vert_tang;
			attribute vec3 vert_bitang;
			attribute vec2 vert_uv;

			varying vec2 frag_uv;
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
				//pass texture coordinate to fragment shader
				vTexCoord = aTexCoord;
				v_redYGradient = redYGradient;

				vec4 vertPos4 = uMVMatrix * vec4(vPos, 1.0);
				vertPos = vec3(vertPos4) / vertPos4.w;	

				//transforms vertex normal orientation to eye space
				interpVNormal = vec3( uMVMatrix * vec4(vNormal, 0.0)); 

				//gl_Position = uPMatrix * vertPos4; 

			//----BUMP MAP CODE BELOW----------------------------
				//gl_Position = uPMatrix * vec4(vert_pos, 1.0); //OOPS!!!!! See below!

				gl_Position = uPMatrix * uMVMatrix * vec4(vert_pos, 1.0);
				ts_frag_pos = vec3(uMVMatrix * vec4(vert_pos, 1.0));
				vec3 vert_norm = cross(vert_bitang, vert_tang);

				//NOTE: I'm using uMVMatrix instead of uNMatrix b/c that has been working so far
				vec3 t = normalize(mat3(uMVMatrix) * vert_tang);
				vec3 b = normalize(mat3(uMVMatrix) * vert_bitang);
				vec3 n = normalize(mat3(uMVMatrix) * vert_norm);
				mat3 tbn = transpose(mat3(t, b, n));

				vec3 light_pos = vec3(1, 2, 0); //NOTE: not going to change this to "lightPos" from fragShader
				ts_light_pos = tbn * light_pos;
				// Our camera is always at the origin
				ts_view_pos = tbn * vec3(0, 0, 0);
				ts_frag_pos = tbn * ts_frag_pos;
			
				frag_uv = vert_uv;
			}
		</script>  

		<script>
			//grab the filename for the .obj we will first open
			var filename = "<? echo $beginFile ?>";
			//var imageFilename = "<//? echo $beginImageFile ?>"; //NOTE: notice the "//" in "<//?". This comments out the rest too

			//register callbacks for mesh loading; <main class="js"></main>
			setupLoadingCallbacks();

			//register callbacks for texture loading; <main class="js"></main>
			//setupLoadingTextureCallbacks();

			//call the main mesh Loading function; main.js
			executeMainLoop(filename); 
			//executeMainLoop(filename, imageFilename); 

		</script>

	</body>
</html>
