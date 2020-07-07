//
//  StaticMethods.cpp
//  NovelSome
//
//  Created by Никита Исаенко on 26/08/2018.
//  Copyright © 2018 Melancholy Hill. All rights reserved.
//

#include "Settings.hpp"

namespace ns
{
    void GlobalSettings::Load(const std::wstring& path)
    {
        std::wifstream wif;
#ifdef _WIN32
        wif.open(base::utf16(documentsPath()) + path);
#else
        wif.open(documentsPath() + base::utf8(path));
#endif
        wif.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        
        if (wif.is_open())
        {
            std::wstring line;
            nss::CommandSettings command;
            command.lowercaseCommand = false;
            
            while (!wif.eof())
            {
                std::getline(wif, line);
                command.Command(line);
                
                if (nss::Command(command, L"framerateLimit:"))
                {
                    int integer = nss::ParseAsInt(command);
                    if (integer >= 0) gs::framerateLimit = integer;
                }
                else if (nss::Command(command, L"framerateNoFocus:"))
                {
                    int integer = nss::ParseAsInt(command);
                    if (integer >= 0) gs::framerateNoFocus = integer;
                }
                else if (nss::Command(command, L"resolutionClassSetting:") || nss::Command(command, L"resolutionClass:"))
                {
                    int integer = nss::ParseAsInt(command);
                    if (integer >= -1) gs::resolutionClass = gs::resolutionClassSetting = integer;
                }
                else if (nss::Command(command, L"isParallaxEnabled:"))
                    gs::isParallaxEnabled = nss::ParseAsBool(command);
                else if (nss::Command(command, L"isVerticalSyncEnabled:"))
                    gs::isVerticalSyncEnabled = nss::ParseAsBool(command);
                else if (nss::Command(command, L"maxVolumeGlobal:"))
                {
                    float integer = nss::ParseAsFloat(command);
                    if (integer >= 0) gs::maxVolumeGlobal = integer;
                }
                else if (nss::Command(command, L"maxVolumeMusic:"))
                {
                    float integer = nss::ParseAsFloat(command);
                    if (integer >= 0) gs::maxVolumeMusic = integer;
                }
                else if (nss::Command(command, L"maxVolumeAmbeint:"))
                {
                    float integer = nss::ParseAsFloat(command);
                    if (integer >= 0) gs::maxVolumeAmbient = integer;
                }
                else if (nss::Command(command, L"maxVolumeSound:"))
                {
                    float integer = nss::ParseAsFloat(command);
                    if (integer >= 0) gs::maxVolumeSound = integer;
                }
            }
            
            wif.close();
        }
    }
    void GlobalSettings::Save(const std::wstring& path)
    {
        if (!base::FileExists(path)) base::CreateDirectory(base::utf16(documentsPath()));
        
        std::wofstream wof;
#ifdef _WIN32
        wof.open(base::utf16(documentsPath()) + path);
#else
        wof.open(documentsPath() + base::utf8(path));
#endif
        wof.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        if (wof.is_open())
        {
            wof << " " << endl;
        }
    }
    
    
    
    
    
    
    sf::RenderWindow* gs::window = nullptr;
    
    unsigned int gs::width = 0;
    unsigned int gs::height = 0;
    
    unsigned int gs::relativeWidth = 0;
    unsigned int gs::relativeHeight = 0;
    float gs::scale = 1.f, gs::scScale = 1.f;
    float gs::scalex = 1.f, gs::scaley = 1.f;
    bool gs::verticalOrientation = false, gs::trueVerticalOrientation = false;
    int gs::screenOffsetTop{ 0 }, gs::notchEffectFromTop{ 0 };
    float gs::scaleUI{ 1.f };
    bool gs::fullscreen{ false };
    
    float gs::deltaVelocity = 1.f;
    float gs::fixedUpdate = 1.f;
    
    std::pair<int, int> gs::lastMousePos = { 0, 0 };
#ifdef _WIN32
    int gs::windowPositionOffset = 0;//IsWindows8OrGreater() ? GetSystemMetrics(SM_CXSIZEFRAME) : 0;
#else
    int gs::windowPositionOffset = 0;
#endif
    
    bool gs::isVerticalSyncEnabled = true;
    int gs::framerateLimit = 0;
    int gs::framerateNoFocus = 10;
    
    bool gs::isResolutionClassEnabled = true;
    int gs::resolutionClassSetting = -1;
    int gs::resolutionClass = 1;
    bool gs::prefer0xRatherThatpx = false;
    
    bool gs::isPause = false;
    bool gs::isPauseEnabled = true;
    bool gs::pauseOnFocusLost = true;
    bool gs::inGame = false;
    bool gs::ignoreEvent = false, gs::forceIgnoreEvent = false, gs::ignoreDraw = false;
    bool gs::requestWindowRefresh = true;
    float gs::autosaveDeltaTime = 240.f;

    bool gs::listenForTextInput{ false }, gs::rememberUsername{ true }, gs::rememberPassword{ false };
    std::wstring gs::username; std::string gs::password;
    
    std::vector<void*> gs::activeInterfaces;
    void gs::PushInterface(void* address) { activeInterfaces.push_back(address); ignoreEvent = true; }
    void gs::RemoveInterface(void* address) { activeInterfaces.erase(std::remove(activeInterfaces.begin(), activeInterfaces.end(), address), activeInterfaces.end()); ignoreEvent = !activeInterfaces.empty(); }
    bool gs::isActiveInterface(void* address) { if (!address) return activeInterfaces.empty(); else return (activeInterfaces.back() == address); }
    
    float gs::maxVolumeGlobal = 1.f, gs::maxVolumeMusic = 1.f, gs::maxVolumeAmbient = 1.f, gs::maxVolumeSound = 1.f;
    
    bool gs::isParallaxEnabled = true;
    float gs::defaultParallaxBackground = 0.018, gs::defaultParallaxFar = 0.032, gs::defaultParallaxNormal = 0.062, gs::defaultParallaxClose = 0.105, gs::defaultParallaxFrontground = 0.13;
    
    bool gs::buttonHovering = true, gs::drawGUIBoundaries = false, gs::forcePressInsideDialogue = true;
}
