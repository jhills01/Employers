<?php
?>
<html>
	<head>
		<meta charset="utf-8">
		<title>Model Viewer</title>
		<!-- include all javascript source files -->
		<script src="//ajax.googleapis.com/ajax/libs/jquery/1.10.2/jquery.min.js"></script>
		<script type="text/javascript" src="js/sylvester.js"></script>
		<script type="text/javascript" src="js/math.js"></script>
		<script type="text/javascript" src="js/glUtils.js"></script>
		<script type="text/javascript" src="js/meshLoader.js"></script>
		<script type="text/javascript" src="js/arcball.js"></script>
		<script type="text/javascript" src="js/demo.js"></script>
		<script type="text/javascript" src="js/main.js"></script>
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


		<!-- Fragment Shader -->
		<script id="FragmentShader1" type="x-shader/x-fragment">
			#ifdef GL_OES_standard_derivatives
				#extension GL_OES_standard_derivatives : enable
			#endif

			precision mediump float;
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

			void main(void) 
			{
				ambientColor =  vec3(v_redYGradient[1], 0.1, 0.0);
				diffuseColor =  vec3(v_redYGradient[1], 0.4, 0.0);
				specularColor = vec3(v_redYGradient[1], 1.0, 1.0);
				
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

				gl_FragColor = vec4(Ka * ambientColor +
               				        Kd * lambertian * diffuseColor +
               				        Ks * specular * specularColor, 1.0);
			}

		</script>

		<!-- Vertex Shader -->
		<script id="VertexShader1" type="x-shader/x-vertex">
			attribute vec3 vPos; //vertex position
			attribute vec3 vNormal; //vertex normal
			attribute vec3 redYGradient; //values for linear relationship between y-component of vertices and red of RGB
			varying vec3 interpVNormal;
			varying vec3 vertPos;
			varying vec3 v_redYGradient;
			
			uniform mat4 uMVMatrix;//modelviewmatrix
			uniform mat4 uPMatrix;//projectionmatrix
			
			void main(void) 
			{
				v_redYGradient = redYGradient;
				vec4 vertPos4 = uMVMatrix * vec4(vPos, 1.0);
				vertPos = vec3(vertPos4) / vertPos4.w;	

				//transforms vertex normal orientation to eye space
				interpVNormal = vec3( uMVMatrix * vec4(vNormal, 0.0)); 
				
				gl_Position = uPMatrix * vertPos4;
			}
		</script>  

		<script>
			//grab the filename for the .obj we will first open
			var filename = "<? echo $beginFile ?>";

			//register callbacks for mesh loading
			setupLoadingCallbacks();

			//call the main mesh Loading function; main.js
			executeMainLoop(filename); 
		</script>

	</body>
</html>
