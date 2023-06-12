//-----------------------------------------------------------------------------
// COMPILE-TIME OPTIONS FOR DEAR IMGUI
// Runtime options (clipboard callbacks, enabling various features, etc.) can generally be set via the ImGuiIO structure.
// You can use ImGui::SetAllocatorFunctions() before calling ImGui::CreateContext() to rewire memory allocation functions.
//-----------------------------------------------------------------------------
// A) You may edit imconfig.h (and not overwrite it when updating Dear ImGui, or maintain a patch/branch with your modifications to imconfig.h)
// B) or add configuration directives in your own file and compile with #define IMGUI_USER_CONFIG "myfilename.h"
// If you do so you need to make sure that configuration settings are defined consistently _everywhere_ Dear ImGui is used, which include
// the imgui*.cpp files but also _any_ of your code that uses Dear ImGui. This is because some compile-time options have an affect on data structures.
// Defining those options in imconfig.h will ensure every compilation unit gets to see the same data structure layouts.
// Call IMGUI_CHECKVERSION() from your .cpp files to verify that the data structures your files are using are matching the ones imgui.cpp is using.
//-----------------------------------------------------------------------------

#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

#include "imgui-SFML_export.h"

#define IM_VEC2_CLASS_EXTRA                                             \
    template <typename T>                                               \
    ImVec2(const sf::Vector2<T>& v) {                                   \
        x = static_cast<float>(v.x);                                    \
        y = static_cast<float>(v.y);                                    \
    }                                                                   \
                                                                        \
    template <typename T>                                               \
    operator sf::Vector2<T>() const {                                   \
        return sf::Vector2<T>(x, y);                                    \
    }

#define IM_VEC4_CLASS_EXTRA                                             \
    ImVec4(const sf::Color & c)                                         \
        : x(c.r / 255.f), y(c.g / 255.f), z(c.b / 255.f), w(c.a / 255.f)\
    {}                                                                  \
    operator sf::Color() const {                                        \
        return sf::Color(                                               \
            static_cast<sf::Uint8>(x * 255.f),                          \
            static_cast<sf::Uint8>(y * 255.f),                          \
            static_cast<sf::Uint8>(z * 255.f),                          \
            static_cast<sf::Uint8>(w * 255.f));                         \
    }

#define ImTextureID unsigned int
