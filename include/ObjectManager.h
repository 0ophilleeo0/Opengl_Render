#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <glad/glad.h>
#include <unordered_map>
#include <string>

enum class ObjectType {
	mesh2D,
	mesh3D,
	cube,
	plane,
};
class Object {
public:
	~Object();

	GLuint GetMesh2DVAO() {return mesh2DVAO;}
	GLuint GetMesh3DVAO() { return mesh3DVAO; }
	GLuint GetPlaneVAO() { return planeVAO; }
	GLuint GetCubeVAO() { return cubeVAO; }

	void CreatMesh2DVAO();
	void CreatMesh3DVAO();
	void CreatCubeVAO();
	void CreatPlaneVAO();

private:
	std::unordered_map<ObjectType, GLuint> vboTypeIDMap;

	GLuint mesh2DVAO = 0;
	GLuint mesh3DVAO = 0;
	GLuint planeVAO = 0;
	GLuint cubeVAO = 0;
};


#endif