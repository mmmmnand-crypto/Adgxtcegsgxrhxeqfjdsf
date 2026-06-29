#include <jni.h>
#include <android/native_window_jni.h>
#include <android/log.h>
#include <./imgui/imgui.h>
#include <./imgui/backends/imgui_impl_android.h>
#include <./imgui/backends/imgui_impl_opengl3.h>

#define LOG_TAG "ImGuiJNI"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

static ANativeWindow* g_window = nullptr;
static bool g_initialized = false;

extern "C" {

// Инициализация ImGui
JNIEXPORT void JNICALL
Java_com_blackhub_bronline_game_core_JNILib_init(
    JNIEnv* env, 
    jclass clazz, 
    jobject surface) {
    
    if (g_initialized) return;
    
    // Получаем NativeWindow из Surface
    g_window = ANativeWindow_fromSurface(env, surface);
    if (!g_window) {
        LOGI("Failed to get ANativeWindow");
        return;
    }
    
    // Инициализация ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(1080.0f, 2400.0f); // Установите свой размер
    
    // Инициализация для Android
    ImGui_ImplAndroid_Init(g_window);
    ImGui_ImplOpenGL3_Init("#version 300 es");
    
    // Настройка стиля
    ImGui::StyleColorsDark();
    
    g_initialized = true;
    LOGI("ImGui initialized successfully");
}

// Главный цикл рендеринга
JNIEXPORT void JNICALL
Java_com_blackhub_bronline_game_core_JNILib_loop(
    JNIEnv* env, 
    jclass clazz) {
    
    if (!g_initialized || !g_window) {
        LOGI("Not initialized or window is null");
        return;
    }
    
    // Новый кадр ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();
    
    // === ОКНО ИГРОКА ===
    ImGui::Begin("Player Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Health: 100/100");
    ImGui::Text("Armor: 50/100");
    ImGui::Text("Position: (%.2f, %.2f, %.2f)", 10.5f, 20.3f, 5.0f);
    ImGui::End();
    
    // === ОКНО НАСТРОЕК ===
    ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    static float sensitivity = 1.0f;
    ImGui::SliderFloat("Sensitivity", &sensitivity, 0.1f, 5.0f);
    
    static bool crosshair = true;
    ImGui::Checkbox("Show Crosshair", &crosshair);
    
    static int fov = 90;
    ImGui::SliderInt("FOV", &fov, 60, 120);
    
    if (ImGui::Button("Save Settings")) {
        LOGI("Settings saved!");
    }
    ImGui::End();
    
    // === ДЕБАГ ОКНО ===
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("Frame time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
    
    static float test_array[10] = {0.0f};
    for (int i = 0; i < 10; i++) {
        ImGui::SliderFloat(("Value " + std::to_string(i)).c_str(), &test_array[i], 0.0f, 1.0f);
    }
    ImGui::End();
    
    // Рендеринг
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


}
