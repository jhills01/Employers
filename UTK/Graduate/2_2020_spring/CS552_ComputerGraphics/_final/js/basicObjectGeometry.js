/**
 * Create a model of a sphere.  The z-axis is the axis of the sphere,
 * with the north pole on the positive z-axis and the center at (0,0,0).
 * @param radius the radius of the sphere, default 0.5 if not specified.
 * @param slices the number of lines of longitude, default 32
 * @param stacks the number of lines of latitude plus 1, default 16.  (This 
 *    is the number of vertical slices, bounded by lines of latitude, the
 *    north pole and the south pole.)
 */

function uvSphere(radius, slices, stacks) 
{
    radius = radius || 0.5;
    slices = slices || 32;
    stacks = stacks || 16;
    var vertexCount = (slices+1)*(stacks+1);
    var vertices = new Float32Array( 3*vertexCount );
    var normals = new Float32Array( 3* vertexCount );
    var texCoords = new Float32Array( 2*vertexCount );
    var indices = new Uint16Array( 2*slices*stacks*3 );
    var du = 2*Math.PI/slices;
    var dv = Math.PI/stacks;
    var i,j,u,v,x,y,z;
    var indexV = 0;
    var indexT = 0;
    for (i = 0; i <= stacks; i++) 
    {
       v = -Math.PI/2 + i*dv;
       for (j = 0; j <= slices; j++) 
       {
          u = j*du;
          x = Math.cos(u)*Math.cos(v);
          y = Math.sin(u)*Math.cos(v);
          z = Math.sin(v);
          vertices[indexV] = radius*x;
          normals[indexV++] = x;
          vertices[indexV] = radius*y;
          normals[indexV++] = y;
          vertices[indexV] = radius*z;
          normals[indexV++] = z;
          texCoords[indexT++] = j/slices;
          texCoords[indexT++] = i/stacks;
       } 
    }
    var k = 0;
    for (j = 0; j < stacks; j++) 
    {
       var row1 = j*(slices+1);
       var row2 = (j+1)*(slices+1);
       for (i = 0; i < slices; i++) 
       {
           indices[k++] = row1 + i;
           indices[k++] = row2 + i + 1;
           indices[k++] = row2 + i;
           indices[k++] = row1 + i;
           indices[k++] = row1 + i + 1;
           indices[k++] = row2 + i + 1;
       }
    }
    return {
        vertexPositions: vertices,
        vertexNormals: normals,
        vertexTextureCoords: texCoords,
        indices: indices
    };
 }

  /**
  * Create a model of a cube, centered at the origin.  (This is not
  * a particularly good format for a cube, since an IFS representation
  * has a lot of redundancy.)
  * @side the length of a side of the cube.  If not given, the value will be 1.
  */
function cube(side) 
{
    var s = (side || 1)/2;
    var coords = [];
    var normals = [];
    var texCoords = [];
    var indices = [];
    
    function face(xyz, nrm) 
    {
       var start = coords.length/3;
       var i;
       for (i = 0; i < 12; i++) {
          coords.push(xyz[i]);
       }
       for (i = 0; i < 4; i++) {
          normals.push(nrm[0],nrm[1],nrm[2]);
       }
       texCoords.push(0,0,1,0,1,1,0,1);
       indices.push(start,start+1,start+2,start,start+2,start+3);
    }

    face( [-s,-s,s, s,-s,s, s,s,s, -s,s,s], [0,0,1] );
    face( [-s,-s,-s, -s,s,-s, s,s,-s, s,-s,-s], [0,0,-1] );
    face( [-s,s,-s, -s,s,s, s,s,s, s,s,-s], [0,1,0] );
    face( [-s,-s,-s, s,-s,-s, s,-s,s, -s,-s,s], [0,-1,0] );
    face( [s,-s,-s, s,s,-s, s,s,s, s,-s,s], [1,0,0] );
    face( [-s,-s,-s, -s,-s,s, -s,s,s, -s,s,-s], [-1,0,0] );
    return {
       vertexPositions: new Float32Array(coords),
       vertexNormals: new Float32Array(normals),
       vertexTextureCoords: new Float32Array(texCoords),
       indices: new Uint16Array(indices)
    }
 } 