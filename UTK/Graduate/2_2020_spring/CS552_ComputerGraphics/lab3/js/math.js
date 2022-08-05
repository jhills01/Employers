function isPowerOf2(value) 
{
  return (value & (value - 1)) == 0;
}

// augment Sylvester some
Matrix.Translation = function (v)
{
  if (v.elements.length == 2) 
  {
    var r = Matrix.I(3);
    r.elements[2][0] = v.elements[0];
    r.elements[2][1] = v.elements[1];
    return r;
  }

  if (v.elements.length == 3) 
  {
    var r = Matrix.I(4);
    r.elements[0][3] = v.elements[0];
    r.elements[1][3] = v.elements[1];
    r.elements[2][3] = v.elements[2];
    return r;
  }

  throw "Invalid length for Translation";
}

Matrix.prototype.flatten = function ()
{
    var result = [];
    if (this.elements.length == 0)
        return [];


    for (var j = 0; j < this.elements[0].length; j++)
        for (var i = 0; i < this.elements.length; i++)
            result.push(this.elements[i][j]);
    
    return result;
}

Matrix.prototype.ensure4x4 = function()
{
    if (this.elements.length == 4 &&
        this.elements[0].length == 4)
        return this;

    if (this.elements.length > 4 ||
        this.elements[0].length > 4)
        return null;

    for (var i = 0; i < this.elements.length; i++) 
    {
        for (var j = this.elements[i].length; j < 4; j++) 
        {
            if (i == j)
                this.elements[i].push(1);
            else
                this.elements[i].push(0);
        }
    }

    for (var i = this.elements.length; i < 4; i++) 
    {
        if (i == 0)
            this.elements.push([1, 0, 0, 0]);
        else if (i == 1)
            this.elements.push([0, 1, 0, 0]);
        else if (i == 2)
            this.elements.push([0, 0, 1, 0]);
        else if (i == 3)
            this.elements.push([0, 0, 0, 1]);
    }

    return this;
};

Matrix.prototype.make3x3 = function()
{
    if (this.elements.length != 4 ||
        this.elements[0].length != 4)
        return null;

    return Matrix.create([[this.elements[0][0], this.elements[0][1], this.elements[0][2]],
                          [this.elements[1][0], this.elements[1][1], this.elements[1][2]],
                          [this.elements[2][0], this.elements[2][1], this.elements[2][2]]]);
};

Vector.prototype.flatten = function ()
{
    return this.elements;
};

function mht(m) 
{
    var s = "";
    if (m.length == 16) 
    {
        for (var i = 0; i < 4; i++) 
        {
            s += "<span style='font-family: monospace'>[" + m[i*4+0].toFixed(4) + "," + m[i*4+1].toFixed(4) + "," + m[i*4+2].toFixed(4) + "," + m[i*4+3].toFixed(4) + "]</span><br>";
        }
    } 
    else if (m.length == 9) 
    {
        for (var i = 0; i < 3; i++) 
        {
            s += "<span style='font-family: monospace'>[" + m[i*3+0].toFixed(4) + "," + m[i*3+1].toFixed(4) + "," + m[i*3+2].toFixed(4) + "]</font><br>";
        }
    } 
    else 
    {
        return m.toString();
    }
    
    return s;
}

//utility functions for arcball/sylvester operations------------------------------------------------------

//return vector length
function Vector3fLength(v)
{
    return Math.sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
}

//create a 3x3 rotation from a quaternion
//adapted from NeHe arcball rotation tutorial
function Matrix3fSetRotationFromQuat4f(q)
{
    var n,s;
    var xs,ys,zs;
    var wx,wy,wz;
    var xx,xy,xz;
    var yy,yz,zz;

    n = q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3];
    s = (n > 0.0) ? (2.0/n) : 0.0;

    xs = q[0] * s; ys = q[1] * s; zs = q[2] * s;
    wx = q[3] * xs; wy = q[3] * ys; wz = q[3] * zs;
    xx = q[0] * xs; xy = q[0] * ys; xz = q[0] * zs;
    yy = q[1] * ys; yz = q[1] * zs; zz = q[2] * zs;

    var rot = [];

    rot[0] = 1.0 - (yy + zz); rot[1] = xy - wz; rot[2] = xz + wy;
    rot[3] = xy + wz; rot[4] = 1.0 - (xx + zz); rot[5] = yz - wx;
    rot[6] = xz - wy; rot[7] = yz + wx; rot[8] = 1.0 - (xx + yy);

    return rot;
}

//create a sylvester matrix from a flat array
function ArrayToSylvesterMatrix(m,size)
{
    var M = [];
    var tmp;
    for(var i = 0; i < size; i++){
        tmp = [];
        for(var j = 0; j < size; j++){
            tmp.push(m[i*size+j]);
        }
        M.push(tmp);
    }
    return $M(M);
};

//create a flat array from a sylvester matrix
function SylvesterToArray(M)
{
    var m = [];
    for(var i = 0; i < M.elements.length; i++){
        for(var j = 0; j < M.elements[i].length; j++){
            m.push(M.elements[i][j]);
        }
    }
    return m;
}

//create a 4x4 rotation matrix as a flat array from a 3x3 sylvester matrix
function SetRotationMatrixFrom3f(M)
{
    var m = SylvesterToArray(M);
    if(m.length != 9)
    {
        console.log("SetRotationMatrixFrom3f requires 9 number array");
        return;
    }

    var R = [];
    R[0] = m[0]; R[1] = m[1]; R[2] = m[2]; R[3] = 0;
    R[4] = m[3]; R[5] = m[4]; R[6] = m[5]; R[7] = 0;
    R[8] = m[6]; R[9] = m[7]; R[10] = m[8]; R[11] = 0;
    R[12] = 0;   R[13] = 0;   R[14] = 0;    R[15] = 1;

    return R;
}

//perform dot product on two arbitrary length arrays
function dot(a,b)
{
    var la = a.length;
    var lb = b.length;
    if(la != lb)
    {
        console.log("dot product vectors not same length");
        return null;
    }
    var result = 0.0;
    for(var i = 0; i < lb; i++)
    {
        result += a[i]*b[i];
    }
    return result;
}

//perform cross product on two 3 element vectors
function cross(v1, v2) 
{
  var vR = [0.0,0.0,0.0];
  vR[0] =   ( (v1[1] * v2[2]) - (v1[2] * v2[1]) );
  vR[1] = - ( (v1[0] * v2[2]) - (v1[2] * v2[0]) );
  vR[2] =   ( (v1[0] * v2[1]) - (v1[1] * v2[0]) );
  return vR;
}

//Tom Hills_LAB2 - Step 1
//calculates per vertex normals
//subtracts two 3d vertices and returns resulting vector obj
//NOTE: v1 and v2 are just 
function vertSubtraction(v1, v2)
{
    //console.log("v1: " + v1);
    //console.log("v2: " + v2);

    return [ (v1[0] - v2[0]), (v1[1] - v2[1]), (v1[2] - v2[2]) ];
}

function mat4Transpose(a, transposed) 
{
    var t = 0;
    for (var i = 0; i < 4; ++i) 
    {
      for (var j = 0; j < 4; ++j) 
      {
        transposed[t++] = a[j * 4 + i];
      }
    }
}

function mat4Invert(m, inverse) 
{
    var inv = new Float32Array(16);
    inv[0] = m[5]*m[10]*m[15]-m[5]*m[11]*m[14]-m[9]*m[6]*m[15]+
             m[9]*m[7]*m[14]+m[13]*m[6]*m[11]-m[13]*m[7]*m[10];
    inv[4] = -m[4]*m[10]*m[15]+m[4]*m[11]*m[14]+m[8]*m[6]*m[15]-
             m[8]*m[7]*m[14]-m[12]*m[6]*m[11]+m[12]*m[7]*m[10];
    inv[8] = m[4]*m[9]*m[15]-m[4]*m[11]*m[13]-m[8]*m[5]*m[15]+
             m[8]*m[7]*m[13]+m[12]*m[5]*m[11]-m[12]*m[7]*m[9];
    inv[12]= -m[4]*m[9]*m[14]+m[4]*m[10]*m[13]+m[8]*m[5]*m[14]-
             m[8]*m[6]*m[13]-m[12]*m[5]*m[10]+m[12]*m[6]*m[9];
    inv[1] = -m[1]*m[10]*m[15]+m[1]*m[11]*m[14]+m[9]*m[2]*m[15]-
             m[9]*m[3]*m[14]-m[13]*m[2]*m[11]+m[13]*m[3]*m[10];
    inv[5] = m[0]*m[10]*m[15]-m[0]*m[11]*m[14]-m[8]*m[2]*m[15]+
             m[8]*m[3]*m[14]+m[12]*m[2]*m[11]-m[12]*m[3]*m[10];
    inv[9] = -m[0]*m[9]*m[15]+m[0]*m[11]*m[13]+m[8]*m[1]*m[15]-
             m[8]*m[3]*m[13]-m[12]*m[1]*m[11]+m[12]*m[3]*m[9];
    inv[13]= m[0]*m[9]*m[14]-m[0]*m[10]*m[13]-m[8]*m[1]*m[14]+
             m[8]*m[2]*m[13]+m[12]*m[1]*m[10]-m[12]*m[2]*m[9];
    inv[2] = m[1]*m[6]*m[15]-m[1]*m[7]*m[14]-m[5]*m[2]*m[15]+
             m[5]*m[3]*m[14]+m[13]*m[2]*m[7]-m[13]*m[3]*m[6];
    inv[6] = -m[0]*m[6]*m[15]+m[0]*m[7]*m[14]+m[4]*m[2]*m[15]-
             m[4]*m[3]*m[14]-m[12]*m[2]*m[7]+m[12]*m[3]*m[6];
    inv[10]= m[0]*m[5]*m[15]-m[0]*m[7]*m[13]-m[4]*m[1]*m[15]+
             m[4]*m[3]*m[13]+m[12]*m[1]*m[7]-m[12]*m[3]*m[5];
    inv[14]= -m[0]*m[5]*m[14]+m[0]*m[6]*m[13]+m[4]*m[1]*m[14]-
             m[4]*m[2]*m[13]-m[12]*m[1]*m[6]+m[12]*m[2]*m[5];
    inv[3] = -m[1]*m[6]*m[11]+m[1]*m[7]*m[10]+m[5]*m[2]*m[11]-
             m[5]*m[3]*m[10]-m[9]*m[2]*m[7]+m[9]*m[3]*m[6];
    inv[7] = m[0]*m[6]*m[11]-m[0]*m[7]*m[10]-m[4]*m[2]*m[11]+
             m[4]*m[3]*m[10]+m[8]*m[2]*m[7]-m[8]*m[3]*m[6];
    inv[11]= -m[0]*m[5]*m[11]+m[0]*m[7]*m[9]+m[4]*m[1]*m[11]-
             m[4]*m[3]*m[9]-m[8]*m[1]*m[7]+m[8]*m[3]*m[5];
    inv[15]= m[0]*m[5]*m[10]-m[0]*m[6]*m[9]-m[4]*m[1]*m[10]+
             m[4]*m[2]*m[9]+m[8]*m[1]*m[6]-m[8]*m[2]*m[5];

    var det = m[0]*inv[0]+m[1]*inv[4]+m[2]*inv[8]+m[3]*inv[12];
    if (det == 0) return false;
    det = 1.0 / det;
    for (var i = 0; i < 16; i++) inverse[i] = inv[i] * det;
    return true;
}
