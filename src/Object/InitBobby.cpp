#include "GLContext.class.hpp"
#include "Object/GLObject.class.hpp"

using namespace notrealengine;

std::shared_ptr<GLObject> InitBobby(void)
{

  std::shared_ptr<Mesh> mesh(new Mesh(GLContext::cube));
  std::vector<std::shared_ptr<Mesh>> meshes;
  meshes.push_back(mesh);
  std::shared_ptr<GLObject>	character(new GLObject(meshes));
  character->setName("Bobby");
  character->setShader(GLContext::getShader("color"));
  character->transform.move(mft::vec3(0.0f, 1.15f, 0.0f));
  Mesh& torso = (*character->getMeshes()[0]);


  //torso.addMesh(std::shared_ptr<Mesh>(new Mesh(GLContext::cube)));
  torso.setName("Torso");
  torso.setColor(mft::vec3(0.0f, 0.5f, 0.0f));
  //torso.transform.move(mft::vec3(-0.25f, 0.0f, 0.0f));
  torso.transform.scale(mft::vec3(0.50f, 0.75f, 0.15f));
  //torso.transform.rotate(mft::quat::rotation(mft::vec3(0.0f, mft::radians(30.0f), 0.0f)));


  //torso.addMesh(std::shared_ptr<Mesh>(new Mesh(GLContext::cube)));
  //torso.addMesh(std::shared_ptr<Mesh>(new Mesh(GLContext::cube)));

  torso.addMesh(std::shared_ptr<Mesh>(new Mesh(GLContext::cube)));
  Mesh&	head = (*torso.getChildren()[0]);
  head.setName("Head");
  head.setColor(mft::vec3(0.6f, 0.6f, 0.0f));
  head.transform.move(mft::vec3(0.0f, 0.725f, 0.0f));
  head.transform.scale(mft::vec3(0.75f, 0.45f, 2.0f));

  torso.addMesh(std::shared_ptr<Mesh>(new Mesh(GLContext::cube)));
  Mesh& upperLeftArm = (*torso.getChildren()[1]);
  upperLeftArm.setName("Upper left arm");
  upperLeftArm.setColor(mft::vec3(0.5f, 0.5f, 0.0f));
  upperLeftArm.transform.move(mft::vec3(0.75f, 0.2f, 0.0f));
  upperLeftArm.transform.scale(mft::vec3(0.35f, 0.475f, 1.0f));
  upperLeftArm.transform.rotate(mft::quat(0.996196f, 0.0f, 0.0f, 0.0871558f));

  upperLeftArm.addMesh(std::shared_ptr<Mesh>(new Mesh(GLContext::cube)));
  Mesh& lowerLeftArm = (*upperLeftArm.getChildren()[0]);
  lowerLeftArm.setName("Lower left arm");
  lowerLeftArm.setColor(mft::vec3(0.6f, 0.6f, 0.0f));
  lowerLeftArm.transform.move(mft::vec3(0.0f, -1.0f, 0.0f));

  torso.addMesh(std::shared_ptr<Mesh>(new Mesh(GLContext::cube)));
  Mesh&	upperRightArm = (*torso.getChildren()[2]);
  upperRightArm.setName("Upper right arm");
  upperRightArm.setColor(mft::vec3(0.5f, 0.5f, 0.0f));
  upperRightArm.transform.move(mft::vec3(-0.75f, 0.2f, 0.0f));
  upperRightArm.transform.scale(mft::vec3(0.35f, 0.475f, 1.0f));
  upperRightArm.transform.rotate(mft::quat(0.996195f, 0.0f, 0.0f, -0.0871557f));

  upperRightArm.addMesh(std::shared_ptr<Mesh>(new Mesh(GLContext::cube)));
  Mesh& lowerRightArm = (*upperRightArm.getChildren()[0]);
  lowerRightArm.setName("Lower right arm");
  lowerRightArm.setColor(mft::vec3(0.6f, 0.6f, 0.0f));
  lowerRightArm.transform.move(mft::vec3(0.0f, -1.0f, 0.0f));

  torso.addMesh(std::shared_ptr<Mesh>(new Mesh(GLContext::cube)));
  Mesh&	leftThigh = (*torso.getChildren()[3]);
  leftThigh.setName("Left thigh");
  leftThigh.setColor(mft::vec3(0.0f, 0.0f, 0.5f));
  leftThigh.transform.move(mft::vec3(-0.25f, -0.75f, 0.0f));
  leftThigh.transform.scale(mft::vec3(0.35f, 0.5f, 1.0f));

  leftThigh.addMesh(std::shared_ptr<Mesh>(new Mesh(GLContext::cube)));
  Mesh& leftCalf = (*leftThigh.getChildren()[0]);
  leftCalf.setName("Left calf");
  leftCalf.setColor(mft::vec3(0.0f, 0.0f, 0.6f));
  leftCalf.transform.move(mft::vec3(0.0f, -1.0f, 0.0f));

  torso.addMesh(std::shared_ptr<Mesh>(new Mesh(GLContext::cube)));
  Mesh& rightThigh = (*torso.getChildren()[4]);
  rightThigh.setName("Right thigh");
  rightThigh.setColor(mft::vec3(0.0f, 0.0f, 0.5f));
  rightThigh.transform.move(mft::vec3(0.25f, -0.75f, 0.0f));
  rightThigh.transform.scale(mft::vec3(0.35f, 0.5f, 1.0f));

  rightThigh.addMesh(std::shared_ptr<Mesh>(new Mesh(GLContext::cube)));
  Mesh& rightCalf = (*rightThigh.getChildren()[0]);
  rightCalf.setName("Right calf");
  rightCalf.setColor(mft::vec3(0.0f, 0.0f, 0.6f));
  rightCalf.transform.move(mft::vec3(0.0f, -1.0f, 0.0f));

  return character;
}
