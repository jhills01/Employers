//function executeMainLoop(filename, imageFilename)
function executeMainLoop(filename)
{
    alert("Hello Mr. Preston. I know the default skeleton code is...irksome...to navigate"
    + " through so this message is to help guide you to the important bits for the midterm."
    + " All of the 'borrowed code' is in 'bump_mapping_INTEGRATED.js' source file. Since"
    + " the original 'bump_mapping.js' source lumped alot of functionality in the few"
    + " functions it does have, I decided to rewrite it into more modular form; hence my"
    + " new JS file. In order to integrate this into my existing code from Step 1 of lab3,"
    + " I simply organized my functions so that only two calls need to be made in my"
    + " existing Step 1 code. These locations are: 'initTextures_BMCube(GC, gl);' on line"
    + " 69, and 'drawScene_BMCube(GC, gl);' on line 390 in demo.js. The rest of demo.js code is"
    + " unaltered. Also as part of the integration, I wanted to use the arcball and"
    + " existing matrix code from demo.js so you'll notice (and I won't blame you if"
    + " you don't look :)) that I only copied over geometry and texture code from"
    + " 'bump_mapping.js'. For my shader code in index.php, you'll notice that I left"
    + " my original lab3/Step1 code in there to avoid errors since I left demo.js"
    + " pretty much unaltered. To go straight to the new bump_mapping code, look under"
    + " 'bump_mapping_INTEGRATED related items' comment for new variables and"
    + " 'BUMP MAP CODE BELOW' for new main code in both shaders. I hope this info helps you"
    + " navigate the mess better :). Godspeed!" 
    );

//console.log(filename);
//console.log(imageFilename);
//console.log("calling processMeshAndTex in executeMainLoop");
    //processMeshAndTex(filename, imageFilename);

///*
    //jQuery ajax call to load the .obj file from the local directory
    $.ajax(
	{
        url:"./"+filename,
        //data: {filename: filename, imageFilename: imageFilename},
        //data: "{filename: filename, imageFilename: imageFilename}",
        success:processMesh // --- call the function "processMesh" when the .obj is loaded
    });

//console.log("Made it past ajax block in executeMainLoop");
//*/
}

//function to load the mesh and setup the opengl rendering demo
function processMesh(data)
{
// console.log("entered processMesh");

// console.log("data:");
// console.log(data);

// console.log("just printed data");

	if(data.target != undefined)
	{
		//console.log("Data != UNDEFINDED");
		//console.log(data);
		
		var mesh = {model: new modelLoader.Mesh(data.target.result)}
	} 
	else 
	{
		//console.log("Data == UNDEFINDED"); //NOTE: This branch seems to be the only one taken
										   // 	   ..for selecting .obj files from dropdown
		//console.log(data);
		var mesh = {model: new modelLoader.Mesh(data)}
    }

//console.log("made it to end of mesh");
///*
    //create a new model viewing demo
    var myDemo = new demo("glcanvas",mesh);

    //setup the webgl context and initialize everything
    //NOTE: I'm going to just stick to doing texture stuff in demo object
    myDemo.init();

    //enter the event driven loop; ---- demo.js
    //NOTE: MainLoop only calls "drawScene" which is purposely NOT a member of demo...
    //...no idea why, but I'm leaving it
    myDemo.MainLoop();

//*/
}

//NOTE: I believe this code allows the model to change when a new seletion is made!
//		...That's why it's called BEFORE "executeMainLoop()"
function setupLoadingCallbacks()
{
    //handles when another mesh is selected via the dropdown
	$("#meshSelect").change(function()
	{
		$("select option:selected").each(function()
		{
            filename = $(this).text(); // grab the filename from the selection
            $.ajax(
			{
                url:"./"+filename,
                success:processMesh// --- call the function "processMesh" when the .obj is loaded
            });
        });
    });

    //handles when user uploads a file
	$("#files").change(function(evt)
	{
        var objFile = evt.target.files[0];
        var reader = new FileReader();

        //set a callback for when the file is finished uploading
        reader.onload = processMesh;

        //tell the reader to load the file as a text file
        reader.readAsText(objFile);
    });
}
