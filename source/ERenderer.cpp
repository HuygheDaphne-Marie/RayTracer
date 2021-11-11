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
#include "Material.h"

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

	//Ray ray{};
	//HitRecord hitRecord{};
	FPoint3 camPos{ pCamera->GetLookAtMatrix()[3].xyz };

	//Loop over all the pixels
	for (uint32_t row = 0; row < m_Height; ++row)
	{
		const float screenSpaceRow = CalculateScreenSpaceRow(static_cast<int>(row), static_cast<int>(m_Height));

		for (uint32_t col = 0; col < m_Width; ++col)
		{
			const float screenSpaceCol = CalculateScreenSpaceColumn(static_cast<int>(col), static_cast<int>(m_Width));

			// Reset ray
			Ray ray{};
			HitRecord hitRecord{};

			// Set Ray origin (Raster to camera space)
			ray.origin.x = screenSpaceCol * pCamera->GetAspectRatio() * pCamera->GetFieldOfView();
			ray.origin.y = screenSpaceRow * pCamera->GetFieldOfView();
			ray.origin.z = -1.0f;

			// Transform
			FPoint4 pos = pCamera->GetLookAtMatrix() * FPoint4{ ray.origin.x, ray.origin.y, ray.origin.z, 0 };
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
					const FVector3 lightDirection = light->GetDirection(hitRecord.hitPoint);
					const float distanceToLight = Magnitude(lightDirection);
					const FVector3 lightDirectionNormalized = lightDirection / distanceToLight;

					const float lambertCosineLawDot = Dot(hitRecord.normal, light->GetDirectionNormalised(hitRecord.hitPoint));
					if (lambertCosineLawDot < 0)
						continue; // if dot result is smaller than 0, reflection is pointing away from the light


					if (m_CastShadows)
					{
						// Occlusions
						Ray hitPointToLight{ hitRecord.hitPoint, lightDirectionNormalized, 0.0001f, distanceToLight };
						HitRecord lightCheckHitRecord{};
						if (activeScene.Hit(hitPointToLight, lightCheckHitRecord))
							continue; // if we hit anything there's an obstacle between the hitPoint and the light
					}

					switch (m_LightEquationTerms)
					{
					case LightEquationTerms::irradianceOnly:
						totalIrradiance += light->CalculateIrradiance(hitRecord.hitPoint)
							* lambertCosineLawDot;
						break;
					case LightEquationTerms::BRDFOnly:
						totalIrradiance += hitRecord.pMaterial->Shade(hitRecord, lightDirectionNormalized, -ray.direction)
							* lambertCosineLawDot;
						break;
					default: 
						totalIrradiance += light->CalculateIrradiance(hitRecord.hitPoint) // Ergb
							* hitRecord.pMaterial->Shade(hitRecord, lightDirectionNormalized, -ray.direction) // BRDFrgb
							* lambertCosineLawDot;
					}
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
					static_cast<uint8_t>(200.f),
					static_cast<uint8_t>(130.f),
					static_cast<uint8_t>(130.f));
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

void Renderer::ToggleCastShadows()
{
	m_CastShadows = !m_CastShadows;
}

void Renderer::ToggleLightEquationTerms()
{
	int settingAsInt = static_cast<int>(m_LightEquationTerms);
	constexpr int lastOption = static_cast<int>(LightEquationTerms::LAST);

	settingAsInt = (settingAsInt + 1) % lastOption; // 0 - 1 - 2
	m_LightEquationTerms = static_cast<LightEquationTerms>(settingAsInt);
}
