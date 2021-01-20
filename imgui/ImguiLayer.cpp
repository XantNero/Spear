#include "ImguiLayer.h"
#include "../../vendor/imgui/imgui.h"
#include "../../vendor/imgui/imgui_impl_glfw.h"
#include "../../vendor/imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "../ModelApplication.h"

ImguiLayer::ImguiLayer() 
{
}

ImguiLayer::~ImguiLayer() 
{
    
}

void ImguiLayer::onAttach() 
{
    const char* glsl_version = "#version 130";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    //TODO:: CORRECT M_wINDOW
    //ImGui_ImplGlfw_InitForOpenGL(ModelApplication::getApp()->getWindow()->m_Window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

}

void ImguiLayer::onDetach() 
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImguiLayer::begin() 
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImguiLayer::end() 
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImguiLayer::onImguiRender() 
{
    static bool show_demo_window = true;
    if (show_demo_window)
	    ImGui::ShowDemoWindow(&show_demo_window);
}

//{
    //         static float f = 0.0f;
    //         static int counter = 0;

    //         ImGui::SliderFloat3("COLOR", &cl.x, 0.0f, 1.0f);
    //         ImGui::SliderInt("gen speed", &genRate, 10, 2000);
    //         ImGui::SliderFloat("time speed", &time, 0.1f, 10.0f);
    //         ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    //     }