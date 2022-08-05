//NOTE: all functions that DON'T return a value are non-async!!!

//
async function initGL()
{
    //try { gl = canvas.getContext("webgl"); }        
    try { gl = canvas.getContext("webgl2"); }

    catch(e) {console.log("Error initializing Webgl2 context: " + e);}

    if (!gl) 
    {
        alert("Unable to initialize Webgl. Your browser may not support it.");
        //return null;
    }

    // Init gl flags
    {
        gl.clearColor(0.0,0.0,0.0,1.0); //background to black
        gl.clearDepth(1.0);             //set depth to yon plane
        gl.enable(gl.DEPTH_TEST);       //enable depth test
        gl.depthFunc(gl.LEQUAL);        //change depth test to use LEQUAL
        gl.enable(gl.CULL_FACE);
    }
}

//
async function initShaderPgm_GetAttrLocations(so) //so == scene object
{
    //create shader program
    await selectShaders(so.name); //shaders.js

    var frag = get_shader(gl, frag_src, true);
    var vert = get_shader(gl, vert_src, false);
    
    so.pgm = gl.createProgram();
    gl.attachShader(so.pgm, vert);
    gl.attachShader(so.pgm, frag);
    gl.linkProgram(so.pgm);

    if (!gl.getProgramParameter(so.pgm, gl.LINK_STATUS)) 
    {
        console.log("Unable to initialize the shader program: " +
                    gl.getProgramInfoLog(shader));
    }

    //initalize shader program attributes
    gl.useProgram(so.pgm); 
    await getEnableAttrLocations(so);
}

function get_shader(gl, src, is_frag) 
{
    var shader = gl.createShader(is_frag ? gl.FRAGMENT_SHADER :
                                 gl.VERTEX_SHADER);
    gl.shaderSource(shader, src);
    gl.compileShader(shader);

    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
        console.log("(get_shader) An error occurred compiling the shaders: " +
              gl.getShaderInfoLog(shader));
        return null;
    }

    return shader;
}

async function getEnableAttrLocations(so)
{
    switch(so.name)
    {
        case "skybox":
            so.attr_pos = gl.getAttribLocation(so.pgm, "vert_pos");
            gl.enableVertexAttribArray(so.attr_pos);     

            so.attr_norm = gl.getAttribLocation(so.pgm, "vert_norm");
            gl.enableVertexAttribArray(so.attr_norm);
            
            so.attr_uv = gl.getAttribLocation(so.pgm, "vert_uv");
            gl.enableVertexAttribArray(so.attr_uv);   
            break;
        
        case "sun":
            so.attr_pos = gl.getAttribLocation(so.pgm, "vert_pos");
            gl.enableVertexAttribArray(so.attr_pos);

            so.attr_norm = gl.getAttribLocation(so.pgm, "vert_norm");
            gl.enableVertexAttribArray(so.attr_norm);    

            so.attr_uv = gl.getAttribLocation(so.pgm, "vert_uv");
            gl.enableVertexAttribArray(so.attr_uv);        
            break;
        
        case "earth":
            so.attr_pos = gl.getAttribLocation(so.pgm, "vert_pos");
            gl.enableVertexAttribArray(so.attr_pos);
            so.attr_tang = gl.getAttribLocation(so.pgm, "vert_tang");
            gl.enableVertexAttribArray(so.attr_tang);
            so.attr_bitang = gl.getAttribLocation(so.pgm, "vert_bitang");
            gl.enableVertexAttribArray(so.attr_bitang);
            so.attr_uv = gl.getAttribLocation(so.pgm, "vert_uv");
            gl.enableVertexAttribArray(so.attr_uv);        
            break;
        
        case "planet":
            so.attr_pos = gl.getAttribLocation(so.pgm, "vert_pos");
            gl.enableVertexAttribArray(so.attr_pos);

            so.attr_norm = gl.getAttribLocation(so.pgm, "vert_norm");
            gl.enableVertexAttribArray(so.attr_norm);    

            so.attr_uv = gl.getAttribLocation(so.pgm, "vert_uv");
            gl.enableVertexAttribArray(so.attr_uv);        
            break;    
    }
}

async function reenableAttrLocations(so)
{
    switch(so.name)
    {
        case "skybox":
            gl.enableVertexAttribArray(so.attr_pos);
            gl.enableVertexAttribArray(so.attr_norm);
            gl.enableVertexAttribArray(so.attr_uv);
            break;
        
        case "sun":
            gl.enableVertexAttribArray(so.attr_pos);
            gl.enableVertexAttribArray(so.attr_norm);    
            gl.enableVertexAttribArray(so.attr_uv);        
            break;
        
        case "earth":
            gl.enableVertexAttribArray(so.attr_pos);
            gl.enableVertexAttribArray(so.attr_tang);
            gl.enableVertexAttribArray(so.attr_bitang);
            gl.enableVertexAttribArray(so.attr_uv);        
            break;
        
        case "planet":
            gl.enableVertexAttribArray(so.attr_pos);
            gl.enableVertexAttribArray(so.attr_norm);    
            gl.enableVertexAttribArray(so.attr_uv);        
            break;    
    }
}

//
async function disableUnusedAttrLocations(so)
{
    switch(so.name)
    {
        case "skybox":
            gl.disableVertexAttribArray(so.attr_pos);
            gl.disableVertexAttribArray(so.attr_norm);
            gl.disableVertexAttribArray(so.attr_uv);
            break;
        
        case "sun":
            gl.disableVertexAttribArray(so.attr_pos);
            gl.disableVertexAttribArray(so.attr_norm);    
            gl.disableVertexAttribArray(so.attr_uv);        
            break;
        
        case "earth":
            gl.disableVertexAttribArray(so.attr_pos);
            gl.disableVertexAttribArray(so.attr_tang);
            gl.disableVertexAttribArray(so.attr_bitang);
            gl.disableVertexAttribArray(so.attr_uv);        
            break;
        
        case "planet":
            gl.disableVertexAttribArray(so.attr_pos);
            gl.disableVertexAttribArray(so.attr_norm);    
            gl.disableVertexAttribArray(so.attr_uv);        
            break;    
    }
}

//gl GEOMETRY BUFFERS--------------------------------------------------------------------
//
async function initGeometryBuffers(so) //so == scene object
{
    switch(so.name)
    {
        //sun geometry buffers----------------------------------------------------------
        case "skybox":
            console.log("skybox geometry buffers selected");
            await initSkyboxBuffers(so);
            break;
        
        //sun geometry buffers----------------------------------------------------------
        case "sun":
            console.log("sun geometry buffers selected");
            await initSunBuffers(so);
            break;
        
        //earth geometry buffers-------------------------------------------------------
        case "earth":
            console.log("earth geometry buffers selected");
            await initEarthBuffers(so);
            break;
        
        //planet geometry buffers------------------------------------------------------
        case "planet":
            console.log("planet geometry buffers selected");
            await initPlanetBuffers(so);
            break;    
    }
}

async function initSkyboxBuffers(so)
{
        let side = 1.0;
        var skybox = cube(side); //NOTE: this returns cube centered at origin with NEGATIVE vert positions
                                 //      ...Must use vertexTextureCoords to get the correct uv values
console.log(skybox);
        
        // Positions
        so.vbo_pos = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, so.vbo_pos);
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(skybox.vertexPositions), gl.STATIC_DRAW);
        
        // Normals
        so.vbo_norm = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, so.vbo_norm);
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(skybox.vertexNormals), gl.STATIC_DRAW);

        // UVs
        so.vbo_uv = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, so.vbo_uv);
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(skybox.vertexTextureCoords), gl.STATIC_DRAW);

        // Indices for drawElements
        so.index_buffer = gl.createBuffer();
        gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, so.index_buffer);
        gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(skybox.indices), gl.STATIC_DRAW);
        
        so.num_indices = skybox.indices.length;    
} 

async function initSunBuffers(so)
{
    let radius = 1.0, longitudeLines = 32.0, latitudeLinesPlusOne = 16.0;
    var sphere = uvSphere(radius, longitudeLines, latitudeLinesPlusOne);

    // Positions
    so.vbo_pos = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, so.vbo_pos);
    //gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(mesh.model.vCoords), gl.STATIC_DRAW);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(sphere.vertexPositions), gl.STATIC_DRAW);

    // Normals
    so.vbo_norm = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, so.vbo_norm);
    //gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(mesh.model.nCoords), gl.STATIC_DRAW);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(sphere.vertexNormals), gl.STATIC_DRAW);

    // UVs
    so.vbo_uv = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, so.vbo_uv);
    //gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(mesh.model.uvCoords), gl.STATIC_DRAW);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(sphere.vertexTextureCoords), gl.STATIC_DRAW);

    //indices
    so.index_buffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, so.index_buffer);
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(sphere.indices), gl.STATIC_DRAW);
    
    so.num_indices = sphere.indices.length;    
} 

async function initEarthBuffers(so)
{
        // Positions
        so.vbo_pos = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, so.vbo_pos);
        let verts = [
            -1, -1,  1,   1,  1,  1,  -1,  1,  1,   1, -1,  1, // Front
            -1, -1, -1,   1,  1, -1,  -1,  1, -1,   1, -1, -1, // Back
             1, -1, -1,   1,  1,  1,   1, -1,  1,   1,  1, -1, // Right
            -1, -1, -1,  -1,  1,  1,  -1, -1,  1,  -1,  1, -1, // Left
            -1,  1, -1,   1,  1,  1,  -1,  1,  1,   1,  1, -1, // Top
            -1, -1, -1,   1, -1,  1,  -1, -1,  1,   1, -1, -1, // Bottom
        ];
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(verts), gl.STATIC_DRAW);
    
        // Tangents
        so.vbo_tang = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, so.vbo_tang);
        let tangs = [
            1,  0,  0,   1,  0,  0,   1,  0,  0,   1,  0,  0, // Front
            -1,  0,  0,  -1,  0,  0,  -1,  0,  0,  -1,  0,  0, // Back
            0,  0, -1,   0,  0, -1,   0,  0, -1,   0,  0, -1, // Right
            0,  0,  1,   0,  0,  1,   0,  0,  1,   0,  0,  1, // Left
            1,  0,  0,   1,  0,  0,   1,  0,  0,   1,  0,  0, // Top
            1,  0,  0,   1,  0,  0,   1,  0,  0,   1,  0,  0, // Bottom
        ];
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(tangs), gl.STATIC_DRAW);
    
        // Bitangents
        so.vbo_bitang = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, so.vbo_bitang);
        let bitangs = [
            0, -1,  0,   0, -1,  0,   0, -1,  0,   0, -1,  0, // Front
            0, -1,  0,   0, -1,  0,   0, -1,  0,   0, -1,  0, // Back
            0, -1,  0,   0, -1,  0,   0, -1,  0,   0, -1,  0, // Right
            0, -1,  0,   0, -1,  0,   0, -1,  0,   0, -1,  0, // Left
            0,  0,  1,   0,  0,  1,   0,  0,  1,   0,  0,  1, // Top
            0,  0, -1,   0,  0, -1,   0,  0, -1,   0,  0, -1, // Bot
        ];
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(bitangs), gl.STATIC_DRAW);
    
        // UVs
        so.vbo_uv = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, so.vbo_uv);
        let uvs = [
            0,  1,  1,  0,  0,  0,  1,  1, // Front
            1,  1,  0,  0,  1,  0,  0,  1, // Back
            1,  1,  0,  0,  0,  1,  1,  0, // Right
            0,  1,  1,  0,  1,  1,  0,  0, // Left
            0,  0,  1,  1,  0,  1,  1,  0, // Top
            0,  1,  1,  0,  0,  0,  1,  1, // Bottom
        ];
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(uvs), gl.STATIC_DRAW);
    
        so.index_buffer = gl.createBuffer();
        gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, so.index_buffer);
        let indices = [
            0 , 1 , 2 ,    0 , 3 , 1 , // Front
            4 , 6 , 5 ,    4 , 5 , 7 , // Back
            8 , 9 , 10,    8 , 11, 9 , // Right
            12, 14, 13,    12, 13, 15, // Left
            16, 18, 17,    16, 17, 19, // Top
            20, 21, 22,    20, 23, 21, // Bottom
        ];
        gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(indices),
                    gl.STATIC_DRAW);
        
        so.num_indices = indices.length;    
} 

async function initPlanetBuffers(so)
{
    let radius = 1.0, longitudeLines = 32.0, latitudeLinesPlusOne = 16.0;
    var sphere = uvSphere(radius, longitudeLines, latitudeLinesPlusOne);

    // Positions
    so.vbo_pos = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, so.vbo_pos);
    //gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(mesh.model.vCoords), gl.STATIC_DRAW);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(sphere.vertexPositions), gl.STATIC_DRAW);

    // Normals
    so.vbo_norm = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, so.vbo_norm);
    //gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(mesh.model.nCoords), gl.STATIC_DRAW);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(sphere.vertexNormals), gl.STATIC_DRAW);

    // UVs
    so.vbo_uv = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, so.vbo_uv);
    //gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(mesh.model.uvCoords), gl.STATIC_DRAW);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(sphere.vertexTextureCoords), gl.STATIC_DRAW);

    //indices
    so.index_buffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, so.index_buffer);
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(sphere.indices), gl.STATIC_DRAW);
    
    so.num_indices = sphere.indices.length;    
} 

//gl TEXTURES--------------------------------------------------------------------

async function initTextures(so)
{
    switch(so.name)
    {
        //sun textures----------------------------------------------------------
        case "skybox":
            console.log("skybox textures selected");
            load_cubemap(so); //NOTE: array of cubemap image paths is defined within function
            //so.tex = load_texture("./imageFiles/aBMTEST.bmp");
            break;
        
        //sun textures----------------------------------------------------------
        case "sun":
            console.log("sun textures selected");
            so.tex = load_texture("./imageFiles/sun.jpg");
            break;
        
        //earth textures-------------------------------------------------------
        case "earth":
            console.log("earth textures selected");
            so.tex_norm = load_texture("./imageFiles/bumpMapImages/toy_box_normal.png");
            so.tex_diffuse = load_texture("./imageFiles/bumpMapImages/toy_box_diffuse.png");
            so.tex_depth = load_texture("./imageFiles/bumpMapImages/toy_box_disp.png");
            break;
        
        //planet textures------------------------------------------------------
        case "planet":
            console.log("planet textures selected");
            for (let i = 0; i < planetTextures.length; ++i)
            {
                so.planetTextures.push(load_texture(planetTextures[i]));
            } 
            break;    
    }
}

//
function load_texture(img_path) 
{
    var tex = gl.createTexture();
    gl.bindTexture(gl.TEXTURE_2D, tex);
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, 1, 1, 0, gl.RGBA, gl.UNSIGNED_BYTE,
          new Uint8Array([255, 0, 0, 255])); // red

    var img = new Image();
    img.onload = function() {
        gl.bindTexture(gl.TEXTURE_2D, tex);
        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, img);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
    }
    img.src = img_path;
    return tex;
}

function load_cubemap(so)
{
        //get samplerCube from FRAGMENT shader
        //NOTE: right now, only Skybox has texUniLocation member. Will comment out general purpose version for now
        //so.texUniLocation = gl.getUniformLocation(so.pgm, "u_texture");
        sb.texUniLocation = gl.getUniformLocation(sb.pgm, "u_texture");
    
        let cubeMapImage = "./imageFiles/stars_square.jpg"; //NOTE: only one image for now to load on all faces
        //let cubeMapImage = "./imageFiles/aBMTEST.bmp"; //NOTE: only one image for now to load on all faces

        //OLD IMAGE ARRAY
{        
/*
        var imagePaths = ["./imagefiles/environmentCubeMap/plate1.bmp", //+z
                          "./imagefiles/environmentCubeMap/plate2.bmp", //-x
                          "./imagefiles/environmentCubeMap/plate3.bmp", //-y
                          "./imagefiles/environmentCubeMap/plate4.bmp", //+x
                          "./imagefiles/environmentCubeMap/plate5.bmp", //+y: 
                          "./imagefiles/environmentCubeMap/plate6.bmp"  //-z: 
                         ];           
//*/
}    
        //var axis = ["-z", "-x", "+z", "+x", "+y", "-y"];
        //var axis = ["+z", "-x", "-z", "+x", "+y", "-y"];
        //var axis = ["+z", "-x", "-y", "+x", "+y", "-z"];
        let axis = ["+z", "-x", "-y", "+x", "+y", "-z"];
    
        //NOTE: I believe only one texture needs to be created and THEN i can load 6 images and place on 6 faces of cube
        //so.tex = gl.createTexture();
        sb.tex = gl.createTexture();
        gl.bindTexture(gl.TEXTURE_CUBE_MAP, sb.tex);
    
        //This is required to stop "RENDER WARNING: texture bound to texture unit # is not renderable" 
        initCubemapWithSinglePixelTexture(); 
    
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
            //images[i].src = imagePaths[i];
            images[i].src = cubeMapImage;
            images[i].onload = function() { handleCubemapTextureLoaded(sb.tex, images[i], axis[i]); }
        }
}

function initCubemapWithSinglePixelTexture()
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

function handleCubemapTextureLoaded(cubemapTexture, image, axis) 
{
    gl.bindTexture(gl.TEXTURE_CUBE_MAP, cubemapTexture);
   
    //choose which cube map side based on axis
    //each texture passed in should have a different image corresponding to the relevant cube map side
    //NOTE: There should be 6 different textures passed in for cube map
    if (axis == "+x")
    {
        gl.texImage2D(gl.TEXTURE_CUBE_MAP_POSITIVE_X, 0, gl.RGBA, gl.RGBA,
            gl.UNSIGNED_BYTE, image); // typically a different image each face
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

    gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_WRAP_S, gl.REPEAT);
    gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_WRAP_T, gl.REPEAT);
    //gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_WRAP_R, gl.REPEAT);
    gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
    gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_MIN_FILTER, gl.LINEAR); 
}

//CODE STORAGE----------------------------------------------------------------------
/*
    async function originalBumpMappingBufferCode(so)
    {
        // Positions
        so.vbo_pos = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, so.vbo_pos);
        let verts = [
            -1, -1,  1,   1,  1,  1,  -1,  1,  1,   1, -1,  1, // Front
            -1, -1, -1,   1,  1, -1,  -1,  1, -1,   1, -1, -1, // Back
             1, -1, -1,   1,  1,  1,   1, -1,  1,   1,  1, -1, // Right
            -1, -1, -1,  -1,  1,  1,  -1, -1,  1,  -1,  1, -1, // Left
            -1,  1, -1,   1,  1,  1,  -1,  1,  1,   1,  1, -1, // Top
            -1, -1, -1,   1, -1,  1,  -1, -1,  1,   1, -1, -1, // Bottom
        ];
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(verts), gl.STATIC_DRAW);
    
        // Tangents
        so.vbo_tang = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, so.vbo_tang);
        let tangs = [
            1,  0,  0,   1,  0,  0,   1,  0,  0,   1,  0,  0, // Front
            -1,  0,  0,  -1,  0,  0,  -1,  0,  0,  -1,  0,  0, // Back
            0,  0, -1,   0,  0, -1,   0,  0, -1,   0,  0, -1, // Right
            0,  0,  1,   0,  0,  1,   0,  0,  1,   0,  0,  1, // Left
            1,  0,  0,   1,  0,  0,   1,  0,  0,   1,  0,  0, // Top
            1,  0,  0,   1,  0,  0,   1,  0,  0,   1,  0,  0, // Bottom
        ];
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(tangs), gl.STATIC_DRAW);
    
        // Bitangents
        so.vbo_bitang = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, so.vbo_bitang);
        let bitangs = [
            0, -1,  0,   0, -1,  0,   0, -1,  0,   0, -1,  0, // Front
            0, -1,  0,   0, -1,  0,   0, -1,  0,   0, -1,  0, // Back
            0, -1,  0,   0, -1,  0,   0, -1,  0,   0, -1,  0, // Right
            0, -1,  0,   0, -1,  0,   0, -1,  0,   0, -1,  0, // Left
            0,  0,  1,   0,  0,  1,   0,  0,  1,   0,  0,  1, // Top
            0,  0, -1,   0,  0, -1,   0,  0, -1,   0,  0, -1, // Bot
        ];
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(bitangs), gl.STATIC_DRAW);
    
        // UVs
        so.vbo_uv = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, so.vbo_uv);
        let uvs = [
            0,  1,  1,  0,  0,  0,  1,  1, // Front
            1,  1,  0,  0,  1,  0,  0,  1, // Back
            1,  1,  0,  0,  0,  1,  1,  0, // Right
            0,  1,  1,  0,  1,  1,  0,  0, // Left
            0,  0,  1,  1,  0,  1,  1,  0, // Top
            0,  1,  1,  0,  0,  0,  1,  1, // Bottom
        ];
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(uvs), gl.STATIC_DRAW);
    
        so.index_buffer = gl.createBuffer();
        gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, so.index_buffer);
        let indices = [
            0 , 1 , 2 ,    0 , 3 , 1 , // Front
            4 , 6 , 5 ,    4 , 5 , 7 , // Back
            8 , 9 , 10,    8 , 11, 9 , // Right
            12, 14, 13,    12, 13, 15, // Left
            16, 18, 17,    16, 17, 19, // Top
            20, 21, 22,    20, 23, 21, // Bottom
        ];
        gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(indices),
                    gl.STATIC_DRAW);
        
        so.num_indices = indices.length;    
    }


//*/