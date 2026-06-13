#define LOG_TAG "QomxCoreShim"

#include <dlfcn.h>
#include <log/log.h>
#include <OMX_Core.h>

static void* load_omx_symbol(const char* name) {
    static void* handle = NULL;
    if (handle == NULL) {
        handle = dlopen("libOmxCore.so", RTLD_NOW | RTLD_GLOBAL);
    }
    if (handle == NULL) {
        ALOGE("Failed to load libOmxCore.so: %s", dlerror());
        return NULL;
    }

    void* symbol = dlsym(handle, name);
    if (symbol == NULL) {
        ALOGE("Missing %s in libOmxCore.so: %s", name, dlerror());
    }
    return symbol;
}

extern "C" OMX_ERRORTYPE OMX_Init() {
    typedef OMX_ERRORTYPE (*Fn)();
    Fn fn = reinterpret_cast<Fn>(load_omx_symbol("OMX_Init"));
    return fn != NULL ? fn() : OMX_ErrorUndefined;
}

extern "C" OMX_ERRORTYPE OMX_Deinit() {
    typedef OMX_ERRORTYPE (*Fn)();
    Fn fn = reinterpret_cast<Fn>(load_omx_symbol("OMX_Deinit"));
    return fn != NULL ? fn() : OMX_ErrorUndefined;
}

extern "C" OMX_ERRORTYPE OMX_GetHandle(OMX_HANDLETYPE* handle,
                                        OMX_STRING component_name,
                                        OMX_PTR app_data,
                                        OMX_CALLBACKTYPE* callbacks) {
    typedef OMX_ERRORTYPE (*Fn)(OMX_HANDLETYPE*, OMX_STRING, OMX_PTR, OMX_CALLBACKTYPE*);
    Fn fn = reinterpret_cast<Fn>(load_omx_symbol("OMX_GetHandle"));
    return fn != NULL ? fn(handle, component_name, app_data, callbacks) : OMX_ErrorUndefined;
}

extern "C" OMX_ERRORTYPE OMX_FreeHandle(OMX_HANDLETYPE handle) {
    typedef OMX_ERRORTYPE (*Fn)(OMX_HANDLETYPE);
    Fn fn = reinterpret_cast<Fn>(load_omx_symbol("OMX_FreeHandle"));
    return fn != NULL ? fn(handle) : OMX_ErrorUndefined;
}

extern "C" OMX_ERRORTYPE OMX_ComponentNameEnum(OMX_STRING component_name,
                                               OMX_U32 name_length,
                                               OMX_U32 index) {
    typedef OMX_ERRORTYPE (*Fn)(OMX_STRING, OMX_U32, OMX_U32);
    Fn fn = reinterpret_cast<Fn>(load_omx_symbol("OMX_ComponentNameEnum"));
    return fn != NULL ? fn(component_name, name_length, index) : OMX_ErrorUndefined;
}

extern "C" OMX_ERRORTYPE OMX_SetupTunnel(OMX_HANDLETYPE output,
                                         OMX_U32 port_output,
                                         OMX_HANDLETYPE input,
                                         OMX_U32 port_input) {
    typedef OMX_ERRORTYPE (*Fn)(OMX_HANDLETYPE, OMX_U32, OMX_HANDLETYPE, OMX_U32);
    Fn fn = reinterpret_cast<Fn>(load_omx_symbol("OMX_SetupTunnel"));
    return fn != NULL ? fn(output, port_output, input, port_input) : OMX_ErrorUndefined;
}

extern "C" OMX_ERRORTYPE OMX_GetContentPipe(OMX_HANDLETYPE* pipe,
                                            OMX_STRING uri) {
    typedef OMX_ERRORTYPE (*Fn)(OMX_HANDLETYPE*, OMX_STRING);
    Fn fn = reinterpret_cast<Fn>(load_omx_symbol("OMX_GetContentPipe"));
    return fn != NULL ? fn(pipe, uri) : OMX_ErrorUndefined;
}

extern "C" OMX_ERRORTYPE OMX_GetComponentsOfRole(OMX_STRING role,
                                                 OMX_U32* num_comps,
                                                 OMX_U8** comp_names) {
    typedef OMX_ERRORTYPE (*Fn)(OMX_STRING, OMX_U32*, OMX_U8**);
    Fn fn = reinterpret_cast<Fn>(load_omx_symbol("OMX_GetComponentsOfRole"));
    return fn != NULL ? fn(role, num_comps, comp_names) : OMX_ErrorUndefined;
}

extern "C" OMX_ERRORTYPE OMX_GetRolesOfComponent(OMX_STRING comp_name,
                                                 OMX_U32* num_roles,
                                                 OMX_U8** roles) {
    typedef OMX_ERRORTYPE (*Fn)(OMX_STRING, OMX_U32*, OMX_U8**);
    Fn fn = reinterpret_cast<Fn>(load_omx_symbol("OMX_GetRolesOfComponent"));
    return fn != NULL ? fn(comp_name, num_roles, roles) : OMX_ErrorUndefined;
}
