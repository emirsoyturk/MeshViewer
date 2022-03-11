#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "Camera.h"
#include <unistd.h>
#include "Model.h"
#include "Angel.h"


void display(void);
void init(void);
void keyboard(unsigned char, int, int);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void specialInput(int, int, int);
void timer(int ms);
void myReshape(int width, int height);
char** strSplit(char* str, int size);
double randfrom(double , double);

enum Mode
{
    vertex,
    edge,
    face
};

Mode mode = face;

GLfloat theta[3] = {0.0, 0.0, 0.0};
GLfloat scale[3] = {1.0, 1.0, 1.0};
GLfloat move[3] = {0.0, 0.0, 0.0};

GLuint thetaLocation;
GLuint scaleLocation;
GLuint moveLocation;
GLuint cameraAdress;
GLuint projectionAdress;
GLuint ambient_productAdress;
GLuint diffuse_productAdress;
GLuint specular_productAdress;

float angleY = 0.0;

Camera mainCamera = Camera();

vec4 light_position = vec4(0.0, 0.0, 3.0, 0.0); //directional light source
vec4 light_ambient = vec4(0.2, 0.2, 0.2, 1.0);
vec4 light_diffuse = vec4(1.0, 1.0, 1.0, 1.0);
vec4 light_specular = vec4(1.0, 1.0, 1.0, 1.0);

vec4 material_ambient(200.0/255.0, 200.0/255.0, 200.0/255.0, 1.0);
vec4 material_diffuse(200.0/255.0, 200.0/255.0, 200.0/255.0, 1.0);
vec4 material_specular(200.0/255.0, 200.0/255.0, 200.0/255.0, 1.0);
float material_shininess = 100.0;

vec4 ambient_product = light_ambient * material_ambient;
vec4 diffuse_product = light_diffuse * material_diffuse;
vec4 specular_product = light_specular * material_specular;

Model currentModel;
int numberOfModels;

char **fileNames;

int prevMouseX;
int prevMouseY;
bool first = true;

int currentModelId = 0;

int main(int argc, char **argv )
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(300, 300);
    glutCreateWindow("Model Viewer");

    glewExperimental = GL_TRUE;
    glewInit();

    fileNames = argv;
    numberOfModels = argc-1;

    currentModel.ReadModel(fileNames[currentModelId+1]);
    mainCamera.setEye(mainCamera.theta, mainCamera.phi);

    glutReshapeFunc(myReshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutSpecialFunc(specialInput);
    glutDisplayFunc(display);
    glutTimerFunc(1000.0/60.0, timer, 0);
    glEnable(GL_CULL_FACE);  
    glCullFace(GL_BACK);

    init();

    glutMainLoop();

    return 0;
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat tempScale[3] = {scale[0], scale[1], scale[2]};
    GLfloat tempMove[3] = {move[0], move[1], move[2]};

    scale[0] *= 1/currentModel.scale;
    scale[1] *= 1/currentModel.scale;
    scale[2] *= 1/currentModel.scale;

    move[0] -= currentModel.middlePoint.x;
    move[1] -= currentModel.middlePoint.y;
    move[2] -= currentModel.middlePoint.z;

    glUniformMatrix4fv(cameraAdress, 1, GL_TRUE, mainCamera.getLookAt());
    glUniformMatrix4fv(projectionAdress, 1, GL_TRUE, mainCamera.getProjection());
    glUniform4fv(ambient_productAdress, 1, ambient_product);
    glUniform4fv(diffuse_productAdress, 1, diffuse_product);
    glUniform4fv(specular_productAdress, 1, specular_product);
    glUniform3fv(thetaLocation, 1, theta);
    glUniform3fv(scaleLocation, 1, scale);
    glUniform3fv(moveLocation, 1, move);

    move[0] = tempMove[0];
    move[1] = tempMove[1];
    move[2] = tempMove[2];

    scale[0] = tempScale[0];
    scale[1] = tempScale[1];
    scale[2] = tempScale[2];

    if(mode == vertex)
    {
        glDrawArrays(GL_POINTS, 0, currentModel.numberOfFaces*3);
    }
    else
    {
        if(mode == edge)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        glDrawArrays(GL_TRIANGLES, 0, currentModel.numberOfFaces*3);
    }

    glutSwapBuffers();
}

void init(void){
    currentModel.ReadModel(fileNames[currentModelId+1]);

    int sizeOfData = sizeof(vec4) * currentModel.numberOfFaces * 3;
    int sizeOfNormals = sizeof(vec4) * currentModel.numberOfFaces * 3;

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeOfData + sizeOfNormals, NULL, GL_STATIC_DRAW); //buffer in boyutunu belirliyoruz fakat deger koymuyoruz
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeOfData, currentModel.points); //buffer'in ilk kismini location ile doldurduk
    glBufferSubData(GL_ARRAY_BUFFER, sizeOfData, sizeOfNormals, currentModel.normals); //buffer'in ilk kismini location ile doldurduk

    GLuint program = InitShader("vs.glsl", "fs.glsl");
    glUseProgram(program);

    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4) * currentModel.numberOfFaces * 3));

    thetaLocation = glGetUniformLocation(program, "theta");
    scaleLocation = glGetUniformLocation(program, "scale");
    moveLocation = glGetUniformLocation(program, "move");
    cameraAdress = glGetUniformLocation(program, "Camera");
    projectionAdress = glGetUniformLocation(program, "Projection");


    ambient_productAdress = glGetUniformLocation(program, "AmbientProduct");
    diffuse_productAdress = glGetUniformLocation(program, "DiffuseProduct");
    specular_productAdress = glGetUniformLocation(program, "SpecularProduct");
    glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position );
    glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess );

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 1.0);

}

void timer(int ms)
{
    glutPostRedisplay();
    glutTimerFunc(1000/60, timer, 0);
}

void keyboard(unsigned char key, int x, int y)
{
    if(key == 'P' || key == 'p'){
        if(currentModelId > 0)
        {
            currentModelId -= 1;
        }
        else
        {
            currentModelId = numberOfModels-1;
        }
        init();
    }
    else if(key == 'N' || key == 'n'){
        if(currentModelId < numberOfModels-1)
        {
            currentModelId += 1;
        }
        else
        {
            currentModelId = 0;
        }
        init();
    }
    else if(key == 'V' || key == 'v'){
        mode = vertex;
    }
    else if(key == 'E' || key == 'e'){
        mode = edge;
    }
    else if(key == 'F' || key == 'f'){
        mode = face;
    }
}

void specialInput(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            theta[0] += 0.05;
            break;
        case GLUT_KEY_DOWN:
            theta[0] -= 0.05;
            break;
        case GLUT_KEY_LEFT:
            theta[1] += 0.05;
            break;
        case GLUT_KEY_RIGHT:
            theta[1] -= 0.05;
            break;
    }
}

void mouse(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        first = true;
    }
    if(state == GLUT_UP)
    {
        if(button == 4)
        {
            mainCamera.radius += 0.1;
        }
        else if(button == 3)
        {
            mainCamera.radius -= 0.1;
        }
    }

    mainCamera.setEye(mainCamera.theta, mainCamera.phi);
}

void motion(int x, int y){

    if(!first)
    {
        int diffX = prevMouseX - x;
        int diffY = prevMouseY - y;
        float sens = 0.5;

        mainCamera.phi += diffY * sens;
        mainCamera.theta += diffX * sens;

        if(mainCamera.phi > 88.0)
        {
            mainCamera.phi = 88.0;
        }
        else if(mainCamera.phi < -88.0)
        {
            mainCamera.phi = -88.0;
        }

        mainCamera.setEye(mainCamera.theta, mainCamera.phi);
            
        first = false;
    }
    

    prevMouseX = x;
    prevMouseY = y;

    first = false;
}

void myReshape(int width, int height)
{
    glViewport(0, 0, width, height);
    mainCamera.setAspect(((float) width)/height);
    glUniformMatrix4fv(projectionAdress, 1, GL_TRUE, mainCamera.getProjection());
}

double randfrom(double min, double max)
{
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}
