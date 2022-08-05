function executeMainLoop(filename)
{
    //alert("Hi Preston. I have a message for you!");
    
    //jQuery ajax call to load the .obj file from the local directory
    $.ajax(
	{
        url:"./"+filename,
        success:processMesh // --- call the function "processMesh" when the .obj is loaded
    });
}


//function to load the mesh and setup the opengl rendering demo
function processMesh(data)
{

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

    //create a new model viewing demo
    var myDemo = new demo("glcanvas",mesh);

    //setup the webgl context and initialize everything
    myDemo.init();

    //enter the event driven loop; ---- demo.js
    //NOTE: MainLoop only calls "drawScene" which is purposely NOT a member of demo...no idea why, but I'm leaving it
    myDemo.MainLoop();
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
