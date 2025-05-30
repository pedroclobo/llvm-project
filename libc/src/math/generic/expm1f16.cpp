//===-- Half-precision e^x - 1 function -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/math/expm1f16.h"
#include "expxf16.h"
#include "hdr/errno_macros.h"
#include "hdr/fenv_macros.h"
#include "src/__support/FPUtil/FEnvImpl.h"
#include "src/__support/FPUtil/FPBits.h"
#include "src/__support/FPUtil/PolyEval.h"
#include "src/__support/FPUtil/cast.h"
#include "src/__support/FPUtil/except_value_utils.h"
#include "src/__support/FPUtil/multiply_add.h"
#include "src/__support/FPUtil/rounding_mode.h"
#include "src/__support/common.h"
#include "src/__support/macros/config.h"
#include "src/__support/macros/optimization.h"

namespace LIBC_NAMESPACE_DECL {

#ifndef LIBC_MATH_HAS_SKIP_ACCURATE_PASS
static constexpr fputil::ExceptValues<float16, 1> EXPM1F16_EXCEPTS_LO = {{
    // (input, RZ output, RU offset, RD offset, RN offset)
    // x = 0x1.564p-5, expm1f16(x) = 0x1.5d4p-5 (RZ)
    {0x2959U, 0x2975U, 1U, 0U, 1U},
}};

#ifdef LIBC_TARGET_CPU_HAS_FMA_FLOAT
static constexpr size_t N_EXPM1F16_EXCEPTS_HI = 2;
#else
static constexpr size_t N_EXPM1F16_EXCEPTS_HI = 3;
#endif

static constexpr fputil::ExceptValues<float16, N_EXPM1F16_EXCEPTS_HI>
    EXPM1F16_EXCEPTS_HI = {{
        // (input, RZ output, RU offset, RD offset, RN offset)
        // x = 0x1.c34p+0, expm1f16(x) = 0x1.34cp+2 (RZ)
        {0x3f0dU, 0x44d3U, 1U, 0U, 1U},
        // x = -0x1.e28p-3, expm1f16(x) = -0x1.adcp-3 (RZ)
        {0xb38aU, 0xb2b7U, 0U, 1U, 1U},
#ifndef LIBC_TARGET_CPU_HAS_FMA_FLOAT
        // x = 0x1.a08p-3, exp10m1f(x) = 0x1.cdcp-3 (RZ)
        {0x3282U, 0x3337U, 1U, 0U, 0U},
#endif
    }};
#endif // !LIBC_MATH_HAS_SKIP_ACCURATE_PASS

LLVM_LIBC_FUNCTION(float16, expm1f16, (float16 x)) {
  using FPBits = fputil::FPBits<float16>;
  FPBits x_bits(x);

  uint16_t x_u = x_bits.uintval();
  uint16_t x_abs = x_u & 0x7fffU;

  // When |x| <= 2^(-3), or |x| >= -11 * log(2), or x is NaN.
  if (LIBC_UNLIKELY(x_abs <= 0x3000U || x_abs >= 0x47a0U)) {
    // expm1(NaN) = NaN
    if (x_bits.is_nan()) {
      if (x_bits.is_signaling_nan()) {
        fputil::raise_except_if_required(FE_INVALID);
        return FPBits::quiet_nan().get_val();
      }

      return x;
    }

    // expm1(+/-0) = +/-0
    if (x_abs == 0)
      return x;

    // When x >= 16 * log(2).
    if (x_bits.is_pos() && x_abs >= 0x498cU) {
      // expm1(+inf) = +inf
      if (x_bits.is_inf())
        return FPBits::inf().get_val();

      switch (fputil::quick_get_round()) {
      case FE_TONEAREST:
      case FE_UPWARD:
        fputil::set_errno_if_required(ERANGE);
        fputil::raise_except_if_required(FE_OVERFLOW | FE_INEXACT);
        return FPBits::inf().get_val();
      default:
        return FPBits::max_normal().get_val();
      }
    }

    // When x <= -11 * log(2).
    if (x_u >= 0xc7a0U) {
      // expm1(-inf) = -1
      if (x_bits.is_inf())
        return FPBits::one(Sign::NEG).get_val();

      // When x > -0x1.0ap+3, round(expm1(x), HP, RN) = -1.
      if (x_u > 0xc828U)
        return fputil::round_result_slightly_up(
            FPBits::one(Sign::NEG).get_val());
      // When x <= -0x1.0ap+3, round(expm1(x), HP, RN) = -0x1.ffcp-1.
      return fputil::round_result_slightly_down(
          fputil::cast<float16>(-0x1.ffcp-1));
    }

    // When 0 < |x| <= 2^(-3).
    if (x_abs <= 0x3000U && !x_bits.is_zero()) {

#ifndef LIBC_MATH_HAS_SKIP_ACCURATE_PASS
      if (auto r = EXPM1F16_EXCEPTS_LO.lookup(x_u);
          LIBC_UNLIKELY(r.has_value()))
        return r.value();
#endif // !LIBC_MATH_HAS_SKIP_ACCURATE_PASS

      float xf = x;
      // Degree-5 minimax polynomial generated by Sollya with the following
      // commands:
      //   > display = hexadecimal;
      //   > P = fpminimax(expm1(x)/x, 4, [|SG...|], [-2^-3, 2^-3]);
      //   > x * P;
      return fputil::cast<float16>(
          xf * fputil::polyeval(xf, 0x1p+0f, 0x1.fffff8p-2f, 0x1.555556p-3f,
                                0x1.55905ep-5f, 0x1.1124c2p-7f));
    }
  }

#ifndef LIBC_MATH_HAS_SKIP_ACCURATE_PASS
  if (auto r = EXPM1F16_EXCEPTS_HI.lookup(x_u); LIBC_UNLIKELY(r.has_value()))
    return r.value();
#endif // !LIBC_MATH_HAS_SKIP_ACCURATE_PASS

  // exp(x) = exp(hi + mid) * exp(lo)
  auto [exp_hi_mid, exp_lo] = exp_range_reduction(x);
  // expm1(x) = exp(hi + mid) * exp(lo) - 1
  return fputil::cast<float16>(fputil::multiply_add(exp_hi_mid, exp_lo, -1.0f));
}

} // namespace LIBC_NAMESPACE_DECL
