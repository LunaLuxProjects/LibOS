#pragma once
#include <jni.h>
#include <Components/FileIO.h>
#include <Components/NetIO.h>
#include <cstring>
#include <stdexcept>

class FileIOHandler
{
    struct FileFrame
    {
        losResult result = LOS_SUCCESS;
        losFileHandle fileHandle = NULL;
        bool free = true;
    } frames[256]; 

    jint getNextFree() const noexcept
    {
        for(jint i = 0; i < 255; i++)
        {
            if(frames[i].free) 
                return i;
        }
        return INT32_MAX;
    }
    public:
    FileIOHandler(const FileIOHandler&) = delete;
    FileIOHandler(FileIOHandler&&) = delete;
    explicit FileIOHandler() {};
    static FileIOHandler& getInstance()
    {
        static FileIOHandler instance;
        return instance;
    }

    jint j_losOpenFile(jint fileBits, const char* path)
    {
        jint id = getNextFree();
        losFileOpenInfo info;
        info.fileBits = fileBits;
        info.path = path;
        info.path_size = std::strlen(path);
        frames[id].result = losOpenFile(&frames[id].fileHandle,info);
        frames[id].free = false;
        return id;
    }
    
    void j_losWriteFile(jint file_id,const char* data)
    {
        frames[file_id].result = losWriteFile(frames[file_id].fileHandle,reinterpret_cast<const void*>(data),std::strlen(data));
    }

    const char* j_losReadFile(jint file_id)
    {
        void* buffer;
        size buffer_size;
        frames[file_id].result = losReadFile(frames[file_id].fileHandle,&buffer,&buffer_size);
        return std::move(static_cast<char*>(buffer));
    }

    void j_losCloseFile(jint file_id)
    {
        frames[file_id].result = losCloseFile(frames[file_id].fileHandle);
        frames[file_id].fileHandle = NULL;
        frames[file_id].free = true;
    }

    losResult j_getCurrentResult(jint file_id) noexcept
    {
        return frames[file_id].result;
    }


    void j_setAssetPath(const char* path)
    {
        if(losSetAssetPath(path) != LOS_SUCCESS) throw std::runtime_error("could not set asset path");
    }
};

class NetIOHandler
{
    struct NetFrame
    {
        losResult result = LOS_SUCCESS;
        losSocket socketHandle = NULL;
        bool free = true;
    } frames[256]; 

    jint getNextFree() const noexcept
    {
        for(jint i = 0; i < 255; i++)
        {
            if(frames[i].free) 
                return i;
        }
        return INT32_MAX;
    }
    public:
    NetIOHandler(const NetIOHandler&) = delete;
    NetIOHandler(FileIOHandler&&) = delete;
    explicit NetIOHandler() {};
    static NetIOHandler& getInstance()
    {
        static NetIOHandler instance;
        return instance;
    }

    jint j_losCreateSocket(jint socketBits, const char* address, jint port)
    {
        jint id = getNextFree();
        losCreateSocketInfo info;
        info.socketBits = socketBits;
        info.address = address;
        info.address_size = std::strlen(address);
        info.port = port;
        frames[id].result = losCreateSocket(&frames[id].socketHandle,info);
        frames[id].free = false;
        return id;
    }
    
    void j_losWriteSocket(jint socket_id,const char* data)
    {
        frames[socket_id].result = losWriteSocket(frames[socket_id].socketHandle,reinterpret_cast<const void*>(data),std::strlen(data));
    }

    const char* j_losReadSocket(jint socket_id,size buffer_size)
    {
        char* buffer = new char[buffer_size];
        frames[socket_id].result = losReadSocket(frames[socket_id].socketHandle,buffer,&buffer_size);
        return std::move(buffer);
    }

    void j_losDestorySocket(jint socket_id)
    {
        frames[socket_id].result = losDestorySocket(frames[socket_id].socketHandle);
        frames[socket_id].socketHandle = NULL;
        frames[socket_id].free = true;
    }

    losResult j_getCurrentResult(jint socket_id) noexcept
    {
        return frames[socket_id].result;
    }
};

