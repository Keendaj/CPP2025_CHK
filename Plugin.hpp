#pragma once


#ifdef PLUGIN_EXPORTS
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT __declspec(dllimport)
#endif

extern "C" void  getCalculation();