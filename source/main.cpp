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
	Elite::Timer* pTimer = new Elite::Timer();
	Elite::Renderer* pRenderer = new Elite::Renderer(pWindow);

	// Initialize Camera
	PerspectiveCamera* pCamera = new PerspectiveCamera(float(width), float(height), 45.f, {0,0,200});

	//Initialize Scenes
	SCENEMANAGER->AddScene(new Scene());
	SCENEMANAGER->GetActiveScene()->AddObjectToScene(new Sphere(FPoint3{ -25, 0, -100 }, RGBColor{ 1,0,0 }, 50.f));
	SCENEMANAGER->GetActiveScene()->AddObjectToScene(new Sphere(FPoint3{ 25, 0, -100 }, RGBColor{ 0,0,1 }, 50.f));
	SCENEMANAGER->GetActiveScene()->AddObjectToScene(new Plane(FPoint3{ 0, -200, 0 }, FVector3{ 0, 1, 0}));
	SCENEMANAGER->GetActiveScene()->AddLightToScene(new PointLight(FPoint3{ 0, 125, -150 }, RGBColor{1,1,1}, 3000));

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
		pCamera->Update(pTimer->GetElapsed());

		//--------- Render ---------
		pRenderer->Render(pCamera);

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
	delete SCENEMANAGER;
	delete pCamera;

	ShutDown(pWindow);
	return 0;
}