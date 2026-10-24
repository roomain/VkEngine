START cmd /c cmake_bat.bat ./glm debug GLM "-DGLM_ENABLE_SIMD_SSE4_2=ON" "-DGLM_ENABLE_SIMD_SSE4_1=ON"
START cmd /c cmake_bat.bat ./glm release GLM  "-DGLM_ENABLE_SIMD_SSE4_2=ON" "-DGLM_ENABLE_SIMD_SSE4_1=ON"

START cmd /c cmake_bat.bat ./asyncplusplus debug asyncplusplus "-DCMAKE_POLICY_VERSION_MINIMUM=3.5"
START cmd /c cmake_bat.bat ./asyncplusplus release asyncplusplus "-DCMAKE_POLICY_VERSION_MINIMUM=3.5"

START cmd /c cmake_bat.bat ./crc32c debug crc32c "-DCRC32C_BUILD_TESTS=OFF" "-DCRC32C_BUILD_BENCHMARKS=OFF" "-DBUILD_SHARED_LIBS=OFF"
START cmd /c cmake_bat.bat ./crc32c release crc32c "-DCRC32C_BUILD_TESTS=OFF" "-DCRC32C_BUILD_BENCHMARKS=OFF" "-DBUILD_SHARED_LIBS=OFF"

cd glslang
python update_glslang_sources.py 
cd ..
START cmd /c cmake_bat.bat ./glslang debug glslang "-DENABLE_HLSL=OFF"
START cmd /c cmake_bat.bat ./glslang release glslang "-DENABLE_HLSL=OFF"

START cmd /c cmake_bat.bat ./libbson debug libbson "-DCMAKE_POLICY_VERSION_MINIMUM=3.5"
START cmd /c cmake_bat.bat ./libbson release libbson "-DCMAKE_POLICY_VERSION_MINIMUM=3.5"

START cmd /c cmake_bat.bat ./slang debug slang "-DBUILD_EXAMPLE=OFF" "-DENABLE_HLSL=OFF"
START cmd /c cmake_bat.bat ./slang release slang "-DBUILD_EXAMPLE=OFF" "-DENABLE_HLSL=OFF"


START cmd /c cmake_bat.bat ./SPIRV-Reflect debug sprivReflect "-DSPIRV_REFLECT_SHARED_LIB=ON"
START cmd /c cmake_bat.bat ./SPIRV-Reflect release sprivReflect "-DSPIRV_REFLECT_SHARED_LIB=ON"