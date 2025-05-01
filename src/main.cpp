/* danielsinkin97@gmail.com */
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"

#include "stb_image.h"
#include "stb_image_write.h"

#include <iostream>

constexpr const char *vertexShaderSource = "#version 330 core\n"
                                           "layout (location = 0) in vec3 aPos;\n"
                                           "void main()\n"
                                           "{\n"
                                           "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                           "}\0";

struct Color {
    float r = 0.1f;
    float g = 0.1f;
    float b = 0.1f;
    float a = 0.1f;

    auto rgb() -> float * { return &r; }
    auto rgb() const -> const float * { return &r; }

    auto rgba() -> float * { return &r; }
    auto rgba() const -> const float * { return &r; }
};

struct Globals {
    GLFWwindow *window = nullptr;
    Color background_color;
};
inline Globals globals;

auto _mainloop_imgui() -> void {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Background Color", globals.background_color.rgb());
        ImGui::End();
    }

    ImGui::Render();
}

auto _mainloop_handle_input() -> void {
    glfwPollEvents();
    if (glfwGetKey(globals.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(globals.window, GLFW_TRUE);
    }
}

auto _mainloop_render() -> void {
    int display_w, display_h;
    glfwGetFramebufferSize(globals.window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    const float *bg = globals.background_color.rgba();
    glClearColor(bg[0], bg[1], bg[2], bg[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    float vertices[] = {
        -0.5f,
        -0.5f,
        0.0f,
        0.5f,
        -0.5f,
        0.0f,
        0.0f,
        0.5f,
        0.0f,
    };

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

auto mainloop_iteration() -> void {
    _mainloop_imgui();

    _mainloop_render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(globals.window);

    _mainloop_handle_input();
}

auto main(int argc, char **argv) -> int {
    if (glfwInit() == GLFW_FALSE) return 1;

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    globals.window = glfwCreateWindow(
        1280,
        720,
        "MyWindow",
        nullptr,
        nullptr);
    if (globals.window == nullptr) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(globals.window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return 1;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(globals.window, true);
    ImGui_ImplOpenGL3_Init("#version 410");

    while (!glfwWindowShouldClose(globals.window)) {
        mainloop_iteration();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(globals.window);
    glfwTerminate();

    return 0;
}