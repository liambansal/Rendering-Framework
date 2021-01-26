//////////////////////////////
// File: GraphicsHelper.cpp.
// Author: Liam Bansal.
// Date Created: 15/11/2020.
//////////////////////////////

/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#include "GraphicsHelper.h" // File's header.
#include <nn/nn_Assert.h>
#include <nn/vi.h>
#include <nv/nv_MemoryManagement.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <cstdlib>


namespace
{
    EGLDisplay g_Display;
    EGLSurface g_Surface;
    EGLContext g_Context;

    nn::vi::NativeWindowHandle g_NativeWindowHandle;
    nn::vi::Display* g_pDisplay;
    nn::vi::Layer* g_pLayer;

    void* NvAllocateFunction(size_t a_size, size_t a_alignment, void* a_userPtr)
    {
        NN_UNUSED(a_userPtr);
        // According to specifications of aligned_alloc(), we need to coordinate the size parameter to become the integral multiple of alignment.
        return aligned_alloc(a_alignment, nn::util::align_up(a_size, a_alignment));
    }

    void NvFreeFunction(void* a_addr, void* a_userPtr)
    {
        NN_UNUSED(a_userPtr);
        free(a_addr);
    }

    void* NvReallocateFunction(void* a_addr, size_t a_newSize, void* a_userPtr)
    {
        NN_UNUSED(a_userPtr);
        return realloc(a_addr, a_newSize);
    }

    void* NvDevtoolsAllocateFunction(size_t a_size, size_t a_alignment, void* a_userPtr)
    {
        NN_UNUSED(a_userPtr);
        // According to specifications of aligned_alloc(), we need to coordinate the size parameter to become the integral multiple of alignment.
        return aligned_alloc(a_alignment, nn::util::align_up(a_size, a_alignment));
    }

    void NvDevtoolsFreeFunction(void* a_addr, void* a_userPtr)
    {
        NN_UNUSED(a_userPtr);
        free(a_addr);
    }

    void* NvDevtoolsReallocateFunction(void* a_addr, size_t a_newSize, void* a_userPtr)
    {
        NN_UNUSED(a_userPtr);
        return realloc(a_addr, a_newSize);
    }
}

void GraphicsHelper::Initialize()
{
    /*
    * Set memory allocator for graphics subsystem.
    * This function must be called before using any graphics API's.
    */
    nv::SetGraphicsAllocator(NvAllocateFunction, NvFreeFunction, NvReallocateFunction, NULL);

    /*
    * Set memory allocator for graphics developer tools and NVN debug layer.
    * This function must be called before using any graphics developer features.
    */
    nv::SetGraphicsDevtoolsAllocator(NvDevtoolsAllocateFunction, NvDevtoolsFreeFunction, NvDevtoolsReallocateFunction, NULL);
    /*
    * Donate memory for graphics driver to work in.
    * This function must be called before using any graphics API's.
    */
    size_t graphicsSystemMemorySize = 8 * 1024 * 1024;
    void* graphicsHeap = malloc(graphicsSystemMemorySize);
    nv::InitializeGraphics(graphicsHeap, graphicsSystemMemorySize);

    /*
    * Initialize Video Interface (VI) system to display
    * to the target's screen
    */
    nn::vi::Initialize();

    nn::Result result = nn::vi::OpenDefaultDisplay(&g_pDisplay);
    NN_ASSERT(result.IsSuccess());

    result = nn::vi::CreateLayer(&g_pLayer, g_pDisplay);
    NN_ASSERT(result.IsSuccess());

    result = nn::vi::GetNativeWindow(&g_NativeWindowHandle, g_pLayer);
    NN_ASSERT(result.IsSuccess());

    /*
    * Initialize EGL
    */
    EGLBoolean eglResult;

    g_Display = ::eglGetDisplay(EGL_DEFAULT_DISPLAY);
    NN_ASSERT(g_Display != NULL, "eglGetDisplay failed.");

    eglResult = ::eglInitialize(g_Display, 0, 0);
    NN_ASSERT(eglResult, "eglInitialize failed.");

    EGLint configAttribs[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_STENCIL_SIZE, 8,
        EGL_NONE
    };
    EGLint numConfigs = 0;
    EGLConfig config;
    eglResult = ::eglChooseConfig(g_Display, configAttribs, &config, 1, &numConfigs);
    NN_ASSERT(eglResult && numConfigs == 1, "eglChooseConfig failed.");

    g_Surface = ::eglCreateWindowSurface(g_Display, config,
        static_cast<NativeWindowType>(g_NativeWindowHandle), 0);
    NN_ASSERT(g_Surface != EGL_NO_SURFACE, "eglCreateWindowSurface failed.");

    /*
    * Set the current rendering API.
    */
    eglResult = eglBindAPI(EGL_OPENGL_API);
    NN_ASSERT(eglResult, "eglBindAPI failed.");

    /*
    * Create new context and set it as current.
    */
    EGLint contextAttribs[] = {
        /* Set target garaphics api version. */
        EGL_CONTEXT_MAJOR_VERSION, 4,
        EGL_CONTEXT_MINOR_VERSION, 5,
        EGL_NONE
    };
    g_Context = ::eglCreateContext(g_Display, config, EGL_NO_CONTEXT, contextAttribs);
    NN_ASSERT(g_Context != EGL_NO_CONTEXT, "eglCreateContext failed. %d", eglGetError());

    eglResult = ::eglMakeCurrent(g_Display, g_Surface, g_Surface, g_Context);
    NN_ASSERT(eglResult, "eglMakeCurrent failed.");
}

void GraphicsHelper::Finalize()
{
    EGLBoolean eglResult;

    eglResult = ::eglMakeCurrent(g_Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    NN_ASSERT(eglResult, "eglMakeCurrent failed.");
    eglResult = ::eglTerminate(g_Display);
    NN_ASSERT(eglResult, "eglTerminate failed.");
    eglResult = ::eglReleaseThread();
    NN_ASSERT(eglResult, "eglReleaseThread failed.");

    nn::vi::DestroyLayer(g_pLayer);
    nn::vi::CloseDisplay(g_pDisplay);
    nn::vi::Finalize();

    nv::FinalizeGraphics();
}

void GraphicsHelper::SwapBuffers()
{
    ::eglSwapBuffers(g_Display, g_Surface);
}
