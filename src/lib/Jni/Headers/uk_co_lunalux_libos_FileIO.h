/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class uk_co_lunalux_libos_FileIO */

#ifndef _Included_uk_co_lunalux_libos_FileIO
#define _Included_uk_co_lunalux_libos_FileIO
#ifdef __cplusplus
extern "C" {
#endif
#undef uk_co_lunalux_libos_FileIO_LOS_FILE_BIT_READ
#define uk_co_lunalux_libos_FileIO_LOS_FILE_BIT_READ 1L
#undef uk_co_lunalux_libos_FileIO_LOS_FILE_BIT_WRITE
#define uk_co_lunalux_libos_FileIO_LOS_FILE_BIT_WRITE 2L
#undef uk_co_lunalux_libos_FileIO_LOS_FILE_BIT_BINARY
#define uk_co_lunalux_libos_FileIO_LOS_FILE_BIT_BINARY 4L
#undef uk_co_lunalux_libos_FileIO_LOS_FILE_BIT_CREATE
#define uk_co_lunalux_libos_FileIO_LOS_FILE_BIT_CREATE 8L
#undef uk_co_lunalux_libos_FileIO_LOS_FILE_BIT_DELETE_AFTER_CLOSE
#define uk_co_lunalux_libos_FileIO_LOS_FILE_BIT_DELETE_AFTER_CLOSE 22L
#undef uk_co_lunalux_libos_FileIO_LOS_FILE_BIT_FLIP_ENDIEN
#define uk_co_lunalux_libos_FileIO_LOS_FILE_BIT_FLIP_ENDIEN 50L
/*
 * Class:     uk_co_lunalux_libos_FileIO
 * Method:    n_openFile
 * Signature: (ILjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_uk_co_lunalux_libos_FileIO_n_1openFile
  (JNIEnv *, jobject, jint, jstring);

/*
 * Class:     uk_co_lunalux_libos_FileIO
 * Method:    n_readFile
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_uk_co_lunalux_libos_FileIO_n_1readFile
  (JNIEnv *, jobject, jint);

/*
 * Class:     uk_co_lunalux_libos_FileIO
 * Method:    n_writeFile
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_uk_co_lunalux_libos_FileIO_n_1writeFile
  (JNIEnv *, jobject, jint, jstring);

/*
 * Class:     uk_co_lunalux_libos_FileIO
 * Method:    n_closeFile
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_uk_co_lunalux_libos_FileIO_n_1closeFile
  (JNIEnv *, jobject, jint);

/*
 * Class:     uk_co_lunalux_libos_FileIO
 * Method:    n_getCurrentResult
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_uk_co_lunalux_libos_FileIO_n_1getCurrentResult
  (JNIEnv *, jobject, jint);

/*
 * Class:     uk_co_lunalux_libos_FileIO
 * Method:    setAssetPath
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_uk_co_lunalux_libos_FileIO_setAssetPath
  (JNIEnv *, jobject, jstring);

#ifdef __cplusplus
}
#endif
#endif
