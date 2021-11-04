//External includes
#include "SDL.h"
#include "SDL_surface.h"

//Project includes
#include "ERenderer.h"
#include "ERGBColor.h"

#include "Ray.h"
#include "HitRecord.h"
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
	m_pBackBufferPixels = (uint32_t*)m_pBackBuffer->pixels;
}

void Elite::Renderer::Render(PerspectiveCamera* pCamera)
{
	SDL_LockSurface(m_pBackBuffer);

	//Loop over all the pixels
	Scene* pActiveScene{ SCENEMANAGER->GetActiveScene() };

	Ray ray{};
	HitRecord hitRecord{};
	FPoint3 camPos{ pCamera->GetLookAtMatrix()[3].xyz }; // WIP

	for (uint32_t r = 0; r < m_Height; ++r)
	{
		float screenSpaceRow{ 1 - 2 * ( (r + 0.5f) / m_Height ) };

		for (uint32_t c = 0; c < m_Width; ++c)
		{
			float screenSpaceColumn{ 2 * ( (c + 0.5f) / m_Width) - 1};
			// Reset ray
			ray.direction = {};
			ray.origin = {};

			// Assign y & x
			ray.origin.x = screenSpaceColumn * pCamera->GetAspectRatio() * pCamera->GetFieldOfView();
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

			if (pActiveScene != nullptr)
			{
				if (pActiveScene->Hit(ray, hitRecord))
				{
					/*m_pBackBufferPixels[c + (r * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
						static_cast<uint8_t>(hitRecord.color.r * 255.f),
						static_cast<uint8_t>(hitRecord.color.g * 255.f),
						static_cast<uint8_t>(hitRecord.color.b * 255.f));*/

					// for each light
						// get irradiance of the light at the point hit
						// add up all irradiance (maxToOne them)
						// add the color of the object

					
					const FPoint3 tPoint = ray.origin + ray.direction * hitRecord.t;
					RGBColor totalIrradiance{};
					for (Light* light : pActiveScene->GetLights())
					{
						totalIrradiance += light->CalculateIrradiance(tPoint);
					}
					//totalIrradiance.MaxToOne();

					// TODO: FIX
					// Hitpoint is likely wrong
					// intensity needs to be WAYYYY up and even then the plane never seems to get lit
					
					m_pBackBufferPixels[c + (r * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
						static_cast<uint8_t>(totalIrradiance.r * 255.f),
						static_cast<uint8_t>(totalIrradiance.g * 255.f),
						static_cast<uint8_t>(totalIrradiance.b * 255.f));
					
				}
				else
				{
					m_pBackBufferPixels[c + (r * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
						static_cast<uint8_t>(0.f),
						static_cast<uint8_t>(0.f),
						static_cast<uint8_t>(0.f));
				}
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
