// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See VSW2_top.h for the primary calling header

#ifndef VERILATED_VSW2_TOP___024ROOT_H_
#define VERILATED_VSW2_TOP___024ROOT_H_  // guard

#include "verilated.h"


class VSW2_top__Syms;

class alignas(VL_CACHE_LINE_BYTES) VSW2_top___024root final {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(a,0,0);
    VL_IN8(b,0,0);
    VL_OUT8(f,0,0);
    CData/*0:0*/ __VstlFirstIteration;
    CData/*0:0*/ __VstlPhaseResult;
    CData/*0:0*/ __VicoFirstIteration;
    CData/*0:0*/ __VicoPhaseResult;
    VlUnpacked<QData/*63:0*/, 1> __VstlTriggered;
    VlUnpacked<QData/*63:0*/, 1> __VicoTriggered;

    // INTERNAL VARIABLES
    VSW2_top__Syms* vlSymsp;
    const char* vlNamep;

    // CONSTRUCTORS
    VSW2_top___024root(VSW2_top__Syms* symsp, const char* namep);
    ~VSW2_top___024root();
    VL_UNCOPYABLE(VSW2_top___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
