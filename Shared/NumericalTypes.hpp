#pragma once

typedef unsigned char  uchar;
typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned long  ulong;

#include <cstdint>
typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

//---------------------------------------------------------------------------------------
// Create typedefs from glm types in case a different library is used in the future.
#include <glm/glm.hpp>

typedef glm::uvec2 uvec2;
typedef glm::uvec3 uvec3;
typedef glm::uvec4 uvec4;

typedef glm::ivec2 ivec2;
typedef glm::ivec3 ivec3;
typedef glm::ivec4 ivec4;

typedef glm::dvec2 dvec2;
typedef glm::dvec3 dvec3;
typedef glm::dvec4 dvec4;

typedef glm::quat  quat;

typedef glm::mat2  mat2;
typedef glm::mat3  mat3;
typedef glm::mat4  mat4;
