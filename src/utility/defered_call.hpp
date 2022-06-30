#pragma once

#include <functional>

using lambda = std::function<void()>;

struct Timer
{
	Timer( float time, lambda call ) : time( time ), call( call ) {}

	float time;
	lambda call;
};

//  don't even need references to declare since it's text replacement!
#define DEFERED( code )  GameManager::defered_call( [&]() { code } )  //  macro are awesome
#define TIMER( time, code )  GameManager::timed_call( time, [&]() { code } )