#include "Object/AssimpHelpers.hpp"

namespace notrealengine
{

  mft::vec3	AssimpToMftVec3(aiVector3D& vec)
  {
    return (mft::vec3(vec.x, vec.y, vec.z));
  }

  mft::vec4	AssimpToMftVec4(aiColor4D& vec)
  {
    return (mft::vec4(vec.r, vec.g, vec.b, vec.a));
  }

  mft::quat	AssimpToMftQuat(aiQuaternion& quat)
  {
    return (mft::quat(quat.w, quat.x, quat.y, quat.z));
  }

  mft::mat4	AssimpToMftMatrix(aiMatrix4x4 mat)
  {
    mft::mat4	res(
      { static_cast<float>(mat.a1), static_cast<float>(mat.a2), static_cast<float>(mat.a3), static_cast<float>(mat.a4) },
      { static_cast<float>(mat.b1), static_cast<float>(mat.b2), static_cast<float>(mat.b3), static_cast<float>(mat.b4) },
      { static_cast<float>(mat.c1), static_cast<float>(mat.c2), static_cast<float>(mat.c3), static_cast<float>(mat.c4) },
      { static_cast<float>(mat.d1), static_cast<float>(mat.d2), static_cast<float>(mat.d3), static_cast<float>(mat.d4) });
    return res;
  }
}
