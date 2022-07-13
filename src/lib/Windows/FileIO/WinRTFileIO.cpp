#include "../../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 1
#    include "../../IFileIO.hpp"
#    include <Components/FileIO.h>
#    include <algorithm>
#    include <winrt/Windows.Foundation.h>
#    include <winrt/Windows.Storage.h>

struct losFileHandle_T
{
    const winrt::Windows::Storage::IStorageFile file;
    bool flipEndienness = false;
    bool closeAfterDone = false;
};
#    define BIT_CASE(bits, flag, c_flag, at) \
    case bits: {                             \
        file_flags = flag;                   \
        create_file_flags = c_flag;          \
        flags_used = bits;                   \
        at_flags = at;                       \
        break;                               \
    }

losResult losOpenFile(losFileHandle *handle, const losFileOpenInfo &info)
{
    if (!(*handle))
        return LOS_ERROR_HANDLE_IN_USE;

    *handle = new losFileHandle_T();

    if(info.fileBits ^ LOS_FILE_BIT_READ)
    {

    }
    if(info.fileBits ^ LOS_FILE_BIT_WRITE)
    {
        
    }
    if(info.fileBits ^ LOS_FILE_BIT_CREATE)
    {
        
    }
    if(info.fileBits ^ LOS_FILE_BIT_BINARY)
    {
        
    }
    if(info.fileBits ^ LOS_FILE_BIT_FLIP_ENDIEN)
    {
        
    }
    return LOS_SUCCESS;
}

losResult losReadFile(losFileHandle handle, void **, size *)
{
    winrt::hstring data;
    try
    {
        const winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> exec_file =
            winrt::Windows::Storage::FileIO::ReadTextAsync(handle->file);
        data = winrt::hstring(exec_file.get());
    }
    catch (const std::exception &)
    {
        return LOS_ERROR_HANDLE_LOSSED;
    }
    return LOS_SUCCESS;
}

losResult losWriteFile(losFileHandle handle, const void *data, const size data_size)
{
    // fixme: THIS could cause problems with buffer overflow
    winrt::array_view<const uint8> write(static_cast<const uint8 *>(data), static_cast<const uint32>(data_size));
    try
    {
        (void)winrt::Windows::Storage::FileIO::WriteBytesAsync(handle->file, write).get();
    }
    catch (const std::exception &)
    {
        return LOS_ERROR_HANDLE_LOSSED;
    }

    return LOS_SUCCESS;
}

losResult losCloseFile(losFileHandle handle)
{
    delete handle;
    return LOS_SUCCESS;
}
#endif