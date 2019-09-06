

#pragma once
#include <iostream>
#include <cassert>

#define YARE_LOG(...) std::cout << __VA_ARGS__

#ifdef YARE_DEBUG
#define YARE_ASSERT(condition, msg) if(! (condition )) {YARE_LOG(msg); assert(condition); }
#else 
#define YARE_ASSERT(condition, msg) 
#endif 

