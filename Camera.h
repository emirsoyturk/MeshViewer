#ifndef CAMERA_H
#define CAMERA_H
#include "Angel.h"

class Camera
{
    
    public:

        Camera();

        void setEye(float x, float y, float z);
        void setEye(float theta, float phi);
        void setEye(vec4 eye);

        void setAt(float x, float y, float z);
        void setAt(vec4 center);
        void setAspect(float _aspect);

        void setUp(float x, float y, float z);
        void setUp(vec4 up);

        void translate(float x, float y, float z);
        void RotateX(float theta);
        void RotateY(float theta);
        void RotateZ(float theta);

        mat4 getMatrix();
        mat4 getLookAt();
        mat4 getProjection();

        vec4 eye;
        vec4 at;
        vec4 up;

        float aspect = 1.0;
        float near = 0.01;
        float far = 100.0;
        float fovy = 120;
        float radius = 1;
        float phi = 0;
        float theta = -270;

    private:
        

};

#endif