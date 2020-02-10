//
//  NovelSomeScript.hpp
//  NovelSome
//
//  Created by Никита Исаенко on 31/08/2018.
//  Copyright © 2018 Melancholy Hill. All rights reserved.
//

#ifndef NovelSomeScript_hpp
#define NovelSomeScript_hpp

#include <iostream>
#include <string>
#include <vector>

// nss::SetStringWithLineBreaks
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Color.hpp>

// for ns::base::LowercaseTheString
#include "../Essentials/Base.hpp"
#include "../Essentials/ResourcePath.hpp"

// for mathparser, stack functionality
#include "List.hpp"

using std::cout;
using std::cin;
using std::endl;
using namespace ns;
using base::utf8;
using base::utf16;

namespace nss
{
    struct CommandSettings
    {
        std::wstring line{ L"" };
        unsigned int lastPos{ 0 };
        bool keepTheLastPos{ true };
        bool lowercaseCommand{ true };
        
        void Command(const std::wstring& command, bool skipSpaces = false);
        bool ExplicitNoMessage();
    };
    
    ///----------------------------------------------------------
    /// \brief Checks if the parsed string begins with command.
    ///
    /// Checks if the parsed string begins with command.
    /// Возвращает истину, если данная строка начинается с указанной команды, иначе - ложь.
    ///
    /// \param results Structure of type CommandSettings that contains specific settings and info.
    /// \param command The command to check for.
    ///
    /// \returns true if parsed string starts with specified command, false otherwise.
    ///----------------------------------------------------------
    bool Command(CommandSettings& results, const std::wstring& command);
    bool Command(const std::wstring& line, const std::wstring& command, bool lowercase = true, bool skipSpaces = true);
    bool Command(const std::string& line, const std::string& command);
    
    //TODO: Documentation
    void SkipSpaces(CommandSettings& results);
    void RemoveSpaces(CommandSettings& results);
    void RemoveSpaces(std::wstring& line);
    void Trim(std::wstring& line);
    bool ContainsUsefulInformation(CommandSettings& results);
    bool ContainsUsefulInformation(const std::wstring& wstr);
    
    //TODO: Documentation
    std::wstring GetFromUntil(const std::wstring& line, unsigned int pos, const wchar_t until);
    
    //TODO: Documentation
    std::wstring ParseUntil(CommandSettings& results, const wchar_t until);
    std::string ParseUntil(std::string line, const char until, unsigned int from = 0);
    std::wstring ParseUntilReverse(std::wstring line, const wchar_t until, unsigned int from = 0);
    std::wstring ParseWhile(CommandSettings& results, const wchar_t until);
    std::wstring ParseAsQuoteString(CommandSettings& results);
    std::wstring ParseAsMaybeQuoteString(CommandSettings& results);
    std::wstring ParseAsMaybeQuoteStringFull(CommandSettings& results);
    int ParseAsInt(CommandSettings& results);
    long ParseAsLong(CommandSettings& results);
    float ParseAsFloat(CommandSettings& results);
    std::wstring ParseAsString(CommandSettings& results);
    std::wstring ParseAsStringWOLowercase(CommandSettings& results);
    bool ParseAsBool(CommandSettings& results);
    
    sf::Color ParseColor(CommandSettings& results);
    int ParseAlpha(CommandSettings& results);
    
    //TODO: Documentation
    std::wstring ParseArgument(CommandSettings& results);
    void ParseArguments(CommandSettings& results, std::vector<std::wstring>& vec);
    std::wstring ArgumentAsString(CommandSettings& results);
    std::wstring ArgumentAsStringWOLowerCase(CommandSettings& results);
    bool ArgumentAsBool(CommandSettings& results);
    float ArgumentAsFloat(CommandSettings& results);
    int ArgumentAsInt(CommandSettings& results);
    
    void SetStringWithLineBreaks(sf::Text& text, const std::wstring& line, unsigned int width, int shift = 0);
    void SetStringWithLineBreaksWOSpaceFinding(sf::Text& text, const std::wstring& line, const unsigned int width);
    std::wstring GetStringWithLineBreaks(sf::Text& text, const std::wstring& line, unsigned int width, unsigned int height = 0, int shift = 0);
    std::wstring GetStringWithLineBreaksWOSpaceFinding(sf::Text& text, const std::wstring& line, const unsigned int width);
    
    std::wstring GetPathWOResourcePath(const std::wstring& path);
    
    float MathParser(const std::wstring& finalLine);
}

#endif /* NovelSomeScript_hpp */
