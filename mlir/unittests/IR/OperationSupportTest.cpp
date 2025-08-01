//===- OperationSupportTest.cpp - Operation support unit tests ------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "mlir/IR/OperationSupport.h"
#include "../../test/lib/Dialect/Test/TestDialect.h"
#include "../../test/lib/Dialect/Test/TestOps.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinTypes.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/Support/FormatVariadic.h"
#include "gtest/gtest.h"

using namespace mlir;
using namespace mlir::detail;

static Operation *createOp(MLIRContext *context, ArrayRef<Value> operands = {},
                           ArrayRef<Type> resultTypes = {},
                           unsigned int numRegions = 0) {
  context->allowUnregisteredDialects();
  return Operation::create(UnknownLoc::get(context),
                           OperationName("foo.bar", context), resultTypes,
                           operands, NamedAttrList(), nullptr, {}, numRegions);
}

namespace {
TEST(OperandStorageTest, NonResizable) {
  MLIRContext context;
  Builder builder(&context);

  Operation *useOp =
      createOp(&context, /*operands=*/{}, builder.getIntegerType(16));
  Value operand = useOp->getResult(0);

  // Create a non-resizable operation with one operand.
  Operation *user = createOp(&context, operand);

  // The same number of operands is okay.
  user->setOperands(operand);
  EXPECT_EQ(user->getNumOperands(), 1u);

  // Removing is okay.
  user->setOperands({});
  EXPECT_EQ(user->getNumOperands(), 0u);

  // Destroy the operations.
  user->destroy();
  useOp->destroy();
}

TEST(OperandStorageTest, Resizable) {
  MLIRContext context;
  Builder builder(&context);

  Operation *useOp =
      createOp(&context, /*operands=*/{}, builder.getIntegerType(16));
  Value operand = useOp->getResult(0);

  // Create a resizable operation with one operand.
  Operation *user = createOp(&context, operand);

  // The same number of operands is okay.
  user->setOperands(operand);
  EXPECT_EQ(user->getNumOperands(), 1u);

  // Removing is okay.
  user->setOperands({});
  EXPECT_EQ(user->getNumOperands(), 0u);

  // Adding more operands is okay.
  user->setOperands({operand, operand, operand});
  EXPECT_EQ(user->getNumOperands(), 3u);

  // Destroy the operations.
  user->destroy();
  useOp->destroy();
}

TEST(OperandStorageTest, RangeReplace) {
  MLIRContext context;
  Builder builder(&context);

  Operation *useOp =
      createOp(&context, /*operands=*/{}, builder.getIntegerType(16));
  Value operand = useOp->getResult(0);

  // Create a resizable operation with one operand.
  Operation *user = createOp(&context, operand);

  // Check setting with the same number of operands.
  user->setOperands(/*start=*/0, /*length=*/1, operand);
  EXPECT_EQ(user->getNumOperands(), 1u);

  // Check setting with more operands.
  user->setOperands(/*start=*/0, /*length=*/1, {operand, operand, operand});
  EXPECT_EQ(user->getNumOperands(), 3u);

  // Check setting with less operands.
  user->setOperands(/*start=*/1, /*length=*/2, {operand});
  EXPECT_EQ(user->getNumOperands(), 2u);

  // Check inserting without replacing operands.
  user->setOperands(/*start=*/2, /*length=*/0, {operand});
  EXPECT_EQ(user->getNumOperands(), 3u);

  // Check erasing operands.
  user->setOperands(/*start=*/0, /*length=*/3, {});
  EXPECT_EQ(user->getNumOperands(), 0u);

  // Destroy the operations.
  user->destroy();
  useOp->destroy();
}

TEST(OperandStorageTest, MutableRange) {
  MLIRContext context;
  Builder builder(&context);

  Operation *useOp =
      createOp(&context, /*operands=*/{}, builder.getIntegerType(16));
  Value operand = useOp->getResult(0);

  // Create a resizable operation with one operand.
  Operation *user = createOp(&context, operand);

  // Check setting with the same number of operands.
  MutableOperandRange mutableOperands(user);
  mutableOperands.assign(operand);
  EXPECT_EQ(mutableOperands.size(), 1u);
  EXPECT_EQ(user->getNumOperands(), 1u);

  // Check setting with more operands.
  mutableOperands.assign({operand, operand, operand});
  EXPECT_EQ(mutableOperands.size(), 3u);
  EXPECT_EQ(user->getNumOperands(), 3u);

  // Check with inserting a new operand.
  mutableOperands.append({operand, operand});
  EXPECT_EQ(mutableOperands.size(), 5u);
  EXPECT_EQ(user->getNumOperands(), 5u);

  // Check erasing operands.
  mutableOperands.clear();
  EXPECT_EQ(mutableOperands.size(), 0u);
  EXPECT_EQ(user->getNumOperands(), 0u);

  // Destroy the operations.
  user->destroy();
  useOp->destroy();
}

TEST(OperandStorageTest, RangeErase) {
  MLIRContext context;
  Builder builder(&context);

  Type type = builder.getNoneType();
  Operation *useOp = createOp(&context, /*operands=*/{}, {type, type});
  Value operand1 = useOp->getResult(0);
  Value operand2 = useOp->getResult(1);

  // Create an operation with operands to erase.
  Operation *user =
      createOp(&context, {operand2, operand1, operand2, operand1});
  BitVector eraseIndices(user->getNumOperands());

  // Check erasing no operands.
  user->eraseOperands(eraseIndices);
  EXPECT_EQ(user->getNumOperands(), 4u);

  // Check erasing disjoint operands.
  eraseIndices.set(0);
  eraseIndices.set(3);
  user->eraseOperands(eraseIndices);
  EXPECT_EQ(user->getNumOperands(), 2u);
  EXPECT_EQ(user->getOperand(0), operand1);
  EXPECT_EQ(user->getOperand(1), operand2);

  // Destroy the operations.
  user->destroy();
  useOp->destroy();
}

TEST(OperationOrderTest, OrderIsAlwaysValid) {
  MLIRContext context;
  Builder builder(&context);

  Operation *containerOp = createOp(&context, /*operands=*/{},
                                    /*resultTypes=*/{},
                                    /*numRegions=*/1);
  Region &region = containerOp->getRegion(0);
  Block *block = new Block();
  region.push_back(block);

  // Insert two operations, then iteratively add more operations in the middle
  // of them. Eventually we will insert more than kOrderStride operations and
  // the block order will need to be recomputed.
  Operation *frontOp = createOp(&context);
  Operation *backOp = createOp(&context);
  block->push_back(frontOp);
  block->push_back(backOp);

  // Chosen to be larger than Operation::kOrderStride.
  int kNumOpsToInsert = 10;
  for (int i = 0; i < kNumOpsToInsert; ++i) {
    Operation *op = createOp(&context);
    block->getOperations().insert(backOp->getIterator(), op);
    ASSERT_TRUE(op->isBeforeInBlock(backOp));
    // Note verifyOpOrder() returns false if the order is valid.
    ASSERT_FALSE(block->verifyOpOrder());
  }

  containerOp->destroy();
}

TEST(OperationFormatPrintTest, CanUseVariadicFormat) {
  MLIRContext context;
  Builder builder(&context);

  Operation *op = createOp(&context);

  std::string str = formatv("{0}", *op).str();
  ASSERT_STREQ(str.c_str(), "\"foo.bar\"() : () -> ()");

  op->destroy();
}

TEST(OperationFormatPrintTest, CanPrintNameAsPrefix) {
  MLIRContext context;
  Builder builder(&context);

  context.allowUnregisteredDialects();
  Operation *op = Operation::create(
      NameLoc::get(StringAttr::get(&context, "my_named_loc")),
      OperationName("t.op", &context), builder.getIntegerType(16), {},
      NamedAttrList(), nullptr, {}, 0);

  std::string str;
  OpPrintingFlags flags;
  flags.printNameLocAsPrefix(true);
  llvm::raw_string_ostream os(str);
  op->print(os, flags);
  ASSERT_STREQ(str.c_str(), "%my_named_loc = \"t.op\"() : () -> i16\n");

  op->destroy();
}

TEST(NamedAttrListTest, TestAppendAssign) {
  MLIRContext ctx;
  NamedAttrList attrs;
  Builder b(&ctx);

  attrs.append(b.getStringAttr("foo"), b.getStringAttr("bar"));
  attrs.append("baz", b.getStringAttr("boo"));

  {
    auto *it = attrs.begin();
    EXPECT_EQ(it->getName(), b.getStringAttr("foo"));
    EXPECT_EQ(it->getValue(), b.getStringAttr("bar"));
    ++it;
    EXPECT_EQ(it->getName(), b.getStringAttr("baz"));
    EXPECT_EQ(it->getValue(), b.getStringAttr("boo"));
  }

  attrs.append("foo", b.getStringAttr("zoo"));
  {
    auto dup = attrs.findDuplicate();
    ASSERT_TRUE(dup.has_value());
  }

  SmallVector<NamedAttribute> newAttrs = {
      b.getNamedAttr("foo", b.getStringAttr("f")),
      b.getNamedAttr("zoo", b.getStringAttr("z")),
  };
  attrs.assign(newAttrs);

  auto dup = attrs.findDuplicate();
  ASSERT_FALSE(dup.has_value());

  {
    auto *it = attrs.begin();
    EXPECT_EQ(it->getName(), b.getStringAttr("foo"));
    EXPECT_EQ(it->getValue(), b.getStringAttr("f"));
    ++it;
    EXPECT_EQ(it->getName(), b.getStringAttr("zoo"));
    EXPECT_EQ(it->getValue(), b.getStringAttr("z"));
  }

  attrs.assign({});
  ASSERT_TRUE(attrs.empty());
}

TEST(OperandStorageTest, PopulateDefaultAttrs) {
  MLIRContext context;
  context.getOrLoadDialect<test::TestDialect>();
  Builder builder(&context);

  OpBuilder b(&context);
  auto req1 = b.getI32IntegerAttr(10);
  auto req2 = b.getI32IntegerAttr(60);
  // Verify default attributes populated post op creation.
  Operation *op = test::OpAttrMatch1::create(b, b.getUnknownLoc(), req1,
                                             nullptr, nullptr, req2);
  auto opt = op->getInherentAttr("default_valued_attr");
  EXPECT_NE(opt, nullptr) << *op;

  op->destroy();
}

TEST(OperationEquivalenceTest, HashWorksWithFlags) {
  MLIRContext context;
  context.getOrLoadDialect<test::TestDialect>();
  OpBuilder b(&context);

  auto *op1 = createOp(&context);
  // `op1` has an unknown loc.
  auto *op2 = createOp(&context);
  op2->setLoc(NameLoc::get(StringAttr::get(&context, "foo")));
  auto getHash = [](Operation *op, OperationEquivalence::Flags flags) {
    return OperationEquivalence::computeHash(
        op, OperationEquivalence::ignoreHashValue,
        OperationEquivalence::ignoreHashValue, flags);
  };
  // Check ignore location.
  EXPECT_EQ(getHash(op1, OperationEquivalence::IgnoreLocations),
            getHash(op2, OperationEquivalence::IgnoreLocations));
  EXPECT_NE(getHash(op1, OperationEquivalence::None),
            getHash(op2, OperationEquivalence::None));
  op1->setLoc(NameLoc::get(StringAttr::get(&context, "foo")));
  // Check ignore discardable dictionary attributes.
  SmallVector<NamedAttribute> newAttrs = {
      b.getNamedAttr("foo", b.getStringAttr("f"))};
  op1->setAttrs(newAttrs);
  EXPECT_EQ(getHash(op1, OperationEquivalence::IgnoreDiscardableAttrs),
            getHash(op2, OperationEquivalence::IgnoreDiscardableAttrs));
  EXPECT_NE(getHash(op1, OperationEquivalence::None),
            getHash(op2, OperationEquivalence::None));
  op1->destroy();
  op2->destroy();

  // Check ignore properties.
  auto req1 = b.getI32IntegerAttr(10);
  Operation *opWithProperty1 = test::OpAttrMatch1::create(
      b, b.getUnknownLoc(), req1, nullptr, nullptr, req1);
  auto req2 = b.getI32IntegerAttr(60);
  Operation *opWithProperty2 = test::OpAttrMatch1::create(
      b, b.getUnknownLoc(), req2, nullptr, nullptr, req2);
  EXPECT_EQ(getHash(opWithProperty1, OperationEquivalence::IgnoreProperties),
            getHash(opWithProperty2, OperationEquivalence::IgnoreProperties));
  EXPECT_NE(getHash(opWithProperty1, OperationEquivalence::None),
            getHash(opWithProperty2, OperationEquivalence::None));
  opWithProperty1->destroy();
  opWithProperty2->destroy();
}

} // namespace
