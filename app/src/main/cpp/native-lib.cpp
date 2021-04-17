#include <jni.h>
#include <string>
#include <android/log.h>
#include <unistd.h>

#include "pthread.h"



#define TAG "myDemo-jni" // 这个是自定义的LOG的标识

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_jnidemo_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject obj) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnidemo_MainActivity_callSystemMethod(JNIEnv *env, jobject thiz, jobject context) {
    // TODO: implement CallSystemMethod()

    jclass toastClazz = env->FindClass("android/widget/Toast");
    jmethodID makeTextMethodId = env->GetStaticMethodID(toastClazz,"makeText", "(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;");
    jstring  fromJni = env->NewStringUTF("调用系统方法");
    if (fromJni == NULL){
        return;
    }
    jobject mtObject = env->CallStaticObjectMethod(toastClazz,makeTextMethodId,context,fromJni,1);
    jmethodID showId = env->GetMethodID(toastClazz,"show","()V");
    env->CallVoidMethod(mtObject,showId);
    env->DeleteLocalRef(toastClazz);
    env->DeleteLocalRef(mtObject);
    env->DeleteLocalRef(fromJni);

}extern "C"
JNIEXPORT jint JNICALL
Java_com_example_jnidemo_MainActivity_callJavaField(JNIEnv *env, jobject obj) {
    // TODO: implement CallJavaField()
    jclass  clazz = env->GetObjectClass(obj);
    jfieldID  aFieldId = env->GetStaticFieldID(clazz,"a","I");
    env->SetStaticIntField(clazz,aFieldId,200);
    jint aInt = env->GetStaticIntField(clazz,aFieldId);
    env->DeleteLocalRef(clazz);
    return aInt;
}extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnidemo_MainActivity_callMethod(JNIEnv *env, jobject obj) {
    // TODO: implement callMethod()
    jclass clazz = env->GetObjectClass(obj);
    jmethodID vMethodId = env->GetMethodID(clazz,"toastJavaTest1","()V");
    if (vMethodId == NULL){
        return;
    }
    env->CallVoidMethod(obj,vMethodId);
}extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnidemo_MainActivity_callInitMethod(JNIEnv *env, jobject obj) {
    // TODO: implement callInitMethod()
    jclass clazz = env->FindClass("com/example/jnidemo/ConstrueTest");
    jmethodID initMethod = env->GetMethodID(clazz,"<init>","()V");
    if (initMethod == NULL){
        LOGD("没有找到该构造方法");
        return;
    }
    env->NewObject(clazz,initMethod);

}extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_example_jnidemo_MainActivity_initInt2DArray(JNIEnv *env, jobject obj, jint size) {
    // TODO: implement initInt2DArray()
    jobjectArray result;
    jclass intClazz = env->FindClass("[I");
    if (intClazz == NULL){
        return NULL;
    }

    result = env->NewObjectArray(size,intClazz,NULL);

    if (result == NULL){
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        jint  temp[256];
        jintArray iArr = env->NewIntArray(size);

        if (iArr == NULL){
            return NULL;
        }

        for (int j = 0; j < size; j++) {
            temp[j] = i+j;
        }

        env->SetIntArrayRegion(iArr,0,size,temp);
        env->SetObjectArrayElement(result,i,iArr);
    }
    return result;

}


/**
 * 线程
 */


typedef struct ClassInfo {
    JavaVM *jvm;   //保存java虚拟机,这是在新线程中能够回调到java方法的最重要的参数.
    jobject obj;   //保存java对象
    jmethodID callbackMethodId; //保存methodID
}ClassInfo;

//定义一个全局的ClassInfo
ClassInfo gClassInfo;

//该函数用来在线程中调用, callback函数用来调用java方法
void callback(int i)
{
    JNIEnv *env;


    //通过jvm的接口,attach到当前线程,同时该函数还会获取到所需的env变量.
    //获取jnienv指针
    gClassInfo.jvm->AttachCurrentThread(&env,NULL);
    //回调java方法
    env->CallVoidMethod(gClassInfo.obj, gClassInfo.callbackMethodId, i);
    //调用完之后,detach.
    gClassInfo.jvm->DetachCurrentThread();
}


//static const char *classPathName = "com/example/jnidemo/MainActivity";
//static JNINativeMethod methods[] = {"createNativeThread", "()V",
////                                  static_cast<void *>(Java_com_example_jnidemo_MainActivity_createNativeThread)};
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;

     //获取JNI版本
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
    {
         LOGE("GetEnv failed!");
         return result;
    }

//    if(env->RegisterNatives(classPathName,methods, sizeof(methods)/ sizeof(methods[0])))

    return JNI_VERSION_1_4;
}

//线程工作函数
int quit;
//用来退出线程循环,实际项目,尽量不要使用全局变量
void* work(void *arg) {
    LOGD("native thread begin...");
    int i = 0;
    quit = 1;
    while(quit) {
        if(i == 10) {
            break;
        }
        i++;
        callback(i);
        //耗时操作
        sleep(1);
    }
    LOGD("native thread exit...");
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnidemo_MainActivity_createNativeThread(JNIEnv *env, jobject obj) {
    // TODO: implement createNativeThread()
    /** 说明:在jni层如果有多线程,实际上JNIEnv(jni环境变量)是不能够在多线程中共用的, env只能在当前线程有效,
	  * 但是JavaVM可以,JavaVM指Java虚拟机,这个变量是进程可共用的.所以要想在其他线程中回调java方法,需要保存的是jvm.
	  */
    //获取jvm
    env->GetJavaVM(&gClassInfo.jvm);
    jclass cls = env->GetObjectClass(obj);
    if (cls == NULL){
        return;
    }

    //获取MethodID
    gClassInfo.callbackMethodId = env->GetMethodID(cls, "callbackFromJNI", "(I)V");
    if (gClassInfo.callbackMethodId == NULL){
        return;
    }
    /** 说明:为了能够在其它线程得到java的对象,必须要obj转化为全局对象,这样在其它线程才能得到当前java对象的索引.
      * 否则在其它线程要用到当前java对象时,会出现无效引用的错误.
      */
    gClassInfo.obj = env->NewGlobalRef(obj);
    if (gClassInfo.obj == NULL){
        return;
    }
    //创建一个线程
    pthread_t pid;
    pthread_create(&pid, NULL, work, NULL);

}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnidemo_MainActivity_releaseNativeThread(JNIEnv *env, jobject thiz) {
    // TODO: implement releaseNativeThread()
    quit = 0;
    gClassInfo.obj = NULL;
    env->DeleteGlobalRef(gClassInfo.obj);
    pthread_exit(NULL);
}extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnidemo_MainActivity_exceptionTest(JNIEnv *env, jobject obj) {
    // TODO: implement exceptionTest()
    jclass clazz = env->GetObjectClass(obj);
    jmethodID methodId = env->GetMethodID(clazz,"exception","()V");
    env->CallVoidMethod(obj,methodId);
    if (env->ExceptionOccurred()){ //判断异常  env->ExceptionCheck();这个只是返回一个boolean对象
        env->ExceptionDescribe(); //打印异常
        env->ExceptionClear(); //清除异常
        jclass iaClazz = env->FindClass("java/lang/NullPointerException");
        if (iaClazz == NULL){
            return;
        }
        //抛出异常
        env->ThrowNew(iaClazz,"throw from C throwable");
    }
}


/**
 * 使用前初始化
 */
jmethodID  callBackMethodId;
extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnidemo_MainActivity_initData(JNIEnv *env, jclass obj) {
    // TODO: implement initData()
    callBackMethodId = env->GetMethodID(obj,"exception","()V");
}