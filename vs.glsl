#version 150

in  vec4 vPosition;
in  vec3 vNormal;

out  vec3 fN;
out  vec3 fE;
out  vec3 fL;

uniform vec3 theta;
uniform vec3 scale;
uniform vec3 move;
uniform mat4 Camera;
uniform mat4 Projection;
uniform vec4 LightPosition;

mat4 Translate(float x, float y, float z)
{
    mat4 t = mat4
        (
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            x,   y,   z,   1.0
        );

    return t;
}

mat4 Scale(float x, float y, float z)
{
    mat4 s = mat4
        (
            x,   0.0, 0.0, 0.0,
            0.0, y,   0.0, 0.0,
            0.0, 0.0, z,   0.0,
            0.0, 0.0, 0.0, 1.0
        );

    return s;
}

mat4 RotateX(float theta)
{   
    mat4 rotate = mat4
        ( 
            1.0,  0.0,         0.0,        0.0,
		    0.0,  cos(theta),  sin(theta), 0.0,
		    0.0, -sin(theta),  cos(theta), 0.0,
		    0.0,  0.0,         0.0,        1.0 
        );

    return rotate;
}

mat4 RotateY(float theta)
{
    mat4 rotate = mat4
        ( 
            cos(theta),  0.0, -sin(theta), 0.0,
            0.0,         1.0,  0.0,        0.0,
            sin(theta),  0.0,  cos(theta), 0.0,
            0.0,         0.0,  0.0,        1.0 
        );

    return rotate;
}

mat4 RotateZ(float theta){
    mat4 rotate = mat4
        ( 
            cos(theta), sin(theta), 0.0, 0.0,
           -sin(theta), cos(theta), 0.0, 0.0,
            0.0,        0.0,        1.0, 0.0,
            0.0,        0.0,        0.0, 1.0 
        );

    return rotate;
}

void main() 
{    
    gl_Position = 1
        * Projection
        * Camera
        * RotateX(theta.x) 
        * RotateY(theta.y) 
        * RotateZ(theta.z)
        * Scale(scale.x, scale.y, scale.z) 
        * Translate(move.x, move.y, move.z)
        * vPosition;

    fN = normalize(vec3(RotateX(theta.x) 
        * RotateY(theta.y) 
        * RotateZ(theta.z)
        * Scale(scale.x, scale.y, scale.z) 
        * Translate(move.x, move.y, move.z) * vec4(vNormal,0.0)));
    fE = -normalize(vec3(gl_Position));
    fL = normalize(vec3(LightPosition-gl_Position)); //light in camera coords
    
    
    

                
}

