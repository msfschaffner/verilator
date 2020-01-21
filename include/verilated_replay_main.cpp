// -*- mode: C++; c-file-style: "cc-mode" -*-
//*************************************************************************
//
// Copyright 2003-2020 by Todd Strader. This program is free software; you can
// redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License.
// Version 2.0.
//
// Verilator is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
//=========================================================================
///
/// \file
/// \brief Verilator: Main used by verilator_replay
///
///     This utility will replay trace files onto a verilated design.
///     It is inteded to be used in conjunction with verilator_replay.
///
/// Code available from: http://www.veripool.org/verilator
///
//=========================================================================


#include "verilated_replay.h"

double simTime = 0;
double sc_time_stamp() {
    return simTime;
}

int main(int argc, char** argv) {
    // TODO -- actual arg parsing
    std::string fstFilename(argv[1]);
    VL_PRINTF("FST = %s\n", fstFilename.c_str());

    VerilatedReplay replay(fstFilename, simTime);
    if (replay.init()) exit(-1);

    if (replay.replay()) exit(-1);

    return 0;
}
