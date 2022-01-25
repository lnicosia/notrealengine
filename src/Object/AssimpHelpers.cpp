#include "Object/AssimpHelpers.hpp"

namespace notrealengine
{

  mft::vec3	AssimpToMftVec3(aiVector3D& vec)
  {
    return (mft::vec3(vec.x, vec.y, vec.z));
  }

  mft::quat	AssimpToMftQuat(aiQuaternion& quat)
  {
    return (mft::quat(quat.w, quat.x, quat.y, quat.z));
  }

  mft::mat4	AssimpToMftMatrix(aiMatrix4x4 mat)
  {
    mft::mat4	res(
      { mat.a1, mat.a2, mat.a3, mat.a4 },
      { mat.b1, mat.b2, mat.b3, mat.b4 },
      { mat.c1, mat.c2, mat.c3, mat.c4 },
      { mat.d1, mat.d2, mat.d3, mat.d4 });
    return res;
  }
}
