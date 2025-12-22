/*
    GLAD - OpenGL Loader Generator
    Implementation for OpenGL 3.3 Core
*/

#include "glad.h"

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    static HMODULE libGL;
    
    static int open_gl(void) {
        libGL = LoadLibraryA("opengl32.dll");
        return libGL != NULL;
    }
    
    static void close_gl(void) {
        if (libGL) {
            FreeLibrary(libGL);
            libGL = NULL;
        }
    }
    
    static void* get_proc(const char* name) {
        void* proc = (void*)wglGetProcAddress(name);
        if (!proc || proc == (void*)0x1 || proc == (void*)0x2 || 
            proc == (void*)0x3 || proc == (void*)-1) {
            proc = (void*)GetProcAddress(libGL, name);
        }
        return proc;
    }
#else
    #include <dlfcn.h>
    static void* libGL;
    
    static int open_gl(void) {
        #ifdef __APPLE__
            libGL = dlopen("/System/Library/Frameworks/OpenGL.framework/OpenGL", RTLD_LAZY | RTLD_LOCAL);
        #else
            libGL = dlopen("libGL.so.1", RTLD_LAZY | RTLD_LOCAL);
            if (!libGL) libGL = dlopen("libGL.so", RTLD_LAZY | RTLD_LOCAL);
        #endif
        return libGL != NULL;
    }
    
    static void close_gl(void) {
        if (libGL) {
            dlclose(libGL);
            libGL = NULL;
        }
    }
    
    static void* get_proc(const char* name) {
        return dlsym(libGL, name);
    }
#endif

// Function pointer definitions
PFNGLCLEARPROC glClear = NULL;
PFNGLCLEARCOLORPROC glClearColor = NULL;
PFNGLENABLEPROC glEnable = NULL;
PFNGLDISABLEPROC glDisable = NULL;
PFNGLVIEWPORTPROC glViewport = NULL;
PFNGLBLENDFUNCPROC glBlendFunc = NULL;
PFNGLCULLFACEPROC glCullFace = NULL;
PFNGLFRONTFACEPROC glFrontFace = NULL;
PFNGLDEPTHFUNCPROC glDepthFunc = NULL;
PFNGLGETSTRINGPROC glGetString = NULL;
PFNGLGETERRORPROC glGetError = NULL;

PFNGLCREATESHADERPROC glCreateShader = NULL;
PFNGLSHADERSOURCEPROC glShaderSource = NULL;
PFNGLCOMPILESHADERPROC glCompileShader = NULL;
PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;
PFNGLDELETESHADERPROC glDeleteShader = NULL;
PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
PFNGLATTACHSHADERPROC glAttachShader = NULL;
PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = NULL;
PFNGLDELETEPROGRAMPROC glDeleteProgram = NULL;
PFNGLUSEPROGRAMPROC glUseProgram = NULL;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;
PFNGLUNIFORM1IPROC glUniform1i = NULL;
PFNGLUNIFORM1FPROC glUniform1f = NULL;
PFNGLUNIFORM3FPROC glUniform3f = NULL;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = NULL;

PFNGLGENBUFFERSPROC glGenBuffers = NULL;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = NULL;
PFNGLBINDBUFFERPROC glBindBuffer = NULL;
PFNGLBUFFERDATAPROC glBufferData = NULL;

PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = NULL;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = NULL;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = NULL;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = NULL;

PFNGLDRAWARRAYSPROC glDrawArrays = NULL;
PFNGLDRAWELEMENTSPROC glDrawElements = NULL;

PFNGLGENTEXTURESPROC glGenTextures = NULL;
PFNGLDELETETEXTURESPROC glDeleteTextures = NULL;
PFNGLBINDTEXTUREPROC glBindTexture = NULL;
PFNGLTEXIMAGE2DPROC glTexImage2D = NULL;
PFNGLTEXPARAMETERIPROC glTexParameteri = NULL;
PFNGLACTIVETEXTUREPROC glActiveTexture = NULL;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap = NULL;

int gladLoadGL(void) {
    if (!open_gl()) return 0;
    
    // Load core functions
    glClear = (PFNGLCLEARPROC)get_proc("glClear");
    glClearColor = (PFNGLCLEARCOLORPROC)get_proc("glClearColor");
    glEnable = (PFNGLENABLEPROC)get_proc("glEnable");
    glDisable = (PFNGLDISABLEPROC)get_proc("glDisable");
    glViewport = (PFNGLVIEWPORTPROC)get_proc("glViewport");
    glBlendFunc = (PFNGLBLENDFUNCPROC)get_proc("glBlendFunc");
    glCullFace = (PFNGLCULLFACEPROC)get_proc("glCullFace");
    glFrontFace = (PFNGLFRONTFACEPROC)get_proc("glFrontFace");
    glDepthFunc = (PFNGLDEPTHFUNCPROC)get_proc("glDepthFunc");
    glGetString = (PFNGLGETSTRINGPROC)get_proc("glGetString");
    glGetError = (PFNGLGETERRORPROC)get_proc("glGetError");
    
    // Load shader functions
    glCreateShader = (PFNGLCREATESHADERPROC)get_proc("glCreateShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC)get_proc("glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADERPROC)get_proc("glCompileShader");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)get_proc("glGetShaderiv");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)get_proc("glGetShaderInfoLog");
    glDeleteShader = (PFNGLDELETESHADERPROC)get_proc("glDeleteShader");
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)get_proc("glCreateProgram");
    glAttachShader = (PFNGLATTACHSHADERPROC)get_proc("glAttachShader");
    glLinkProgram = (PFNGLLINKPROGRAMPROC)get_proc("glLinkProgram");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)get_proc("glGetProgramiv");
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)get_proc("glGetProgramInfoLog");
    glDeleteProgram = (PFNGLDELETEPROGRAMPROC)get_proc("glDeleteProgram");
    glUseProgram = (PFNGLUSEPROGRAMPROC)get_proc("glUseProgram");
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)get_proc("glGetUniformLocation");
    glUniform1i = (PFNGLUNIFORM1IPROC)get_proc("glUniform1i");
    glUniform1f = (PFNGLUNIFORM1FPROC)get_proc("glUniform1f");
    glUniform3f = (PFNGLUNIFORM3FPROC)get_proc("glUniform3f");
    glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)get_proc("glUniformMatrix4fv");
    
    // Load buffer functions
    glGenBuffers = (PFNGLGENBUFFERSPROC)get_proc("glGenBuffers");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)get_proc("glDeleteBuffers");
    glBindBuffer = (PFNGLBINDBUFFERPROC)get_proc("glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)get_proc("glBufferData");
    
    // Load VAO functions
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)get_proc("glGenVertexArrays");
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)get_proc("glDeleteVertexArrays");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)get_proc("glBindVertexArray");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)get_proc("glEnableVertexAttribArray");
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)get_proc("glVertexAttribPointer");
    
    // Load draw functions
    glDrawArrays = (PFNGLDRAWARRAYSPROC)get_proc("glDrawArrays");
    glDrawElements = (PFNGLDRAWELEMENTSPROC)get_proc("glDrawElements");
    
    // Load texture functions
    glGenTextures = (PFNGLGENTEXTURESPROC)get_proc("glGenTextures");
    glDeleteTextures = (PFNGLDELETETEXTURESPROC)get_proc("glDeleteTextures");
    glBindTexture = (PFNGLBINDTEXTUREPROC)get_proc("glBindTexture");
    glTexImage2D = (PFNGLTEXIMAGE2DPROC)get_proc("glTexImage2D");
    glTexParameteri = (PFNGLTEXPARAMETERIPROC)get_proc("glTexParameteri");
    glActiveTexture = (PFNGLACTIVETEXTUREPROC)get_proc("glActiveTexture");
    glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)get_proc("glGenerateMipmap");
    
    return 1;
}
