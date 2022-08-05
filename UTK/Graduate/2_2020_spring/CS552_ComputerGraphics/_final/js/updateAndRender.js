async function update_and_render()
{
    //NOTE: skybox updating SHOULDN"T be necessary. Draw once, and forget it. 
    //      ...if not, easy fix: create "update_and_render_skybox"!
    
    if (!time_paused) 
    {
        time++;
        //time = (time + 1) % 837.0;
    }

    switch (document.querySelector('input[name="shading_type"]:checked').value) 
    {
        case "shadowsOFF":
            sunlightZ = 0.0;
            break;
        case "shadowsON": 
            sunlightZ = camera.position[2];
            break;
    }

    //NOTE: originally, gl.clear was in all render functions [sun(), planets(), etc]. 
    //      ...HOWEVER, this was causing some objects not to be drawn. 
    //      ...E.g., calling sun() --> planets(2) caused only the 2 planets to be drawn
    //      ...turns out, I was calling gl.clear in planets(2) which apparently cleared sun() render. OOPS! 
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    await render_skybox();

    //update and render individual heavenly bodies
    await update_and_render_sun();
    await update_and_render_earth();
    await update_and_render_planets(8);
}

//draw skybox
async function render_skybox()
    {
        gl.useProgram(sb.pgm);

        //disable unused attributes from other shader programs.
        //if this isn't performed, "drawElements: no buffer is bound to enabled attribute" error...
        //could occur
        disableUnusedAttrLocations(s);  //disable sun
        disableUnusedAttrLocations(e);  //disable earth
        disableUnusedAttrLocations(p);  //disable planets
        reenableAttrLocations(sb);      //re-enable skybox

        let pers_matrix = mtx_perspective(45, width/height, 0.1, 2000.0);
        //let b = mtx_translation(0.0, 0.0, camera.position[2]); 
        let b = mtx_translation(0.0, 0.0, 100.0); 
        //let model = mtx_mul(a,b); //GOD DAMMIT ALL!!!! I'm multiplying by perspective TWICE!
        let model = b; //GOD DAMMIT ALL!!!! I'm multiplying by perspective TWICE!
        
        //let sbSize = mtx_scale(3500.0, 3500.0, 3500.0);
        let sbSize = mtx_scale(4200.0, 4200.0, 4200.0);
        model = mtx_mul(model, sbSize);

        {
            let uni = gl.getUniformLocation(sb.pgm, "model_mtx");
            gl.uniformMatrix4fv(uni, false, model);
        }

        {
            let uni = gl.getUniformLocation(sb.pgm, "norm_mtx");
            gl.uniformMatrix4fv(uni, false, mtx_transpose(mtx_inverse(model)));
        }

        {
            let uni = gl.getUniformLocation(sb.pgm, "proj_mtx");
            gl.uniformMatrix4fv(uni, false, mtx_mul(pers_matrix, model));
        }

        {
            gl.uniform1i(sb.texUniLocation, 0);
        }

        gl.bindBuffer(gl.ARRAY_BUFFER, sb.vbo_pos);
        gl.vertexAttribPointer(sb.attr_pos, 3, gl.FLOAT, false, 0, 0);

        //NOTE: forgot to add this! Was getting drawElement/no buffer is bound..." error
        gl.bindBuffer(gl.ARRAY_BUFFER, sb.vbo_norm);
        gl.vertexAttribPointer(sb.attr_norm, 3, gl.FLOAT, false, 0, 0);

        gl.bindBuffer(gl.ARRAY_BUFFER, sb.vbo_uv);
        gl.vertexAttribPointer(sb.attr_uv, 2, gl.FLOAT, false, 0, 0);

        gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, sb.index_buffer);
        gl.drawElements(gl.TRIANGLES, sb.num_indices, gl.UNSIGNED_SHORT, 0);
    }    

async function update_and_render_sun()
{
    //choose program with correct shaders
    gl.useProgram(s.pgm); 

    //disable unused attributes from other shader programs.
    //if this isn't performed, "drawElements: no buffer is bound to enabled attribute" error...
    //could occur
    disableUnusedAttrLocations(sb); //disable skybox
    disableUnusedAttrLocations(e);  //disable earth
    disableUnusedAttrLocations(p);  //disable planets
    reenableAttrLocations(s);       //re-enable sun

    //matrix looks at origin and zooms along the -z axis
    let pers_matrix = mtx_perspective(45, width/height, 0.1, 2000.0);
    let b, c, d, orbitalRadius;
    //camera.position[0] = 0.0;
    
    for (let i = 0; i < 1; ++i)
    {
        //---create/apply matrix transformations and send results to shaders
        b = mtx_translation(0.0, 0.0, camera.position[2]); //zoom in/out from world origin with all planets @ origin
        //c = mtx_rotation_x(0.4);
        //c = mtx_rotation_x(Math.PI/2 + 0.4);
        c = mtx_rotation_x(Math.PI/2 + 0.2);
        //d = mtx_rotation_y(-time * 0.0075);
        d = mtx_rotation_z(time * 0.002);
        
        //d = mtx_rotation_z(0.75*time);
        //f = mtx_rotation_y(0.75*time);
        //g = mtx_rotation_x(0.75*time);

        orbitalRadius = mtx_translation(sunOrbitalRadius, 0.0, 0.0);
        //scaleFactor = 4;
        //planetSize = mtx_scale(scaleFactor, scaleFactor, scaleFactor); 
        
        planetSize = mtx_scale(sunScale, sunScale, sunScale); 

        //VERY VERY IMPORTANT NOTE:
        //In vertex shader, I was applying the model matrix TWICE to projection matrix!!!!
            /*
                vec4 vert_pos4 = model_mtx * vec4(vert_pos, 1.0);
                gl_Position = proj_mtx * vert_pos4; //OOOOOOPS!!
            */
        //OOPPSSSS!!!!
        //Correction:  gl_Position = proj_mtx * vec4(vert_pos, 1.0);
        let model = mtx_mul(b, c);
        model = mtx_mul(model, d);

        // model = mtx_mul(model, f);
        // model = mtx_mul(model, g);

        model = mtx_mul(model, orbitalRadius);
        model = mtx_mul(model, planetSize);

        {
            let uni = gl.getUniformLocation(s.pgm, "model_mtx");
            gl.uniformMatrix4fv(uni, false, model);
        }

        {
            let uni = gl.getUniformLocation(s.pgm, "norm_mtx");
            gl.uniformMatrix4fv(uni, false, mtx_transpose(mtx_inverse(model)));
        }

        {
            let uni = gl.getUniformLocation(s.pgm, "proj_mtx");
            gl.uniformMatrix4fv(uni, false, mtx_mul(pers_matrix, model));
        }

        //---prepare and send textures to shaders
        {
            gl.activeTexture(gl.TEXTURE0);
            gl.bindTexture(gl.TEXTURE_2D, s.tex);
            let uni = gl.getUniformLocation(s.pgm, "tex");
            gl.uniform1i(uni, 0);
        }

        //---send mesh geometry data to shaders
        gl.bindBuffer(gl.ARRAY_BUFFER, s.vbo_pos);
        gl.vertexAttribPointer(s.attr_pos, 3, gl.FLOAT, false, 0, 0);

        gl.bindBuffer(gl.ARRAY_BUFFER, s.vbo_norm);
        gl.vertexAttribPointer(s.attr_norm, 3, gl.FLOAT, false, 0, 0);

        gl.bindBuffer(gl.ARRAY_BUFFER, s.vbo_uv);
        gl.vertexAttribPointer(s.attr_uv, 2, gl.FLOAT, false, 0, 0);

        //---draw result
        gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, s.index_buffer);
        gl.drawElements(gl.TRIANGLES, s.num_indices, gl.UNSIGNED_SHORT, 0);
        
        //gl.drawArrays(gl.TRIANGLES, 0, mesh.model.vIndices.length);
    }
}

async function update_and_render_earth()
{
    //choose program with correct shaders
    gl.useProgram(e.pgm); 

    //disable unused attributes from other shader programs.
    //if this isn't performed, "drawElements: no buffer is bound to enabled attribute" error...
    //could occur
    disableUnusedAttrLocations(sb); //disable skybox
    disableUnusedAttrLocations(s);  //disable sun
    disableUnusedAttrLocations(p);  //disable planets
    reenableAttrLocations(e);       //re-enable earth
    
    //matrix looks at origin and zooms along the -z axis
    let pers_matrix = mtx_perspective(45, width/height, 0.1, 2000.0);
    let b, c, d, orbitalRadius;
    
    for (let i = 0; i < 1; ++i)
    {
        b = mtx_translation(0.0, 0.0, camera.position[2]); //zoom in/out from world origin with all planets @ origin
        //c = mtx_rotation_x(0.4);
        c = mtx_rotation_x(0.2);
        //d = mtx_rotation_y(time * 0.0075);
        //d = mtx_rotation_y(time * earthOrbitalSpeed);
        d = mtx_rotation_y(time * earthOrbitalSpeed * (speedChange/50));
        orbitalRadius = mtx_translation(earthOrbitalRadius, 0.0, 0.0);
        //scaleFactor = 0.5;
        //planetSize = mtx_scale(scaleFactor, scaleFactor, scaleFactor); 
        planetSize = mtx_scale(earthScale, earthScale, earthScale); 
        
        let model = mtx_mul(mtx_mul(b, c), d);
        model = mtx_mul(model, orbitalRadius);
        model = mtx_mul(model, planetSize);

        {
            let uni = gl.getUniformLocation(e.pgm, "model_mtx");
            gl.uniformMatrix4fv(uni, false, model);
        }

        {
            let uni = gl.getUniformLocation(e.pgm, "norm_mtx");
            gl.uniformMatrix4fv(uni, false, mtx_transpose(mtx_inverse(model)));
        }

        {
            let uni = gl.getUniformLocation(e.pgm, "proj_mtx");
            gl.uniformMatrix4fv(uni, false, mtx_mul(pers_matrix, model));
        }

        {
            gl.activeTexture(gl.TEXTURE0);
            gl.bindTexture(gl.TEXTURE_2D, e.tex_norm);
            let uni = gl.getUniformLocation(e.pgm, "tex_norm");
            gl.uniform1i(uni, 0);
        }

        {
            gl.activeTexture(gl.TEXTURE1);
            gl.bindTexture(gl.TEXTURE_2D, e.tex_diffuse);
            let uni = gl.getUniformLocation(e.pgm, "tex_diffuse");
            gl.uniform1i(uni, 1);
        }

        {
            gl.activeTexture(gl.TEXTURE2);
            gl.bindTexture(gl.TEXTURE_2D, e.tex_depth);
            let uni = gl.getUniformLocation(e.pgm, "tex_depth");
            gl.uniform1i(uni, 2);
        }

        {
            let uni = gl.getUniformLocation(e.pgm, "sun_light_Z");
            gl.uniform1f(uni, sunlightZ);
        }


        gl.bindBuffer(gl.ARRAY_BUFFER, e.vbo_pos);
        gl.vertexAttribPointer(e.attr_pos, 3, gl.FLOAT, false, 0, 0);

        gl.bindBuffer(gl.ARRAY_BUFFER, e.vbo_tang);
        gl.vertexAttribPointer(e.attr_tang, 3, gl.FLOAT, false, 0, 0);

        gl.bindBuffer(gl.ARRAY_BUFFER, e.vbo_bitang);
        gl.vertexAttribPointer(e.attr_bitang, 3, gl.FLOAT, false, 0, 0);

        gl.bindBuffer(gl.ARRAY_BUFFER, e.vbo_uv);
        gl.vertexAttribPointer(e.attr_uv, 2, gl.FLOAT, false, 0, 0);

        gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, e.index_buffer);
        gl.drawElements(gl.TRIANGLES, e.num_indices, gl.UNSIGNED_SHORT, 0);
    }
}

async function update_and_render_planets(numOfPlanets)
{
    //choose program with correct shaders
    gl.useProgram(p.pgm); 
    
    //disable unused attributes from other shader programs.
    //if this isn't performed, "drawElements: no buffer is bound to enabled attribute" error...
    //could occur
    disableUnusedAttrLocations(sb); //disable skybox
    disableUnusedAttrLocations(s);  //disable sun
    disableUnusedAttrLocations(e);  //disable earth
    reenableAttrLocations(p);       //re-enable planets

    //matrix looks at origin and zooms along the -z axis
    let pers_matrix = mtx_perspective(45, width/height, 0.1, 2000.0);
    let b, c, d, orbitalRadius, planetSize;
    
    for (let i = 0; i < numOfPlanets; ++i)
    {
        b = mtx_translation(0.0, 0.0, camera.position[2]); //zoom in/out from world origin with all planets @ origin
        c = mtx_rotation_x(Math.PI/2 + 0.2);
        //c = mtx_rotation_x(Math.PI/2 + 0.4);
        //d = mtx_rotation_z(-time * planetOrbitalSpeed[i]);
        d = mtx_rotation_z(-time * planetOrbitalSpeed[i] * (speedChange/50) );
        orbitalRadius = mtx_translation(planetOrbitalRadius[i], 0.0, 0.0);
        //scaleFactor = 1;
        //planetSize = mtx_scale(scaleFactor, scaleFactor, scaleFactor); 
        planetSize = mtx_scale(planetScale[i], planetScale[i], planetScale[i]); 

        let model = mtx_mul(b, c);
        model = mtx_mul(model, d);
        model = mtx_mul(model, orbitalRadius);
        model = mtx_mul(model, planetSize);

        {
            let uni = gl.getUniformLocation(p.pgm, "model_mtx");
            gl.uniformMatrix4fv(uni, false, model);
        }

        {
            let uni = gl.getUniformLocation(p.pgm, "norm_mtx");
            gl.uniformMatrix4fv(uni, false, mtx_transpose(mtx_inverse(model)));
        }

        {
            let uni = gl.getUniformLocation(p.pgm, "proj_mtx");
            gl.uniformMatrix4fv(uni, false, mtx_mul(pers_matrix, model));
        }

        {
            let uni = gl.getUniformLocation(p.pgm, "sun_light_Z");
            gl.uniform1f(uni, sunlightZ);
        }

        //---prepare and send textures to shaders
        {
            //p.tex = p.planetTextures[i];
            gl.activeTexture(gl.TEXTURE0);
            //gl.bindTexture(gl.TEXTURE_2D, p.tex);
            gl.bindTexture(gl.TEXTURE_2D, p.planetTextures[i]);
            let uni = gl.getUniformLocation(p.pgm, "tex");
            gl.uniform1i(uni, 0);
        }

        //---send mesh geometry data to shaders
        gl.bindBuffer(gl.ARRAY_BUFFER, p.vbo_pos);
        gl.vertexAttribPointer(p.attr_pos, 3, gl.FLOAT, false, 0, 0);

        gl.bindBuffer(gl.ARRAY_BUFFER, p.vbo_norm);
        gl.vertexAttribPointer(p.attr_norm, 3, gl.FLOAT, false, 0, 0);

        gl.bindBuffer(gl.ARRAY_BUFFER, p.vbo_uv);
        gl.vertexAttribPointer(p.attr_uv, 2, gl.FLOAT, false, 0, 0);

        //---draw result
        gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, p.index_buffer);
        gl.drawElements(gl.TRIANGLES, p.num_indices, gl.UNSIGNED_SHORT, 0);
    }
}
