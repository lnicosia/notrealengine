#include "Tests/ColladaParserTests.hpp"
#include "ColladaParser/ColladaException.class.hpp"
#include "ColladaParser/ColladaSceneBuilder.class.hpp"
#include "Object/AssimpHelpers.hpp"
//	Fix for assimp
#undef max
#undef min
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <filesystem>
#include <fstream>
#include <csignal>
#include <unistd.h>
#include <sys/wait.h>

using namespace notrealengine;

std::ofstream out;

void catchSegfault(int sigNum)
{
  std::cout << "Test segfaulted" << std::endl;
  out << "--Crash--" << std::endl;
  out.close();
  exit(-1);
}

template < typename T >
int compareValues(const T& value1, const T& value2, std::ofstream& out,
  const std::string& what)
{
  if (value1 != value2)
  {
    out << "-------------------------" << std::endl;
    out << "Custom importer reads " << value1 << what << std::endl;
    out << "Assimp importer reads " << value2 << what << std::endl;
    out << "-------------------------" << std::endl;
    return -1;
  }
  return 0;
}

template < >
int compareValues(const mft::mat4& value1, const mft::mat4& value2, std::ofstream& out,
  const std::string& what)
{
  for (unsigned int i = 0; i < 4; i++)
  {
    for (unsigned int j = 0; j < 4; j++)
    {
      if (fabs(value1[i][j] - value2[i][j]) >= 0.0005f)
      {
        out << "-------------------------" << std::endl;
        out << "value1[" + std::to_string(i) + "][" + std::to_string(j);
        out << "] = " + std::to_string(value1[i][j]) << std::endl;
        out << "value2[" + std::to_string(i) + "][" + std::to_string(j);
        out << "] = " + std::to_string(value2[i][j]) << std::endl;
        out << "Custom importer reads " << value1 << what << std::endl;
        out << "Assimp importer reads " << value2 << what << std::endl;
        out << "-------------------------" << std::endl;
        return -1;
      }
    }
  }
  return 0;
}

template < >
int compareValues(const mft::vec3& value1, const mft::vec3& value2, std::ofstream& out,
  const std::string& what)
{
  for (unsigned int i = 0; i < 3; i++)
  {
    if (fabs(value1[i] - value2[i]) >= 0.0005f)
    {
      out << "-------------------------" << std::endl;
      out << "value1[" + std::to_string(i) + "] = " + std::to_string(value1[i]) << std::endl;
      out << "value2[" + std::to_string(i) + "] = " + std::to_string(value2[i]) << std::endl;
      out << "Custom importer reads " << value1 << what << std::endl;
      out << "Assimp importer reads " << value2 << what << std::endl;
      out << "-------------------------" << std::endl;
      return -1;
    }
  }
  return 0;
}

template < >
int compareValues(const float& value1, const float& value2, std::ofstream& out,
  const std::string& what)
{
  if (fabs(value1 - value2) >= 0.0005f)
  {
    out << "-------------------------" << std::endl;
    out << "value1 = " + std::to_string(value1) << std::endl;
    out << "value2 = " + std::to_string(value2) << std::endl;
    out << "Custom importer reads " << value1 << what << std::endl;
    out << "Assimp importer reads " << value2 << what << std::endl;
    out << "-------------------------" << std::endl;
    return -1;
  }
  return 0;
}

template < >
int compareValues(const double& value1, const double& value2, std::ofstream& out,
  const std::string& what)
{
  if (std::abs(value1 - value2) >= 0.0005)
  {
    out << "-------------------------" << std::endl;
    out << "value1 = " + std::to_string(value1) << std::endl;
    out << "value2 = " + std::to_string(value2) << std::endl;
    out << "Custom importer reads " << value1 << what << std::endl;
    out << "Assimp importer reads " << value2 << what << std::endl;
    out << "-------------------------" << std::endl;
    return -1;
  }
  return 0;
}

static int compareNodes(const cpNode* customNode, const aiNode* assimpNode,
std::ofstream& out)
{
  int res = 0;
  int finalRes = 0;

  std::string assimpName(assimpNode->mName.C_Str());
  std::string parentName = "(null)";
  if (customNode->mParent != nullptr)
    parentName = customNode->mParent->mName;
  res = compareValues(customNode->mName, assimpName,
    out, " for node's name (parent = " + parentName + ")");
  if (res == -1)
    finalRes = -1;
  res = compareValues(customNode->mNumChildren, assimpNode->mNumChildren,
    out, " child(ren) for node " + customNode->mName);
  if (res == -1)
    finalRes = -1;
  res = compareValues(customNode->mNumMeshes, assimpNode->mNumMeshes,
    out, " mesh(es) for node " + customNode->mName);
  if (res == -1)
    finalRes = -1;
  res = compareValues(customNode->mTransformation, AssimpToMftMatrix(assimpNode->mTransformation),
    out, " for node " + customNode->mName + "'s transformation");
  if (res == -1)
    finalRes = -1;
  if (customNode->mNumChildren == assimpNode->mNumChildren)
  {
    for (unsigned int i = 0; i < customNode->mNumChildren; i++)
    {
      res = compareNodes(customNode->mChildren[i], assimpNode->mChildren[i], out);
      if (res == -1)
        finalRes = -1;
    }
  }
  if (customNode->mNumMeshes == assimpNode->mNumMeshes)
  {
    for (unsigned int i = 0; i < customNode->mNumMeshes; i++)
    {
      res = compareValues(customNode->mMeshes[i], assimpNode->mMeshes[i], out,
      " for node " + customNode->mName + "'s mesh[" + std::to_string(i) + "]");
      if (res == -1)
        finalRes = -1;
    }
  }
  return finalRes;
}

static int compareFaces(const cpFace* customFace, const aiFace* assimpFace,
std::ofstream& out)
{
  int res = 0;
  int finalRes = 0;
  res = compareValues(customFace->mNumIndices, assimpFace->mNumIndices,
    out, " indices for face");
  if (res == -1)
    finalRes = -1;
  if (customFace->mNumIndices == assimpFace->mNumIndices)
  {
    for (unsigned int i = 0; i < customFace->mNumIndices; i++)
    {
      res = compareValues(customFace->mIndices[i], assimpFace->mIndices[i],
        out, " for face[" + std::to_string(i) + "]");
      if (res == -1)
        finalRes = -1;
    }
  }
  return finalRes;
}

static int compareWeights(const cpVertexWeight* customWeight, const aiVertexWeight* assimpWeight,
std::ofstream& out)
{
  int res = 0;
  int finalRes = 0;
  res = compareValues(customWeight->mVertexId, assimpWeight->mVertexId,
    out, " for vertex weight's vertex ID");
  if (res == -1)
    finalRes = -1;
  res = compareValues(customWeight->mWeight, assimpWeight->mWeight,
    out, " for vertex weight's weight");
  if (res == -1)
    finalRes = -1;
  return res;
}

static int compareBones(const cpBone* customBone, const aiBone* assimpBone,
std::ofstream& out)
{
  int res = 0;
  int finalRes = 0;
  res = compareValues(customBone->mName, std::string(assimpBone->mName.C_Str()),
    out, " for bone's name");
  if (res == -1)
    finalRes = -1;
  res = compareValues(customBone->mOffsetMatrix, AssimpToMftMatrix(assimpBone->mOffsetMatrix),
    out, " for bone " + customBone->mName + "'s offset matrix");
  if (res == -1)
    finalRes = -1;
  res = compareValues(customBone->mNumWeights, assimpBone->mNumWeights,
      out, " weight(s) for bone " + customBone->mName);
  if (res == -1)
    finalRes = -1;
  if (customBone->mNumWeights == assimpBone->mNumWeights)
  {
    for (unsigned int i = 0; i < customBone->mNumWeights; i++)
    {
      res = compareWeights(&customBone->mWeights[i], &assimpBone->mWeights[i], out);
      if (res == -1)
        finalRes = -1;
    }
  }
  return finalRes;
}

static int compareMeshes(const cpMesh* customMesh, const aiMesh* assimpMesh,
std::ofstream& out)
{
  int res = 0;
  int finalRes = 0;
  res = compareValues(customMesh->mName, std::string(assimpMesh->mName.C_Str()),
    out, " for mesh's name");
  if (res == -1)
    finalRes = -1;
  res = compareValues(customMesh->mNumVertices, assimpMesh->mNumVertices,
    out, " vertex/ices for mesh " + customMesh->mName);
  if (res == -1)
    finalRes = -1;
  res = compareValues(customMesh->mNumFaces, assimpMesh->mNumFaces,
    out, " face(es) for mesh " + customMesh->mName);
  if (res == -1)
    finalRes = -1;
  res = compareValues(customMesh->mMaterialIndex, assimpMesh->mMaterialIndex,
    out, " for mesh " + customMesh->mName + "'s material index");
  if (res == -1)
    finalRes = -1;
  res = compareValues(customMesh->mNumBones, assimpMesh->mNumBones,
    out, " bone(s) for mesh " + customMesh->mName);
  if (res == -1)
    finalRes = -1;
  for (unsigned int i = 0; i < MAX_TEXTURE_CHANNELS; i++)
  {
    res = compareValues(customMesh->mNumUVComponents[i], assimpMesh->mNumUVComponents[i],
      out, " for mesh " + customMesh->mName
      + "'s mNumUVComponents[" + std::to_string(i) + "]");
    if (res == -1)
      finalRes = -1;
    if (customMesh->mNumVertices == assimpMesh->mNumVertices)
    {
      for (unsigned int j = 0; j < customMesh->mNumVertices; j++)
      {
        if (customMesh->mTextureCoords[i] != nullptr
          && assimpMesh->mTextureCoords[i] != nullptr)
        {
          res = compareValues(customMesh->mTextureCoords[i][j],
            AssimpToMftVec3(assimpMesh->mTextureCoords[i][j]),
            out, " for mesh " + customMesh->mName
            + "'s mTextureCoords[" + std::to_string(i) + "][" + std::to_string(j) + "]");
          if (res == -1)
            finalRes = -1;
        }
        if (customMesh->mColors[i] != nullptr
          && assimpMesh->mColors[i] != nullptr)
        {
          res = compareValues(customMesh->mColors[i][j],
            AssimpToMftVec4(assimpMesh->mColors[i][j]),
            out, " for mesh " + customMesh->mName
            + "'s mColors[" + std::to_string(i) + "][" + std::to_string(j) + "]");
          if (res == -1)
            finalRes = -1;
        }
      }
    }
  }
  if (customMesh->mNumVertices == assimpMesh->mNumVertices)
  {
    for (unsigned int i = 0; i < customMesh->mNumVertices; i++)
    {
      res = compareValues(customMesh->mVertices[i],
        AssimpToMftVec3(assimpMesh->mVertices[i]),
        out, " for mesh " + customMesh->mName
        + "'s mVertices[" + std::to_string(i) + "]");
      if (res == -1)
        finalRes = -1;
      res = compareValues(customMesh->mNormals[i],
        AssimpToMftVec3(assimpMesh->mNormals[i]),
        out, " for mesh " + customMesh->mName
        + "'s mNormals[" + std::to_string(i) + "]");
      if (res == -1)
        finalRes = -1;
    }
  }
  if (customMesh->mNumFaces == assimpMesh->mNumFaces)
  {
    for (unsigned int i = 0; i < customMesh->mNumFaces; i++)
    {
      res = compareFaces(&customMesh->mFaces[i], &assimpMesh->mFaces[i], out);
      if (res == -1)
        finalRes = -1;
    }
  }
  //if (customMesh->mNumBones == assimpMesh->mNumBones)
  //{
    for (unsigned int i = 0; i < customMesh->mNumBones; i++)
    {
      res = compareBones(customMesh->mBones[i], assimpMesh->mBones[i], out);
      if (res == -1)
        finalRes = -1;
    }
  //}
  return finalRes;
}

static int compareChannels(const cpNodeAnim* customChannels,
  const aiNodeAnim* assimpChannels, std::ofstream& out)
{
  int res = 0;
  int finalRes = 0;
  return finalRes;
}

static int compareAnimations(const cpAnimation* customAnim,
  const aiAnimation* assimpAnim, std::ofstream& out)
{
  int res = 0;
  int finalRes = 0;
  res = compareValues(customAnim->mName, std::string(assimpAnim->mName.C_Str()),
    out, " for animation's name");
  if (res == -1)
    finalRes = -1;
  res = compareValues(customAnim->mNumChannels, assimpAnim->mNumChannels,
    out, " for animation " + customAnim->mName + "'s num channels");
  if (res == -1)
    finalRes = -1;
  res = compareValues(customAnim->mTicksPerSecond, assimpAnim->mTicksPerSecond,
    out, " for animation " + customAnim->mName + "'s ticks per second");
  if (res == -1)
    finalRes = -1;
  res = compareValues(customAnim->mDuration, assimpAnim->mDuration,
    out, " for animation " + customAnim->mName + "'s duration");
  if (res == -1)
    finalRes = -1;
  unsigned int numChannels =
    std::min(customAnim->mNumChannels, assimpAnim->mNumChannels);
  for (unsigned int i = 0; i < numChannels; i++)
  {
    res = compareChannels(customAnim->mChannels[i], assimpAnim->mChannels[i], out);
    if (res == -1)
      finalRes = -1;
  }
  return finalRes;
}

static int compareScenes(const cpScene* customScene, const aiScene* assimpScene,
std::ofstream& out)
{
  int res = 0;
  int finalRes = 0;
  res = compareValues(customScene->mNumAnimations, assimpScene->mNumAnimations,
    out, " animation(s)");
  if (res == -1)
    finalRes = -1;
  for (unsigned int i = 0; i < customScene->mNumAnimations; i++)
  {
    res = compareAnimations(customScene->mAnimations[i],
      assimpScene->mAnimations[i], out);
    if (res == -1)
      finalRes = -1;
  }
  if (res == -1)
    finalRes = -1;
  res = compareValues(customScene->mNumMeshes, assimpScene->mNumMeshes,
    out, " mesh(es)");
  if (res == -1)
    finalRes = -1;
  res = compareValues(customScene->mNumMaterials, assimpScene->mNumMaterials,
    out, " material(s)");
  if (res == -1)
    finalRes = -1;
  res = compareValues(customScene->mNumTextures, assimpScene->mNumTextures,
    out, " texture(s)");
  if (res == -1)
    finalRes = -1;
  res = compareNodes(customScene->mRootNode, assimpScene->mRootNode, out);
  if (res == -1)
    finalRes = -1;
  if (customScene->mNumMeshes == assimpScene->mNumMeshes)
  {
    for (unsigned int i = 0; i < customScene->mNumMeshes; i++)
    {
      res = compareMeshes(customScene->mMeshes[i], assimpScene->mMeshes[i], out);
      if (res == -1)
        finalRes = -1;
    }
  }
  return finalRes;
}

static int compareParsers(const std::string& path)
{
  std::string outputPath = LOGS_DIRECTORY;
  outputPath += "/testParser/" + path.substr(path.find_last_of('/') + 1);
  std::cout << "Comparing custom and assimp parser for " << path << std::endl;

  const cpScene* customScene = nullptr;
  const aiScene* assimpScene = nullptr;

  pid_t id = fork();
  if (id != 0)
  {
    int ret;
    wait(&ret);
    if (ret == 0)
      std::cout << "VALID" << std::endl;
    else
      std::cout << "INVALID" << std::endl;
    return ret;
  }
  try
  {
    out.open(outputPath);
  }
  catch (std::ios_base::failure& e)
  {
    std::cerr << "Could not open log file '" << outputPath << "': " << e.what();
    std::cerr << std::endl;
    exit(-1);
  }
  ColladaSceneBuilder importer;
  try
  {
    signal(SIGSEGV, catchSegfault);
    signal(SIGABRT, catchSegfault);
    customScene = importer.ReadFile(path, aiProcess_Triangulate
      | aiProcess_GenUVCoords);
    if (!customScene || !customScene->mRootNode)
    {
      out << "Collada parser failed to import object" << std::endl;
      out.close();
      exit(-1);
    }
  }
  catch (ColladaException& e)
  {
    out << "nre: " << std::endl;
    if (customScene != nullptr)
      delete customScene;
    out.close();
    exit(-1);
  }

  Assimp::Importer	aimporter;
  if (path.substr(path.find_last_of('.')) == ".dae")
    assimpScene = aimporter.ReadFile(path, aiProcess_Triangulate
      | aiProcess_GenUVCoords);
  else
    assimpScene = aimporter.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs
      | aiProcess_GenUVCoords);

  if (!assimpScene || assimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimpScene->mRootNode)
  {
    out << "assimp: " << aimporter.GetErrorString() << std::endl;
    delete customScene;
    out.close();
    exit(-1);
  }

  int res = compareScenes(customScene, assimpScene, out);

  delete customScene;

  out.close();
  exit(res);
}

int testParser(TestType type, std::string path)
{
  if (type == TestType::SingleFile)
  {
    return compareParsers(path);
  }
  else
  {
    int res = 0;
    int finalRes = 0;
    std::string dir = "./resources/objects";
    for (const auto& file: std::filesystem::directory_iterator(dir))
    {
      std::filesystem::path filePath = file.path();
      if (filePath.extension() == ".dae")
      {
        res = compareParsers(filePath.string());
        if (res != 0)
          finalRes = -1;
      }
    }
    return finalRes;
  }
  return 0;
}
