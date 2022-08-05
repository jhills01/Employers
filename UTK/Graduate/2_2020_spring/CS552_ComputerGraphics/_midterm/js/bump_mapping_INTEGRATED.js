function initTextures_BMCube(GC, gl)
{
//console.log("Entered initTextures: ");

    //---GC should now have memory varibles corresponding to new shader variables for processing textures
    getShaderLocations_BMCube(GC, gl); 

    //
    initGeometry_BMCube(GC, gl);
    
    //load textures for Bump Map
    /*
    GC.tex_norm = loadTexture_BMCube(gl, "./imagefiles/bumpMapImages/bump_normal.png");
    GC.tex_diffuse = loadTexture_BMCube(gl, "./imagefiles/bumpMapImages/bump_diffuse.png");
    GC.tex_depth = loadTexture_BMCube(gl, "./imagefiles/bumpMapImages/bump_depth.png");
    //*/
    GC.tex_norm = loadTexture_BMCube(gl, "./imagefiles/bumpMapImages/toy_box_normal.png");
    GC.tex_diffuse = loadTexture_BMCube(gl, "./imagefiles/bumpMapImages/toy_box_diffuse.png");
    GC.tex_depth = loadTexture_BMCube(gl, "./imagefiles/bumpMapImages/toy_box_disp.png");

}

function getShaderLocations_BMCube(GC, gl)
{
    GC.attr_pos = gl.getAttribLocation(GC.shaderProgram, "vert_pos");
    gl.enableVertexAttribArray(GC.attr_pos);
    
    GC.attr_tang = gl.getAttribLocation(GC.shaderProgram, "vert_tang");
    gl.enableVertexAttribArray(GC.attr_tang);
    
    GC.attr_bitang = gl.getAttribLocation(GC.shaderProgram, "vert_bitang");
    gl.enableVertexAttribArray(GC.attr_bitang);
    
    GC.attr_uv = gl.getAttribLocation(GC.shaderProgram, "vert_uv");
    gl.enableVertexAttribArray(GC.attr_uv);
}

function initGeometry_BMCube(GC, gl)
{
        // Positions
        GC.vbo_pos = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, GC.vbo_pos);
        let verts = 
        [
            -1, -1,  1,   1,  1,  1,  -1,  1,  1,   1, -1,  1, // Front
            -1, -1, -1,   1,  1, -1,  -1,  1, -1,   1, -1, -1, // Back
             1, -1, -1,   1,  1,  1,   1, -1,  1,   1,  1, -1, // Right
            -1, -1, -1,  -1,  1,  1,  -1, -1,  1,  -1,  1, -1, // Left
            -1,  1, -1,   1,  1,  1,  -1,  1,  1,   1,  1, -1, // Top
            -1, -1, -1,   1, -1,  1,  -1, -1,  1,   1, -1, -1, // Bottom
        ];
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(verts), gl.STATIC_DRAW);

        // Tangents
        GC.vbo_tang = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, GC.vbo_tang);
        let tangs = 
        [
             1,  0,  0,   1,  0,  0,   1,  0,  0,   1,  0,  0, // Front
            -1,  0,  0,  -1,  0,  0,  -1,  0,  0,  -1,  0,  0, // Back
             0,  0, -1,   0,  0, -1,   0,  0, -1,   0,  0, -1, // Right
             0,  0,  1,   0,  0,  1,   0,  0,  1,   0,  0,  1, // Left
             1,  0,  0,   1,  0,  0,   1,  0,  0,   1,  0,  0, // Top
             1,  0,  0,   1,  0,  0,   1,  0,  0,   1,  0,  0, // Bottom
        ];
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(tangs), gl.STATIC_DRAW);

        // Bitangents
        GC.vbo_bitang = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, GC.vbo_bitang);
        let bitangs = 
        [
             0, -1,  0,   0, -1,  0,   0, -1,  0,   0, -1,  0, // Front
             0, -1,  0,   0, -1,  0,   0, -1,  0,   0, -1,  0, // Back
             0, -1,  0,   0, -1,  0,   0, -1,  0,   0, -1,  0, // Right
             0, -1,  0,   0, -1,  0,   0, -1,  0,   0, -1,  0, // Left
             0,  0,  1,   0,  0,  1,   0,  0,  1,   0,  0,  1, // Top
             0,  0, -1,   0,  0, -1,   0,  0, -1,   0,  0, -1, // Bot
        ];
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(bitangs), gl.STATIC_DRAW);

        // UVs
        GC.vbo_uv = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, GC.vbo_uv);
        let uvs = 
        [
             0,  1,  1,  0,  0,  0,  1,  1, // Front
             1,  1,  0,  0,  1,  0,  0,  1, // Back
             1,  1,  0,  0,  0,  1,  1,  0, // Right
             0,  1,  1,  0,  1,  1,  0,  0, // Left
             0,  0,  1,  1,  0,  1,  1,  0, // Top
             0,  1,  1,  0,  0,  0,  1,  1, // Bottom
        ];
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(uvs), gl.STATIC_DRAW);

        // NOTE: not sure what kind of buffer "gl.ELEMENT_ARRAY_BUFFER" is, or how it's used yet
        GC.index_buffer = gl.createBuffer();
        gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, GC.index_buffer);1
        GC.indices = 
        [
            0 , 1 , 2 ,    0 , 3 , 1 , // Front
            4 , 6 , 5 ,    4 , 5 , 7 , // Back
            8 , 9 , 10,    8 , 11, 9 , // Right
            12, 14, 13,    12, 13, 15, // Left
            16, 18, 17,    16, 17, 19, // Top
            20, 21, 22,    20, 23, 21, // Bottom
        ];
        gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(GC.indices),
                      gl.STATIC_DRAW);

        
}    

function loadTexture_BMCube(gl, img_path) 
{
    var tex = gl.createTexture();
    gl.bindTexture(gl.TEXTURE_2D, tex);
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, 1, 1, 0, gl.RGBA, gl.UNSIGNED_BYTE,
          new Uint8Array([255, 0, 0, 255])); // red

    var img = new Image();
    img.onload = function() 
    {
        gl.bindTexture(gl.TEXTURE_2D, tex);
        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, img);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
    }
    img.src = img_path;
    return tex;
}

function drawScene_BMCube(GC, gl)
{
    {
        gl.activeTexture(gl.TEXTURE0);
        gl.bindTexture(gl.TEXTURE_2D, GC.tex_norm);
        var uni = gl.getUniformLocation(GC.shaderProgram, "tex_norm");
        gl.uniform1i(uni, 0);
    }

    {
        gl.activeTexture(gl.TEXTURE1);
        gl.bindTexture(gl.TEXTURE_2D, GC.tex_diffuse);
        var uni = gl.getUniformLocation(GC.shaderProgram, "tex_diffuse");
        gl.uniform1i(uni, 1);
    }

    {
        gl.activeTexture(gl.TEXTURE2);
        gl.bindTexture(gl.TEXTURE_2D, GC.tex_depth);
        var uni = gl.getUniformLocation(GC.shaderProgram, "tex_depth");
        gl.uniform1i(uni, 2);
    }

    gl.bindBuffer(gl.ARRAY_BUFFER, GC.vbo_pos);
    gl.vertexAttribPointer(GC.attr_pos, 3, gl.FLOAT, false, 0, 0);

    gl.bindBuffer(gl.ARRAY_BUFFER, GC.vbo_tang);
    gl.vertexAttribPointer(GC.attr_tang, 3, gl.FLOAT, false, 0, 0);

    gl.bindBuffer(gl.ARRAY_BUFFER, GC.vbo_bitang);
    gl.vertexAttribPointer(GC.attr_bitang, 3, gl.FLOAT, false, 0, 0);

    gl.bindBuffer(gl.ARRAY_BUFFER, GC.vbo_uv);
    gl.vertexAttribPointer(GC.attr_uv, 2, gl.FLOAT, false, 0, 0);

    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, GC.index_buffer);
    //gl.drawElements(gl.TRIANGLES, num_indices, gl.UNSIGNED_SHORT, 0);
    gl.drawElements(gl.TRIANGLES, GC.indices.length, gl.UNSIGNED_SHORT, 0);
}
