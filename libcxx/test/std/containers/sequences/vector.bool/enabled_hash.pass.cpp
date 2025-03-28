//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <vector>

// Test that <vector> provides all of the arithmetic, enum, and pointer
// hash specializations.

#include <vector>

#include "poisoned_hash_helper.h"
#include "test_macros.h"
#include "min_allocator.h"

TEST_CONSTEXPR_CXX20 bool test() {
  test_hash_enabled<std::vector<bool> >();
  test_hash_enabled<std::vector<bool, min_allocator<bool>>>();

  return true;
}

int main(int, char**) {
  test_library_hash_specializations_available();
  test();
#if TEST_STD_VER > 17
  static_assert(test());
#endif

  return 0;
}
