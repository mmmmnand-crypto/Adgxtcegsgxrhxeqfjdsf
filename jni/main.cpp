
#include <link.h>
#include <GLES3/gl3.h>  // Для OpenGL ES 3.0
#include <GLES3/gl3ext.h> // Расl
#include <jni.h>
#include <iostream>
#include <cstring>
#include <cstdint>
#include <vector>
#include <dlfcn.h>
#include <fstream>
#include <cstdlib>

//static uintptr_t g_module_addr = 0;
std::vector<std::vector<uint64_t>> hook;
using glDrawElements_t = void(*)(GLenum mode, GLsizei count, GLenum type, const void* indices);
static glDrawElements_t my_glDrawElements = nullptr;


<<<<<<< HEAD
extern "C" {

// Инициализация ImGui
JNIEXPORT void JNICALL
Java_com_blackhub_bronline_game_core_JNILib_initIm(
    JNIEnv* env, 
    jclass clazz, 
    jobject surface) {
    
    if (g_initialized) return;
    
    // Получаем NativeWindow из Surface
    g_window = ANativeWindow_fromSurface(env, surface);
    if (!g_window) {
        LOGI("Failed to get ANativeWindow");
        return;
=======
/*int callback(struct dl_phdr_info *info, size_t size, void *data) {
    if (std::strstr(info->dlpi_name, "libblackrussia-client.so")) {
        g_module_addr = info->dlpi_addr;
        return 1;
>>>>>>> a63958f (134)
    }
    return 0;
}*/

//*void init() {
	if (g_module_addr == 0) {
		dl_iterate_phdr(callback, nullptr);
	}
	glDrawElements_t glDrawElements = (glDrawElements_t)(g_module_addr + offsetDraw);
	
}*/
//*extern "C" void CustomCommand(uint64_t x0) {
	if (std::strcmp((const char*)x0, "/record") == 0) {
		
	}
}*/
extern "C" void DrawElements(uint32_t w0, uint32_t w1, uint32_t w2, uint64_t x3) {
	if (hook.size() == 30) {
		std::ofstream file("/sdcard/Android/data/com.br.top/files/test.txt");
		if (file.is_open()) {
     		   for (const auto& call : hook) {
        	    file << call[0] << call[1] << call[2] << std::hex << call[3] << std::dec << '\n';
        	   }
	    	}
		file.close();
		std::abort();
	}
	hook.push_back({(uint64_t)w0,(uint64_t)w1,(uint64_t)w2,x3});
	my_glDrawElements(w0, w1, w2, x3);
}


//*extern "C" JNIEXPORT void JNICALL
Java_com_blackhub_bronline_game_core_JNILib_initGame(JNIEnv* env, jobject) {
	init();
}*/
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    void* handle = dlopen("libGLESv2.so", RTLD_NOW);
    if (handle) {
        my_glDrawElements = (glDrawElements_t)dlsym(handle, "glDrawElements");
        dlclose(handle);
    }

    return JNI_VERSION_1_6;
}
