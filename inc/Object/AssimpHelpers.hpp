#ifndef _ASSIMP_HELPERS_H_
#define _ASSIMP_HELPERS_H_

#include "mft/mft.hpp"

//	Fix for assimp
#undef max
#undef min
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace notrealengine
{
  mft::quat
    AssimpToMftQuat(aiQuaternion& quat);
  mft::vec3
    AssimpToMftVec3(aiVector3D& vec);
  mft::mat4
    AssimpToMftMatrix(aiMatrix4x4 mat);
}

#endif
