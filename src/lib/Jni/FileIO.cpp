#include "Handlers.hpp"
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     uk_co_lunalux_libos_FileIO
 * Method:    n_openFile
 * Signature: (ILjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_uk_co_lunalux_libos_FileIO_n_1openFile(JNIEnv * jvm, jobject, jint fileBits, jstring j_path)
{
    return FileIOHandler::getInstance().j_losOpenFile(fileBits,jvm->GetStringUTFChars(j_path, 0));
}
/*
 * Class:     uk_co_lunalux_libos_FileIO
 * Method:    n_readFile
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_uk_co_lunalux_libos_FileIO_n_1readFile(JNIEnv * jvm, jobject, jint file_id)
{
    return jvm->NewStringUTF(FileIOHandler::getInstance().j_losReadFile(file_id));
}
/*
 * Class:     uk_co_lunalux_libos_FileIO
 * Method:    n_writeFile
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_uk_co_lunalux_libos_FileIO_n_1writeFile(JNIEnv * jvm, jobject, jint file_id, jstring j_path)
{
    FileIOHandler::getInstance().j_losWriteFile(file_id,jvm->GetStringUTFChars(j_path, 0));
}
/*
 * Class:     uk_co_lunalux_libos_FileIO
 * Method:    n_closeFile
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_uk_co_lunalux_libos_FileIO_n_1closeFile(JNIEnv *, jobject, jint file_id)
{
    FileIOHandler::getInstance().j_losCloseFile(file_id);
}
/*
 * Class:     uk_co_lunalux_libos_FileIO
 * Method:    n_getCurrentResult
 * Signature: (I)Luk/co/lunalux/libos/Defines/losResult;
 */
JNIEXPORT jint JNICALL Java_uk_co_lunalux_libos_FileIO_n_1getCurrentResult(JNIEnv *, jobject, jint file_id)
{
    return FileIOHandler::getInstance().j_getCurrentResult(file_id);
}
/*
 * Class:     uk_co_lunalux_libos_FileIO
 * Method:    setAssetPath
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_uk_co_lunalux_libos_FileIO_setAssetPath(JNIEnv * jvm, jobject, jstring j_path)
{
    FileIOHandler::getInstance().j_setAssetPath(jvm->GetStringUTFChars(j_path, 0));
}
#ifdef __cplusplus
}
#endif