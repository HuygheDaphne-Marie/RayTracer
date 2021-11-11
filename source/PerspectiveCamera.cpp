#include "PerspectiveCamera.h"
#include "SDL.h"
#include "EMath.h"
#include <iostream>

PerspectiveCamera::PerspectiveCamera(
	const float width, 
	const float height, 
	const float fieldOfViewAngleDegrees, 
	const FPoint3& position,
	float angularSpeedDegrees
)
	: m_AspectRatio{width / height}
	, m_FieldOfViewAngle{ToRadians(fieldOfViewAngleDegrees)}
	, m_FOVScaleFactor{0}
	, m_LookAt{ m_LookAt.Identity() }
	, m_AngularSpeed(ToRadians(angularSpeedDegrees))
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
	SDL_PumpEvents();
	MouseInput mouseInput{};

	mouseInput.mouseButtons = SDL_GetMouseState(nullptr, nullptr);
	SDL_GetRelativeMouseState(&mouseInput.x, &mouseInput.y);


	HandleMovement(mouseInput, deltaTime);
	HandleRotation(mouseInput, deltaTime);
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

void PerspectiveCamera::HandleMovement(MouseInput mouseInput, float deltaTime)
{
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
	

	const bool isLeftPressed = mouseInput.mouseButtons & SDL_BUTTON_LMASK;
	const bool isRightPressed = mouseInput.mouseButtons & SDL_BUTTON_RMASK;
	if (mouseInput.y != 0 && (isLeftPressed && !isRightPressed))
	{
		translation += GetForward() * static_cast<float>(mouseInput.y);
	}
	else if (mouseInput.y != 0 && (isLeftPressed && isRightPressed))
	{
		translation += FVector3{ 0, 1, 0 } * static_cast<float>(-mouseInput.y);
	}


	Translate(GetNormalized(translation));
}

void PerspectiveCamera::HandleRotation(MouseInput mouseInput, float deltaTime)
{
	const bool isLeftPressed = mouseInput.mouseButtons & SDL_BUTTON_LMASK;
	const bool isRightPressed = mouseInput.mouseButtons & SDL_BUTTON_RMASK;

	// if x != 0
	// if not both left && right pressed
	// if left xor right is pressed
	if (mouseInput.x != 0 && !(isLeftPressed && isRightPressed) && (isLeftPressed || isRightPressed)) // Yaw (left - right)
	{
		const FMatrix3 rotation = MakeRotationY((-m_AngularSpeed * static_cast<float>(mouseInput.x)));
		Rotate(rotation);
	}

	if (mouseInput.y != 0 && isRightPressed && !(isLeftPressed)) // Pitch (up - down)
	{
		const FMatrix3 rotation = MakeRotationX((-m_AngularSpeed * static_cast<float>(mouseInput.y)));
		Rotate(rotation);
	}
}

