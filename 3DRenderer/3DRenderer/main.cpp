#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Shader.h"
#include "Objects.h"
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


    Cube* plane = new Cube();
    Cube* cube = new Cube();
    //cube->SetPosition(pos);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    Vector3D move = Vector3D(0.0, 0.0, 0.01);

    Vector3D* add = cube->GetPosition();
    Vector3D* add1 = plane->GetPosition();
    add->z = 3;
    add->x = -0.5;
    add1->z = 3;
    add1->x = 0.5;
    float* proj = new float[16] {
                                1, 0, 0, 0,
                                0, 1, 0, 0,
                                0, 0, 1, 0,
                                0, 0, 1, 0
    };
    int width, height;


    float fov = linmath::deg2radians(45);
    float far = 20;
    float near = 0.5;

    float* rot3 = new float[16] {
                            1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            0, 0, 1, 0
    };

    Vector3D i = Vector3D();
    Vector3D i1 = Vector3D();
    Vector3D* scale = cube->GetScale();
    scale->x = 1;
    scale->y = 1;
    scale->z = 1;
    cube->material->SetAlbedo(1, 0, 0, 1);
    cube->Update(proj);

    glUseProgram(plane->material->program);
    plane->material->SetAlbedo(0, 0, 1, 1);
    plane->Update(proj);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //cube->Move(move);
        i.x += 1;
        i1.z += 1;
        
        glUseProgram(cube->material->program);

        cube->SetRotation(i);

        linmath::perspective(width, height, fov, far, near, proj);

        cube->material->SetProj(proj);
        
        glBindVertexArray(cube->vao);

        glDrawElements(GL_TRIANGLES, cube->geometry->indeciesCount, GL_UNSIGNED_INT, nullptr);

        glUseProgram(plane->material->program);

        //plane->Move(move);

        plane->SetRotation(i1);

        plane->material->SetProj(proj);

        glBindVertexArray(plane->vao);

        glDrawElements(GL_TRIANGLES, plane->geometry->indeciesCount, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    delete plane, cube;
    return 0;
}