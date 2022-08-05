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

    var verts = [];
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
        verts.push( line[ 1 ] );
        verts.push( line[ 2 ] );
        verts.push( line[ 3 ] );
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

            //NOTE: Uncomment this IF AND ONLY IF you comment out all 3 fanTriangulation functions below
            //vIndices4Tex.push(lineElementSplit[0] - 1);
            //vtIndices4Tex.push(lineElementSplit[1] - 1);
            //vnIndices4Tex.push(lineElementSplit[2] - 1);
          }

          //triangulate all indices associated with an individual face
          fanTriangulation(packed.vIndices, vLineElements, line.length);
          fanTriangulation(packed.vtIndices, vtLineElements, line.length);
          fanTriangulation(packed.vnIndices, vnLineElements, line.length);

          /*
          //separate out vertex index from #/#/# format and subtract 1 to make it zero-indexed.
          let lineItems = [];
          for (let j=1; j <= line.length; j++) { lineItems.push(line[j-1].split('/')[0] - 1); }

          //push all lineItems into packed.indices4Tex as long as vertex count of face is 4
          if (lineItems.length == 4) packed.vIndices4Tex.push(lineItems[0], lineItems[1], lineItems[2],lineItems[3]);
          else {console.log("lineItems.length != 4!!! Returning!"); return;}
//console.log("lineItems: ");
//console.log(lineItems);
          //push triangle made from first three vertices
          packed.indices.push(lineItems[0],lineItems[1],lineItems[2]);

          //finish fan triangulation algorithm for the other vertices
          for (let j=2; j <= line.length - 2; j++) { packed.indices.push(lineItems[0], lineItems[j], lineItems[j+1]); }
          */
//print last 6 elements of packed.indices
//console.log("last 6 elements of packed.indices: ");
//console.log(packed.indices.slice(Math.max(packed.indices.length - 6, 0)));

//console.log("packed.indices: ");
//console.log(packed.indices);

        }
      }
    }

    this.vn = vn;
    this.vt = vt;
    this.vertices = verts;
    this.vIndices = packed.vIndices; //NOTE: I'm commenting this out b/c not using "this.indices" causes errors in demo.js. 
    this.vtIndices = packed.vtIndices;
    this.vnIndices = packed.vnIndices;
    let norms = calNorms(this.vertices, this.vIndices);
    this.vertNorms = norms[0];
    this.fragNorms = norms[1];

    /*
console.log("verts: ");
console.log(verts);
console.log("vt: ");
console.log(vt);
//var blah = vt.length/2 + vt.length;
//console.log("vt.length + (1/3)*verts.length: " + blah);
console.log("vn: ");
console.log(vn);

console.log("packed.vIndices: ");
console.log(packed.vIndices);
console.log("packed.vnIndices: ");
console.log(packed.vnIndices);
console.log("packed.vtIndices: ");
console.log(packed.vtIndices);
//*/

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