//var mesh;
//var mesh = this;

async function meshProcessor(filename)
{
    //jQuery ajax call to load the .obj file from the local directory
    //NOTE: just putting "await" b4 ajax works WITHOUT puting "async" on processMesh...WEIRD!!!
    await $.ajax(
	{
        url:"./"+filename,
        success:processMesh // --- call the function "processMesh" when the .obj is loaded
    });

    console.log("exiting meshProcessor");
}

//function to load the mesh 
function processMesh(data)
{
    //path taken if local file is uploaded
	if(data.target != undefined)
	{
		mesh = {model: new modelLoader.Mesh(data.target.result)}
    } 
    //path taken if file is chosen from drop-down
	else 
	{
		mesh = {model: new modelLoader.Mesh(data)}
    }

    console.log("Mesh assigned. Exiting processMesh");
}

//NOTE: I believe this code allows the model to change when a new seletion is made!
//		...That's why it's called BEFORE "meshProcessor()"
function setupMeshCallbacks()
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
