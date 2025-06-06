// Purpose:
//      Test that \DexFinishTest can be used with a hit_count, so the test exits
//      after the line referenced by \DexFinishTest has been stepped on a
//      specific number of times.
//      Test using the conditional controller (using \DexLimitSteps).
//
// The dbgeng driver doesn't support \DexLimitSteps yet.
// UNSUPPORTED: system-windows
//
// RUN: %dexter_regression_test_cxx_build %s -o %t
// RUN: %dexter_regression_test_run --binary %t -- %s | FileCheck %s
// CHECK: limit_steps_hit_count.cpp

int main() {
    for (int x = 0; x < 10; ++x)
        (void)0; // DexLabel('finish_line')
}

// DexLimitSteps(on_line=ref('finish_line'))
// DexFinishTest(on_line=ref('finish_line'), hit_count=5)
// DexExpectWatchValue('x', 0, 1, 2, 3, 4, 5)
