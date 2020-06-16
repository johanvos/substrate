/*
 * Copyright (c) 2020, Gluon
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLUON BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdlib.h>
#include <string.h>
#include "grandroid.h"

jclass nativeWebViewClass;
jobject nativeWebViewObj;
jmethodID nativeWebView_init;
jmethodID nativeWebView_loadUrl;
int reg = -1;

void registerJavaFXMethodHandles(JNIEnv *aenv)
{
    if (reg < 0) {
        nativeWebViewClass = (*aenv)->NewGlobalRef(aenv, (*aenv)->FindClass(aenv, "com/gluonhq/helloandroid/NativeWebView"));
        nativeWebView_init = (*aenv)->GetMethodID(aenv, nativeWebViewClass, "<init>", "()V");
        nativeWebView_loadUrl = (*aenv)->GetMethodID(aenv, nativeWebViewClass, "loadUrl", "(Ljava/lang/String;)V");
        reg = 1;
    }
}


JNIEXPORT void JNICALL Java_com_gluonhq_helloandroid_MainActivity_nativeSetSurface(JNIEnv *env, jobject activity, jobject surface)
{
    LOGE(stderr, "nativeSetSurface called, env at %p and size %ld, surface at %p\n", env, sizeof(JNIEnv), surface);
    window = ANativeWindow_fromSurface(env, surface);
    androidJfx_setNativeWindow(window);
    LOGE(stderr, "native setSurface Ready, native window at %p\n", window);
}

JNIEXPORT void JNICALL Java_com_gluonhq_helloandroid_MainActivity_nativeSetDataDir(JNIEnv *env, jobject that, jstring jdir)
{
    const char *cdir = (*env)->GetStringUTFChars(env, jdir, 0);
    int len = strnlen(cdir, 512);
    appDataDir = (char *)malloc(len + 1);
    strcpy(appDataDir, cdir);
    LOGE(stderr, "appDataDir: %s", appDataDir);
}

JNIEXPORT void JNICALL Java_com_gluonhq_helloandroid_MainActivity_nativeSetTimezone(JNIEnv *env, jobject that, jstring jtz)
{
    const char *ctz = (*env)->GetStringUTFChars(env, jtz, 0);
    int len = strnlen(ctz, 512);
    timeZone = (char *)malloc(len + 1);
    strcpy(timeZone, ctz);
    LOGE(stderr, "timeZone: %s", timeZone);
}

JNIEXPORT jlong JNICALL Java_com_gluonhq_helloandroid_MainActivity_surfaceReady(JNIEnv *env, jobject activity, jobject surface, jfloat mydensity)
{
    LOGE(stderr, "SurfaceReady, surface at %p\n", surface);
    window = ANativeWindow_fromSurface(env, surface);
    androidJfx_setNativeWindow(window);
    androidJfx_setDensity(mydensity);
    LOGE(stderr, "SurfaceReady, native window at %p\n", window);
    density = mydensity;
    return (jlong)window;
}

JNIEXPORT void JNICALL Java_com_gluonhq_helloandroid_MainActivity_nativeSurfaceRedrawNeeded(JNIEnv *env, jobject activity)
{
    LOGE(stderr, "launcher, nativeSurfaceRedrawNeeded called. Invoke method on glass_monocle\n");
    androidJfx_requestGlassToRedraw();
}

JNIEXPORT jint JNICALL
JNI_OnLoad_javafx_font(JavaVM *vm, void *reserved)
{
    LOGE(stderr, "In dummy JNI_OnLoad_javafx_font\n");
#ifdef JNI_VERSION_1_8
    //min. returned JNI_VERSION required by JDK8 for builtin libraries
    JNIEnv *env;
    if ((*vm)->GetEnv(vm, (void **)&env, JNI_VERSION_1_8) != JNI_OK)
    {
        return JNI_VERSION_1_4;
    }
    return JNI_VERSION_1_8;
#else
    return JNI_VERSION_1_4;
#endif
}

void showSoftwareKeyboard()
{
    ATTACH_DALVIK();
    LOGE(stderr, "now I have to show keyboard, invoke method %p on env %p\n", activity_showIME, dalvikEnv);
    (*dalvikEnv)->CallStaticVoidMethod(dalvikEnv, activityClass, activity_showIME);
    LOGE(stderr, "I did show keyboard\n");
    DETACH_DALVIK();
}

JNIEXPORT void JNICALL
Java_javafx_scene_control_skin_TextAreaSkinAndroid_showSoftwareKeyboard(JNIEnv *env, jobject textareaskin)
{
    showSoftwareKeyboard();
}

JNIEXPORT void JNICALL
Java_javafx_scene_control_skin_TextFieldSkinAndroid_showSoftwareKeyboard(JNIEnv *env, jobject textfieldskin)
{
    showSoftwareKeyboard();
}

void hideSoftwareKeyboard()
{
    ATTACH_DALVIK();
    LOGE(stderr, "now I have to hide keyboard, invoke method %p on env %p\n", activity_hideIME, dalvikEnv);
    (*dalvikEnv)->CallStaticVoidMethod(dalvikEnv, activityClass, activity_hideIME);
    LOGE(stderr, "I did hide keyboard\n");
    DETACH_DALVIK();
}

JNIEXPORT void JNICALL
Java_javafx_scene_control_skin_TextFieldSkinAndroid_hideSoftwareKeyboard(JNIEnv *env, jobject textfieldskin)
{
    hideSoftwareKeyboard();
}

JNIEXPORT void JNICALL
Java_javafx_scene_control_skin_TextAreaSkinAndroid_hideSoftwareKeyboard(JNIEnv *env, jobject textareaskin)
{
    hideSoftwareKeyboard();
}

void substrate_showWebView() {
    fprintf(stderr, "Substrate needs to show webview\n");
    ATTACH_DALVIK();
    // registerJavaFXMethodHandles(dalvikEnv);
    LOGE(stderr, "Substrate needs to show Android WebView\n");
    jobject tmpobj = (jobject)((*dalvikEnv)->NewObject(dalvikEnv, nativeWebViewClass, nativeWebView_init));
    nativeWebViewObj = (jobject)((*dalvikEnv)->NewGlobalRef(dalvikEnv, tmpobj));
fprintf(stderr, "tmpo = %p and wvo = %p\n", tmpobj, nativeWebViewObj);
    LOGE(stderr, "Substrate Created Android WebView\n");
    if ((*dalvikEnv)->ExceptionOccurred(dalvikEnv)) {
fprintf(stderr, "EXCEPTION CREATING WEBVIEW\n");
    } else {
fprintf(stderr, "NOEXCEPTION CREATING WEBVIEW\n");
    }
    DETACH_DALVIK();
}

void substrate_loadUrl(char* curl) {
    ATTACH_DALVIK();
fprintf(stderr, "load curl: %s\n", curl);
    jstring jurl = (*dalvikEnv)->NewStringUTF(dalvikEnv, curl);
fprintf(stderr, "call loadurl and wvo = %p\n", nativeWebViewObj);
    (*dalvikEnv)->CallVoidMethod(dalvikEnv, nativeWebViewObj, nativeWebView_loadUrl, jurl);
    DETACH_DALVIK();
}
