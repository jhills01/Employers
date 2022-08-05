
function initTextures(GC, gl)
{
//console.log("Entered initTextures: ");

  const texture = loadTexture(gl, "./imagefiles/Power-T-transparent.png");
  //const texture = loadTexture(gl, "./imagefiles/Power-T.jpg");
  //const texture = loadTexture(gl, "./imagefiles/UT.png");
  texBuffers(GC, gl);
  getShaderLocations(GC, gl); //GC should now have memory varibles corresponding to new shader variables for processing textures
}

// Initialize a texture and load an image.
// When the image finished loading copy it into the texture.
//textureManager.prototype.loadTexture =  function (gl, imageData)
function loadTexture (gl, imageData)
{
//console.log("Entered loadTexture:");
//console.log(gl);
//console.log(imageData);

    const texture = gl.createTexture();
    gl.bindTexture(gl.TEXTURE_2D, texture);
  
    // Because images have to be download over the internet
    // they might take a moment until they are ready.
    // Until then put a single pixel in the texture so we can
    // use it immediately. When the image has finished downloading
    // we'll update the texture with the contents of the image.
    const level = 0;
    const internalFormat = gl.RGBA;
    const width = 1;
    const height = 1;
    const border = 0;
    const srcFormat = gl.RGBA;
    const srcType = gl.UNSIGNED_BYTE;
    const pixel = new Uint8Array([0, 0, 255, 255]);  // opaque blue
    gl.texImage2D(gl.TEXTURE_2D, level, internalFormat,
                  width, height, border, srcFormat, srcType,
                  pixel);
  
    const image = new Image();
    image.onload = function() 
    {
      gl.bindTexture(gl.TEXTURE_2D, texture);
      gl.texImage2D(gl.TEXTURE_2D, level, internalFormat, srcFormat, srcType, image);
      
      // WebGL1 has different requirements for power of 2 images
      // vs non power of 2 images so check if the image is a
      // power of 2 in both dimensions.
      if (isPowerOf2(image.width) && isPowerOf2(image.height)) 
      {
         // Yes, it's a power of 2. Generate mips.
         gl.generateMipmap(gl.TEXTURE_2D);
         console.log("mipmap created in textureManager!");
      } 
      
      else 
      {
         // No, it's not a power of 2. Turn off mips and set
         // wrapping to clamp to edge
         
         //Prevents s-coordinate wrapping (repeating).
         gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
         // Prevents t-coordinate wrapping (repeating).
         gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
         // gl.NEAREST is also allowed, instead of gl.LINEAR, as neither mipmap.
         gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);

         console.log("mipmap COULD NOT BE created in textureManager!");
      }
    };

    //image.src = url;
    image.src = imageData;
  
    //This is how I'll "return" the created texture
    GC.texture = texture;
    //return texture;
}

//textureManager.prototype.texBuffer =  function ()
function texBuffers (GC, gl)
{
//console.log("Entered texBuffers: ");

  //VERY IMPORTANT NOTE: I forgot to convert vtIndices to textureCoordinates!!!!
  let textureCoordinates = [];

  //helper function for determining corresponding texture coordinates for mesh vertices in order
  //modes:
  //------------------------------------------------------------------------------------------------
  // vt:               uses "vt" texture coordinates in given .obj file 
  //------------------------------------------------------------------------------------------------
  // manualSquare:     texture coordinates I manually assigned based on the vertex indexing order 
  //                   ...in "cube-3d-shape.obj" file
  //------------------------------------------------------------------------------------------------
  // calculatedSphere: BLAH
  
  //textureCoordinates = texBufferHelper(GC, "vt");
  //textureCoordinates = texBufferHelper(GC, "manualSquare");
  textureCoordinates = texBufferHelper(GC, "calculatedSphere");

  GC.textureCoordBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, GC.textureCoordBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(textureCoordinates),
                gl.STATIC_DRAW);
}

function texBufferHelper(GC, mode)
{
  let textureCoordinates = [];

  if (mode == "vt")
  {
    let vt = GC.mesh.model.vt;
    let vtIndices = GC.mesh.model.vtIndices;
    vtIndices.forEach(function(vtIndex,i)
    {
      //grab the x,y,z position and normal values for the current vertex
      //grab s,t position values from vt array 
      //NOTE: "parseFloat" converts the strings from "vertices" to numbers
      //NOTE: must be "vtIndex*2" b/c each vt line item from mesh file only has 2 floats (s,t), not 3 (x,y,z) 
      vt_u = (parseFloat(vt[vtIndex*2]));
      vt_v = (parseFloat(vt[vtIndex*2+1]));
      //vt_SpaceFillerToMake3D = -90000.0; //90,000.0

      //add this texture coordinates to our array
      //textureCoordinates.push(vt_s,vt_t,vt_SpaceFillerToMake3D);
      textureCoordinates.push(vt_u,vt_v); //NOTE: Going to try this and hope that pulling 2 form this buffer
                                          // requires the same number of iterations for the other vec3 buffers
    });
  }

  else if (mode == "manualSquare")
  {
    let imageXYCorners = 
    [
      //NOTE: This is the same coordinate order given as "front" at...
      // https://developer.mozilla.org/en-US/docs/Web/API/WebGL_API/Tutorial/Using_textures_in_WebGL
      0.0, 0.0, //Index 0: top-left corner
      1.0, 0.0, //Index 1: top-right corner
      1.0, 1.0, //Index 2: bottom-right corner
      0.0, 1.0  //Index 3: bottom-left corner, i.e. Origin
    ];
    
    //Index values based on "imageXYCorners" above.
    //the index/face order is based on "f" (i.e., face) values in "cube-3d-shape.obj" file
    let textureQuadFaceIndices = 
    [
      0, 3, 2, 1, //Index 0: +x cube face
      0, 3, 2, 1, //Index 1: -x cube face
      2, 1, 0, 3, //Index 2: +y cube face
      2, 1, 0, 3, //Index 3: -y cube face 
      0, 3, 2, 1, //Index 4: +z cube face
      0, 3, 2, 1  //Index 5: -z cube face   
    ];
 
    //perform fan triangulation algorithm on textureIndices
    let textureTriFaceIndices = [];
    let numOfFaces = 6;
    let lineItems = [0, 0, 0, 0];
    for (let i = 0; i < numOfFaces; ++i)
    {
      lineItems = [textureQuadFaceIndices[i*4],
                   textureQuadFaceIndices[i*4 + 1],
                   textureQuadFaceIndices[i*4 + 2],
                   textureQuadFaceIndices[i*4 + 3]
                  ];
      //triangulates a single face with arbitrary number of indices
      //function defined in meshLoader.js
      fanTriangulation(textureTriFaceIndices, lineItems, lineItems.length);
    }

    //finally create textureCoordinates
    textureTriFaceIndices.forEach(function(tIndex,i)
    {
      //grab the x,y,z position and normal values for the current vertex
      //grab s,t position values from vt array 
      //NOTE: "parseFloat" converts the strings from "vertices" to numbers
      //NOTE: must be "vtIndex*2" b/c each vt line item from mesh file only has 2 floats (s,t), not 3 (x,y,z) 
      u = (parseFloat(imageXYCorners[tIndex*2]));
      v = (parseFloat(imageXYCorners[tIndex*2+1]));

      //textureCoordinates.push(v,u);
      //add this texture coordinates to our array
      textureCoordinates.push(u,v); //NOTE: Going to try this and hope that pulling 2 form this buffer
                                          // requires the same number of iterations for the other vec3 buffers
    });
  }
  else if (mode == "calculatedSphere")
  {
    var m = GC.mesh.model;

    // Go through triangulated vertex indices array and use x,y,z coordinates to calculate...
    // u,v texture coordinates
    m.vIndices.forEach(function(verticesIndex,i) 
    { 
      vx = (parseFloat(m.vertices[verticesIndex*3]));
      vy = (parseFloat(m.vertices[verticesIndex*3+1]));
      vz = (parseFloat(m.vertices[verticesIndex*3+2]));

      r = Math.sqrt(vx*vx + vy*vy + vz*vz);

      u = 0.5 + (Math.atan2(vz/r, vx/r)) / (2*Math.PI);
      v = 0.5 - (Math.asin(vy/r)) / (Math.PI);

      //absolutely NO CLUE as to why this is required to "thin out" duplicate "seam" image.
      //freakin ridiculous!
      if ( ((vz <= 0.0) && (vz >= -4.0)) && ((vx <= 0.0) && (vx >= -20.0)))
      {
        u = u + 1.7976931348623157e+308*Math.PI; 
        //u = u + 20*Math.PI; 
      }

      //store final texture coordinates
      textureCoordinates.push(u,v);
    });
  }

  //return result
  return textureCoordinates;
}

function getShaderLocations(GC, gl)
{
//console.log("Entered getShaderLocations: ");

  GC.aTextureCoord = gl.getAttribLocation(GC.shaderProgram, 'aTexCoord');
  gl.enableVertexAttribArray(GC.textureCoord); 

  GC.uSampler = gl.getUniformLocation(GC.shaderProgram, 'uSampler');
}

function addTexInfoToDrawScene(GC, gl)
{
//console.log("Entered addTexInfoToDrawScene: ");

  // tell webgl how to pull out the texture coordinates from buffer
  const numComponents = 2; // every coordinate composed of 2 values
  const type = gl.FLOAT; // the data in the buffer is 32 bit float
  const normalize = false; // don't normalize
  const stride = 0; // how many bytes to get from one set to the next
  const offset = 0; // how many bytes inside the buffer to start from
  gl.bindBuffer(gl.ARRAY_BUFFER, GC.textureCoordBuffer); //"GC.textureCoordBuffer" created in texBuffers above.
  gl.vertexAttribPointer(
      //programInfo.attribLocations.textureCoord,
      GC.aTextureCoord,
      numComponents,
      type,
      normalize,
      stride,
      offset);
  //gl.enableVertexAttribArray(
      //programInfo.attribLocations.textureCoord);
  //    GC.textureCoord);

  // Tell WebGL we want to affect texture unit 0
  gl.activeTexture(gl.TEXTURE0);

  // Bind the texture to texture unit 0
  //gl.bindTexture(gl.TEXTURE_2D, texture);
  gl.bindTexture(gl.TEXTURE_2D, GC.texture); //GC.texture created in loadTexture

  // Tell the shader we bound the texture to texture unit 0
  //gl.uniform1i(programInfo.uniformLocations.uSampler, 0);
  gl.uniform1i(GC.uSampler, 0);
}

// CODE STORAGE-------------------------------------------------------------------------------------------------------------
// //NOTE: I believe this code allows the texture Image to change when a new seletion is made!
// //		...That's why it's called BEFORE "executeMainLoop()"
// function setupLoadingTextureCallbacks()
// {
//     //handles when another image is selected via the dropdown
// 	$("#texSelect").change(function()
// 	{
// 		$("select option:selected").each(function()
// 		{
//             imageFilename = $(this).text(); // grab the filename from the selection
            
//             loadTexture(gl, imageFilename);
//             /*
//             $.ajax(
// 			      {
//                 url:"./"+filename,
//                 success:loadTexture// --- call the function "loadTexture" when the image is loaded
//             });
//             */
//         });
//     });
// }

// function texBuffers ()
// {
//   const textureCoordBuffer = gl.createBuffer();
//   gl.bindBuffer(gl.ARRAY_BUFFER, textureCoordBuffer);

//   const textureCoordinates = [
//     // Front
//     0.0,  0.0,
//     1.0,  0.0,
//     1.0,  1.0,
//     0.0,  1.0,
//     // Back
//     0.0,  0.0,
//     1.0,  0.0,
//     1.0,  1.0,
//     0.0,  1.0,
//     // Top
//     0.0,  0.0,
//     1.0,  0.0,
//     1.0,  1.0,
//     0.0,  1.0,
//     // Bottom
//     0.0,  0.0,
//     1.0,  0.0,
//     1.0,  1.0,
//     0.0,  1.0,
//     // Right
//     0.0,  0.0,
//     1.0,  0.0,
//     1.0,  1.0,
//     0.0,  1.0,
//     // Left
//     0.0,  0.0,
//     1.0,  0.0,
//     1.0,  1.0,
//     0.0,  1.0,
//   ];

//   gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(textureCoordinates),
//                 gl.STATIC_DRAW);

//   return 
//   {
//     position: positionBuffer,
//     textureCoord: textureCoordBuffer,
//     indices: indexBuffer,
//   };
// }

// function isPowerOf2(value) 
// {
//   return (value & (value - 1)) == 0;
// }