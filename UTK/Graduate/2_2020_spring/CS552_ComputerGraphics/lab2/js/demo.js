var gl = null; //our OpenGL handler

var GC = {};   //the graphics context

//initialize the graphics context variables
GC.shaderProgram = null;          //our GLSL program
GC.vertexPositionAttribute = null;//location of vertex positions in GLSL program
GC.vNormalAttribute = null;       //location of normal positions in GLSL program
GC.redYGradientAttribute = null;  //location of red gradient along y in GLSL
GC.perspectiveMatrix = null;      //the Perspective matrix
GC.mvMatrix = null;               //the ModelView matrix
GC.mvMatrixStack = [];            //the ModelView matrix stack
GC.mesh = null;                   //the current mesh
GC.mouseDown = null;              //boolean check for mouseDown
GC.width = 640;                   //render area width
GC.height = 480;                  //render area height
GC.normalMatrix = 0;
GC.camPos = [0.0, 0.0, 0.0];


var camera = new ArcBall();              //create a new arcball camera
camera.setBounds(GC.width,GC.height);    //initialize camera with screen space dimensions

//demo constructor
function demo(canvasName,Mesh) 
{
    this.canvasName = canvasName;
    GC.mesh = Mesh;
}

//initialize webgl, populate all buffers, load shader programs, and start drawing
demo.prototype.init = function()
{
    this.canvas = document.getElementById(this.canvasName);
    this.canvas.width = GC.width;
    this.canvas.height = GC.height;

    //Here we check to see if WebGL is supported 
    //NOTE: separate function below that simple calls "gl = this.canvas.getContext("experimental-webgl");"
    this.initWebGL(this.canvas);

    //NOTE: gl is set by "this.canvas.getContext("experimental-webgl");" line in initWebGL function BELOW!
    gl.clearColor(0.0,0.0,0.0,1.0);     //background to black
    gl.clearDepth(1.0);                 //set depth to yon plane
    gl.enable(gl.DEPTH_TEST);           //enable depth test
    gl.depthFunc(gl.LEQUAL);            //change depth test to use LEQUAL

    //set mouse event callbacks
    this.setMouseEventCallbacks();

    //set keyboard event callbacks
    this.setKeyboardEventCallbacks();

    //Get opengl derivative extension -- enables using fwidth in shader
    //NOTE!!!: This step is probably not necessary because I'm using WebGL2!!!! This code was designed for WebGL1
    gl.getExtension("OES_standard_derivatives");
    
    //init the shader programs --- STUDIED
    this.initShaders();

    //init the vertex buffer --- STUDIED
    this.initGeometryBuffers();
}

//initialize webgl
demo.prototype.initWebGL = function()
{
    gl = null;

    try 
    {
        gl = this.canvas.getContext("experimental-webgl");
    }
    catch(e) 
    {
        //pass through
    }

    // If we don't have a GL context, give up now
    if (!gl) 
    {
        alert("Unable to initialize WebGL. Your browser may not support it.");
    }
}

//initialize the shaders and grab the shader variable attributes
demo.prototype.initShaders = function()
{
    //Load the shaders
    //NOTE: "getShader" method defined below --- it compiles sourceCode from html into returned shader object
    var fragmentShader = this.getShader("FragmentShader1");
    var vertexShader = this.getShader("VertexShader1");

    this.shaderProgram = gl.createProgram();
    gl.attachShader(this.shaderProgram, vertexShader);
    gl.attachShader(this.shaderProgram, fragmentShader);
    gl.linkProgram(this.shaderProgram);

    if(!gl.getProgramParameter(this.shaderProgram, gl.LINK_STATUS))
    {
        console.log("unable to init shader program");
    }

    gl.useProgram(this.shaderProgram);

    //vertex array
    GC.vertexPositionAttribute = gl.getAttribLocation(this.shaderProgram, "vPos");
    gl.enableVertexAttribArray(GC.vertexPositionAttribute);

    //fragment normal array    
    GC.vNormalAttribute = gl.getAttribLocation(this.shaderProgram, "vNormal");
    gl.enableVertexAttribArray(GC.vNormalAttribute);

    //red gradient along y array    
    GC.redYGradientAttribute = gl.getAttribLocation(this.shaderProgram, "redYGradient");
    gl.enableVertexAttribArray(GC.redYGradientAttribute);

    GC.shaderProgram = this.shaderProgram;
}

//compile shader located within a script tag
demo.prototype.getShader = function(id)
{
    var shaderScript, theSource, currentChild, shader;

    shaderScript = document.getElementById(id);
    if(!shaderScript)
    {
        return null;
    }

    //init the source code variable
    theSource = "";

    //begin reading the shader source from the beginning
    currentChild = shaderScript.firstChild;

    //read the shader source as text
    while(currentChild)
    {
        if(currentChild.nodeType == currentChild.TEXT_NODE)
        {
            theSource += currentChild.textContent;
        }
        currentChild = currentChild.nextSibling;
    }

    //check type of shader to give openGL the correct hint
    if(shaderScript.type == "x-shader/x-fragment")
    {
        shader = gl.createShader(gl.FRAGMENT_SHADER);
    } 
    else if(shaderScript.type == "x-shader/x-vertex")
    {
        shader = gl.createShader(gl.VERTEX_SHADER);
    } 
    else 
    {
        return null;
    }
    
    //add the shader source code to the created shader object
    gl.shaderSource(shader, theSource);

    //compile the shader
    gl.compileShader(shader);

    if(!gl.getShaderParameter(shader, gl.COMPILE_STATUS))
    {
        console.log("error compiling shaders -- " + gl.getShaderInfoLog(shader));
        return null;
    }

    return shader;
}

//initialize the buffers for drawing and the edge highlights
demo.prototype.initGeometryBuffers = function()
{
  //NOTE: mesh is an object created in processMesh of main.js with "model" member that stores meshLoader.Mesh data
  //...basically, .model is the "Mesh" member of modelLoader 
  var m = GC.mesh.model;

  var verts = [];                   //array to hold vertices laid out according to "indices" (i.e., triangle vertices)
  var nVerts = [];                  //array to hold vertex normals laid according to "indices" (i.e., triangle vertices)
  var redYGradient = [];
  var bary = [];                    //array of 1s and 0s passed to GLSL to draw wireframe
  var min = [90000,90000,90000];    //used for bounding box calculations
  var max = [-90000,-90000,-90000]; //used for bounding box calculations

    // Loop through the indices array and create a vertices array (this means
    // ...duplicating data) from the listed indices
    //"indices" contains vertex indices of each triangle
    m.indices.forEach(function(d,i)
    {
        //grab the x,y,z position and normal values for the current vertex
        //NOTE: "parseFloat" converts the strings from "vertices" to numbers
        vx = (parseFloat(m.vertices[d*3]));
        vy = (parseFloat(m.vertices[d*3+1]));
        vz = (parseFloat(m.vertices[d*3+2]));

        nx = m.vertNorms[d*3];
        ny = m.vertNorms[d*3+1];
        nz = m.vertNorms[d*3+2];

        //add this vertex to our array
        verts.push(vx,vy,vz);
        nVerts.push(nx, ny, nz);

        //check to see if we need to update the min/max
        if(vx < min[0]) min[0] = vx;
        if(vy < min[1]) min[1] = vy;
        if(vz < min[2]) min[2] = vz;
        if(vx > max[0]) max[0] = vx;
        if(vy > max[1]) max[1] = vy;
        if(vz > max[2]) max[2] = vz;

        //What does this do?
        //Answer: Looks like unit vectors for each x,y,z triplet
        //        ...x: 0,3,6,9 -> y: 1,4,7,10 -> z: 2,5,8,11
        if(i%3 == 0)
        {
            bary.push(1,0,0);
        } 
        else if(i % 3 == 1)
        {
            bary.push(0,1,0);
        } 
        else if(i % 3 == 2)
        {
            bary.push(0,0,1);
        }
    });

  //set the min/max variables
  m.minX = min[0]; m.minY = min[1]; m.minZ = min[2];
  m.maxX = max[0]; m.maxY = max[1]; m.maxZ = max[2];

  m.indices.forEach(function(d,i)
  {
      //grab the x,y,z position 
      vx = (parseFloat(m.vertices[d*3]));
      vy = (parseFloat(m.vertices[d*3+1]));
      vz = (parseFloat(m.vertices[d*3+2]));

      //calculate what the red value should be based on linear relation:
      //Red = [255/(ymax - ymin)]*[y - ymin]
      //NOTE: I forgot the RGB values range from 0.0 to 1.0 in gl_FragColor!!
      //      ...need to divde result by 255!!
      red = ( (255.0/(m.maxY - m.minY))*(vy - m.minY) ) / 255.0;
      
      //add this vertex to our array
      redYGradient.push(-1.0,red,-1.0); //using -1 in hopes it will throw error if I use wrong
                                        //...value in shader
  });

  //calculate the largest range in x,y,z
  var s = Math.max( Math.abs(min[0]-max[0]),
                    Math.abs(min[1]-max[1]),
                    Math.abs(min[2]-max[2]))
  
  //calculate the distance to place camera from model
  var d = (s/2.0)/Math.tan(45/2.0);

  //place the camera at the calculated position
  camera.position[2] = d;

  //orient the camera to look at the center of the model
  camera.lookAt = [(m.minX+m.maxX)/2.0,(m.minY+m.maxY)/2.0,(m.minZ+m.maxZ)/2.0];

  //create an OpenGL buffers
  m.vertexBuffer = gl.createBuffer();
  m.vNormalBuffer = gl.createBuffer();
  m.redYGradientBuffer = gl.createBuffer();

  //create array containing absolute values of fragNorms
  let absVertNorms = nVerts;
  absVertNorms.forEach(function(fn, i){ absVertNorms[i] = Math.abs(fn); });

  //bind the data we placed in the verts array to an OpenGL buffer
  gl.bindBuffer(gl.ARRAY_BUFFER, m.vertexBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(verts), gl.STATIC_DRAW);

  //bind the data we placed in the normal array to an OpenGL buffer
  gl.bindBuffer(gl.ARRAY_BUFFER, m.vNormalBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(absVertNorms), gl.STATIC_DRAW);

   //bind the data we placed in the red Y gradient array to an OpenGL buffer
   gl.bindBuffer(gl.ARRAY_BUFFER, m.redYGradientBuffer);
   gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(redYGradient), gl.STATIC_DRAW);
}

demo.prototype.MainLoop = function()
{
    drawScene();
}

//the drawing function
function drawScene()
{
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    //NOTE: mesh is an object created in processMesh of main.js with "model" member that stores meshLoader.Mesh data 
    //...basically, .model is the "Mesh" member of modelLoader 
    var m = GC.mesh.model

    //setup perspective and lookat matrices
    GC.perspectiveMatrix = makePerspective(45, GC.width/GC.height, 0.1, Math.max(2000.0,m.maxZ));
    var lookAtMatrix = makeLookAt(camera.position[0],camera.position[1],camera.position[2],
                                  camera.lookAt[0],camera.lookAt[1],camera.lookAt[2],
                                  0,1,0);

    //set initial camera lookat matrix
    mvLoadIdentity(GC); //GC.mvMatrix == 4x4 identity matrix

    //multiply by our lookAt matrix
    mvMultMatrix(lookAtMatrix,GC); //GC.mvMatrix === lookAtMatrix
    
//--------- camera rotation matrix multiplicaton
    //translate to origin of model for rotation
    mvTranslate([(m.minX+m.maxX)/2.0,(m.minY+m.maxY)/2.0,(m.minZ+m.maxZ)/2.0],GC);

    mvMultMatrix(camera.Transform,GC);//multiply by the transformation

    //translate back to original origin
    mvTranslate([-(m.minX+m.maxX)/2.0,-(m.minY+m.maxY)/2.0,-(m.minZ+m.maxZ)/2.0],GC);
//---------

    //passes modelview and projection matrices to the vertex shader
    setMatrixUniforms(GC);

    //pass the vertex buffer to the shader
    gl.bindBuffer(gl.ARRAY_BUFFER, m.vertexBuffer);
    gl.vertexAttribPointer(GC.vertexPositionAttribute, 3, gl.FLOAT, false, 0, 0);

    //pass the fragment normal buffer to the shader
    gl.bindBuffer(gl.ARRAY_BUFFER, m.vNormalBuffer);
    gl.vertexAttribPointer(GC.vNormalAttribute, 3, gl.FLOAT, false, 0, 0);

    //pass the red gradient along Y buffer to the shader
    gl.bindBuffer(gl.ARRAY_BUFFER, m.redYGradientBuffer);
    gl.vertexAttribPointer(GC.redYGradientAttribute, 3, gl.FLOAT, false, 0, 0);

    //draw everything
    gl.drawArrays(gl.TRIANGLES,0,m.indices.length);
}

demo.prototype.setMouseEventCallbacks = function()
{
//-------- set callback functions
    this.canvas.onmousedown = this.mouseDown;
    this.canvas.onmousewheel = this.mouseWheel;

    //--Why set these to callbacks for the document object?
    document.onmouseup = this.mouseUp;          
    document.onmousemove = this.mouseMove;
    
    //--touch event callbacks
    this.canvas.ontouchstart = this.touchDown;
    this.canvas.ontouchend = this.touchUp;
    this.canvas.ontouchmove = this.touchMove;
//-------- end set callback functions
}

demo.prototype.setKeyboardEventCallbacks = function()
{
    //--Why set these to callbacks for the document object?
    document.onkeydown = this.keyDown;          
}

//handle mousedown
demo.prototype.mouseDown = function(event)
{
    GC.mouseDown = true;

    //update the base rotation so model doesn't jerk around upon new clicks
    camera.LastRot = camera.ThisRot;
    camera.click(event.clientX,event.clientY);

    return false;
}

//handle mouseup
demo.prototype.mouseUp = function(event)
{
    GC.mouseDown = false;
    return false;
}

//handle mouse movement
demo.prototype.mouseMove = function(event)
{
    if(GC.mouseDown == true)
    {
       X = event.clientX;
       Y = event.clientY;

       //call camera function for handling mouse movement
       camera.move(X,Y)

       drawScene();
    }
    return false;
}

//handle mouse scroll event
demo.prototype.mouseWheel = function(event)
{
    camera.zoomScale -= event.wheelDeltaY*0.0005;
    camera.Transform.elements[3][3] = camera.zoomScale;

    drawScene();
    return false;
}

//--------- handle keyboard events
demo.prototype.keyDown = function(e)
{
    camera.LastRot = camera.ThisRot;
    var center = {x: GC.width/2, y:GC.height/2}; 
    var delta = 8;

    switch(e.keyCode)
    {
        case 37: //Left arrow
            camera.click(center.x, center.y);
            camera.move(center.x - delta, center.y);
        break;
        case 38: //Up arrow
            camera.click(center.x, center.y);
            camera.move(center.x, center.y - delta);
        break;
        case 39: //Right arrow
            camera.click(center.x, center.y);
            camera.move(center.x + delta, center.y);
        break;
        case 40: //Down arrow
            camera.click(center.x, center.y);
            camera.move(center.x, center.y + delta);
        break;
    }

    //redraw
    drawScene();
}

// --------- handle touch events
demo.prototype.touchDown = function(event)
{
    GC.mouseDown = true;

    //update the base rotation so model doesn't jerk around upon new clicks
    camera.LastRot = camera.ThisRot;

    //tell the camera where the touch event happened
    camera.click(event.changedTouches[0].pageX,event.changedTouches[0].pageY);

    return false;
}

//handle touchEnd
demo.prototype.touchUp = function(event)
{
    GC.mouseDown = false;
    return false;
}

//handle touch movement
demo.prototype.touchMove = function(event)
{
    if(GC.mouseDown == true)
    {
        X = event.changedTouches[0].pageX;
        Y = event.changedTouches[0].pageY;

       //call camera function for handling mouse movement
       camera.move(X,Y)

       drawScene();
    }
    return false;
}
// --------- end handle touch events
