//===-- Unittests for sigaltstack -----------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "hdr/signal_macros.h"
#include "hdr/stdint_proxy.h"
#include "src/__support/OSUtil/syscall.h" // For internal syscall function.
#include "src/__support/libc_errno.h"
#include "src/signal/linux/signal_utils.h"
#include "src/signal/raise.h"
#include "src/signal/sigaction.h"
#include "src/signal/sigaltstack.h"
#include "test/UnitTest/ErrnoSetterMatcher.h"
#include "test/UnitTest/Test.h"

#include <sys/syscall.h>

constexpr int LOCAL_VAR_SIZE = 512;
constexpr int ALT_STACK_SIZE = SIGSTKSZ + LOCAL_VAR_SIZE * 2;
static uint8_t alt_stack[ALT_STACK_SIZE];

using LIBC_NAMESPACE::testing::ErrnoSetterMatcher::Fails;
using LIBC_NAMESPACE::testing::ErrnoSetterMatcher::Succeeds;

static bool good_stack;
static void handler(int) {
  // Allocate a large stack variable so that it does not get optimized
  // out or mapped to a register.
  uint8_t var[LOCAL_VAR_SIZE];
  for (int i = 0; i < LOCAL_VAR_SIZE; ++i)
    var[i] = i;
  // Verify that array is completely on the alt_stack.
  for (int i = 0; i < LOCAL_VAR_SIZE; ++i) {
    if (!(uintptr_t(var + i) < uintptr_t(alt_stack + ALT_STACK_SIZE) &&
          uintptr_t(alt_stack) <= uintptr_t(var + i))) {
      good_stack = false;
      return;
    }
  }
  good_stack = true;
}

TEST(LlvmLibcSignalTest, SigaltstackRunOnAltStack) {
  struct sigaction action;
  libc_errno = 0;
  ASSERT_THAT(LIBC_NAMESPACE::sigaction(SIGUSR1, nullptr, &action),
              Succeeds(0));
  action.sa_handler = handler;
  // Indicate that the signal should be delivered on an alternate stack.
  action.sa_flags = SA_ONSTACK;
  ASSERT_THAT(LIBC_NAMESPACE::sigaction(SIGUSR1, &action, nullptr),
              Succeeds(0));

  stack_t ss;
  ss.ss_sp = alt_stack;
  ss.ss_size = ALT_STACK_SIZE;
  ss.ss_flags = 0;
  // Setup the alternate stack.
  ASSERT_THAT(LIBC_NAMESPACE::sigaltstack(&ss, nullptr), Succeeds(0));

  good_stack = false;
  LIBC_NAMESPACE::raise(SIGUSR1);
  EXPECT_TRUE(good_stack);
}

// This tests for invalid input.
TEST(LlvmLibcSignalTest, SigaltstackInvalidStack) {
  stack_t ss;
  ss.ss_sp = alt_stack;
  ss.ss_size = 0;
  ss.ss_flags = SS_ONSTACK;
  ASSERT_THAT(LIBC_NAMESPACE::sigaltstack(&ss, nullptr), Fails(EINVAL));

  ss.ss_flags = 0;
  ASSERT_THAT(LIBC_NAMESPACE::sigaltstack(&ss, nullptr), Fails(ENOMEM));
}
