#include "PerspectiveCamera.h"
#include "SDL.h"

#include <iostream>

PerspectiveCamera::PerspectiveCamera(
	const float width, 
	const float height, 
	const float fieldOfViewAngleDegrees, 
	const Elite::FPoint3& position
)
	: m_AspectRatio{width / height}
	, m_FieldOfViewAngle{Elite::ToRadians(fieldOfViewAngleDegrees)}
	, m_FOVScaleFactor{0}
	, m_LookAt{ m_LookAt.Identity() }
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
const Elite::FMatrix4& PerspectiveCamera::GetLookAtMatrix() const
{
	return m_LookAt;
}

void PerspectiveCamera::Update(float deltaTime)
{
	Elite::FVector3 translation{ 0,0,0 };
	const float speed{ 50 };
	const float angularSpeed{ Elite::ToRadians(20.f) };

	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_A])
	{
		translation.x += speed * deltaTime;
	}
	if (pStates[SDL_SCANCODE_D])
	{
		translation.x += -speed * deltaTime;
	}
	Translate(translation);

	// Left-right
	if (pStates[SDL_SCANCODE_Q])
	{
		Elite::FMatrix3 rotation = Elite::MakeRotationY(angularSpeed * deltaTime);
		Rotate(rotation);
	}
	if (pStates[SDL_SCANCODE_E])
	{
		Elite::FMatrix3 rotation = Elite::MakeRotationY(-angularSpeed * deltaTime);
		Rotate(rotation);
	}
	// Up-down
	if (pStates[SDL_SCANCODE_R])
	{
		Elite::FMatrix3 rotation = Elite::MakeRotationX(angularSpeed * deltaTime);
		Rotate(rotation);
	}
	if (pStates[SDL_SCANCODE_F])
	{
		Elite::FMatrix3 rotation = Elite::MakeRotationX(-angularSpeed * deltaTime);
		Rotate(rotation);
	}
}

void PerspectiveCamera::RecalculateFOVScaleFactor()
{
	m_FOVScaleFactor = tan(m_FieldOfViewAngle / 2);
}
void PerspectiveCamera::RecalculateONBMatrix(const Elite::FPoint3& position, const Elite::FVector3& camForward)
{
	const Elite::FVector3 worldUp{ 0,1,0 };
	const Elite::FVector3 camForwardNormal{ Elite::GetNormalized(camForward) };
	m_LookAt[0] = Elite::FVector4{ Elite::GetNormalized(Elite::Cross(worldUp, camForwardNormal)), 0 };
	m_LookAt[1] = Elite::FVector4{ Elite::GetNormalized(Elite::Cross(camForwardNormal, m_LookAt[0].xyz)), 0 };
	m_LookAt[2] = Elite::FVector4{ camForwardNormal, 0 };
	m_LookAt[3] = Elite::FVector4{ position.x, position.y, position.z, 1 };

//#pragma region ErrorChecks
//
//	// Check lengths
//	if (Elite::Magnitude(m_LookAt[0]) > 1.0001f)
//	{
//		// right too long
//		return;
//	}
//	if (Elite::Magnitude(m_LookAt[1]) > 1.0001f)
//	{
//		// up too long
//		return;
//	}
//	if (Elite::Magnitude(m_LookAt[2]) > 1.0001f)
//	{
//		// forward too long
//		return;
//	}
//
//	// print out dot products (Should all be 0)
//	const float dotRightUp{ Elite::Dot(m_LookAt[0], m_LookAt[1]) };
//	const float dotRightForward{ Elite::Dot(m_LookAt[0], m_LookAt[2]) };
//	const float dotForwardUp{ Elite::Dot(m_LookAt[2], m_LookAt[1]) };
//	std::cout << "Right . Up: " << dotRightUp << '\n';
//	std::cout << "Right . Forward: " << dotRightForward << '\n';
//	std::cout << "Forward . Up: " << dotForwardUp << '\n';
//
//	if (dotRightUp > 0.0001f || dotRightUp < -0.0001f)
//	{
//		// not perpendicular
//		return;
//	}
//	if (dotRightForward > 0.0001f || dotRightForward < -0.0001f)
//	{
//		// not perpendicular
//		return;
//	}
//	if (dotForwardUp > 0.0001f || dotForwardUp < -0.0001f)
//	{
//		// not perpendicular
//		return;
//	}
//
//	// All is okay if none of these trigger
//
//#pragma endregion
}

void PerspectiveCamera::Rotate(const Elite::FMatrix4& rotationMatrix)
{
	m_LookAt[2] = rotationMatrix * m_LookAt[2];

	RecalculateONBMatrix(Elite::FPoint3{ m_LookAt[3].xyz }, m_LookAt[2].xyz);
}
void PerspectiveCamera::Translate(const Elite::FVector3& translation)
{
	m_LookAt[3] += translation; // needs to be the related unit vector * translation equivilent as a scalar
}

