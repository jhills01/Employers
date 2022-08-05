<?php
?>
<html>
	<head>
		<meta charset="utf-8">
		<title>Model Viewer</title>
		<!-- include all javascript source files 1
			scrips loaded first have contents accessible to later loaded scripts-->
		<script src="//ajax.googleapis.com/ajax/libs/jquery/1.10.2/jquery.min.js"></script>
		<script type="text/javascript" src="js/globals.js"></script>
		<script type="text/javascript" src="js/matrixVectorMath.js"></script>
		<script type="text/javascript" src="js/basicObjectGeometry.js"></script>
		<script type="text/javascript" src="js/meshLoader.js"></script>
		<script type="text/javascript" src="js/meshProcessor.js"></script>
		<script type="text/javascript" src="js/shaders.js"></script>
		<script type="text/javascript" src="js/glUtilities.js"></script>
		<script type="text/javascript" src="js/updateAndRender.js"></script>
		<script type="text/javascript" src="js/main.js"></script>
	</head>

	<body>
		<canvas id="gl_canvas">canvas not supported</canvas>

		<div id="meshSelect-wrapper">
			<span>Select PLANET object from this directory (.obj files only)</span>
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

		<!-- Present shader options as radio buttons-->
		<p>
			<input id = "1" type="radio" name="shading_type" value="shadowsOFF">Turn shadows OFF<br>
			<input id = "2" type="radio" name="shading_type" value="shadowsON">Turn shadows ON<br>
		</p>

		<p>Choose time rate here (default = 50): <span id="demo"></span></p>
		<div class="rangeslider"> 
			<input type="range" min="1" max="5000" value="50"
						class="myslider" id="sliderRange"> 
		</div> 
		
		<script> 
			// var rangeslider = document.getElementById("sliderRange"); 
			// var output = document.getElementById("demo"); 
			//output.innerHTML = rangeslider.value; 
			
			// rangeslider.oninput = function() 
			// { 
			// 	output.innerHTML = this.value; 
				
			// } 
		</script>

		<script>
			//grab the filename for the .obj we will first open
			var filename = "<? echo $beginFile ?>";

			//set default radio button to phong shader
			radiobtn = document.getElementById("1");
			radiobtn.checked = true;
			   
			//register callbacks for mesh loading
			setupMeshCallbacks();
			
			console.log("Entering MAIN from index.php script");
			main(filename);

		</script>

	</body>
</html>
