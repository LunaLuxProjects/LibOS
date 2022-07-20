#pragma once
#include <Compiler.h>
#include <lstd/String.h>
typedef enum CompilerBackend
{
    SPIRV,
    HLSL
};

class  LibShaderCompiler
{
    bool has_had_an_error = false
  public:
    LibShaderCompiler() noexcept = default;

    lstd::string compileSource(lstd::string source,CompilerBackend backend) noexcept
    {
        switch (backend)
        {
        default:
            has_had_an_error = true;
            return "";
        }
    }
    ~LibShaderCompiler() noexcept = default;
};