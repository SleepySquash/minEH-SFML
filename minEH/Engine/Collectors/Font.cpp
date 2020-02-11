//
//  Font.cpp
//  NovelSome
//
//  Created by Никита Исаенко on 17/03/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "Font.hpp"

namespace ns
{
    FontCollectorObject::FontCollectorObject(sf::Font* font, int usage, bool destroyable) : font(font), usage(usage), destroyable(destroyable) { }
    sf::Font* FontCollector::GetFont(const std::wstring& fontName)
    {
        auto it = fonts.find(fontName);
        if (it != fonts.end()) return it->second.font;
        else
        {
            it = fonts.find(L"Data/Fonts/" + fontName);
            if (it != fonts.end()) return it->second.font;
            else
            {
                std::wstring fullPath = fontName;
                if (!base::FileExists(fullPath)) fullPath = base::utf16(resourcePath()) + fontName;
                if (!base::FileExists(fullPath)) fullPath = L"Data/Fonts/" + fontName;
                if (!base::FileExists(fullPath)) fullPath = base::utf16(resourcePath()) + L"Data/Fonts/" + fontName;
                if (base::FileExists(fullPath))
                {
                    sf::Font* font = new sf::Font();
                    bool fontLoaded{ false };
#ifdef _WIN32
                    //TODO: Fix memory leak
                    std::ifstream ifStream(fullPath, std::ios::binary | std::ios::ate);
                    if (!ifStream.is_open())
                        std::cerr << "Unable to open file: " << base::utf8(fullPath) << std::endl;
                    else
                    {
                        auto filesize = ifStream.tellg();
                        char* fileInMemory = new char[static_cast<unsigned int>(filesize)];
                        ifStream.seekg(0, std::ios::beg);
                        ifStream.read(fileInMemory, filesize);
                        ifStream.close();
                        
                        fontLoaded = font->loadFromMemory(fileInMemory, filesize);
                    }
#else
                    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
                    std::string u8str = converter.to_bytes(fullPath);
                    if (!(fontLoaded = font->loadFromFile(u8str)))
                        std::cerr << "Unable to open file: " << base::utf8(fullPath) << std::endl;
#endif
                    
                    if (fontLoaded)
                    {
                        fonts.emplace(L"Data/Fonts/" + fontName, font);
                        return font;
                    }
                }
                if (fontName == L"Data/Fonts/Arial.ttf" || fontName == L"Arial.ttf") return nullptr;
                else return GetFont(L"Arial.ttf");
            }
        }
    }
    sf::Font* FontCollector::LoadFont(const std::wstring& fontName)
    {
        auto it = fonts.find(fontName);
        if (it != fonts.end()) { FontCollectorObject* obj = &it->second; ++obj->usage; return obj->font; }
        else
        {
            it = fonts.find(L"Data/Fonts/" + fontName);
            if (it != fonts.end()) { FontCollectorObject* obj = &it->second; ++obj->usage; return obj->font; }
            else
            {
                std::wstring fullPath = fontName;
                if (!base::FileExists(fullPath)) fullPath = base::utf16(resourcePath()) + fontName;
                if (!base::FileExists(fullPath)) fullPath = L"Data/Fonts/" + fontName;
                if (!base::FileExists(fullPath)) fullPath = base::utf16(resourcePath()) + L"Data/Fonts/" + fontName;
                if (base::FileExists(fullPath))
                {
                    sf::Font* font = new sf::Font();
                    bool fontLoaded{ false };
    #ifdef _WIN32
                    //TODO: Fix memory leak
                    std::ifstream ifStream(fullPath, std::ios::binary | std::ios::ate);
                    if (!ifStream.is_open())
                        std::cerr << "Unable to open file: " << base::utf8(fullPath) << std::endl;
                    else
                    {
                        auto filesize = ifStream.tellg();
                        char* fileInMemory = new char[static_cast<unsigned int>(filesize)];
                        ifStream.seekg(0, std::ios::beg);
                        ifStream.read(fileInMemory, filesize);
                        ifStream.close();
                        
                        fontLoaded = font->loadFromMemory(fileInMemory, filesize);
                    }
    #else
                    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
                    std::string u8str = converter.to_bytes(fullPath);
                    if (!(fontLoaded = font->loadFromFile(u8str)))
                        std::cerr << "Unable to open file: " << base::utf8(fullPath) << std::endl;
    #endif
                    
                    if (fontLoaded)
                    {
                        fonts.emplace(L"Data/Fonts/" + fontName, font).first->second.usage = 1;
                        return font;
                    }
                }
                if (fontName == L"Data/Fonts/Arial.ttf" || fontName == L"Arial.ttf") return nullptr;
                else return LoadFont(L"Arial.ttf");
            }
        }
    }
    void FontCollector::DeleteFont(const std::wstring& fontName)
    {
        auto it = fonts.find(fontName);
        if (it != fonts.end())
        {
            FontCollectorObject* obj = &it->second;
            --obj->usage;
            if (obj->usage <= 0 && obj->destroyable)
            {
                if (obj->font) delete obj->font;
                fonts.erase(it);
            }
        }
    }
    bool FontCollector::FontExistsAtPath(const std::wstring& fontName)
    {
        if (fonts.find(fontName) != fonts.end()) return true;
        std::wstring fullPath = fontName;
        if (base::FileExists(fullPath)) return true; else fullPath = base::utf16(resourcePath()) + fontName;
        if (base::FileExists(fullPath)) return true; else fullPath = L"Data/Fonts/" + fontName;
        if (base::FileExists(fullPath)) return true; else fullPath = base::utf16(resourcePath()) + L"Data/Fonts/" + fontName;
        if (base::FileExists(fullPath)) return true;
        return false;
    }
    void FontCollector::FreeFonts()
    {
        for (const auto& key : fonts) if (key.second.font) delete key.second.font;
        fonts.clear();
    }
    std::unordered_map<std::wstring, FontCollectorObject> FontCollector::fonts;
}
