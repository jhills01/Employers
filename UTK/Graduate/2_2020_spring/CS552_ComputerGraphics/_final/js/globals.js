var gl = null;
var time_paused = false;
var time = 0;

var canvas;
// var width = 640;                            //render area width
// var height = 480;                           //render area height
var width = 1024;                            //render area width
var height = 600;                           //render area height

var camera = {};
camera.position = [0.0, 0.0, -75.0];

//starting values 
var shadowsON = camera.position[2];
var shadowsOFF = 0.0;
var sunlightZ;

//slider
// var rangeslider = document.getElementById("sliderRange"); 
// var output = document.getElementById("demo"); 
var rangeslider; 
var output; 
var speedChange = 50.0;


//solar object information
var baseOrbitalRadiusUnit = 9.0;
var sunOrbitalRadius = 0.0 * baseOrbitalRadiusUnit;
var earthOrbitalRadius = 3.0 * baseOrbitalRadiusUnit;
//Sun(NO!), Mercury, Venus, Earth(NO!), Mars, Jupiter, Saturn, Uranus, Neptune, Puto
//var planetOrbitalRadius = [3.0, 6.0, 12.0, 15.0, 18.0, 21.0, 24.0, 27.0];
var planetOrbitalRadius = [
    1.0 * baseOrbitalRadiusUnit, //Mercury
    2.0 * baseOrbitalRadiusUnit, //Venus
    4.0 * baseOrbitalRadiusUnit, //Mars   
    5.0 * baseOrbitalRadiusUnit, //Jupiter
    6.5 * baseOrbitalRadiusUnit, //Saturn
    7.5 * baseOrbitalRadiusUnit, //Uranus
    8.5 * baseOrbitalRadiusUnit, //Neptune
    9.5 * baseOrbitalRadiusUnit  //Pluto
];

var baseScaleUnit = 5.0;
var sunScale = 1.5 * baseScaleUnit;
var earthScale = 0.25 * baseScaleUnit;
//Sun(NO!), Mercury, Venus, Earth(NO!), Mars, Jupiter, Saturn, Uranus, Neptune, Puto
//var planetScale = [3.0, 6.0, 12.0, 15.0, 18.0, 21.0, 24.0, 27.0];
var planetScale = [
    0.1 * baseScaleUnit, //Mercury
    0.2 * baseScaleUnit, //Venus
    0.15 * baseScaleUnit, //Mars   
    0.70 * baseScaleUnit, //Jupiter
    0.6 * baseScaleUnit, //Saturn
    0.4 * baseScaleUnit, //Uranus
    0.45 * baseScaleUnit, //Neptune
    0.1 * baseScaleUnit  //Pluto
];

//var baseOrbitalSpeedUnit = 0.075;
var baseOrbitalSpeedUnit = 0.01;
//var baseOrbitalSpeedUnit = 0.1;
//var sunOrbitalSpeed = 1.0 * baseOrbitalSpeedUnit;
var earthOrbitalSpeed = 1 * baseOrbitalSpeedUnit;
//Sun(NO!), Mercury, Venus, Earth(NO!), Mars, Jupiter, Saturn, Uranus, Neptune, Puto
//var planetOrbitalSpeed = [3.0, 6.0, 12.0, 15.0, 18.0, 21.0, 24.0, 27.0];
var planetOrbitalSpeed = [
    4.0 * baseOrbitalSpeedUnit, //Mercury
    2.0 * baseOrbitalSpeedUnit, //Venus
    0.5 * baseOrbitalSpeedUnit, //Mars   
    0.2 * baseOrbitalSpeedUnit, //Jupiter
    0.1 * baseOrbitalSpeedUnit, //Saturn
    0.01 * baseOrbitalSpeedUnit, //Uranus
    0.008 * baseOrbitalSpeedUnit, //Neptune
    0.006 * baseOrbitalSpeedUnit  //Pluto
];

var planetTextures = [
    "./imageFiles/mercury.jpg",
    "./imageFiles/venus.jpg",
    "./imageFiles/mars.jpg",
    "./imageFiles/jupiter2.jpg",
    "./imageFiles/saturn.jpg",
    "./imageFiles/uranus.jpg",
    "./imageFiles/neptune.jpg",
    "./imageFiles/pluto.jpg"
];

//temporary holder variables for needed shaders
var vert_src;
var frag_src;

//var numOfSceneGroups = 4;                 //current groups: skybox, sun, earth, and planet      
var sceneObjects = [];                      //current groups: skybox, sun, earth, and planet  

//base prototype for all scene objects
function sceneObject()
{
    this.name;
    this.mesh;
    this.pgm, this.vertShader, this.fragShader; //NOTE: storing shaders isn't needed. PGM will hold that info once attached
    this.vbo_pos, this.attr_pos;
    this.vbo_norm, this.attr_norm;
    this.vbo_tang, this.attr_tang;
    this.vbo_bitang, this.attr_bitang;
    this.vbo_uv, this.attr_uv;
    this.index_buffer;
    this.num_indices;
    this.tex, this.tex_norm, this.tex_diffuse, this.tex_depth;
}

//skybox (sb)---------------------------------------------------------------------
function skybox() 
{
    sceneObject.call(this);
    this.name = "skybox";
    this.testSB = "sb";
    this.texUniLocation = {};
}
skybox.prototype = Object.create(sceneObject.prototype);
skybox.prototype.constructor = skybox; //constructor uses skybox() function

//sun (s)---------------------------------------------------------------------
function sun() 
{
    sceneObject.call(this);
    this.name = "sun";
    this.testS = "s";
}
sun.prototype = Object.create(sceneObject.prototype);
sun.prototype.constructor = sun; //constructor uses sun() function

//earth (e)---------------------------------------------------------------------
function earth() 
{
    sceneObject.call(this);
    this.name = "earth";
    this.testE = "e";
}
earth.prototype = Object.create(sceneObject.prototype);
earth.prototype.constructor = earth; //constructor uses earth() function

//planet (p)---------------------------------------------------------------------
function planet() 
{
    sceneObject.call(this);
    this.name = "planet";
    this.testP = "p";
    this.planetTextures = [];
}
planet.prototype = Object.create(sceneObject.prototype);
planet.prototype.constructor = planet; //constructor uses planet() function

//create instances of each scene group object and organize them in array based on creation order
//NOTE: sceneObjects SHOULD contain references to these objects. So I can use the array to modify original objects!
var sb = new skybox;
sceneObjects.push(sb);

var s = new sun;
sceneObjects.push(s);

var e = new earth;
sceneObjects.push(e);

var p = new planet;         //NOTE: I'm assuming I can use the same planet object with different textures to create...
sceneObjects.push(p);
                            //      ...the different planets that are not earth b/c they'll use the same shader program...
                            //      ...but just with different textures!!!!!
                            //      ...THIS MIGHT NOT WORK!!!


//TEST to see if I can overwrite non-object specific variables with object specific pgm data
                            var name;
                            var mesh;

                            // var pgm, vertShader, fragShader; //NOTE: storing shaders isn't needed. PGM will hold that info once attached
                            // var vbo_pos, attr_pos;
                            // var vbo_tang, attr_tang;
                            // var vbo_bitang, attr_bitang;
                            // var vbo_uv, attr_uv;
                            // var index_buffer;
                            // var num_indices;
                            // var tex_norm, tex_diffuse, tex_depth;
                        


//OLD----------------------------------------------------------

//var numOfObjGrps = 3;                   //groups: sun, planets, and skybox
//var solarBodies = ["sun", "earth", "planets"];

// //TESING PURPOSES:
// var numOfSolarBodies = 1;                   
// var solarBodies = ["sun", "planets"];
// var solarBodyPrograms = [];

// //shader program properties
// //NOTE: I might have to wrap up shader variables in another sub-object to distinguish between groups
// //      ...e.g., var planets.vbo_pos, var sun.vbo_pos, etc. I'm not sure if overwriting the same GOD variables
// //      ...with the different shaders will work with "setInterval()" that I'm using. 
// //
// var shaderPair;
// var shaderSequence;

