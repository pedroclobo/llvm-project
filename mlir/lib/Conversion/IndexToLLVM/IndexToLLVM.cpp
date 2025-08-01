//===- IndexToLLVM.cpp - Index to LLVM dialect conversion -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "mlir/Conversion/IndexToLLVM/IndexToLLVM.h"

#include "mlir/Conversion/ConvertToLLVM/ToLLVMInterface.h"
#include "mlir/Conversion/LLVMCommon/Pattern.h"
#include "mlir/Dialect/Index/IR/IndexAttrs.h"
#include "mlir/Dialect/Index/IR/IndexDialect.h"
#include "mlir/Dialect/Index/IR/IndexOps.h"
#include "mlir/Dialect/LLVMIR/LLVMDialect.h"
#include "mlir/Pass/Pass.h"

using namespace mlir;
using namespace index;

namespace {

//===----------------------------------------------------------------------===//
// ConvertIndexCeilDivS
//===----------------------------------------------------------------------===//

/// Convert `ceildivs(n, m)` into `x = m > 0 ? -1 : 1` and then
/// `n*m > 0 ? (n+x)/m + 1 : -(-n/m)`.
struct ConvertIndexCeilDivS : mlir::ConvertOpToLLVMPattern<CeilDivSOp> {
  using ConvertOpToLLVMPattern::ConvertOpToLLVMPattern;

  LogicalResult
  matchAndRewrite(CeilDivSOp op, CeilDivSOpAdaptor adaptor,
                  ConversionPatternRewriter &rewriter) const override {
    Location loc = op.getLoc();
    Value n = adaptor.getLhs();
    Value m = adaptor.getRhs();
    Value zero = LLVM::ConstantOp::create(rewriter, loc, n.getType(), 0);
    Value posOne = LLVM::ConstantOp::create(rewriter, loc, n.getType(), 1);
    Value negOne = LLVM::ConstantOp::create(rewriter, loc, n.getType(), -1);

    // Compute `x`.
    Value mPos =
        LLVM::ICmpOp::create(rewriter, loc, LLVM::ICmpPredicate::sgt, m, zero);
    Value x = LLVM::SelectOp::create(rewriter, loc, mPos, negOne, posOne);

    // Compute the positive result.
    Value nPlusX = LLVM::AddOp::create(rewriter, loc, n, x);
    Value nPlusXDivM = LLVM::SDivOp::create(rewriter, loc, nPlusX, m);
    Value posRes = LLVM::AddOp::create(rewriter, loc, nPlusXDivM, posOne);

    // Compute the negative result.
    Value negN = LLVM::SubOp::create(rewriter, loc, zero, n);
    Value negNDivM = LLVM::SDivOp::create(rewriter, loc, negN, m);
    Value negRes = LLVM::SubOp::create(rewriter, loc, zero, negNDivM);

    // Pick the positive result if `n` and `m` have the same sign and `n` is
    // non-zero, i.e. `(n > 0) == (m > 0) && n != 0`.
    Value nPos =
        LLVM::ICmpOp::create(rewriter, loc, LLVM::ICmpPredicate::sgt, n, zero);
    Value sameSign = LLVM::ICmpOp::create(rewriter, loc,
                                          LLVM::ICmpPredicate::eq, nPos, mPos);
    Value nNonZero =
        LLVM::ICmpOp::create(rewriter, loc, LLVM::ICmpPredicate::ne, n, zero);
    Value cmp = LLVM::AndOp::create(rewriter, loc, sameSign, nNonZero);
    rewriter.replaceOpWithNewOp<LLVM::SelectOp>(op, cmp, posRes, negRes);
    return success();
  }
};

//===----------------------------------------------------------------------===//
// ConvertIndexCeilDivU
//===----------------------------------------------------------------------===//

/// Convert `ceildivu(n, m)` into `n == 0 ? 0 : (n-1)/m + 1`.
struct ConvertIndexCeilDivU : mlir::ConvertOpToLLVMPattern<CeilDivUOp> {
  using ConvertOpToLLVMPattern::ConvertOpToLLVMPattern;

  LogicalResult
  matchAndRewrite(CeilDivUOp op, CeilDivUOpAdaptor adaptor,
                  ConversionPatternRewriter &rewriter) const override {
    Location loc = op.getLoc();
    Value n = adaptor.getLhs();
    Value m = adaptor.getRhs();
    Value zero = LLVM::ConstantOp::create(rewriter, loc, n.getType(), 0);
    Value one = LLVM::ConstantOp::create(rewriter, loc, n.getType(), 1);

    // Compute the non-zero result.
    Value minusOne = LLVM::SubOp::create(rewriter, loc, n, one);
    Value quotient = LLVM::UDivOp::create(rewriter, loc, minusOne, m);
    Value plusOne = LLVM::AddOp::create(rewriter, loc, quotient, one);

    // Pick the result.
    Value cmp =
        LLVM::ICmpOp::create(rewriter, loc, LLVM::ICmpPredicate::eq, n, zero);
    rewriter.replaceOpWithNewOp<LLVM::SelectOp>(op, cmp, zero, plusOne);
    return success();
  }
};

//===----------------------------------------------------------------------===//
// ConvertIndexFloorDivS
//===----------------------------------------------------------------------===//

/// Convert `floordivs(n, m)` into `x = m < 0 ? 1 : -1` and then
/// `n*m < 0 ? -1 - (x-n)/m : n/m`.
struct ConvertIndexFloorDivS : mlir::ConvertOpToLLVMPattern<FloorDivSOp> {
  using ConvertOpToLLVMPattern::ConvertOpToLLVMPattern;

  LogicalResult
  matchAndRewrite(FloorDivSOp op, FloorDivSOpAdaptor adaptor,
                  ConversionPatternRewriter &rewriter) const override {
    Location loc = op.getLoc();
    Value n = adaptor.getLhs();
    Value m = adaptor.getRhs();
    Value zero = LLVM::ConstantOp::create(rewriter, loc, n.getType(), 0);
    Value posOne = LLVM::ConstantOp::create(rewriter, loc, n.getType(), 1);
    Value negOne = LLVM::ConstantOp::create(rewriter, loc, n.getType(), -1);

    // Compute `x`.
    Value mNeg =
        LLVM::ICmpOp::create(rewriter, loc, LLVM::ICmpPredicate::slt, m, zero);
    Value x = LLVM::SelectOp::create(rewriter, loc, mNeg, posOne, negOne);

    // Compute the negative result.
    Value xMinusN = LLVM::SubOp::create(rewriter, loc, x, n);
    Value xMinusNDivM = LLVM::SDivOp::create(rewriter, loc, xMinusN, m);
    Value negRes = LLVM::SubOp::create(rewriter, loc, negOne, xMinusNDivM);

    // Compute the positive result.
    Value posRes = LLVM::SDivOp::create(rewriter, loc, n, m);

    // Pick the negative result if `n` and `m` have different signs and `n` is
    // non-zero, i.e. `(n < 0) != (m < 0) && n != 0`.
    Value nNeg =
        LLVM::ICmpOp::create(rewriter, loc, LLVM::ICmpPredicate::slt, n, zero);
    Value diffSign = LLVM::ICmpOp::create(rewriter, loc,
                                          LLVM::ICmpPredicate::ne, nNeg, mNeg);
    Value nNonZero =
        LLVM::ICmpOp::create(rewriter, loc, LLVM::ICmpPredicate::ne, n, zero);
    Value cmp = LLVM::AndOp::create(rewriter, loc, diffSign, nNonZero);
    rewriter.replaceOpWithNewOp<LLVM::SelectOp>(op, cmp, negRes, posRes);
    return success();
  }
};

//===----------------------------------------------------------------------===//
// CovnertIndexCast
//===----------------------------------------------------------------------===//

/// Convert a cast op. If the materialized index type is the same as the other
/// type, fold away the op. Otherwise, truncate or extend the op as appropriate.
/// Signed casts sign extend when the result bitwidth is larger. Unsigned casts
/// zero extend when the result bitwidth is larger.
template <typename CastOp, typename ExtOp>
struct ConvertIndexCast : public mlir::ConvertOpToLLVMPattern<CastOp> {
  using mlir::ConvertOpToLLVMPattern<CastOp>::ConvertOpToLLVMPattern;

  LogicalResult
  matchAndRewrite(CastOp op, typename CastOp::Adaptor adaptor,
                  ConversionPatternRewriter &rewriter) const override {
    Type in = adaptor.getInput().getType();
    Type out = this->getTypeConverter()->convertType(op.getType());
    if (in == out)
      rewriter.replaceOp(op, adaptor.getInput());
    else if (in.getIntOrFloatBitWidth() > out.getIntOrFloatBitWidth())
      rewriter.replaceOpWithNewOp<LLVM::TruncOp>(op, out, adaptor.getInput());
    else
      rewriter.replaceOpWithNewOp<ExtOp>(op, out, adaptor.getInput());
    return success();
  }
};

using ConvertIndexCastS = ConvertIndexCast<CastSOp, LLVM::SExtOp>;
using ConvertIndexCastU = ConvertIndexCast<CastUOp, LLVM::ZExtOp>;

//===----------------------------------------------------------------------===//
// ConvertIndexCmp
//===----------------------------------------------------------------------===//

/// Assert that the LLVM comparison enum lines up with index's enum.
static constexpr bool checkPredicates(LLVM::ICmpPredicate lhs,
                                      IndexCmpPredicate rhs) {
  return static_cast<int>(lhs) == static_cast<int>(rhs);
}

static_assert(
    LLVM::getMaxEnumValForICmpPredicate() ==
            getMaxEnumValForIndexCmpPredicate() &&
        checkPredicates(LLVM::ICmpPredicate::eq, IndexCmpPredicate::EQ) &&
        checkPredicates(LLVM::ICmpPredicate::ne, IndexCmpPredicate::NE) &&
        checkPredicates(LLVM::ICmpPredicate::sge, IndexCmpPredicate::SGE) &&
        checkPredicates(LLVM::ICmpPredicate::sgt, IndexCmpPredicate::SGT) &&
        checkPredicates(LLVM::ICmpPredicate::sle, IndexCmpPredicate::SLE) &&
        checkPredicates(LLVM::ICmpPredicate::slt, IndexCmpPredicate::SLT) &&
        checkPredicates(LLVM::ICmpPredicate::uge, IndexCmpPredicate::UGE) &&
        checkPredicates(LLVM::ICmpPredicate::ugt, IndexCmpPredicate::UGT) &&
        checkPredicates(LLVM::ICmpPredicate::ule, IndexCmpPredicate::ULE) &&
        checkPredicates(LLVM::ICmpPredicate::ult, IndexCmpPredicate::ULT),
    "LLVM ICmpPredicate mismatches IndexCmpPredicate");

struct ConvertIndexCmp : public mlir::ConvertOpToLLVMPattern<CmpOp> {
  using ConvertOpToLLVMPattern::ConvertOpToLLVMPattern;

  LogicalResult
  matchAndRewrite(CmpOp op, CmpOpAdaptor adaptor,
                  ConversionPatternRewriter &rewriter) const override {
    // The LLVM enum has the same values as the index predicate enums.
    rewriter.replaceOpWithNewOp<LLVM::ICmpOp>(
        op, *LLVM::symbolizeICmpPredicate(static_cast<uint32_t>(op.getPred())),
        adaptor.getLhs(), adaptor.getRhs());
    return success();
  }
};

//===----------------------------------------------------------------------===//
// ConvertIndexSizeOf
//===----------------------------------------------------------------------===//

/// Lower `index.sizeof` to a constant with the value of the index bitwidth.
struct ConvertIndexSizeOf : public mlir::ConvertOpToLLVMPattern<SizeOfOp> {
  using ConvertOpToLLVMPattern::ConvertOpToLLVMPattern;

  LogicalResult
  matchAndRewrite(SizeOfOp op, SizeOfOpAdaptor adaptor,
                  ConversionPatternRewriter &rewriter) const override {
    rewriter.replaceOpWithNewOp<LLVM::ConstantOp>(
        op, getTypeConverter()->getIndexType(),
        getTypeConverter()->getIndexTypeBitwidth());
    return success();
  }
};

//===----------------------------------------------------------------------===//
// ConvertIndexConstant
//===----------------------------------------------------------------------===//

/// Convert an index constant. Truncate the value as appropriate.
struct ConvertIndexConstant : public mlir::ConvertOpToLLVMPattern<ConstantOp> {
  using ConvertOpToLLVMPattern::ConvertOpToLLVMPattern;

  LogicalResult
  matchAndRewrite(ConstantOp op, ConstantOpAdaptor adaptor,
                  ConversionPatternRewriter &rewriter) const override {
    Type type = getTypeConverter()->getIndexType();
    APInt value = op.getValue().trunc(type.getIntOrFloatBitWidth());
    rewriter.replaceOpWithNewOp<LLVM::ConstantOp>(
        op, type, IntegerAttr::get(type, value));
    return success();
  }
};

//===----------------------------------------------------------------------===//
// Trivial Conversions
//===----------------------------------------------------------------------===//

using ConvertIndexAdd = mlir::OneToOneConvertToLLVMPattern<AddOp, LLVM::AddOp>;
using ConvertIndexSub = mlir::OneToOneConvertToLLVMPattern<SubOp, LLVM::SubOp>;
using ConvertIndexMul = mlir::OneToOneConvertToLLVMPattern<MulOp, LLVM::MulOp>;
using ConvertIndexDivS =
    mlir::OneToOneConvertToLLVMPattern<DivSOp, LLVM::SDivOp>;
using ConvertIndexDivU =
    mlir::OneToOneConvertToLLVMPattern<DivUOp, LLVM::UDivOp>;
using ConvertIndexRemS =
    mlir::OneToOneConvertToLLVMPattern<RemSOp, LLVM::SRemOp>;
using ConvertIndexRemU =
    mlir::OneToOneConvertToLLVMPattern<RemUOp, LLVM::URemOp>;
using ConvertIndexMaxS =
    mlir::OneToOneConvertToLLVMPattern<MaxSOp, LLVM::SMaxOp>;
using ConvertIndexMaxU =
    mlir::OneToOneConvertToLLVMPattern<MaxUOp, LLVM::UMaxOp>;
using ConvertIndexMinS =
    mlir::OneToOneConvertToLLVMPattern<MinSOp, LLVM::SMinOp>;
using ConvertIndexMinU =
    mlir::OneToOneConvertToLLVMPattern<MinUOp, LLVM::UMinOp>;
using ConvertIndexShl = mlir::OneToOneConvertToLLVMPattern<ShlOp, LLVM::ShlOp>;
using ConvertIndexShrS =
    mlir::OneToOneConvertToLLVMPattern<ShrSOp, LLVM::AShrOp>;
using ConvertIndexShrU =
    mlir::OneToOneConvertToLLVMPattern<ShrUOp, LLVM::LShrOp>;
using ConvertIndexAnd = mlir::OneToOneConvertToLLVMPattern<AndOp, LLVM::AndOp>;
using ConvertIndexOr = mlir::OneToOneConvertToLLVMPattern<OrOp, LLVM::OrOp>;
using ConvertIndexXor = mlir::OneToOneConvertToLLVMPattern<XOrOp, LLVM::XOrOp>;
using ConvertIndexBoolConstant =
    mlir::OneToOneConvertToLLVMPattern<BoolConstantOp, LLVM::ConstantOp>;

} // namespace

//===----------------------------------------------------------------------===//
// Pattern Population
//===----------------------------------------------------------------------===//

void index::populateIndexToLLVMConversionPatterns(
    const LLVMTypeConverter &typeConverter, RewritePatternSet &patterns) {
  patterns.insert<
      // clang-format off
      ConvertIndexAdd,
      ConvertIndexSub,
      ConvertIndexMul,
      ConvertIndexDivS,
      ConvertIndexDivU,
      ConvertIndexRemS,
      ConvertIndexRemU,
      ConvertIndexMaxS,
      ConvertIndexMaxU,
      ConvertIndexMinS,
      ConvertIndexMinU,
      ConvertIndexShl,
      ConvertIndexShrS,
      ConvertIndexShrU,
      ConvertIndexAnd,
      ConvertIndexOr,
      ConvertIndexXor,
      ConvertIndexCeilDivS,
      ConvertIndexCeilDivU,
      ConvertIndexFloorDivS,
      ConvertIndexCastS,
      ConvertIndexCastU,
      ConvertIndexCmp,
      ConvertIndexSizeOf,
      ConvertIndexConstant,
      ConvertIndexBoolConstant
      // clang-format on
      >(typeConverter);
}

//===----------------------------------------------------------------------===//
// ODS-Generated Definitions
//===----------------------------------------------------------------------===//

namespace mlir {
#define GEN_PASS_DEF_CONVERTINDEXTOLLVMPASS
#include "mlir/Conversion/Passes.h.inc"
} // namespace mlir

//===----------------------------------------------------------------------===//
// Pass Definition
//===----------------------------------------------------------------------===//

namespace {
struct ConvertIndexToLLVMPass
    : public impl::ConvertIndexToLLVMPassBase<ConvertIndexToLLVMPass> {
  using Base::Base;

  void runOnOperation() override;
};
} // namespace

void ConvertIndexToLLVMPass::runOnOperation() {
  // Configure dialect conversion.
  ConversionTarget target(getContext());
  target.addIllegalDialect<IndexDialect>();
  target.addLegalDialect<LLVM::LLVMDialect>();

  // Set LLVM lowering options.
  LowerToLLVMOptions options(&getContext());
  if (indexBitwidth != kDeriveIndexBitwidthFromDataLayout)
    options.overrideIndexBitwidth(indexBitwidth);
  LLVMTypeConverter typeConverter(&getContext(), options);

  // Populate patterns and run the conversion.
  RewritePatternSet patterns(&getContext());
  populateIndexToLLVMConversionPatterns(typeConverter, patterns);

  if (failed(
          applyPartialConversion(getOperation(), target, std::move(patterns))))
    return signalPassFailure();
}

//===----------------------------------------------------------------------===//
// ConvertToLLVMPatternInterface implementation
//===----------------------------------------------------------------------===//

namespace {
/// Implement the interface to convert Index to LLVM.
struct IndexToLLVMDialectInterface : public ConvertToLLVMPatternInterface {
  using ConvertToLLVMPatternInterface::ConvertToLLVMPatternInterface;
  void loadDependentDialects(MLIRContext *context) const final {
    context->loadDialect<LLVM::LLVMDialect>();
  }

  /// Hook for derived dialect interface to provide conversion patterns
  /// and mark dialect legal for the conversion target.
  void populateConvertToLLVMConversionPatterns(
      ConversionTarget &target, LLVMTypeConverter &typeConverter,
      RewritePatternSet &patterns) const final {
    populateIndexToLLVMConversionPatterns(typeConverter, patterns);
  }
};
} // namespace

void mlir::index::registerConvertIndexToLLVMInterface(
    DialectRegistry &registry) {
  registry.addExtension(+[](MLIRContext *ctx, index::IndexDialect *dialect) {
    dialect->addInterfaces<IndexToLLVMDialectInterface>();
  });
}
