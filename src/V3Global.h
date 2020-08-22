// -*- mode: C++; c-file-style: "cc-mode" -*-
//*************************************************************************
// DESCRIPTION: Verilator: Common headers
//
// Code available from: https://verilator.org
//
//*************************************************************************
//
// Copyright 2003-2020 by Wilson Snyder. This program is free software; you
// can redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License
// Version 2.0.
// SPDX-License-Identifier: LGPL-3.0-only OR Artistic-2.0
//
//*************************************************************************

#ifndef _V3GLOBAL_H_
#define _V3GLOBAL_H_ 1

// clang-format off
#include "config_build.h"
#ifndef HAVE_CONFIG_BUILD
# error "Something failed during ./configure as config_build.h is incomplete. Perhaps you used autoreconf, don't."
#endif
// clang-format on

#include "verilatedos.h"

#include "V3Error.h"
#include "V3FileLine.h"
#include "V3Options.h"

#include <string>
#include <unordered_map>

class AstNetlist;
class V3HierBlockPlan;

//======================================================================
// Statics

//######################################################################

class VWidthMinUsage {
public:
    enum en : uint8_t { LINT_WIDTH, MATCHES_WIDTH, VERILOG_WIDTH };
    enum en m_e;
    inline VWidthMinUsage()
        : m_e{LINT_WIDTH} {}
    // cppcheck-suppress noExplicitConstructor
    inline VWidthMinUsage(en _e)
        : m_e{_e} {}
    explicit inline VWidthMinUsage(int _e)
        : m_e(static_cast<en>(_e)) {}  // Need () or GCC 4.8 false warning
    operator en() const { return m_e; }
};
inline bool operator==(const VWidthMinUsage& lhs, const VWidthMinUsage& rhs) {
    return lhs.m_e == rhs.m_e;
}
inline bool operator==(const VWidthMinUsage& lhs, VWidthMinUsage::en rhs) {
    return lhs.m_e == rhs;
}
inline bool operator==(VWidthMinUsage::en lhs, const VWidthMinUsage& rhs) {
    return lhs == rhs.m_e;
}

//######################################################################
// V3Global - The top level class for the entire program

class V3Global {
    // Globals
    AstNetlist* m_rootp;  // Root of entire netlist
    V3HierBlockPlan* m_hierPlanp;  // Hierarchical verilation plan, nullptr unless hier_block
    VWidthMinUsage m_widthMinUsage;  // What AstNode::widthMin() is used for

    int m_debugFileNumber = 0;  // Number to append to debug files created
    bool m_assertDTypesResolved = false;  // Tree should have dtypep()'s
    bool m_constRemoveXs = false;  // Const needs to strip any Xs
    bool m_needHeavy = false;  // Need verilated_heavy.h include
    bool m_needTraceDumper = false;  // Need __Vm_dumperp in symbols
    bool m_dpi = false;  // Need __Dpi include files
    bool m_useParallelBuild = false;  // Use parallel build for model

    // Memory address to short string mapping (for debug)
    typedef std::unordered_map<const void*, std::string> PtrToIdMap;  // The map type
    PtrToIdMap m_ptrToId;  // The actual 'address' <=> 'short string' bijection

public:
    // Options
    V3Options opt;  // All options; let user see them directly

    // CONSTRUCTORS
    V3Global()
        : m_rootp{nullptr}  // created by makeInitNetlist(} so static constructors run first
        , m_hierPlanp{nullptr}  // Set via hierPlanp(V3HierBlockPlan*} when use hier_block
        , m_widthMinUsage{VWidthMinUsage::LINT_WIDTH} {}
    AstNetlist* makeNetlist();
    void boot() {
        UASSERT(!m_rootp, "call once");
        m_rootp = makeNetlist();
    }
    void shutdown();  // Release allocated resorces
    // ACCESSORS (general)
    AstNetlist* rootp() const { return m_rootp; }
    VWidthMinUsage widthMinUsage() const { return m_widthMinUsage; }
    bool assertDTypesResolved() const { return m_assertDTypesResolved; }

    // METHODS
    void readFiles();
    void checkTree() const;
    static void dumpCheckGlobalTree(const string& stagename, int newNumber = 0,
                                    bool doDump = true);
    void assertDTypesResolved(bool flag) { m_assertDTypesResolved = flag; }
    void widthMinUsage(const VWidthMinUsage& flag) { m_widthMinUsage = flag; }
    bool constRemoveXs() const { return m_constRemoveXs; }
    void constRemoveXs(bool flag) { m_constRemoveXs = flag; }
    string debugFilename(const string& nameComment, int newNumber = 0) {
        ++m_debugFileNumber;
        if (newNumber) m_debugFileNumber = newNumber;
        char digits[100];
        sprintf(digits, "%03d", m_debugFileNumber);
        return opt.hierTopDataDir() + "/" + opt.prefix() + "_" + digits + "_" + nameComment;
    }
    bool needHeavy() const { return m_needHeavy; }
    void needHeavy(bool flag) { m_needHeavy = flag; }
    bool needTraceDumper() const { return m_needTraceDumper; }
    void needTraceDumper(bool flag) { m_needTraceDumper = flag; }
    bool dpi() const { return m_dpi; }
    void dpi(bool flag) { m_dpi = flag; }
    V3HierBlockPlan* hierPlanp() const { return m_hierPlanp; }
    void hierPlanp(V3HierBlockPlan* plan) {
        UASSERT(!m_hierPlanp, "call once");
        m_hierPlanp = plan;
    }
    void useParallelBuild(bool flag) { m_useParallelBuild = flag; }
    bool useParallelBuild() const { return m_useParallelBuild; }
    const std::string& ptrToId(const void* p);
};

extern V3Global v3Global;

//######################################################################

#endif  // guard
