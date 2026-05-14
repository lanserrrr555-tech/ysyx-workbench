// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "VSW2_top__pch.h"
#include "verilated_vcd_c.h"

//============================================================
// Constructors

VSW2_top::VSW2_top(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new VSW2_top__Syms(contextp(), _vcname__, this)}
    , a{vlSymsp->TOP.a}
    , b{vlSymsp->TOP.b}
    , f{vlSymsp->TOP.f}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
    contextp()->traceBaseModelCbAdd(
        [this](VerilatedTraceBaseC* tfp, int levels, int options) { traceBaseModel(tfp, levels, options); });
}

VSW2_top::VSW2_top(const char* _vcname__)
    : VSW2_top(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

VSW2_top::~VSW2_top() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void VSW2_top___024root___eval_debug_assertions(VSW2_top___024root* vlSelf);
#endif  // VL_DEBUG
void VSW2_top___024root___eval_static(VSW2_top___024root* vlSelf);
void VSW2_top___024root___eval_initial(VSW2_top___024root* vlSelf);
void VSW2_top___024root___eval_settle(VSW2_top___024root* vlSelf);
void VSW2_top___024root___eval(VSW2_top___024root* vlSelf);

void VSW2_top::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate VSW2_top::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    VSW2_top___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_activity = true;
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        VSW2_top___024root___eval_static(&(vlSymsp->TOP));
        VSW2_top___024root___eval_initial(&(vlSymsp->TOP));
        VSW2_top___024root___eval_settle(&(vlSymsp->TOP));
        vlSymsp->__Vm_didInit = true;
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    VSW2_top___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool VSW2_top::eventsPending() { return false; }

uint64_t VSW2_top::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* VSW2_top::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void VSW2_top___024root___eval_final(VSW2_top___024root* vlSelf);

VL_ATTR_COLD void VSW2_top::final() {
    contextp()->executingFinal(true);
    VSW2_top___024root___eval_final(&(vlSymsp->TOP));
    contextp()->executingFinal(false);
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* VSW2_top::hierName() const { return vlSymsp->name(); }
const char* VSW2_top::modelName() const { return "VSW2_top"; }
unsigned VSW2_top::threads() const { return 1; }
void VSW2_top::prepareClone() const { contextp()->prepareClone(); }
void VSW2_top::atClone() const {
    contextp()->threadPoolpOnClone();
}
std::unique_ptr<VerilatedTraceConfig> VSW2_top::traceConfig() const {
    return std::unique_ptr<VerilatedTraceConfig>{new VerilatedTraceConfig{false}};
};

//============================================================
// Trace configuration

void VSW2_top___024root__trace_decl_types(VerilatedVcd* tracep);

void VSW2_top___024root__trace_init_top(VSW2_top___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD static void trace_init(void* voidSelf, VerilatedVcd* tracep, uint32_t code) {
    // Callback from tracep->open()
    VSW2_top___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<VSW2_top___024root*>(voidSelf);
    VSW2_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (!vlSymsp->_vm_contextp__->calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
            "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vlSymsp->__Vm_baseCode = code;
    tracep->pushPrefix(vlSymsp->name(), VerilatedTracePrefixType::SCOPE_MODULE);
    VSW2_top___024root__trace_decl_types(tracep);
    VSW2_top___024root__trace_init_top(vlSelf, tracep);
    tracep->popPrefix();
}

VL_ATTR_COLD void VSW2_top___024root__trace_register(VSW2_top___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD void VSW2_top::traceBaseModel(VerilatedTraceBaseC* tfp, int levels, int options) {
    (void)levels; (void)options;
    VerilatedVcdC* const stfp = dynamic_cast<VerilatedVcdC*>(tfp);
    if (VL_UNLIKELY(!stfp)) {
        vl_fatal(__FILE__, __LINE__, __FILE__,"'VSW2_top::trace()' called on non-VerilatedVcdC object;"
            " use --trace-fst with VerilatedFst object, and --trace-vcd with VerilatedVcd object");
    }
    stfp->spTrace()->addModel(this);
    stfp->spTrace()->addInitCb(&trace_init, &(vlSymsp->TOP), name(), false, 3);
    VSW2_top___024root__trace_register(&(vlSymsp->TOP), stfp->spTrace());
}
