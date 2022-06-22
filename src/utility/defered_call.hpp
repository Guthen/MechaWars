#pragma once

#include <functional>

using lambda = std::function<void()>;

//  don't even need references to declare since it's text replacement!
#define DEFERED( code )  GameManager::call_defered( [&]() { code } )  //  macro are awesome