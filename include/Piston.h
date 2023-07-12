#ifndef PISTON
#define PISTON
#include "Mesh.h"
#include "GameEvents.h"

class Piston : IRenderable, IUpdatable, IClickable
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
	float pistonAngle = PI / 1.75f;
	float mouseAngle = 0;
	bool shouldRotateByMouse = false;

	void OnRender(OnRenderEvent* args) override;
	void OnUpdate(OnUpdateEvent* args) override;
	void OnClick(OnClickEvent* args) override;
	void OnMouseOver(OnMouseOverEvent* args) override;

public:
	Piston();
	~Piston();


};

#endif // !PISTON