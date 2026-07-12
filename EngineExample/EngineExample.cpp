// EngineExample.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//
#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <iostream>
#include <filesystem>
#include <memory>
#include "events.h"
#include "EngineApplication.h"
#include "EngineParameters.h"
#include "capabilitiesVisitorImpl.h"
#include "VkEnumToString.h"
#include <SDL3/SDL_vulkan.h>

SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
std::shared_ptr<EngineApplication> g_appEngine;

int g_width = 800;
int g_height = 600;

EngineApplicationParameters g_appParam
{
    "EngineExample",
    "profileTest.json",		/*!< parameters file*/
    "Profile_test",		/*!< parameters profile*/
    1		/*!< application version*/
};

int main(int argc, char* argv[])
{
    SDL_SetAppMetadata("EngineExample", "0.0.0.1", "com.example.vulkanEngine");
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed (%s)", SDL_GetError());
        return 1;
    }
    SDL_Vulkan_LoadLibrary(nullptr);

    g_window = SDL_CreateWindow("EngineExample", g_width, g_height, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
    

    CapabilitiesConsoleVisitorImpl visitor;
    visitor.visit(EngineApplication::hostCapabilities());

    g_appParam.parametersFilename = std::filesystem::current_path().string() + "\\" + g_appParam.parametersFilename;

    g_appEngine = std::make_shared<EngineApplication>(g_appParam); 
    auto& caps = g_appEngine->capabilities();
    VkSurfaceKHR surface;
    if (!SDL_Vulkan_CreateSurface(g_window, caps.instance, nullptr, &surface))
    {
        SDL_Log("SDL_Vulkan_CreateSurface failed (%s)", SDL_GetError());
        return 1;
    }
    
    //auto hasProfile = Reflective::instance().hasProfile("Profile_test");

    DeviceParameters devParameters;// <= ERROR NO QUEUE!
    auto suitableDev = g_appEngine->suitableDevices(devParameters, &surface);
    
    if (suitableDev.empty())
    {
		std::cerr << "No suitable device found" << std::endl;
        return -1;
    }

	for (const auto& dev : suitableDev)
	{        
        std::cout << "0 - device index: " << dev.deviceIndex << "\n";
        
        if (!dev.extensions.empty())
        {
            std::cout << "\t*Extensions:\n";
            for (const auto& ext : dev.extensions)
                std::cout << "\t\t" << ext << "\n";
            std::cout << "\n";
        }

        if (!dev.layers.empty())
        {
            std::cout << "\t*Layers:\n";
            for (const auto& layer : dev.layers)
                std::cout << "\t\t" << layer << "\n";
            std::cout << "\n";
        }

        if (!dev.queues.empty())
        {
            std::cout << "\t*Queues:\n";
            for (const auto& queue : dev.queues)
            {
                std::cout << "\t\tFamily index: " << queue.familyIndex << "\n";
                std::cout << "\t\tFlags: " << Flag<VkQueueFlagBits>::to_string(queue.flags) << "\n";
                std::cout << "\t\tCount: " << queue.queueCount << "\n";
                std::cout << "\t\tPriority: " << queue.priority << "\n";
                std::cout << "\t\t-------------------------------\n";
            }
        }
        std::cout << "\n\n";
	}

    int index = 0;
	if (suitableDev.size() > 1)
	{
		std::cout << "Multiple suitable devices found, using the first one" << std::endl;
        std::cin >> index;
	}
    auto device = g_appEngine->createDevice(suitableDev[index]);
    


    SDL_Event event;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (processEvent(event))
            {
                quit = true;
                break;
            }
        }
    }
    return 0;
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
