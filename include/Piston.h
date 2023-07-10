#ifndef PISTON
#define PISTON
#include "Mesh.h"
#include "GameEvents.h"

class Piston : IRenderable, IUpdatable
{
private:
	Mesh* piston;
	Mesh* vertical; //conecta com o pistao
	Mesh* bottonAxis; //conecta com o volant
	Mesh* volant;
	Mesh* topAxis;

	Vector3 bottonAxisCenter;
	Vector3 topAxisCenter;
	Vector3 volantCenter;
	Vector4 verticalBotton = Vector4(0,-99999,0,1);

	Float4x4 transform;

	Float4x4 volantTransform;
	Float4x4 pistonTransform;
	Float4x4 verticalTransform;
	Float4x4 bottonAxisTransform;
	Float4x4 topAxisTransform;

	float currentHeight = 0;

	void OnRender(OnRenderEvent* args) override;
	void OnUpdate(OnUpdateEvent* args) override;

public:
	Piston();
	~Piston();


};

#endif // !PISTON