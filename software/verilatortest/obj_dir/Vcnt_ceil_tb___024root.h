// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vcnt_ceil_tb.h for the primary calling header

#ifndef VERILATED_VCNT_CEIL_TB___024ROOT_H_
#define VERILATED_VCNT_CEIL_TB___024ROOT_H_  // guard

#include "verilated.h"
#include "verilated_timing.h"


class Vcnt_ceil_tb__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vcnt_ceil_tb___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    CData/*0:0*/ cnt_ceil_tb__DOT__clk;
    CData/*0:0*/ cnt_ceil_tb__DOT__rst_n;
    CData/*0:0*/ cnt_ceil_tb__DOT__en;
    CData/*3:0*/ cnt_ceil_tb__DOT__ceil;
    CData/*3:0*/ cnt_ceil_tb__DOT__cnt_ceil_0__DOT__cnt_reg;
    CData/*0:0*/ __Vtrigprevexpr___TOP__cnt_ceil_tb__DOT__clk__0;
    CData/*0:0*/ __Vtrigprevexpr___TOP__cnt_ceil_tb__DOT__rst_n__0;
    CData/*0:0*/ __VactContinue;
    IData/*31:0*/ __VactIterCount;
    VlDelayScheduler __VdlySched;
    VlTriggerVec<2> __VactTriggered;
    VlTriggerVec<2> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vcnt_ceil_tb__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vcnt_ceil_tb___024root(Vcnt_ceil_tb__Syms* symsp, const char* v__name);
    ~Vcnt_ceil_tb___024root();
    VL_UNCOPYABLE(Vcnt_ceil_tb___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
