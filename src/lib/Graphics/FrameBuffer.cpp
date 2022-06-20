#include <RefractileAPI.h>
#include "Structs.h"

refFrameBuffer refGetCurrentWindowFrameBuffer(refHandle handle,refCommandBuffer buffer) noexcept
{
    return handle->framebuffer[buffer->index];
}