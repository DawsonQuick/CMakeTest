#include "GUI/GUI.h"

GUI::GUI(GLFWwindow* glfwWindow) {
	m_GLFWWindow = glfwWindow;

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_GLFWWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");

}

GUI::~GUI() {

}

void GUI::render(ParticleToGUITransfer particleEngineGui) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("FPS");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    static int selectedIndex = 0;
    ImGui::Begin("Emitter Creator");

    const auto& emitterNames = particleEngineGui.getEmitterNames();

    if (!emitterNames.empty()) {
        if (ImGui::BeginCombo("Emitter Type", emitterNames.at(selectedIndex).c_str())) {
            for (int i = 0; i < emitterNames.size(); ++i) {
                bool isSelected = (i == selectedIndex);
                if (ImGui::Selectable(emitterNames.at(i).c_str(), isSelected)) {
                    selectedIndex = i;
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        if (ImGui::Button("Create Emitter")) {
            const std::string& selectedType = emitterNames.at(selectedIndex);
            particleEngineGui.createEmitter(selectedType);
        }
    } else {
        ImGui::Text("No emitters registered.");
    }

     ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}