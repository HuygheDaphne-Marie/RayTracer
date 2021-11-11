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
#include "Triangle.h"
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
		SceneGraph& PBRScene = sceneManager.GetActiveScene();
		PBRScene.InitialiseCamera(width, height, 45.0f, { 0, 3, 9 });

		// Materials
		const std::shared_ptr<PBRMaterial> PBR_RoughMetal = std::make_shared<PBRMaterial>(PBRMaterial{ RGBColor{0.972f, 0.960f, 0.915f}, true, 1.0f });
		const std::shared_ptr<PBRMaterial> PRB_MediumMetal = std::make_shared<PBRMaterial>(PBRMaterial{ RGBColor{0.972f, 0.960f, 0.915f}, true, 0.6f });
		const std::shared_ptr<PBRMaterial> PBR_SmoothMetal = std::make_shared<PBRMaterial>(PBRMaterial{ RGBColor{0.972f, 0.960f, 0.915f}, true, 0.1f });

		const std::shared_ptr<PBRMaterial> PBR_RoughPlastic = std::make_shared<PBRMaterial>(PBRMaterial{ RGBColor{0.75f, 0.75f, 0.75f}, false, 1.0f });
		const std::shared_ptr<PBRMaterial> PBR_MediumPlastic = std::make_shared<PBRMaterial>(PBRMaterial{ RGBColor{0.75f, 0.75f, 0.75f}, false, 0.4f });
		const std::shared_ptr<PBRMaterial> PBR_SmoothPlastic = std::make_shared<PBRMaterial>(PBRMaterial{ RGBColor{0.75f, 0.75f, 0.75f}, false, 0.1f });

		const std::shared_ptr<LambertMaterial> Lambert_GreyBlue = std::make_shared<LambertMaterial>(RGBColor{ 0.49f, 0.57f, 0.57f }, 1.f);
		const std::shared_ptr<LambertMaterial> Lambert_White = std::make_shared<LambertMaterial>(RGBColor{ 1.f, 1.f, 1.f }, 1.f);

		// Geometry
		// Bottom row
		PBRScene.AddGeometryToScene(new Sphere(FPoint3{ -1.75f, 1.f, 0.f }, PBR_RoughMetal, 0.75f));
		PBRScene.AddGeometryToScene(new Sphere(FPoint3{ 0, 1.f, 0.f }, PRB_MediumMetal, 0.75f));
		PBRScene.AddGeometryToScene(new Sphere(FPoint3{ 1.75f, 1.f, 0.f }, PBR_SmoothMetal, 0.75f));

		// Top row
		PBRScene.AddGeometryToScene(new Sphere(FPoint3{ -1.75f, 3.f, 0.f }, PBR_RoughPlastic, 0.75f));
		PBRScene.AddGeometryToScene(new Sphere(FPoint3{ 0.f, 3.f, 0.f }, PBR_MediumPlastic, 0.75f));
		PBRScene.AddGeometryToScene(new Sphere(FPoint3{ 1.75f, 3.f, 0.f }, PBR_SmoothPlastic, 0.75f));

		// Triangles
		PBRScene.AddGeometryToScene(new Triangle(FPoint3{ -1.75f, 4.5f, 0.f }, Lambert_White,
			FPoint3{ -.75f, 1.5f, 0.f }, 
			FPoint3{ -.75f, 0.f, 0.f }, 
			FPoint3{ 0.75f, 0.f, 0.f }, 
			CullingMode::Back));
		PBRScene.AddGeometryToScene(new Triangle(FPoint3{ 0.f, 4.5f, 0.f }, Lambert_White,
			FPoint3{ -.75f, 1.5f, 0.f },
			FPoint3{ -.75f, 0.f, 0.f },
			FPoint3{ 0.75f, 0.f, 0.f },
			CullingMode::Front));
		PBRScene.AddGeometryToScene(new Triangle(FPoint3{ 1.75f, 4.5f, 0.f }, Lambert_White,
			FPoint3{ -.75f, 1.5f, 0.f },
			FPoint3{ -.75f, 0.f, 0.f },
			FPoint3{ 0.75f, 0.f, 0.f },
			CullingMode::None));

		// Planes
		PBRScene.AddGeometryToScene(new Plane(FPoint3{ 0, 0, 0 }, FVector3{ 0, 1, 0 }, Lambert_GreyBlue));
		PBRScene.AddGeometryToScene(new Plane(FPoint3{ 0, 0, -10.f }, FVector3{ 0, 0, 1.f }, Lambert_GreyBlue)); // facing cam
		PBRScene.AddGeometryToScene(new Plane(FPoint3{ 0, 10.f, 0 }, FVector3{ 0, -1.f, 0 }, Lambert_GreyBlue)); // ceiling
		PBRScene.AddGeometryToScene(new Plane(FPoint3{ 5, 0, 0 }, FVector3{ -1, 0, 0 }, Lambert_GreyBlue));
		PBRScene.AddGeometryToScene(new Plane(FPoint3{ -5, 0, 0 }, FVector3{ 1, 0, 0 }, Lambert_GreyBlue));

		// Lights
		PBRScene.AddLightToScene(new PointLight(FPoint3{ 0, 5, -5 }, RGBColor{ 1,.61f,.45f }, 50));
		PBRScene.AddLightToScene(new PointLight(FPoint3{ -2.5f, 5, 5 }, RGBColor{ 1, .8f, .45f }, 70));
		PBRScene.AddLightToScene(new PointLight(FPoint3{ 2.5f, 2.5f, 5.f }, RGBColor{ .34f, .47f, .68f }, 50));
	}

	
	{
		SceneGraph& scene = sceneManager.AddNewScene();
		scene.InitialiseCamera(width, height, 45.0f, { 0, 3, 9 });

		// Materials
		const std::shared_ptr<LambertMaterial> Lambert_GreyBlue = std::make_shared<LambertMaterial>(RGBColor{ 0.49f, 0.57f, 0.57f }, 1.f);

		// Planes
		scene.AddGeometryToScene(new Plane(FPoint3{ 0, 0, 0 }, FVector3{ 0, 1, 0 }, Lambert_GreyBlue));
		scene.AddGeometryToScene(new Plane(FPoint3{ 0, 0, -10.f }, FVector3{ 0, 0, 1.f }, Lambert_GreyBlue)); // facing cam
		scene.AddGeometryToScene(new Plane(FPoint3{ 0, 10.f, 0 }, FVector3{ 0, -1.f, 0 }, Lambert_GreyBlue)); // ceiling
		scene.AddGeometryToScene(new Plane(FPoint3{ 5, 0, 0 }, FVector3{ -1, 0, 0 }, Lambert_GreyBlue));
		scene.AddGeometryToScene(new Plane(FPoint3{ -5, 0, 0 }, FVector3{ 1, 0, 0 }, Lambert_GreyBlue));

		std::vector<FPoint3> vertexBuffer
		{
			FPoint3(-.75, 1.5, 0),
			FPoint3(-.75, 0, 0),
			FPoint3(.75, 0, 0),
			FPoint3(.75, 1.5, 1)
		};
		std::vector<unsigned int> indexBuffer{ 0, 1, 2, 0, 2, 3 };

		// Lights
		scene.AddLightToScene(new PointLight(FPoint3{ 0, 5, -5 }, RGBColor{ 1,.61f,.45f }, 50));
		scene.AddLightToScene(new PointLight(FPoint3{ -2.5f, 5, 5 }, RGBColor{ 1, .8f, .45f }, 70));
		scene.AddLightToScene(new PointLight(FPoint3{ 2.5f, 2.5f, 5.f }, RGBColor{ .34f, .47f, .68f }, 50));
	}

	sceneManager.GotoNextScene();
	

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
				if (e.key.keysym.scancode == SDL_SCANCODE_Q)
					pRenderer->ToggleCastShadows();
				if (e.key.keysym.scancode == SDL_SCANCODE_E)
					pRenderer->ToggleLightEquationTerms();
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