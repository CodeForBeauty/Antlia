#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Shader.h"
#include "Objects.h"
#include "Scene.h"
#include "Lighting.h"
#include "Texture.h"
#include "linmath.h"


int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glewInit();

    glHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 560, "Testing", NULL, NULL);

    if (!window) 
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewInit();

    Scene* scene = new Scene();

    Plane* plane = new Plane();
    Cube* cube = new Cube();

    Material* mat1 = new Material();
    Material* mat2 = new Material();

    glUseProgram(mat1->program);
    mat1->SetAlbedo(0, 0, 1, 1);

    scene->AddObject(plane);
    scene->AddObject(cube);

    mat1->LoadTexture("test.png", 1);
    mat2->LoadTexture("test1.jpg", 2);

    scene->AddMaterial(mat1);
    scene->AddMaterial(mat2);

    scene->SetObjectMaterial(cube, mat1);
    scene->SetObjectMaterial(plane, mat2);

    float* proj = new float[16] {
                                1, 0, 0, 0,
                                0, 1, 0, 0,
                                0, 0, 1, 0,
                                0, 0, 1, 0
    };
    int width, height;


    float fov = linmath::deg2radians(45);
    float far = 200;
    float near = 0.5;

    Vector3D i = Vector3D();
    Vector3D i1 = Vector3D();

    cube->SetPosition(Vector3D(1, -1.2, 3));
    plane->SetPosition(Vector3D(-0.5, 0, 3));

    cube->SetScale(Vector3D(2, 2, 2));

    scene->preview->SetPosition(Vector3D(0.2, 0, 0));
    scene->preview->SetRotation(Vector3D(0, 0, 0));

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        i.x -= 1;
        i1.y += 1;

        //scene->preview->SetRotation(i);

        linmath::perspective(width, height, fov, far, near, proj);

        //cube->SetRotation(i);

        plane->SetRotation(i1);

        scene->Render(proj);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    delete scene;
    return 0;
}