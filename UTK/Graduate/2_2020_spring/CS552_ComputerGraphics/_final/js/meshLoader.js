if (typeof String.prototype.startsWith !== 'function')
{
  String.prototype.startsWith = function (str)
  {
    return this.slice(0, str.length) === str;
  };
}

var modelLoader = {};
modelLoader.Mesh = function( objectData )
{
    //With the given elementID or string of the OBJ, this parses the
    //OBJ and creates the mesh.

    var v = [];
    var vn = [];
    var vt = [];

    // unpacking stuff
    var packed = {};
    packed.vIndices = []; //indices for vertex coordinates (x,y,z) for non-triangular faces
    packed.vtIndices = []; //indices for vt coordinates (s,t) for non-triangular faces
    packed.vnIndices = []; //indices for vn coordinates (x,y,z) for non-triangular faces

    // array of lines separated by the newline
    var lines = objectData.split( '\n' )

    for( var i=0; i<lines.length; i++ )
    {
       lines[i] = lines[i].replace(/\s{2,}/g, " "); // remove double spaces

      // if this is a vertex coordinate
      if( lines[ i ].startsWith( 'v ' ) )
      {
        //line = lines[ i ].slice( 2 ).split( " " )
        line = lines[ i ].split( " " )
        v.push( line[ 1 ] );
        v.push( line[ 2 ] );
        v.push( line[ 3 ] );
      }

      // if this is a vertex normal coordinate
      else if( lines[ i ].startsWith( 'vn' ) )
      {
        //line = lines[ i ].slice( 2 ).split( " " )
        line = lines[ i ].split( " " );
        vn.push( line[ 1 ] );
        vn.push( line[ 2 ] );
        vn.push( line[ 3 ] );
      }

      // if this is a texture coordinate
      else if( lines[ i ].startsWith( 'vt' ) )
      {
        //line = lines[ i ].slice( 2 ).split( " " )
        line = lines[ i ].split( " " );
        vt.push( line[ 1 ] );
        vt.push( line[ 2 ] );
        //vt.push( line[ 3 ] );
      }

      // if this is a face
      //NOTE: This code needs to include fan triangulation for
      else if( lines[ i ].startsWith( 'f ' ) )
      {
        line = lines[ i ].slice( 2 ).split( " " );

        //faces are already triangles
        if (line.length == 3)
        {
          for(var j=1; j <= line.length-2; j++) //NOTE: this looks like really silly loop code.
                                                // if you're expecting triangles, then the loop is not needed
                                                //However, if not triangle, it doesn't work!!! WTF!
          {
              //NOTE: I assume he subtracts 1 to get zero-based indices
              let i1 = line[0].split('/')[0] - 1;
              let i2 = line[j].split('/')[0] - 1;
              let i3 = line[j+1].split('/')[0] - 1;
              //packed.indices.push(i1,i2,i3);
              packed.vIndices.push(i1,i2,i3);
          }
        }

        //faces are NOT triangles. Perform fan triangulation algorithm
        //NOTE: use "cube-3d-shape.obj" to ensure this works correctly
        else
        {
          let vLineElements = [];
          let vtLineElements = [];
          let vnLineElements = [];
          let lineDashSplit = [];

          for (let j=0; j < line.length; j++)
          {
            lineDashSplit = line[j].split("/");

            vLineElements.push(lineDashSplit[0] - 1);
            vtLineElements.push(lineDashSplit[1] - 1);
            vnLineElements.push(lineDashSplit[2] - 1);
          }

          //triangulate all indices associated with an individual face
          fanTriangulation(packed.vIndices, vLineElements, line.length);
          fanTriangulation(packed.vtIndices, vtLineElements, line.length);
          fanTriangulation(packed.vnIndices, vnLineElements, line.length);
        }
      }
    }

    this.vn = vn;
    this.vt = vt;
    this.v = v;
    this.vIndices = packed.vIndices; //NOTE: I'm commenting this out b/c not using "this.indices" causes errors in demo.js. 
    this.vtIndices = packed.vtIndices;
    this.vnIndices = packed.vnIndices;

    let norms = calNorms(this.v, this.vIndices);
    this.vertNorms = norms[0];
    this.fragNorms = norms[1];

// console.log(this.vIndices);
// console.log(this.v);
// console.log(this.vertNorms);

    let vnuvCoords = calVertNormUVCoordinates(this.vIndices, v, this.vertNorms);
    this.vCoords = vnuvCoords.v;
    this.nCoords = vnuvCoords.n;
    this.uvCoords = vnuvCoords.uv;


// console.log("End of meshLoader");
// console.log("this.vCoords");
// console.log(this.vCoords);
// console.log("this.nCoords");
// console.log(this.nCoords);
// console.log("this.uvCoords");
// console.log(this.uvCoords);

} 

//triangulates a single face with aribrary number of indices
function fanTriangulation(indices, lineItems, lineLength)
{
  //push triangle made from first three vertices
  indices.push(lineItems[0],lineItems[1],lineItems[2]);

  //finish fan triangulation algorithm for the other vertices
  for (let j=2; j <= lineLength - 2; j++) { indices.push(lineItems[0], lineItems[j], lineItems[j+1]); }
}

function calNorms(verts, triangles)
{
  let triVec1 = [0.0, 0.0, 0.0];
  let triVec2 = [0.0, 0.0, 0.0];
  let vert1 = [0.0, 0.0, 0.0];
  let vert2 = [0.0, 0.0, 0.0];
  let vertNorms;
  let fragNorms = [];
  let vecLength;
  let j;

  //initialize vertNorms to be the same size as verts containing all zeroes
  (vertNorms = []).length = verts.length;
  vertNorms.fill(0.0);

  for(j = 0; j <= triangles.length - 1; j = j + 3)
  {
      //NOTE: He already subtracted 1 from file indices of faces. Shouldn't have to do that here.
      //create vectors
      vert1 = [ verts[ triangles[j]*3 ], verts[ triangles[j]*3 + 1], verts[ triangles[j]*3 + 2] ];
      vert2 = [ verts[ triangles[j+1]*3 ], verts[ triangles[j+1]*3 + 1], verts[ triangles[j+1]*3 + 2] ];
      triVec1 = vertSubtraction( vert1, vert2 );

      vert1 = [ verts[ triangles[j]*3 ], verts[ triangles[j]*3 + 1], verts[ triangles[j]*3 + 2] ];
      vert2 = [ verts[ triangles[j+2]*3 ], verts[ triangles[j+2]*3 + 1], verts[ triangles[j+2]*3 + 2] ];
      triVec2 = vertSubtraction( vert1, vert2 );

      //create normal vertor
      triVec1 = cross(triVec1, triVec2);

      //store triangle face normal
      fragNorms.push(triVec1[0]);
      fragNorms.push(triVec1[1]);
      fragNorms.push(triVec1[2]);

      //build upon vertex normal for all 3 vertices
      vertNorms[ triangles[j]*3    ] += triVec1[0];
      vertNorms[ triangles[j]*3 + 1] += triVec1[1];
      vertNorms[ triangles[j]*3 + 2] += triVec1[2];

      //NOTE: I FORGOT to apply this normal calculation to the OTHER 2 VERTICES OF THE TRIANGLE!!!
      vertNorms[ triangles[j+1]*3    ] += triVec1[0];
      vertNorms[ triangles[j+1]*3 + 1] += triVec1[1];
      vertNorms[ triangles[j+1]*3 + 2] += triVec1[2];

      vertNorms[ triangles[j+2]*3    ] += triVec1[0];
      vertNorms[ triangles[j+2]*3 + 1] += triVec1[1];
      vertNorms[ triangles[j+2]*3 + 2] += triVec1[2];
  }

  //normalize vertNorms
  for(j = 0; j <= vertNorms.length - 1; j = j + 3)
  {
    vert1 = [ vertNorms[j], vertNorms[j+1], vertNorms[j+2] ];
    vecLength = Vector3fLength(vert1);
    vertNorms[j] /= vecLength;
    vertNorms[j+1] /= vecLength;
    vertNorms[j+2] /= vecLength;
  }

	return [vertNorms, fragNorms];
}

function calVertNormUVCoordinates(vIndices, v_verts, vertNorms)
{
    var verts = [];                   //array to hold vertices laid out according to "vIndices" (i.e., triangle vertices)
    var nVerts = [];                  //array to hold vertex normals laid according to "vIndices" (i.e., triangle vertices)
    var uv = [];
    
    //bb === Bounding box
    var bb = {};
    var min = [90000,90000,90000];    //used for bounding box calculations
    var max = [-90000,-90000,-90000]; //used for bounding box calculations

    vIndices.forEach(function(d,i)
    {
        //grab the x,y,z position and normal values for the current vertex
        //NOTE: "parseFloat" converts the strings from "vertices" to numbers
//HOLY FUCKING SHIT!!! Just spent two motherfucking hours with this 3 line block...
//...having parseFloat return NaN b/c v[d*3], etc was returning undefined. The only...
//...reason I can think this happened is b/c "v" is the name of the array in the calling...
//...function and stupid piece of shit JS was mixing those two up. FUcking horsehit...
//...I hate JS!!!
        vx = (parseFloat( v_verts[d*3]   ));
        vy = (parseFloat( v_verts[d*3+1] ));
        vz = (parseFloat( v_verts[d*3+2] ));

        nx = vertNorms[d*3];
        ny = vertNorms[d*3+1];
        nz = vertNorms[d*3+2];

        //calculate UV coordinates for a SPHERE
        r = Math.sqrt(vx*vx + vy*vy + vz*vz);
        u = 0.5 + (Math.atan2(vz/r, vx/r)) / (2*Math.PI);
        v = 0.5 - (Math.asin(vy/r)) / (Math.PI);
  
/* NOTE: GOING TO TRY AND LEAVE THIS OFF FOR NOW
        //absolutely NO CLUE as to why this is required to "thin out" duplicate "seam" image.
        if ( ((vz <= 0.0) && (vz >= -4.0)) && ((vx <= 0.0) && (vx >= -20.0)))
        {
          u = u + 1.7976931348623157e+308*Math.PI; 
          //u = u + 20*Math.PI; 
        }
//*/
        //make vertices and normals of unit length        
        rn = Math.sqrt(nx*nx + ny*ny + nz*nz);
        vx /= r; vy /= r; vz /= r;
        nx /= rn; ny /= rn; nz /= rn;

        //add all results to corresponding arrays
        verts.push(vx,vy,vz);
        nVerts.push(nx, ny, nz);
        uv.push(u,v);

        //check to see if we need to update the min/max
        if(vx < min[0]) min[0] = vx;
        if(vy < min[1]) min[1] = vy;
        if(vz < min[2]) min[2] = vz;
        if(vx > max[0]) max[0] = vx;
        if(vy > max[1]) max[1] = vy;
        if(vz > max[2]) max[2] = vz;
    });

    //set the min/max variables
    bb.minX = min[0]; bb.minY = min[1]; bb.minZ = min[2];
    bb.maxX = max[0]; bb.maxY = max[1]; bb.maxZ = max[2];

    return {v: verts, n: nVerts, uv: uv, boundingBox: bb};
}