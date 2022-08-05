function initCubeMap(GC, gl) 
{
    //get samplerCube from FRAGMENT shader
    GC.textureLocation = gl.getUniformLocation(GC.shaderProgram, "u_texture");
    
    var imagePaths = ["./imagefiles/environmentCubeMap/plate1.bmp", //+z
                      "./imagefiles/environmentCubeMap/plate2.bmp", //-x
                      "./imagefiles/environmentCubeMap/plate3.bmp", //-y
                      "./imagefiles/environmentCubeMap/plate4.bmp", //+x
                      "./imagefiles/environmentCubeMap/plate5.bmp", //+y: 
                      "./imagefiles/environmentCubeMap/plate6.bmp"  //-z: 
                     ];           

    //var axis = ["-z", "-x", "+z", "+x", "+y", "-y"];
    //var axis = ["+z", "-x", "-z", "+x", "+y", "-y"];
    //var axis = ["+z", "-x", "-y", "+x", "+y", "-z"];
    var axis = ["+z", "-x", "-y", "+x", "+y", "-z"];
    //var cubemapTexture = [];

    //NOTE: I believe only one texture needs to be created and THEN i can load 6 images and place on 6 faces of cube
    var cubemapTexture = gl.createTexture();
    gl.bindTexture(gl.TEXTURE_CUBE_MAP, cubemapTexture);

    //This is required to stop "RENDER WARNING: texture bound to texture unit # is not renderable" 
    initCubemapWithSinglePixelTexture(gl); 

    //NOTE: I'm not sure if I need to keep an array of images!!! Just one var image might suffice!!
    //      Just keep overwriting it with a new Image@!
    var images = [];
    for (let i = 0; i < 6; ++i)
    {
        //webglfundamentals.org recommended I setup this up so face is immediately renderable
        //notice the NULL in place of "image" in the last parameter
        //gl.texImage2D(target, level, internalFormat, width, height, 0, format, type, null);
        
        //NOTE: I'm not sure if I need to keep an array of images!!! Just one var image might suffice!!
        //      Just keep overwriting it with a new Image@!
        images.push( new Image() );
        images[i].src = imagePaths[i];
        images[i].onload = function() { handleCubemapTextureLoaded(cubemapTexture, images[i], axis[i]); }
        //cubemapTexture.push( gl.createTexture() );
        //cubemapTexture[i].image = new Image();
        //cubemapTexture[i].image.onload = function() { handleCubemapTextureLoaded(cubemapTexture[i], axis[i]); }
        //cubemapTexture[i].image.src = images[i]; 
    }

    GC.texture = cubemapTexture;
} 

function initCubemapWithSinglePixelTexture(gl)
{
    const level = 0;
    const internalFormat = gl.RGBA;
    const width = 1;
    const height = 1;
    const border = 0;
    const srcFormat = gl.RGBA;
    const srcType = gl.UNSIGNED_BYTE;
    const pixel = new Uint8Array([0, 0, 255, 255]);  // opaque blue

    gl.texImage2D(gl.TEXTURE_CUBE_MAP_POSITIVE_Y, level, internalFormat,
                width, height, border, srcFormat, srcType,
                pixel);

    gl.texImage2D(gl.TEXTURE_CUBE_MAP_NEGATIVE_Y, level, internalFormat,
                width, height, border, srcFormat, srcType,
                pixel);


    gl.texImage2D(gl.TEXTURE_CUBE_MAP_POSITIVE_Z, level, internalFormat,
                width, height, border, srcFormat, srcType,
                pixel);

    gl.texImage2D(gl.TEXTURE_CUBE_MAP_NEGATIVE_Z, level, internalFormat,
                width, height, border, srcFormat, srcType,
                pixel);

    gl.texImage2D(gl.TEXTURE_CUBE_MAP_POSITIVE_X, level, internalFormat,
                width, height, border, srcFormat, srcType,
                pixel);

    gl.texImage2D(gl.TEXTURE_CUBE_MAP_NEGATIVE_X, level, internalFormat,
                width, height, border, srcFormat, srcType,
                pixel);
} 

/*
function handleCubemapTextureLoaded(texture, axis) 
{
console.log("texture");
console.log(texture);
console.log("axis");
console.log(axis);
//*/  
    //gl.bindTexture(gl.TEXTURE_CUBE_MAP, texture);

function handleCubemapTextureLoaded(cubemapTexture, image, axis) 
{
//console.log("image");
//console.log(image);
//console.log("axis");
//console.log(axis);

    gl.bindTexture(gl.TEXTURE_CUBE_MAP, cubemapTexture);
   
    /*
    //Once texture is bound to TEXTURE_CUBE_MAP, we can then set all properties for it
    //NOTE: Question! Do I need to call these every time I send in a new image!
    gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_WRAP_S, gl.REPEAT);
    gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_WRAP_T, gl.REPEAT);
    //gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_WRAP_R, gl.REPEAT);
    gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
    gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_MIN_FILTER, gl.LINEAR); 
    //*/

    //choose which cube map side based on axis
    //each texture passed in should have a different image corresponding to the relevant cube map side
    //NOTE: There should be 6 different textures passed in for cube map
    if (axis == "+x")
    {
        gl.texImage2D(gl.TEXTURE_CUBE_MAP_POSITIVE_X, 0, gl.RGBA, gl.RGBA,
            gl.UNSIGNED_BYTE, image); // again, different image each face
    }
    else if (axis == "-x") 
    {  
        gl.texImage2D(gl.TEXTURE_CUBE_MAP_NEGATIVE_X, 0, gl.RGBA, gl.RGBA,
            gl.UNSIGNED_BYTE, image);
    }
    else if (axis == "+y")   
    {
        gl.texImage2D(gl.TEXTURE_CUBE_MAP_POSITIVE_Y, 0, gl.RGBA, gl.RGBA,
            gl.UNSIGNED_BYTE, image);
    }   
    else if (axis == "-y")   
    {
        gl.texImage2D(gl.TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, gl.RGBA, gl.RGBA,
            gl.UNSIGNED_BYTE, image);
    }   
    else if (axis == "+z")   
    {
        gl.texImage2D(gl.TEXTURE_CUBE_MAP_POSITIVE_Z, 0, gl.RGBA, gl.RGBA,
            gl.UNSIGNED_BYTE, image);
    }
    else if (axis == "-z")   
    {   
        gl.texImage2D(gl.TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, gl.RGBA, gl.RGBA,
            gl.UNSIGNED_BYTE, image); 
    }
    else console.log("ERROR! 'axis' parameter in 'handleCubemapTextureLoaded' is invalid!");

    mipMap_OR_texparameter(gl, image);
}

function mipMap_OR_texparameter(gl, image)
{
    gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_WRAP_S, gl.REPEAT);
    gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_WRAP_T, gl.REPEAT);
    //gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_WRAP_R, gl.REPEAT);
    gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
    gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_MIN_FILTER, gl.LINEAR); 

    /*
    if (isPowerOf2(image.width) && isPowerOf2(image.height)) 
    {
       // Yes, it's a power of 2. Generate mips.
       gl.generateMipmap(gl.TEXTURE_CUBE_MAP);
       
       //NOTE: Not sure about this line
       //gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_LINEAR);

       console.log("mipmap created!");
    } 
    
    else 
    {
       // No, it's not a power of 2. Turn off mips and set
       // wrapping to clamp to edge
       
       gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_WRAP_S, gl.REPEAT);
       gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_WRAP_T, gl.REPEAT);
       //gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_WRAP_R, gl.REPEAT);
       gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
       gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_MIN_FILTER, gl.LINEAR); 
   
       console.log("mipmap COULD NOT BE created!");
    }
    //*/
}


function addCubeMapToDrawScene(GC, gl)
{
    //gl.activeTexture(gl.TEXTURE0);

    // Bind the texture to texture unit 0
    //gl.bindTexture(gl.TEXTURE_2D, texture);
    //gl.bindTexture(gl.TEXTURE_CUBE_MAP, GC.texture); //GC.texture created in loadTexture
  

    // Tell the shader to use texture unit 0 for u_texture
    gl.uniform1i(GC.textureLocation, 0);
}