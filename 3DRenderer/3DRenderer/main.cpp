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

    Mesh* plane = new Cube();
    Cube* cube = new Cube();
    Cube* cube1 = new Cube();
    Mesh* cube2 = new Sphere(15);
    

    Material* mat1 = new Material();
    mat1->SetMetalic(0.0f);
    mat1->SetRoughness(1.0f);
    mat1->SetSpecular(0.5f);
    Material* mat2 = new Material();
    mat2->SetMetalic(1.0f);
    mat2->SetRoughness(0.0f);
    mat2->SetSpecular(0.5f);

    PointLight* light1 = new PointLight({ 0, 0, 1 }, 20);
    PointLight* light2 = new PointLight({ 1, 0, 0 }, 20);
    PointLight* light3 = new PointLight({ 0, 1, 0 }, 20);
    DirectLight* dirLight = new DirectLight({ 0.8, 0.8, 1 });
    //SpotLight* spotLight = new SpotLight({ 1, 0, 0 }, 3, 25);
    //spotLight->SetPosition(Vector3D(-0.5,1, 2));
    //spotLight->SetRotation(Vector3D(180, 0, 0));
    //dirLight->SetRotation(Vector3D(25, 0, 0));
    //light1->intensity = 0.5;
    light1->SetPosition(Vector3D(-2, 1, 2));
    light2->SetPosition(Vector3D(1, 1, 3));
    light3->SetPosition(Vector3D(0, -1, 2));

    glUseProgram(mat1->program);
    mat1->SetAlbedo(1, 0, 0, 1);

    cube1->SetName("test");

    scene->AddObject(plane);
    scene->AddObject(cube);
    scene->AddObject(cube1);
    scene->AddObject(cube2);

    mat1->LoadTexture("test.png", 1);
    //mat2->LoadTexture("test1.jpg", 1);

    scene->AddMaterial(mat1);
    scene->AddMaterial(mat2);

    scene->SetObjectMaterial(plane, mat1);
    //scene->SetObjectMaterial(cube1, mat1);
    scene->SetObjectMaterial(cube, mat2);

    scene->AddLight(light1);
    //scene->AddLight(spotLight);
    //scene->AddLight(light3);
    scene->AddLight(dirLight);

    //scene->DeleteMaterial(mat2);

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
    cube1->SetPosition(Vector3D(-2, 0, 2));
    cube2->SetPosition(Vector3D(0, 0, 2));
    plane->SetPosition(Vector3D(-0.5, 0, 3));

    //scene->DeleteObject(cube);
    //scene->DeleteLight(spotLight);

    //cube->SetScale(Vector3D(2, 2, 2));

    //scene->DeleteMaterial(mat2);


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