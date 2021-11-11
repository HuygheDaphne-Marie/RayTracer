#pragma once
#include <SDL_stdinc.h>

#include "EMath.h"

using namespace Elite;

class PerspectiveCamera
{
public:
	PerspectiveCamera(
		const float width, 
		const float height, 
		const float fieldOfViewAngleDegrees, 
		const FPoint3& position,
		float angularSpeedDegrees = 1.f
	);
	~PerspectiveCamera() = default;

	float GetAspectRatio() const;
	float GetFieldOfView() const;

	FVector3 GetForward() const;
	FVector3 GetRight() const;
	FVector3 GetUp() const;

	const FMatrix4& GetLookAtMatrix() const;

	void Update(float deltaTime);

private:
	struct MouseInput
	{
		Uint32 mouseButtons;
		int x;
		int y;
	};

	const float m_AspectRatio; // Can theoretically change but not for our purposes

	float m_FieldOfViewAngle;
	float m_FOVScaleFactor;

	FMatrix4 m_LookAt;
	
	float m_AngularSpeed;

	void RecalculateFOVScaleFactor();
	void RecalculateONBMatrix(const FPoint3& position, const FVector3& camForward = {0,0,1});

	void Rotate(const FMatrix4& rotationMatrix);
	void Translate(const FVector3& translation);

	void HandleMovement(MouseInput mouseInput, float deltaTime);
	void HandleRotation(MouseInput mouseInput, float deltaTime);
	
};

