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

    Cube* plane = new Cube();
    Cube* cube = new Cube();

    Material* mat1 = new Material();
    Material* mat2 = new Material();

    PointLight* light1 = new PointLight({ 0, 0, 1 }, 20);
    PointLight* light2 = new PointLight({ 1, 0, 0 }, 20);
    PointLight* light3 = new PointLight({ 0, 1, 0 }, 20);
    DirectLight* dirLight = new DirectLight({ 0.8, 0.8, 1 });
    SpotLight* spotLight = new SpotLight({ 1, 0, 0 }, 3, 25);
    spotLight->SetPosition(Vector3D(-0.5,1, 2));
    spotLight->SetRotation(Vector3D(180, 0, 0));
    std::cout << spotLight->GetVector().y << std::endl;
    dirLight->SetRotation(Vector3D(25, 0, 0));
    //light1->intensity = 0.5;
    light1->SetPosition(Vector3D(-2, 1, 2));
    light2->SetPosition(Vector3D(1, 1, 3));
    light3->SetPosition(Vector3D(0, -1, 2));

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

    scene->AddLight(light1);
    scene->AddLight(spotLight);
    //scene->AddLight(light3);
    scene->AddLight(dirLight);

    scene->DeleteMaterial(mat2);

    float* proj = new float[16] {
                                1, 0, 0, 0,
                                0, 1, 0, 0,
                                0, 0, 1, 0,
                                0, 0, 1, 0
    };
    int width, height;


    float fov = linmath::deg2radians(45);
    float far = 10;
    float near = 0.1;

    Vector3D i = Vector3D();
    Vector3D i1 = Vector3D();

    cube->SetPosition(Vector3D(1, -1, 3));
    plane->SetPosition(Vector3D(-0.5, 0, 3));

    scene->DeleteObject(cube);
    scene->DeleteLight(spotLight);

    //cube->SetScale(Vector3D(2, 2, 2));

    scene->preview->SetPosition(Vector3D(0, 0, 0));
    //scene->preview->SetRotation(Vector3D(360, 360, 360));

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            scene->preview->Move(Vector3D(0, 0, 0.05));
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            scene->preview->Move(Vector3D(0, 0, -0.05));
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            scene->preview->Move(Vector3D(-0.05, 0, 0));
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            scene->preview->Move(Vector3D(0.05, 0, 0));
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            scene->preview->Move(Vector3D(0, 0.05, 0));
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            scene->preview->Move(Vector3D(0, -0.05, 0));
        }
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        i.x += 1;
        i1.y += 1;

        //light1->Move(Vector3D(0.05, 0, 0));
        dirLight->Rotate(Vector3D(1, 0, 0));
        //spotLight->Move(Vector3D(0, -0.1, 0));
        //spotLight->SetAngle(spotLight->GetAngle() + 0.1);
        //std::cout << light1->GetPosition().x << std::endl;

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