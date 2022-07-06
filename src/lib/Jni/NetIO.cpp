#include "Handlers.hpp"
#include "../IFileIO.h"
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     uk_co_lunalux_libos_NetIO
 * Method:    n_createSocket
 * Signature: (ILjava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_uk_co_lunalux_libos_NetIO_n_1createSocket (JNIEnv * jvm, jobject, jint socketBits, jstring address, jint port)
{
    return NetIOHandler::getInstance().j_losCreateSocket(socketBits,jvm->GetStringUTFChars(address,0),port);
}
/*
 * Class:     uk_co_lunalux_libos_NetIO
 * Method:    n_readSocket
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_uk_co_lunalux_libos_NetIO_n_1readSocket (JNIEnv * jvm, jobject, jint socket_id, jint buffer_size)
{
    return jvm->NewStringUTF(std::move(NetIOHandler::getInstance().j_losReadSocket(socket_id, buffer_size)));
}
/*
 * Class:     uk_co_lunalux_libos_NetIO
 * Method:    n_writeSocket
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_uk_co_lunalux_libos_NetIO_n_1writeSocket(JNIEnv * jvm, jobject, jint socket_id, jstring data)
{
    NetIOHandler::getInstance().j_losWriteSocket(socket_id, jvm->GetStringUTFChars(data, 0));
}
/*
 * Class:     uk_co_lunalux_libos_NetIO
 * Method:    n_destroySocket
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_uk_co_lunalux_libos_NetIO_n_1destroySocket(JNIEnv *, jobject, jint socket_id)
{
    NetIOHandler::getInstance().j_losDestorySocket(socket_id);
}
/*
 * Class:     uk_co_lunalux_libos_NetIO
 * Method:    n_getCurrentResult
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_uk_co_lunalux_libos_NetIO_n_1getCurrentResult(JNIEnv *, jobject, jint socket_id)
{
    return NetIOHandler::getInstance().j_getCurrentResult(socket_id);
}
/*
 * Class:     uk_co_lunalux_libos_NetIO
 * Method:    losNetworkBytesToSystemBytes
 * Signature: (Ljava/util/ArrayList;)Ljava/util/ArrayList;
 */
JNIEXPORT jintArray JNICALL Java_uk_co_lunalux_libos_NetIO_losNetworkBytesToSystemBytes(JNIEnv * jvm, jobject, jintArray bytes)
{
    jvm->SetIntArrayRegion(bytes,0,jvm->GetArrayLength(bytes),losNetworkBytesToSystemBytesSigned(jvm->GetIntArrayElements(bytes,0),jvm->GetArrayLength(bytes)));
    return bytes;
}
/*
 * Class:     uk_co_lunalux_libos_NetIO
 * Method:    losSystemBytesToNetworkBytes
 * Signature: (Ljava/util/ArrayList;)Ljava/util/ArrayList;
 */
JNIEXPORT jintArray JNICALL Java_uk_co_lunalux_libos_NetIO_losSystemBytesToNetworkBytes(JNIEnv * jvm, jobject, jintArray bytes)
{
    jvm->SetIntArrayRegion(bytes,0,jvm->GetArrayLength(bytes),losSystemBytesToNetworkBytesSigned(jvm->GetIntArrayElements(bytes,0),jvm->GetArrayLength(bytes)));
    return bytes;
}
#ifdef __cplusplus
}
#endif