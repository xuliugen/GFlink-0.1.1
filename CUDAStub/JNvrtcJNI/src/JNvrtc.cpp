/*
 * CUDAWrapper - Java bindings for NVIDIA CUDA driver and runtime API
 *
 * Copyright (c) 2009-2016 Marco Hutter - http://www.CUDAWrapper.org
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "JNvrtc.hpp"
#include "JNvrtc_common.hpp"
#include <cstring>
#include <string>

/**
 * Called when the library is loaded. Will initialize all
 * required field and method IDs
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved)
{
    JNIEnv *env = NULL;
    if (jvm->GetEnv((void **)&env, JNI_VERSION_1_4))
    {
        return JNI_ERR;
    }

    Logger::log(LOG_TRACE, "Initializing CUDAStub\n");

    jclass cls = NULL;

    // Initialize the JNIUtils and PointerUtils
    if (initJNIUtils(env) == JNI_ERR) return JNI_ERR;
    if (initPointerUtils(env) == JNI_ERR) return JNI_ERR;

    return JNI_VERSION_1_4;
}


JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved)
{
}


/*
* Class:     CUDAWrapper_nvrtc_JNvrtc
* Method:    setLogLevel
* Signature: (I)V
*/
JNIEXPORT void JNICALL Java_CUDAWrapper_nvrtc_JNvrtc_setLogLevel
(JNIEnv *env, jclass cls, jint logLevel)
{
    Logger::setLogLevel((LogLevel)logLevel);
}

/*
* Class:     CUDAWrapper_nvrtc_JNvrtc
* Method:    nvrtcGetErrorStringNative
* Signature: (I)Ljava/lang/String;
*/
JNIEXPORT jstring JNICALL Java_CUDAWrapper_nvrtc_JNvrtc_nvrtcGetErrorStringNative
(JNIEnv *env, jclass cls, jint result)
{
    Logger::log(LOG_TRACE, "Executing nvrtcGetErrorString\n");

    const char *nativeString = nvrtcGetErrorString((nvrtcResult)result);
    jstring string = NULL;
    if (nativeString != NULL)
    {
        string = env->NewStringUTF(nativeString);
    }
    return string;

}

/*
* Class:     CUDAWrapper_nvrtc_JNvrtc
* Method:    nvrtcVersionNative
* Signature: ([I[I)I
*/
JNIEXPORT jint JNICALL Java_CUDAWrapper_nvrtc_JNvrtc_nvrtcVersionNative
(JNIEnv *env, jclass cls, jintArray major, jintArray minor)
{
    if (major == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", 
            "Parameter 'major' is null for nvrtcVersion");
        return CUDA_INTERNAL_ERROR;
    }
    if (minor == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", 
            "Parameter 'minor' is null for nvrtcVersion");
        return CUDA_INTERNAL_ERROR;
    }

    Logger::log(LOG_TRACE, "Executing nvrtcVersion\n");

    int nativeMajor = 0;
    int nativeMinor = 0;
    int result = nvrtcVersion(&nativeMajor, &nativeMinor);
    if (!set(env, major, 0, nativeMajor)) return CUDA_INTERNAL_ERROR;
    if (!set(env, minor, 0, nativeMinor)) return CUDA_INTERNAL_ERROR;
    return result;
}

/*
* Class:     CUDAWrapper_nvrtc_JNvrtc
* Method:    nvrtcCreateProgramNative
* Signature: (LCUDAWrapper/nvrtc/nvrtcProgram;Ljava/lang/String;Ljava/lang/String;I[Ljava/lang/String;[Ljava/lang/String;)I
*/
JNIEXPORT jint JNICALL Java_CUDAWrapper_nvrtc_JNvrtc_nvrtcCreateProgramNative
(JNIEnv *env, jclass cls, jobject prog, jstring src, jstring name, jint numHeaders, jobjectArray headers, jobjectArray includeNames)
{
    if (prog == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", 
            "Parameter 'prog' is null for nvrtcCreateProgram");
        return CUDA_INTERNAL_ERROR;
    }
    if (src == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", 
            "Parameter 'src' is null for nvrtcCreateProgram");
        return CUDA_INTERNAL_ERROR;
    }

    Logger::log(LOG_TRACE, "Executing nvrtcCreateProgram\n");

    nvrtcProgram nativeProg = (nvrtcProgram)(intptr_t)getNativePointerValue(env, prog);
    char *nativeSrc = convertString(env, src);
    char *nativeName = NULL;
    if (name != NULL)
    {
        nativeName = convertString(env, name);
    }
    char **nativeHeaders = NULL;
    if (headers != NULL)
    {
        jsize actualNum = env->GetArrayLength(headers);
        if (actualNum != numHeaders)
        {
            ThrowByName(env, "java/lang/IllegalArgumentException",
                "The headers array length must be numHeaders");
            return CUDA_INTERNAL_ERROR;
        }
        nativeHeaders = convertStringArray(env, headers);
        if (nativeHeaders == NULL)
        {
            return CUDA_INTERNAL_ERROR;
        }
    }
    char **nativeIncludeNames = NULL;
    if (includeNames != NULL)
    {
        jsize actualNum = env->GetArrayLength(includeNames);
        if (actualNum != numHeaders)
        {
            ThrowByName(env, "java/lang/IllegalArgumentException",
                "The includeNames array length must be numHeaders");
            return CUDA_INTERNAL_ERROR;
        }
        nativeIncludeNames = convertStringArray(env, includeNames);
        if (nativeIncludeNames == NULL)
        {
            return CUDA_INTERNAL_ERROR;
        }
    }

    int result = nvrtcCreateProgram(&nativeProg, nativeSrc, nativeName, (int)numHeaders, 
        const_cast<const char**>(nativeHeaders), 
        const_cast<const char**>(nativeIncludeNames));
    setNativePointerValue(env, prog, (jlong)nativeProg);
    delete[] nativeSrc;
    delete[] nativeName;
    deleteStringArray(nativeHeaders, numHeaders);
    deleteStringArray(nativeIncludeNames, numHeaders);

    return result;
}

/*
* Class:     CUDAWrapper_nvrtc_JNvrtc
* Method:    nvrtcDestroyProgramNative
* Signature: (LCUDAWrapper/nvrtc/nvrtcProgram;)I
*/
JNIEXPORT jint JNICALL Java_CUDAWrapper_nvrtc_JNvrtc_nvrtcDestroyProgramNative
(JNIEnv *env, jclass cls, jobject prog)
{
    if (prog == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", 
            "Parameter 'prog' is null for nvrtcDestroyProgram");
        return CUDA_INTERNAL_ERROR;
    }

    Logger::log(LOG_TRACE, "Executing nvrtcDestroyProgram\n");

    nvrtcProgram nativeProg = (nvrtcProgram)(intptr_t)getNativePointerValue(env, prog);
    int result = nvrtcDestroyProgram(&nativeProg);
    setNativePointerValue(env, prog, (jlong)nativeProg);

    return result;
}

/*
* Class:     CUDAWrapper_nvrtc_JNvrtc
* Method:    nvrtcCompileProgramNative
* Signature: (LCUDAWrapper/nvrtc/nvrtcProgram;I[Ljava/lang/String;)I
*/
JNIEXPORT jint JNICALL Java_CUDAWrapper_nvrtc_JNvrtc_nvrtcCompileProgramNative
(JNIEnv *env, jclass cls, jobject prog, jint numOptions, jobjectArray options)
{
    if (prog == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", 
            "Parameter 'prog' is null for nvrtcCompileProgram");
        return CUDA_INTERNAL_ERROR;
    }

    Logger::log(LOG_TRACE, "Executing nvrtcCompileProgram\n");

    nvrtcProgram nativeProg = (nvrtcProgram)(intptr_t)getNativePointerValue(env, prog);
    char **nativeOptions = NULL;
    if (options != NULL)
    {
        jsize actualNum = env->GetArrayLength(options);
        if (actualNum != numOptions)
        {
            ThrowByName(env, "java/lang/IllegalArgumentException",
                "The options array length must be numOptions");
            return CUDA_INTERNAL_ERROR;
        }
        nativeOptions = convertStringArray(env, options);
        if (nativeOptions == NULL)
        {
            return CUDA_INTERNAL_ERROR;
        }
    }

    int result = nvrtcCompileProgram(nativeProg, (int)numOptions, 
        const_cast<const char**>(nativeOptions));
    setNativePointerValue(env, prog, (jlong)nativeProg);
    deleteStringArray(nativeOptions, numOptions);

    return result;
}

/*
* Class:     CUDAWrapper_nvrtc_JNvrtc
* Method:    nvrtcGetPTXSizeNative
* Signature: (LCUDAWrapper/nvrtc/nvrtcProgram;[J)I
*/
JNIEXPORT jint JNICALL Java_CUDAWrapper_nvrtc_JNvrtc_nvrtcGetPTXSizeNative
(JNIEnv *env, jclass cls, jobject prog, jlongArray ptxSizeRet)
{
    if (prog == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", 
            "Parameter 'prog' is null for nvrtcGetPTXSize");
        return CUDA_INTERNAL_ERROR;
    }
    if (ptxSizeRet == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", 
            "Parameter 'ptxSizeRet' is null for nvrtcGetPTXSize");
        return CUDA_INTERNAL_ERROR;
    }

    Logger::log(LOG_TRACE, "Executing nvrtcGetPTXSize\n");

    nvrtcProgram nativeProg = (nvrtcProgram)(intptr_t)getNativePointerValue(env, prog);
    size_t nativePtxSizeRet = 0;
    int result = nvrtcGetPTXSize(nativeProg, &nativePtxSizeRet);
    if (!set(env, ptxSizeRet, 0, nativePtxSizeRet)) return CUDA_INTERNAL_ERROR;
    return result;
}

/*
* Class:     CUDAWrapper_nvrtc_JNvrtc
* Method:    nvrtcGetPTXNative
* Signature: (LCUDAWrapper/nvrtc/nvrtcProgram;[Ljava/lang/String;)I
*/
JNIEXPORT jint JNICALL Java_CUDAWrapper_nvrtc_JNvrtc_nvrtcGetPTXNative
(JNIEnv *env, jclass cls, jobject prog, jobjectArray ptx)
{
    if (prog == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", 
            "Parameter 'prog' is null for nvrtcGetPTX");
        return CUDA_INTERNAL_ERROR;
    }
    if (ptx == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", 
            "Parameter 'ptx' is null for nvrtcGetPTX");
        return CUDA_INTERNAL_ERROR;
    }

    Logger::log(LOG_TRACE, "Executing nvrtcGetPTX\n");

    int length = env->GetArrayLength(ptx);
    if (length == 0)
    {
        ThrowByName(env, "java/lang/IllegalArgumentException",
            "String array must at least have length 1");
        return CUDA_INTERNAL_ERROR;
    }
    nvrtcProgram nativeProg = (nvrtcProgram)(intptr_t)getNativePointerValue(env, prog);
    size_t nativePtxSizeRet = 0;
    int result = nvrtcGetPTXSize(nativeProg, &nativePtxSizeRet);
    if (result != NVRTC_SUCCESS)
    {
        return result;
    }
    char *nativePtx = new char[nativePtxSizeRet];
    if (nativePtx == NULL)
    {
        ThrowByName(env, "java/lang/OutOfMemoryError",
            "Out of memory while creating result string");
        return CUDA_INTERNAL_ERROR;
    }
    result = nvrtcGetPTX(nativeProg, nativePtx);
    jstring ptxElement = env->NewStringUTF(nativePtx);
    delete[] nativePtx;
    if (ptxElement == NULL)
    {
        ThrowByName(env, "java/lang/OutOfMemoryError",
            "Out of memory while creating result string");
        return CUDA_INTERNAL_ERROR;
    }
    env->SetObjectArrayElement(ptx, 0, ptxElement);
    return result;
}

/*
* Class:     CUDAWrapper_nvrtc_JNvrtc
* Method:    nvrtcGetProgramLogSizeNative
* Signature: (LCUDAWrapper/nvrtc/nvrtcProgram;[J)I
*/
JNIEXPORT jint JNICALL Java_CUDAWrapper_nvrtc_JNvrtc_nvrtcGetProgramLogSizeNative
(JNIEnv *env, jclass cls, jobject prog, jlongArray logSizeRet)
{
    if (prog == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", 
            "Parameter 'prog' is null for nvrtcGetProgramLogSize");
        return CUDA_INTERNAL_ERROR;
    }
    if (logSizeRet == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException", 
            "Parameter 'logSizeRet' is null for nvrtcGetProgramLogSize");
        return CUDA_INTERNAL_ERROR;
    }

    Logger::log(LOG_TRACE, "Executing nvrtcGetProgramLogSize\n");

    nvrtcProgram nativeProg = (nvrtcProgram)(intptr_t)getNativePointerValue(env, prog);
    size_t nativeLogSizeRet = 0;
    int result = nvrtcGetProgramLogSize(nativeProg, &nativeLogSizeRet);
    if (!set(env, logSizeRet, 0, nativeLogSizeRet)) return CUDA_INTERNAL_ERROR;
    return result;

}

/*
* Class:     CUDAWrapper_nvrtc_JNvrtc
* Method:    nvrtcGetProgramLogNative
* Signature: (LCUDAWrapper/nvrtc/nvrtcProgram;[Ljava/lang/String;)I
*/
JNIEXPORT jint JNICALL Java_CUDAWrapper_nvrtc_JNvrtc_nvrtcGetProgramLogNative
(JNIEnv *env, jclass cls, jobject prog, jobjectArray log)
{
    if (prog == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException",
            "Parameter 'prog' is null for nvrtcGetProgramLog");
        return CUDA_INTERNAL_ERROR;
    }
    if (log == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException",
            "Parameter 'log' is null for nvrtcGetProgramLog");
        return CUDA_INTERNAL_ERROR;
    }

    Logger::log(LOG_TRACE, "Executing nvrtcGetProgramLog\n");

    int length = env->GetArrayLength(log);
    if (length == 0)
    {
        ThrowByName(env, "java/lang/IllegalArgumentException",
            "String array must at least have length 1");
        return CUDA_INTERNAL_ERROR;
    }
    nvrtcProgram nativeProg = (nvrtcProgram)(intptr_t)getNativePointerValue(env, prog);
    size_t nativeLogSizeRet = 0;
    int result = nvrtcGetProgramLogSize(nativeProg, &nativeLogSizeRet);
    if (result != NVRTC_SUCCESS)
    {
        return result;
    }
    char *nativeLog = new char[nativeLogSizeRet];
    if (nativeLog == NULL)
    {
        ThrowByName(env, "java/lang/OutOfMemoryError",
            "Out of memory while creating result string");
        return CUDA_INTERNAL_ERROR;
    }
    result = nvrtcGetProgramLog(nativeProg, nativeLog);
    jstring logElement = env->NewStringUTF(nativeLog);
    delete[] nativeLog;
    if (logElement == NULL)
    {
        ThrowByName(env, "java/lang/OutOfMemoryError",
            "Out of memory while creating result string");
        return CUDA_INTERNAL_ERROR;
    }
    env->SetObjectArrayElement(log, 0, logElement);
    return result;

}


/*
* Class:     CUDAWrapper_nvrtc_JNvrtc
* Method:    nvrtcAddNameExpressionNative
* Signature: (LCUDAWrapper/nvrtc/nvrtcProgram;Ljava/lang/String;)I
*/
JNIEXPORT jint JNICALL Java_CUDAWrapper_nvrtc_JNvrtc_nvrtcAddNameExpressionNative
(JNIEnv *env, jclass cls, jobject prog, jstring name_expression)
{
    if (prog == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException",
            "Parameter 'prog' is null for nvrtcAddNameExpression");
        return CUDA_INTERNAL_ERROR;
    }
    if (name_expression == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException",
            "Parameter 'name_expression' is null for nvrtcAddNameExpression");
        return CUDA_INTERNAL_ERROR;
    }

    Logger::log(LOG_TRACE, "Executing nvrtcAddNameExpression\n");

    nvrtcProgram nativeProg = (nvrtcProgram)(intptr_t)getNativePointerValue(env, prog);
    char *nativeName_expression = convertString(env, name_expression);

    int result = nvrtcAddNameExpression(nativeProg, nativeName_expression);

    delete[] nativeName_expression;
    return result;
}

/*
* Class:     CUDAWrapper_nvrtc_JNvrtc
* Method:    nvrtcGetLoweredNameNative
* Signature: (LCUDAWrapper/nvrtc/nvrtcProgram;Ljava/lang/String;[Ljava/lang/String;)I
*/
JNIEXPORT jint JNICALL Java_CUDAWrapper_nvrtc_JNvrtc_nvrtcGetLoweredNameNative
(JNIEnv *env, jclass cls, jobject prog, jstring name_expression, jobjectArray lowered_name)
{
    if (prog == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException",
            "Parameter 'prog' is null for nvrtcGetLoweredName");
        return CUDA_INTERNAL_ERROR;
    }
    if (name_expression == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException",
            "Parameter 'name_expression' is null for nvrtcGetLoweredName");
        return CUDA_INTERNAL_ERROR;
    }
    if (lowered_name == NULL)
    {
        ThrowByName(env, "java/lang/NullPointerException",
            "Parameter 'lowered_name' is null for nvrtcGetLoweredName");
        return CUDA_INTERNAL_ERROR;
    }
    int length = env->GetArrayLength(lowered_name);
    if (length == 0)
    {
        ThrowByName(env, "java/lang/IllegalArgumentException",
            "String array lowered_name must at least have length 1");
        return CUDA_INTERNAL_ERROR;
    }

    Logger::log(LOG_TRACE, "Executing nvrtcGetLoweredName\n");

    nvrtcProgram nativeProg = (nvrtcProgram)(intptr_t)getNativePointerValue(env, prog);
    char *nativeName_expression = convertString(env, name_expression);
    const char *nativeLowered_name;

    int result = nvrtcGetLoweredName(nativeProg, nativeName_expression, &nativeLowered_name);

    jstring lowered_nameElement = env->NewStringUTF(nativeLowered_name);
    if (lowered_nameElement == NULL)
    {
        ThrowByName(env, "java/lang/OutOfMemoryError",
            "Out of memory while creating result string");
        return CUDA_INTERNAL_ERROR;
    }
    env->SetObjectArrayElement(lowered_name, 0, lowered_nameElement);

    delete[] nativeName_expression;
    return result;
}

