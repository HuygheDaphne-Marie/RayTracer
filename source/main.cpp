//External includes
#include "vld.h"
#include "SDL.h"
#include "SDL_surface.h"
#undef main

//Standard includes
#include <iostream>

//Project includes
#include "ETimer.h"
#include "ERenderer.h"

#include "SceneManager.h"
#include "Sphere.h"
#include "Plane.h"
#include "PerspectiveCamera.h"
#include "PointLight.h"

#include "Material.h"
#include "LambertMaterial.h"
#include "LambertPhongMaterial.h"
#include "PBRMaterial.h"

void ShutDown(SDL_Window* pWindow)
{
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Unreferenced parameters
	(void)argc;
	(void)args;

	// Init Singletons
	SceneManager& sceneManager = SceneManager::GetInstance();

	//Create window + surfaces
	SDL_Init(SDL_INIT_VIDEO);

	const uint32_t width = 640;
	const uint32_t height = 480;
	SDL_Window* pWindow = SDL_CreateWindow(
		"RayTracer - Huyghe Henri-Thibault",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height, 0);

	if (!pWindow)
		return 1;

	//Initialize "framework"
	Timer* pTimer = new Timer();
	Renderer* pRenderer = new Renderer(pWindow);

	//Initialize Scenes
	{
		// Spheres PRBMaterial Scene
		SceneGraph& scene = sceneManager.GetActiveScene();
		scene.InitialiseCamera(width, height, 45.0f, { 0,1,10 });

		// Materials
		const std::shared_ptr<PBRMaterial> dielectric1 = std::make_shared<PBRMaterial>(PBRMaterial{ RGBColor{0.5f, 0.5f, 0.5f}, false, 1.0f });
		const std::shared_ptr<PBRMaterial> dielectric2 = std::make_shared<PBRMaterial>(PBRMaterial{ RGBColor{0.5f, 0.5f, 0.5f}, false, 0.6f });
		const std::shared_ptr<PBRMaterial> dielectric3 = std::make_shared<PBRMaterial>(PBRMaterial{ RGBColor{0.5f, 0.5f, 0.5f}, false, 0.1f });

		const std::shared_ptr<PBRMaterial> metallic1 = std::make_shared<PBRMaterial>(PBRMaterial{ RGBColor{0.5f, 0.5f, 0.5f}, true, 1.0f });
		const std::shared_ptr<PBRMaterial> metallic2 = std::make_shared<PBRMaterial>(PBRMaterial{ RGBColor{0.5f, 0.5f, 0.5f}, true, 0.6f });
		const std::shared_ptr<PBRMaterial> metallic3 = std::make_shared<PBRMaterial>(PBRMaterial{ RGBColor{0.5f, 0.5f, 0.5f}, true, 0.1f });

		const std::shared_ptr<PBRMaterial> planeMat = std::make_shared<PBRMaterial>(PBRMaterial{ RGBColor{0.5f, 0.5f, 0.5f}, false, 1.0f });

		// Geometry
		// Top row (dielectrics)
		scene.AddGeometryToScene(new Sphere(FPoint3{ -2.5f, 4, 0 }, dielectric1, 1.f));
		scene.AddGeometryToScene(new Sphere(FPoint3{ 0, 4, 0 }, dielectric2, 1.f));
		scene.AddGeometryToScene(new Sphere(FPoint3{ 2.5f, 4, 0 }, dielectric3, 1.f));

		// Bottom row (metals)
		scene.AddGeometryToScene(new Sphere(FPoint3{ -2.5f, 1, 0 }, metallic1, 1.f));
		scene.AddGeometryToScene(new Sphere(FPoint3{ 0, 1, 0 }, metallic2, 1.f));
		scene.AddGeometryToScene(new Sphere(FPoint3{ 2.5f, 1, 0 }, metallic3, 1.f));

		// Planes
		scene.AddGeometryToScene(new Plane(FPoint3{ 0, 0, 0 }, FVector3{ 0, 1, 0 }, planeMat));

		// Lights
		scene.AddLightToScene(new PointLight(FPoint3{ -5, 5, -5 }, RGBColor{ 1,1,1 }, 25));
		scene.AddLightToScene(new PointLight(FPoint3{ -5, 5, 5 }, RGBColor{ 1,1,1 }, 25));
		scene.AddLightToScene(new PointLight(FPoint3{ 0, 5, -5 }, RGBColor{ 1,1,1 }, 25));
		scene.AddLightToScene(new PointLight(FPoint3{ 0, 5, 5 }, RGBColor{ 1,1,1 }, 25));
		scene.AddLightToScene(new PointLight(FPoint3{ 5, 5, -5 }, RGBColor{ 1,1,1 }, 25));
		scene.AddLightToScene(new PointLight(FPoint3{ 5, 5, 5 }, RGBColor{ 1,1,1 }, 25));
	}

	

	//Start loop
	pTimer->Start();
	float printTimer = 0.f;
	bool isLooping = true;
	bool takeScreenshot = false;
	while (isLooping)
	{
		//--------- Get input events ---------
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				isLooping = false;
				break;
			case SDL_KEYUP:
				if(e.key.keysym.scancode == SDL_SCANCODE_X)
					takeScreenshot = true;
				break;
			}
		}
		// Update Camera
		sceneManager.GetActiveScene().GetCamera()->Update(pTimer->GetElapsed());

		//--------- Render ---------
		pRenderer->Render();

		//--------- Timer ---------
		pTimer->Update();
		printTimer += pTimer->GetElapsed();
		if (printTimer >= 1.f)
		{
			printTimer = 0.f;
			std::cout << "FPS: " << pTimer->GetFPS() << std::endl;
		}

		//Save screenshot after full render
		if (takeScreenshot)
		{
			if (!pRenderer->SaveBackbufferToImage())
				std::cout << "Screenshot saved!" << std::endl;
			else
				std::cout << "Something went wrong. Screenshot not saved!" << std::endl;
			takeScreenshot = false;
		}
	}
	pTimer->Stop();

	//Shutdown "framework"
	delete pRenderer;
	delete pTimer;

	ShutDown(pWindow);
	return 0;
}