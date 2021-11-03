#pragma once
#include "EMath.h"

class PerspectiveCamera
{
public:
	PerspectiveCamera(
		const float width, 
		const float height, 
		const float fieldOfViewAngleDegrees, 
		const Elite::FPoint3& position
	);
	~PerspectiveCamera() = default;

	float GetAspectRatio() const;
	float GetFieldOfView() const;

	const Elite::FMatrix4& GetLookAtMatrix() const;

	void Update(float deltaTime);

private:
	const float m_AspectRatio; // Can theoretically change but not for our purposes

	float m_FieldOfViewAngle;
	float m_FOVScaleFactor;

	Elite::FMatrix4 m_LookAt;

	void RecalculateFOVScaleFactor();
	void RecalculateONBMatrix(const Elite::FPoint3& position, const Elite::FVector3& camForward = {0,0,1});

	void Rotate(const Elite::FMatrix4& rotationMatrix);
	void Translate(const Elite::FVector3& translation);


	
};

