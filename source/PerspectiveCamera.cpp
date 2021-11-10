#include "PerspectiveCamera.h"
#include "SDL.h"
#include "EMath.h"
#include <iostream>

PerspectiveCamera::PerspectiveCamera(
	const float width, 
	const float height, 
	const float fieldOfViewAngleDegrees, 
	const FPoint3& position,
	int angularSpeedDegrees
)
	: m_AspectRatio{width / height}
	, m_FieldOfViewAngle{ToRadians(fieldOfViewAngleDegrees)}
	, m_FOVScaleFactor{0}
	, m_LookAt{ m_LookAt.Identity() }
	, m_AngularSpeed(ToRadians(fieldOfViewAngleDegrees))
{
	RecalculateONBMatrix(position);
	RecalculateFOVScaleFactor();
}

float PerspectiveCamera::GetAspectRatio() const
{
	return m_AspectRatio;
}
float PerspectiveCamera::GetFieldOfView() const
{
	return m_FOVScaleFactor;
}

FVector3 PerspectiveCamera::GetForward() const
{
	return m_LookAt[2].xyz;
}

FVector3 PerspectiveCamera::GetRight() const
{
	return m_LookAt[0].xyz;
}

FVector3 PerspectiveCamera::GetUp() const
{
	return m_LookAt[1].xyz;
}

const FMatrix4& PerspectiveCamera::GetLookAtMatrix() const
{
	return m_LookAt;
}

void PerspectiveCamera::Update(float deltaTime)
{
	// this just needs to be better
	

	FVector3 translation{ 0,0,0 };

	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	if (pStates[SDL_SCANCODE_A] || pStates[SDL_SCANCODE_LEFT])
	{
		translation -= GetRight();
	}
	if (pStates[SDL_SCANCODE_D] || pStates[SDL_SCANCODE_RIGHT])
	{
		translation += GetRight();
	}
	if (pStates[SDL_SCANCODE_W] || pStates[SDL_SCANCODE_UP])
	{
		translation -= GetForward();
	}
	if (pStates[SDL_SCANCODE_S] || pStates[SDL_SCANCODE_DOWN])
	{
		translation += GetForward();
	}
	
	Translate(GetNormalized(translation));

	// TODO Replace with mouse movement
	// Left-right
	if (pStates[SDL_SCANCODE_Q])
	{
		FMatrix3 rotation = MakeRotationY(m_AngularSpeed * deltaTime);
		Rotate(rotation);
	}
	if (pStates[SDL_SCANCODE_E])
	{
		FMatrix3 rotation = MakeRotationY(-m_AngularSpeed * deltaTime);
		Rotate(rotation);
	}
	// Up-down
	if (pStates[SDL_SCANCODE_R])
	{
		FMatrix3 rotation = MakeRotationX(m_AngularSpeed * deltaTime);
		Rotate(rotation);
	}
	if (pStates[SDL_SCANCODE_F])
	{
		FMatrix3 rotation = MakeRotationX(-m_AngularSpeed * deltaTime);
		Rotate(rotation);
	}
}

void PerspectiveCamera::RecalculateFOVScaleFactor()
{
	m_FOVScaleFactor = tan(m_FieldOfViewAngle / 2);
}
void PerspectiveCamera::RecalculateONBMatrix(const FPoint3& position, const FVector3& camForward)
{
	const FVector3 worldUp{ 0,1,0 };
	const FVector3 camForwardNormal{ GetNormalized(camForward) };
	m_LookAt[0] = FVector4{ GetNormalized(Cross(worldUp, camForwardNormal)), 0 };
	m_LookAt[1] = FVector4{ GetNormalized(Cross(camForwardNormal, m_LookAt[0].xyz)), 0 };
	m_LookAt[2] = FVector4{ camForwardNormal, 0 };
	m_LookAt[3] = FVector4{ position.x, position.y, position.z, 1 };
}

void PerspectiveCamera::Rotate(const FMatrix4& rotationMatrix)
{
	m_LookAt[2] = rotationMatrix * m_LookAt[2];

	RecalculateONBMatrix(FPoint3{ m_LookAt[3].xyz }, m_LookAt[2].xyz);
}
void PerspectiveCamera::Translate(const FVector3& translation)
{
	m_LookAt[3] += translation; // needs to be the related unit vector * translation equivilent as a scalar
}

