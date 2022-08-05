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
    packed.indices = [];
    
    // array of lines separated by the newline
    var lines = objectData.split( '\n' )

    for( var i=0; i<lines.length; i++ )
    {
       lines[i] = lines[i].replace(/\s{2,}/g, " "); // remove double spaces

      // if this is a vertex
      if( lines[ i ].startsWith( 'v ' ) )
      {
        //line = lines[ i ].slice( 2 ).split( " " )
        line = lines[ i ].split( " " )
        verts.push( line[ 1 ] );
        verts.push( line[ 2 ] );
        verts.push( line[ 3 ] );
      }

      // if this is a vertex normal
      else if( lines[ i ].startsWith( 'vn' ) )
      {
        //line = lines[ i ].slice( 2 ).split( " " )
        line = lines[ i ].split( " " );
        vn.push( line[ 1 ] );
        vn.push( line[ 2 ] );
        vn.push( line[ 3 ] );
      }

      // if this is a texture
      else if( lines[ i ].startsWith( 'vt' ) )
      {
        //line = lines[ i ].slice( 2 ).split( " " )
        line = lines[ i ].split( " " );
        vt.push( line[ 1 ] );
        vt.push( line[ 2 ] );
        vt.push( line[ 3 ] );
      }

      // if this is a face
      else if( lines[ i ].startsWith( 'f ' ) )
      {
        line = lines[ i ].slice( 2 ).split( " " );

        for(var j=1; j <= line.length-2; j++)
        {
            //NOTE: I assume he subtracts 1 to get zero-based indices
            var i1 = line[0].split('/')[0] - 1;
            var i2 = line[j].split('/')[0] - 1;
            var i3 = line[j+1].split('/')[0] - 1;
            packed.indices.push(i1,i2,i3);
        }
      }
    }

    this.vertices = verts;
    this.indices = packed.indices;
    let norms = calNorms(this.vertices, this.indices);
    this.vertNorms = norms[0];
    this.fragNorms = norms[1];
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