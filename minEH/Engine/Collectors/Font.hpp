//
//  Font.hpp
//  NovelSome
//
//  Created by Никита Исаенко on 17/03/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef FontCollector_hpp
#define FontCollector_hpp

#include <iostream>
#include <unordered_map>

#include <thread>
#include <codecvt>
#include <fstream>
#include <memory>

#include <SFML/Graphics/Font.hpp>

#include "../../Essentials/ResourcePath.hpp"
#include "../../Essentials/Base.hpp"
#include "etc.hpp"

using base::utf8;
using base::utf16;
using std::pair;
using std::make_pair;
using std::cout;
using std::endl;

namespace ns
{
    struct FontCollectorObject
    {
        sf::Font* font{ nullptr };
        int usage{ 0 };
        bool destroyable{ true };
        
        FontCollectorObject(sf::Font* font = nullptr, int usage = 0, bool destroyable = true);
    };
    class FontCollector
    {
    private:
        FontCollector() { }
        
    public:
        static std::unordered_map<std::wstring, FontCollectorObject> fonts;
        
        static sf::Font* GetFont(const std::wstring& fontName); /// TODO: obsolete, will not load the font in the future
                                                                /// TODO: !!! Use instead LoadFont() and DeleteFont()
        
        static sf::Font* LoadFont(const std::wstring& fontName);
        static void DeleteFont(const std::wstring& fontName);
        static bool FontExistsAtPath(const std::wstring& fontName);
        static void FreeFonts();
    };
    
    typedef FontCollector fc;
}

#endif /* Font_hpp */
