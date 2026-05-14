// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals

#include "verilated_vcd_c.h"
#include "VSW2_top__Syms.h"


VL_ATTR_COLD void VSW2_top___024root__trace_init_sub__TOP__0(VSW2_top___024root* vlSelf, VerilatedVcd* tracep) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSW2_top___024root__trace_init_sub__TOP__0\n"); );
    VSW2_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const int c = vlSymsp->__Vm_baseCode;
    VL_TRACE_PUSH_PREFIX(tracep, "$rootio", VerilatedTracePrefixType::SCOPE_MODULE, 0, 0);
    VL_TRACE_DECL_BIT(tracep,c+0,0,"a",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC);
    VL_TRACE_DECL_BIT(tracep,c+1,0,"b",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC);
    VL_TRACE_DECL_BIT(tracep,c+2,0,"f",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC);
    VL_TRACE_POP_PREFIX(tracep);
    VL_TRACE_PUSH_PREFIX(tracep, "SW2_top", VerilatedTracePrefixType::SCOPE_MODULE, 0, 0);
    VL_TRACE_DECL_BIT(tracep,c+0,0,"a",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC);
    VL_TRACE_DECL_BIT(tracep,c+1,0,"b",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC);
    VL_TRACE_DECL_BIT(tracep,c+2,0,"f",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC);
    VL_TRACE_POP_PREFIX(tracep);
}

VL_ATTR_COLD void VSW2_top___024root__trace_init_top(VSW2_top___024root* vlSelf, VerilatedVcd* tracep) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSW2_top___024root__trace_init_top\n"); );
    VSW2_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    VSW2_top___024root__trace_init_sub__TOP__0(vlSelf, tracep);
}

VL_ATTR_COLD void VSW2_top___024root__trace_const_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
VL_ATTR_COLD void VSW2_top___024root__trace_full_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void VSW2_top___024root__trace_chg_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void VSW2_top___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/);

VL_ATTR_COLD void VSW2_top___024root__trace_register(VSW2_top___024root* vlSelf, VerilatedVcd* tracep) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSW2_top___024root__trace_register\n"); );
    VSW2_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    tracep->addConstCb(&VSW2_top___024root__trace_const_0, 0, vlSelf);
    tracep->addFullCb(&VSW2_top___024root__trace_full_0, 0, vlSelf);
    tracep->addChgCb(&VSW2_top___024root__trace_chg_0, 0, vlSelf);
    tracep->addCleanupCb(&VSW2_top___024root__trace_cleanup, vlSelf);
}

VL_ATTR_COLD void VSW2_top___024root__trace_const_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSW2_top___024root__trace_const_0\n"); );
    // Body
    VSW2_top___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<VSW2_top___024root*>(voidSelf);
    VSW2_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
}

VL_ATTR_COLD void VSW2_top___024root__trace_full_0_sub_0(VSW2_top___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void VSW2_top___024root__trace_full_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSW2_top___024root__trace_full_0\n"); );
    // Body
    VSW2_top___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<VSW2_top___024root*>(voidSelf);
    VSW2_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VSW2_top___024root__trace_full_0_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void VSW2_top___024root__trace_full_0_sub_0(VSW2_top___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSW2_top___024root__trace_full_0_sub_0\n"); );
    VSW2_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    bufp->fullBit(oldp+0,(vlSelfRef.a));
    bufp->fullBit(oldp+1,(vlSelfRef.b));
    bufp->fullBit(oldp+2,(vlSelfRef.f));
}
