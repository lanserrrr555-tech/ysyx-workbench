// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VSW2_top.h for the primary calling header

#include "VSW2_top__pch.h"

void VSW2_top___024root___ctor_var_reset(VSW2_top___024root* vlSelf);

VSW2_top___024root::VSW2_top___024root(VSW2_top__Syms* symsp, const char* namep)
 {
    vlSymsp = symsp;
    vlNamep = strdup(namep);
    // Reset structure values
    VSW2_top___024root___ctor_var_reset(this);
}

void VSW2_top___024root::__Vconfigure(bool first) {
    (void)first;  // Prevent unused variable warning
}

VSW2_top___024root::~VSW2_top___024root() {
    VL_DO_DANGLING(std::free(const_cast<char*>(vlNamep)), vlNamep);
}
