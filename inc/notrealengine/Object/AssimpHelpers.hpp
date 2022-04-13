#ifndef _ASSIMP_HELPERS_H_
#define _ASSIMP_HELPERS_H_

#include "mft/mft.hpp"
#include "UsingExternalLibs.hpp"

#ifdef USING_EXTERNAL_LIBS
//	Fix for assimp
#undef max
#undef min
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace notrealengine
{
  mft::quat
    AssimpToMftQuat(const aiQuaternion& quat);
  mft::vec3
    AssimpToMftVec3(const aiVector3D& vec);
  mft::vec4
    AssimpToMftVec4(const aiColor4D& vec);
  mft::mat4
    AssimpToMftMatrix(const aiMatrix4x4 mat);
}
#endif // !USING_EXTERNAL_LIBS

#endif // !_ASSIMP_HELPERS_H_
