#ifndef PISTON
#define PISTON
#include "Mesh.h"
#include "GameEvents.h"

class Piston : IRenderable, IUpdatable
{
private:
	Mesh* rightPiston;
	Mesh* rightVertical; //conecta com o pistao
	Mesh* leftPiston;
	Mesh* leftVertical; 
	Mesh* bottonAxis; //conecta com o volant
	Mesh* volant;
	Mesh* topAxis;

	Vector3 bottonAxisCenter;
	Vector3 topAxisCenter;
	Vector3 volantCenter;
	Vector4 verticalBotton = Vector4(0,-99999,0,1);

	Float4x4 generalTransform;
	float currentAngle = 0;

	void OnRender(OnRenderEvent* args) override;
	void OnUpdate(OnUpdateEvent* args) override;

public:
	Piston();
	~Piston();


};

#endif // !PISTON