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

    window = glfwCreateWindow(850, 600, "Testing", NULL, NULL);

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

    DirectLight* dirLight = new DirectLight({ 0.8, 0.8, 1 });
    DirectLight* dirLight1 = new DirectLight({ 1, 0.5, 0.5 });
    DirectLight* dirLight2 = new DirectLight({ 1, 0.5, 0.5 });
    SpotLight* dirLight3 = new SpotLight({ 1, 0, 0 });

    dirLight3->SetPosition(Vector3D(0, 4, 0));
    dirLight3->SetRotation(Vector3D(0, 0, 0));

    dirLight1->SetRotation(Vector3D(25, 0, 0));
    dirLight2->SetRotation(Vector3D(-20, 0, 0));
    dirLight->SetRotation(Vector3D(0, 0, 0));


    mat1->LoadTexture("test.png", TEXTURE_ALBEDO);
    mat1->LoadTexture("waterNormal.jpg", TEXTURE_NORMAL);

    scene->AddMaterial(mat1);

    scene->AddLight(dirLight2);
    scene->AddLight(dirLight1);
    scene->AddLight(dirLight);
    scene->AddLight(dirLight3);


    float* proj = new float[16] {
                                1, 0, 0, 0,
                                0, 1, 0, 0,
                                0, 0, 1, 0,
                                0, 0, 1, 0
    };
    int width, height;


    float fov = linmath::deg2radians(45);
    float far = 50;
    float near = 0;

    Vector3D i = Vector3D();
    Vector3D i1 = Vector3D();

    std::vector<Mesh*> objs = load::loadObj("C:/Users/bosse/Desktop/cube.obj", scene);
    Mesh* suzanne = objs[0];


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);
    
    

    double xpos, ypos;
    double lastxpos, lastypos;
    glfwGetCursorPos(window, &lastxpos, &lastypos);

    double lastTime = glfwGetTime();
    double time, deltaTime;
    float fps;
    std::string title = "";

    load::SaveScene("test.scene", scene);

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

        fps = 1 / deltaTime;

        title = "Testing - " + std::to_string(fps);

        glfwSetWindowTitle(window, title.c_str());

        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        i.y += 1;
        i1.y += 1;

        //suzanne->SetRotation(i);
        //suzanne->Move(i);

        dirLight3->Rotate(Vector3D(1, 0, 0));
        //dirLight3->Move(Vector3D(0.0, 0.05, 0.0));

        //scene->preview->SetRotation(i);

        linmath::perspective(width, height, fov, far, near, proj);
        //linmath::orthographic(0.5, -0.5, 0.5 * ((float)height / width), -0.5 * ((float)height / width), -0.5, 0.5, proj);

        //cube->SetRotation(i);


        scene->Render(proj, width, height);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    delete scene;
    return 0;
}