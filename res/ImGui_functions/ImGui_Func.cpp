#include "../../src/include/imgui/imgui.h"
#include "../../module.h"

#include "ImGui_Func.h"
#include"../../src/include/imgui/backends/imgui_impl_glfw.h"
#include"../../src/include/imgui/backends/imgui_impl_opengl3.h"

const char* themes[] = { "Light", "Dark", "Classic", "Extasy" };
int num_themes = IM_ARRAYSIZE(themes);
int ThemeIndex = 0; 


void InitImGui(GLFWwindow* window){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // enable keyboard controls
    ImGui::StyleColorsLight();
    ImFont* font = io.Fonts->AddFontFromFileTTF("res\\ImGui_functions\\RobotoMono-Regular.ttf", 16); 
    ImGui::PushFont(font); // applies the font 
    //ImGui::Spectrum::StyleColorsSpectrum(); 
    ImGui_ImplGlfw_InitForOpenGL(window, true); 
    ImGui_ImplOpenGL3_Init();
}

void SetImGuiStyle()
{

    // color theme is from 
    // https://www.unknowncheats.me/forum/c-and-c-/189635-imgui-style-settings.html

    ImGuiStyle* style = &ImGui::GetStyle();
    /*
    style.WindowRounding = 10.0f;   
    style.FrameBorderSize = 1.0f;
    style.FrameRounding = 6.0f;
    style.GrabRounding = 6.0f;
*/
    style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;
 
	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Separator] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
    style->Colors[ImGuiCol_Border] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f); 
}

void DrawImGui(GLFWwindow* window){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //ImGui::ShowDemoWindow();
    
    if(ImGui::Begin("Visual", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        ImGui::SliderFloat("fov", &fov, 30.0f, 120.0f);
        
        ImGui::PushItemWidth(150.0f);
        {        
            ImGui::ColorPicker3("SkyRGB", SkyRGB);  
            ImGui::ColorPicker3("LightColor", Light_Color);  
            ImGui::PopItemWidth(); 
        }

        ImGui::End();
    }
    
    if(ImGui::Begin("Models", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        ImGui::SliderFloat("Temple Size", &TempleModelScale, 0.1f, 100.0f);
        ImGui::SliderFloat("Light X", &LightXPos, -1000.0f, 1000.0f);
        ImGui::SliderFloat("Light Y", &LightYPos, -1000.0f, 1000.0f);
        ImGui::SliderFloat("Light Z", &LightZPos, -1000.0f, 1000.0f);
        
        if(ImGui::Button("                                   Light Animation                                     "))
        {
            LightAnim = !LightAnim;
        }

        //ImGui::SliderFloat3("Light Color", Light_Color, 0.0f, 1.0f, "%.3f",0);
        ImGui::SliderFloat("Shininess", &Material_Shininess, -10.0f, 120.0f);
        ImGui::SliderFloat("ambient", &ambientSlider, -10.0f, 10.0f);
        ImGui::SliderFloat("specular", &specularSlider, -10.0f, 10.0f);

        if(ImGui::Button("                                   Enable V-Sync                                       "))
        {
            VSYNC = !VSYNC;
        }

        if(ImGui::Button("                                   Enable MSSA                                         "))
        {
            MSSA = !MSSA;
        }



        ImGui::End();
    }
    if(ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        ImGui::SliderFloat("Gravity", &gravity, -50.0f, 0.0f);

        ImGui::SliderFloat("Jump Power", &jumpStr, 0.0f, 50.0f);

                 
        if(ImGui::ListBox("Theme", &ThemeIndex, themes, num_themes, -1))
        {
            if(strcmp(themes[ThemeIndex], "Light") == 0){
                ImGui::StyleColorsLight();
            }
            if(strcmp(themes[ThemeIndex], "Dark") == 0){
                ImGui::StyleColorsDark();
            }
            if(strcmp(themes[ThemeIndex], "Classic") == 0){
                ImGui::StyleColorsClassic();
            }
            if(strcmp(themes[ThemeIndex], "Extasy") == 0){
                SetImGuiStyle();
            }

        }

        ImGui::SliderFloat("Fly Speed", &cameraSpeed, 0.0f, 50.0f);
        ImGui::SliderFloat("Fly Boost", &spdboost, 0.0f, 50.0f);




        if(ImGui::Button("              Quit              "))
        {
            glfwSetWindowShouldClose(window, true);
        }
         

        ImGui::End();
    }
    if(ImGui::Begin("Performance"))
    {
        ImGui::Text("Fps: %d", DisplayFps);  
        ImGui::End(); 
    }


 
}
