#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Shader.h"
#include "Objects.h"
#include "Scene.h"
#include "Lighting.h"
#include "Texture.h"
#include "linmath.h"

#include"FileLoad.h"


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
    

    Material* mat1 = new Material();
    mat1->SetMetalic(0.0f);
    mat1->SetRoughness(1.0f);
    mat1->SetSpecular(0.5f);
    Material* mat2 = new Material();
    mat2->SetMetalic(1.0f);
    mat2->SetRoughness(0.3f);
    mat2->SetSpecular(0.5f);
    mat2->SetAlbedo(0.8, 0.2, 0.2, 1.0);

    PointLight* light1 = new PointLight({ 0, 0, 1 }, 20);
    SpotLight* spotLight = new SpotLight({ 0, 1, 0 }, 20, 45);
    PointLight* light3 = new PointLight({ 1, 0, 0 }, 20);
    DirectLight* dirLight = new DirectLight({ 0.8, 0.8, 1 });

    light1->SetPosition(Vector3D(0, 1, 0));
    spotLight->SetPosition(Vector3D(0, 1, 1.3));
    light3->SetPosition(Vector3D(0, -1, 2));

    mat1->LoadTexture("test.png", TEXTURE_ALBEDO);
    mat1->LoadTexture("waterNormal.jpg", TEXTURE_NORMAL);
    //mat2->LoadTexture("test1.jpg", 1);

    scene->AddMaterial(mat1);
    scene->AddMaterial(mat2);

    scene->AddLight(light1);
    scene->AddLight(spotLight);
    scene->AddLight(light3);
    scene->AddLight(dirLight);


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

    std::vector<Mesh*> objs = load::loadObj("C:/Users/bosse/Desktop/cube.obj", scene);
    std::cout << objs.size();
    Mesh* suzanne = objs[0];


    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);

    double xpos, ypos;
    double lastxpos, lastypos;
    glfwGetCursorPos(window, &lastxpos, &lastypos);

    double lastTime = glfwGetTime();
    double time, deltaTime;

    while (!glfwWindowShouldClose(window))
    {
        time = glfwGetTime();
        deltaTime = time - lastTime;
        lastTime = time;

        glfwGetCursorPos(window, &xpos, &ypos);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            scene->preview->Move(scene->preview->forward * 0.05f);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            scene->preview->Move(scene->preview->forward * -0.05f);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            scene->preview->Move(scene->preview->right * -0.05f);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            scene->preview->Move(scene->preview->right * 0.05f);
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            scene->preview->Move(Vector3D(0, 0.05, 0));
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            scene->preview->Move(Vector3D(0, -0.05, 0));
        }
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && (xpos != lastxpos || ypos != lastypos))
        {
            scene->preview->Rotate(Vector3D((lastypos - ypos) * 10 * deltaTime, (xpos - lastxpos) * 10 * deltaTime, 0));
        }
        lastxpos = xpos;
        lastypos = ypos;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        i.y += 1;
        i1.y += 1;

        suzanne->SetRotation(i);
        //suzanne->Move(i);

        spotLight->Rotate(Vector3D(1, 0, 0));

        //scene->preview->SetRotation(i);

        linmath::perspective(width, height, fov, far, near, proj);

        //cube->SetRotation(i);


        scene->Render(proj);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    delete scene;
    return 0;
}