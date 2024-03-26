#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"


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

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    Scene scene = Scene();

    
    Material* mat1 = new Material();
    mat1->SetMetalic(0.0f);
    mat1->SetRoughness(1.0f);
    mat1->SetSpecular(0.5f);

    Material* mat2 = new Material();
    mat2->SetMetalic(1.0f);
    mat2->SetRoughness(0.3f);
    mat2->SetSpecular(0.5f);
    mat2->SetAlbedo(0.8, 0.2, 0.2, 1.0);

    DirectLight* dirLight = new DirectLight({ 0, 0, 1 });
    SpotLight* spotLight = new SpotLight({ 0, 1, 0 });
    PointLight* pointLight = new PointLight({ 1, 0, 0 });

    spotLight->SetPosition({ 0, 5, 0 });
    dirLight->SetRotation({ 0, 0, 0 });
    pointLight->SetPosition({ 0, 5, 0 });

    
    mat1->LoadTexture("test.png", TEXTURE_ALBEDO);
    mat1->LoadTexture("waterNormal.jpg", TEXTURE_NORMAL);

    scene.AddMaterial(mat1);

    scene.AddLight(dirLight);
    scene.AddLight(spotLight);
    scene.AddLight(pointLight);


    int width, height;

    float fov = lm::radians(45);
    float far = 50;
    float near = 0;

    lm::vec3 i = {};
    lm::vec3 i1 = {};

    std::vector<Mesh*> objs = load::loadObj("TestModels/cube.obj", &scene);
    Mesh* suzanne = objs[0];



    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);
    
    
    double xpos, ypos;
    double lastxpos, lastypos;
    glfwGetCursorPos(window, &lastxpos, &lastypos);

    float lastTime = glfwGetTime();
    float time, deltaTime;
    float fps;
    std::string title = "";

    load::SaveScene("test.scene", &scene);

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (!glfwWindowShouldClose(window))
    {
        time = glfwGetTime();
        deltaTime = time - lastTime;
        lastTime = time;

        glfwGetCursorPos(window, &xpos, &ypos);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            scene.preview.Move(scene.preview.forward * 0.05f);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            scene.preview.Move(scene.preview.forward * -0.05f);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            scene.preview.Move(scene.preview.right * -0.05f);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            scene.preview.Move(scene.preview.right * 0.05f);
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            scene.preview.Move({ 0, 0.05, 0 });
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            scene.preview.Move({ 0, -0.05, 0 });
        }
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && (xpos != lastxpos || ypos != lastypos))
        {
            scene.preview.Rotate({ (float)(lastypos - ypos) * 10 * deltaTime, (float)(xpos - lastxpos) * 10 * deltaTime, 0 });
        }
        lastxpos = xpos;
        lastypos = ypos;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        fps = 1 / deltaTime;

        title = "Testing - " + std::to_string(fps);

        glfwSetWindowTitle(window, title.c_str());

        i.y += 1;
        i1.y += 1;
        suzanne->SetRotation(i);

        scene.Render(width, height);

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}