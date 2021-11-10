//External includes
#include "SDL.h"
#include "SDL_surface.h"

//Project includes
#include "ERenderer.h"
#include "ERGBColor.h"

#include "Ray.h"
#include "HitRecord.h"
#include "MathHelper.h"
#include "SceneManager.h"
#include "PerspectiveCamera.h"

Elite::Renderer::Renderer(SDL_Window * pWindow)
{
	//Initialize
	m_pWindow = pWindow;
	m_pFrontBuffer = SDL_GetWindowSurface(pWindow);
	int width, height = 0;
	SDL_GetWindowSize(pWindow, &width, &height);
	m_Width = static_cast<uint32_t>(width);
	m_Height = static_cast<uint32_t>(height);
	m_pBackBuffer = SDL_CreateRGBSurface(0, m_Width, m_Height, 32, 0, 0, 0, 0);
	m_pBackBufferPixels = static_cast<uint32_t*>(m_pBackBuffer->pixels);
}

void Elite::Renderer::Render()
{
	SDL_LockSurface(m_pBackBuffer);

	const SceneGraph& activeScene = SceneManager::GetInstance().GetActiveScene();
	PerspectiveCamera* pCamera = activeScene.GetCamera();

	Ray ray{};
	HitRecord hitRecord{};
	FPoint3 camPos{ pCamera->GetLookAtMatrix()[3].xyz };

	//Loop over all the pixels
	for (uint32_t row = 0; row < m_Height; ++row)
	{
		const float screenSpaceRow = CalculateScreenSpaceRow(static_cast<int>(row), static_cast<int>(m_Height));

		for (uint32_t col = 0; col < m_Width; ++col)
		{
			const float screenSpaceCol = CalculateScreenSpaceColumn(static_cast<int>(col), static_cast<int>(m_Width));

			// Reset ray
			ray.direction = {};
			ray.origin = {};

			// Assign y & x
			ray.origin.x = screenSpaceCol * pCamera->GetAspectRatio() * pCamera->GetFieldOfView();
			ray.origin.y = screenSpaceRow * pCamera->GetFieldOfView();
			hitRecord.t = ray.tMax;

			// Transform
			FPoint4 pos = pCamera->GetLookAtMatrix() * FPoint4{ ray.origin.x, ray.origin.y, -1, 1 };
			ray.origin.x = pos.x;
			ray.origin.y = pos.y;
			ray.origin.z = pos.z;

			// Get Direction
			ray.direction = ray.origin - camPos;
			Normalize(ray.direction);
			if (activeScene.Hit(ray, hitRecord))
			{
				RGBColor totalIrradiance{};
				for (Light* light : activeScene.GetLights())
				{
					const float lambertCosineDot = Dot(hitRecord.normal, -light->GetDirection(hitRecord.hitPoint));
					if (lambertCosineDot < 0)
						continue; // if dot result is smaller than 0, reflection is pointing away from the light

					FVector3 toLight = -light->GetDirection(hitRecord.hitPoint);
					float distanceToLight = Magnitude(toLight);
					Ray hitPointToLight{ hitRecord.hitPoint, toLight / distanceToLight, 0.0001f, distanceToLight + 1.0f };
					HitRecord lightCheckHitRecord{};
					if (activeScene.Hit(hitPointToLight, lightCheckHitRecord))
						continue; // if we hit anything there's an obstacle between the hitPoint and the light

					totalIrradiance += light->CalculateIrradiance(hitRecord.hitPoint) // Ergb
						* hitRecord.pMaterial->Shade(hitRecord, light->GetDirection(hitRecord.hitPoint), -ray.direction) // BRDFrgb
						* lambertCosineDot;
				}

				totalIrradiance.MaxToOne();
				m_pBackBufferPixels[col + (row * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
					static_cast<uint8_t>(totalIrradiance.r * 255.f),
					static_cast<uint8_t>(totalIrradiance.g * 255.f),
					static_cast<uint8_t>(totalIrradiance.b * 255.f));
			}
			else
			{
				m_pBackBufferPixels[col + (row * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
					static_cast<uint8_t>(0.f),
					static_cast<uint8_t>(0.f),
					static_cast<uint8_t>(0.f));
			}
		}
	}

	SDL_UnlockSurface(m_pBackBuffer);
	SDL_BlitSurface(m_pBackBuffer, 0, m_pFrontBuffer, 0);
	SDL_UpdateWindowSurface(m_pWindow);
}

bool Elite::Renderer::SaveBackbufferToImage() const
{
	return SDL_SaveBMP(m_pBackBuffer, "BackbufferRender.bmp");
}
