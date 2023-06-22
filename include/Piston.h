#ifndef PISTON
#define PISTON
#include "Mesh.h"
#include "GameEvents.h"

class Piston : IRenderable
{
private:
	Mesh* piston;
	Mesh* bottonAxis; //conecta com o pistao
	Mesh* topAxis; //conecta com o volant
	Mesh* vertical;
	Mesh* volant;
	Float4x4 transform;

	void OnRender(OnRenderEvent* args) override;

public:
	Piston();
	~Piston();


};

#endif // !PISTON