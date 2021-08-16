#include "graphics.h"
#include <GL/glew.h>
#include "SDL.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

class Application {
    private:
        // bool isRunning = true;
        WindowManager windowManager;
        GraphicsEngine Graphics;
        const size_t pixelShaderBufferSize = 8192;
        char pixelShaderBuffer[8192] = "main() {\n\toColour = vec3(0.2, 0.3, 0.4);\n}";

    public:
        Application() 
        : windowManager(1280, 720), Graphics(&windowManager)
        {
            // initialise GLEW
            // glewExperimental = GL_TRUE;
            // glewInit();

            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
            //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

            // Setup Dear ImGui style
            ImGui::StyleColorsDark();
            //ImGui::StyleColorsClassic();

            // Setup Platform/Renderer backends
            ImGui_ImplSDL2_InitForOpenGL(windowManager.window, windowManager.gl_context);
            ImGui_ImplOpenGL3_Init(windowManager.glsl_version);

            // Our state
            static bool show_editor_window = true;
            static bool show_render_window = true;
            ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

            // Main loop
            bool done = false;
            while (!done)
            {
                // Poll and handle events (inputs, window resize, etc.)
                // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
                // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
                // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
                // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
                windowManager.PollEvents();
                done = windowManager.isQuit;

                // Start the Dear ImGui frame
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplSDL2_NewFrame();
                ImGui::NewFrame();

                bool opt_fullscreen = true;
                ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
                
                const ImGuiViewport* viewport = ImGui::GetMainViewport();
                if (opt_fullscreen) {
                    ImGui::SetNextWindowPos(viewport->WorkPos);
                    ImGui::SetNextWindowSize(viewport->WorkSize);
                    ImGui::SetNextWindowViewport(viewport->ID);
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
                }

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
                ImGui::Begin("DockSpaceWindow", NULL, window_flags);
                ImGui::PopStyleVar(3);


                ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
                ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
                ImVec2 dockspace_size = ImVec2(0, 0);
                ImGui::DockSpace(dockspace_id, dockspace_size, dockspace_flags);

                static bool resetDocking = true;
                if (resetDocking) {
                    ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
                    ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
                    ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

                    // split the dockspace into 2 nodes -- DockBuilderSplitNode takes in the following args in the following order
                    // window ID to split, direction, fraction (between 0 and 1), 
                    //          the final two setting let's us choose which id we want (which ever one we DON'T set as NULL, will be returned by the function)
                    //          out_id_at_dir is the id of the node in the direction we specified earlier, out_id_at_opposite_dir is in the opposite direction
                    auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.3f, nullptr, &dockspace_id);

                    // dock windows into the docking node we made above
                    ImGui::DockBuilderDockWindow("Render Window", dockspace_id);
                    ImGui::DockBuilderDockWindow("Pixel Shader Editor", dock_id_left);
                    ImGui::DockBuilderFinish(dockspace_id);
                    resetDocking = false;
                }
                

                if (ImGui::BeginMenuBar()) {
                    if (ImGui::BeginMenu("File")) {
                        ImGui::MenuItem("New");
                        ImGui::MenuItem("Options");
                        ImGui::Separator();
                        ImGui::MenuItem("Close", NULL, false, true);
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenuBar();
                }
                ImGui::End();

                // Show editor window
                ImGuiWindowFlags editorWindowFlags = ImGuiWindowFlags_HorizontalScrollbar;
                if(show_editor_window) { 
                    static int counter = 0;
                    
                    ImGui::Begin("Pixel Shader Editor", &show_editor_window, editorWindowFlags);
                    // Compile button
                    if (ImGui::Button("Compile"))                
                        counter++;
                    ImGui::SameLine();
                    ImGui::Text("counter = %d", counter);
                    // RichText box for writing shader
                    ImGuiInputTextFlags textAreaFlags = ImGuiInputTextFlags_AllowTabInput;
                    ImVec2 textAreaSize = ImVec2(0, 0);
                    // Instance InputText with no label
                    ImGui::InputTextMultiline("TextArea Label", pixelShaderBuffer, 8192, textAreaSize, textAreaFlags);
                    // ImGui::PushItemWidth(-1);
                    // if (ImGui::InputTextMultiline("##TextArea Label", pixelShaderBuffer, pixelShaderBufferSize, textAreaSize, textAreaFlags))
                    //     std::cout << "TEXT CHANGED" << std::endl;
                    // ImGui::PopItemWidth();

                    // Clear colour select RGB
                    ImGuiColorEditFlags colourEditFlags = ImGuiColorEditFlags_NoLabel;
                    ImGui::ColorEdit3("clear color", (float*)&clear_color, colourEditFlags);
                    ImGui::End();
                }

                // Show render window.
                ImGuiWindowFlags renderWindowFlags = ImGuiWindowFlags_HorizontalScrollbar;
                if (show_render_window) {
                    ImGui::Begin("Render Window", &show_render_window, renderWindowFlags);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                    ImVec2 wsize = ImGui::GetWindowSize();
                    //Graphics.Render();
                    ImGui::Image((ImTextureID)Graphics.texColour, wsize, ImVec2(0, 1), ImVec2(1, 0));
                    ImGui::End();
                }

                // Rendering
                ImGui::Render();

                glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
                glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
                glClear(GL_COLOR_BUFFER_BIT);

                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
                if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                    ImGui::UpdatePlatformWindows();
                    ImGui::RenderPlatformWindowsDefault();
                }
                windowManager.SwapBuffers();
            }

            // Cleanup
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplSDL2_Shutdown();
            ImGui::DestroyContext();
        }

};