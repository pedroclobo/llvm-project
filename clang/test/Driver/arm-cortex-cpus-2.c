// NOTE: Continuation of arm-cortex-cpus-1.c. The tests are split across
// multiple files, to avoid excessive test times for large single
// test files.
// TODO: The files should be split up by categories, e.g. by architecture versions.
//
// Once we have CPUs with optional v8.2-A FP16, we will need a way to turn it
// on and off. Cortex-A53 is a placeholder for now.
// RUN: %clang -target armv8a-linux-eabi -mcpu=cortex-a53+fp16 -### -c %s 2>&1 | FileCheck --check-prefix CHECK-CORTEX-A53-FP16 %s
// RUN: %clang -target armv8a-linux-eabi -mcpu=cortex-a53+nofp16 -### -c %s 2>&1 | FileCheck --check-prefix CHECK-CORTEX-A53-NOFP16 %s
// CHECK-CORTEX-A53-FP16: "-cc1" {{.*}}"-target-cpu" "cortex-a53" {{.*}}"-target-feature" "+fullfp16"
// CHECK-CORTEX-A53-FP16-NOT: "-target-feature" "+fp16fml"
// CHECK-CORTEX-A53-NOFP16-NOT: "+fullfp16"
// CHECK-CORTEX-A53-NOFP16-NOT: "+fp16fml"

// RUN: %clang -target armv8a-linux-eabi -march=armv8-a -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-V8A-NOFP16FML %s
// CHECK-V8A-NOFP16FML-NOT: "-target-feature" "+fp16fml"
// CHECK-V8A-NOFP16FML-NOT: "-target-feature" "+fullfp16"

// RUN: %clang -target armv8a-linux-eabi -march=armv8-a+fp16 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-V8A-FP16 %s
// CHECK-V8A-FP16-NOT: "-target-feature" "+fp16fml"
// CHECK-V8A-FP16: "-target-feature" "+fullfp16"
// CHECK-V8A-FP16-NOT: "-target-feature" "+fp16fml"
// CHECK-V8A-FP16-SAME: {{$}}

// RUN: %clang -target armv8a-linux-eabi -march=armv8-a+fp16fml -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-V8A-FP16FML %s
// CHECK-V8A-FP16FML: "-target-feature" "+fp16fml" "-target-feature" "+fullfp16"

// RUN: %clang -target armv8a-linux-eabi -march=armv8.2-a -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-V82A-NOFP16FML %s
// CHECK-V82A-NOFP16FML-NOT: "-target-feature" "+fp16fml"
// CHECK-V82A-NOFP16FML-NOT: "-target-feature" "+fullfp16"

// RUN: %clang -target armv8a-linux-eabi -march=armv8.2-a+fp16 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-V82A-FP16 %s
// CHECK-V82A-FP16-NOT: "-target-feature" "+fp16fml"
// CHECK-V82A-FP16: "-target-feature" "+fullfp16"
// CHECK-V82A-FP16-NOT: "-target-feature" "+fp16fml"
// CHECK-V82A-FP16-SAME: {{$}}

// RUN: %clang -target armv8a-linux-eabi -march=armv8.2-a+fp16fml -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-V82A-FP16FML %s
// CHECK-V82A-FP16FML: "-target-feature" "+fp16fml" "-target-feature" "+fullfp16"

// RUN: %clang -target armv8a-linux-eabi -march=armv8.2-a+fp16+nofp16fml -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-V82A-FP16-NOFP16FML %s
// CHECK-V82A-FP16-NOFP16FML: "-target-feature" "+fullfp16" "-target-feature" "-fp16fml"

// RUN: %clang -target armv8a-linux-eabi -march=armv8.2-a+nofp16fml+fp16 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-V82A-NOFP16FML-FP16 %s
// CHECK-V82A-NOFP16FML-FP16: "-target-feature" "+fullfp16" "-target-feature" "-fp16fml"

// RUN: %clang -target armv8a-linux-eabi -march=armv8.2-a+fp16fml+nofp16 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-V82A-FP16FML-NOFP16 %s
// CHECK-V82A-FP16FML-NOFP16: "-target-feature" "-fullfp16" "-target-feature" "-fp16fml"

// RUN: %clang -target armv8a-linux-eabi -march=armv8.2-a+nofp16+fp16fml -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-V82A-NOFP16-FP16FML %s
// CHECK-V82A-NOFP16-FP16FML: "-target-feature" "+fp16fml" "-target-feature" "+fullfp16"

// RUN: %clang -target armv8a-linux-eabi -march=armv8.3-a -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-V83A-NOFP16FML %s
// CHECK-V83A-NOFP16FML-NOT: "-target-feature" "+fp16fml"
// CHECK-V83A-NOFP16FML-NOT: "-target-feature" "+fullfp16"

// RUN: %clang -target armv8a-linux-eabi -march=armv8.3-a+fp16 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-V83A-FP16 %s
// CHECK-V83A-FP16-NOT: "-target-feature" "+fp16fml"
// CHECK-V83A-FP16: "-target-feature" "+fullfp16"
// CHECK-V83A-FP16-NOT: "-target-feature" "+fp16fml"
// CHECK-V83A-FP16-SAME: {{$}}

// RUN: %clang -target armv8a-linux-eabi -march=armv8.3-a+fp16fml -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-V83A-FP16FML %s
// CHECK-V83A-FP16FML: "-target-feature" "+fp16fml" "-target-feature" "+fullfp16"

// RUN: %clang -target armv8a-linux-eabi -march=armv8.3-a+fp16+nofp16fml -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-V83A-FP16-NOFP16FML %s
// CHECK-V83A-FP16-NOFP16FML: "-target-feature" "+fullfp16" "-target-feature" "-fp16fml"

// RUN: %clang -target armv8a-linux-eabi -march=armv8.3-a+nofp16fml+fp16 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-V83A-NOFP16FML-FP16 %s
// CHECK-V83A-NOFP16FML-FP16: "-target-feature" "+fullfp16" "-target-feature" "-fp16fml"

// RUN: %clang -target armv8a-linux-eabi -march=armv8.3-a+fp16fml+nofp16 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-V83A-FP16FML-NOFP16 %s
// CHECK-V83A-FP16FML-NOFP16: "-target-feature" "-fullfp16" "-target-feature" "-fp16fml"

// RUN: %clang -target armv8a-linux-eabi -march=armv8.3-a+nofp16+fp16fml -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-V83A-NOFP16-FP16FML %s
// CHECK-V83A-NOFP16-FP16FML: "-target-feature" "+fp16fml" "-target-feature" "+fullfp16"

// RUN: %clang -target armv8a-linux-eabi -march=armv8.4-a -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-V84A-NOFP16FML %s
// CHECK-V84A-NOFP16FML-NOT: "-target-feature" "+fp16fml"
// CHECK-V84A-NOFP16FML-NOT: "-target-feature" "+fullfp16"

// RUN: %clang -target armv8a-linux-eabi -march=armv8.4-a+fp16 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-V84A-FP16 %s
// CHECK-V84A-FP16: "-target-feature" "+fullfp16" "-target-feature" "+fp16fml"

// RUN: %clang -target armv8a-linux-eabi -march=armv8.4-a+fp16fml -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-V84A-FP16FML %s
// CHECK-V84A-FP16FML: "-target-feature" "+fp16fml" "-target-feature" "+fullfp16"

// RUN: %clang -target armv8a-linux-eabi -march=armv8.4-a+fp16+nofp16fml -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-V84A-FP16-NOFP16FML %s
// CHECK-V84A-FP16-NOFP16FML: "-target-feature" "+fullfp16" "-target-feature" "-fp16fml"

// RUN: %clang -target armv8a-linux-eabi -march=armv8.4-a+nofp16fml+fp16 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-V84A-NOFP16FML-FP16 %s
// CHECK-V84A-NOFP16FML-FP16: "-target-feature" "+fullfp16" "-target-feature" "+fp16fml"

// RUN: %clang -target armv8a-linux-eabi -march=armv8.4-a+fp16fml+nofp16 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-V84A-FP16FML-NOFP16 %s
// CHECK-V84A-FP16FML-NOFP16: "-target-feature" "-fullfp16" "-target-feature" "-fp16fml"

// RUN: %clang -target armv8a-linux-eabi -march=armv8.4-a+nofp16+fp16fml -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-V84A-NOFP16-FP16FML %s
// CHECK-V84A-NOFP16-FP16FML: "-target-feature" "+fp16fml" "-target-feature" "+fullfp16"

// RUN: %clang -target armv8a-linux-eabi -march=armv8.5-a+fp16 -### -c %s 2>&1 | FileCheck --check-prefix CHECK-V85A-FP16 %s
// CHECK-V85A-FP16: "-cc1"{{.*}} "-triple" "armv8.5{{.*}}" "-target-cpu" "generic" {{.*}}"-target-feature" "+fullfp16"

// RUN: %clang -target armv8a-linux-eabi -march=armv8.6-a+bf16 -### -c %s 2>&1 | FileCheck --check-prefix CHECK-V86A-BF16 %s
// CHECK-V86A-BF16: "-cc1"{{.*}} "-triple" "armv8.6{{.*}}" "-target-cpu" "generic" {{.*}}"-target-feature" "+bf16"

// RUN: %clang -target arm -march=armv8.2-a+fp16 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-FULLFP16-SOFT %s
// RUN: %clang -target arm -march=armv8.2-a+fp16fml -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-FULLFP16-SOFT %s
// RUN: %clang -target arm -march=armv8.2-a+fp16+fp16fml -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-FULLFP16-SOFT %s
// RUN: %clang -target arm -march=armv8.2-a+fp16fml+fp16 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-FULLFP16-SOFT %s
// RUN: %clang -target arm -march=armv8.4-a+fp16 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-FULLFP16-SOFT %s
// RUN: %clang -target arm -march=armv8.4-a+fp16fml -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-FULLFP16-SOFT %s
// RUN: %clang -target arm -march=armv8.4-a+fp16+fp16fml -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-FULLFP16-SOFT %s
// RUN: %clang -target arm -march=armv8.4-a+fp16fml+fp16 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-FULLFP16-SOFT %s
// CHECK-FULLFP16-SOFT-NOT: "-target-feature" "+fullfp16"
// CHECK-FULLFP16-SOFT-NOT: "-target-feature" "+fp16fml"

// RAS is on by default for v8.2-a (this is handled in the backend), but can be
// optionally enabled for v8.0-a and v8.1-a. Cortex-A53 does not have RAS, but
// is used here to check that this option will work on any future CPUs that may
// have optional RAS.
// RUN: %clang -target armv8a -march=armv8-a+ras -### -c %s 2>&1 | FileCheck -check-prefix=RAS %s
// RUN: %clang -target armv8a -march=armv8.1-a+ras -### -c %s 2>&1 | FileCheck -check-prefix=RAS %s
// RUN: %clang -target armv8a -mcpu=cortex-a53+ras -### -c %s 2>&1 | FileCheck -check-prefix=RAS %s
// RAS: "-target-feature" "+ras"

// RUN: %clang -target armv8m.base %s -### -c 2>&1 | FileCheck %s --check-prefix=V8M_BASELINE
// RUN: %clang -target arm -march=armv8-m.base %s -### -c 2>&1 | FileCheck %s --check-prefix=V8M_BASELINE
// RUN: %clang -target arm -march=armv8m.base %s -### -c 2>&1 | FileCheck %s --check-prefix=V8M_BASELINE
// RUN: %clang -target armv8m.base -mbig-endian %s -### -c 2>&1 | FileCheck %s --check-prefix=EBV8M_BASELINE
// RUN: %clang -target arm -march=armv8-m.base -mbig-endian %s -### -c 2>&1 | FileCheck %s --check-prefix=EBV8M_BASELINE
// RUN: %clang -target arm -march=armv8m.base -mbig-endian %s -### -c 2>&1 | FileCheck %s --check-prefix=EBV8M_BASELINE
// V8M_BASELINE: "-cc1"{{.*}} "-triple" "thumbv8m.base-{{.*}} "-target-cpu" "generic"
// EBV8M_BASELINE: "-cc1"{{.*}} "-triple" "thumbebv8m.base-{{.*}} "-target-cpu" "generic"

// RUN: %clang -target armv8m.main %s -### -c 2>&1 | FileCheck %s --check-prefix=V8M_MAINLINE
// RUN: %clang -target arm -march=armv8-m.main %s -### -c 2>&1 | FileCheck %s --check-prefix=V8M_MAINLINE
// RUN: %clang -target arm -march=armv8m.main %s -### -c 2>&1 | FileCheck %s --check-prefix=V8M_MAINLINE
// RUN: %clang -target armv8m.main -mbig-endian %s -### -c 2>&1 | FileCheck %s --check-prefix=EBV8M_MAINLINE
// RUN: %clang -target arm -march=armv8-m.main -mbig-endian %s -### -c 2>&1 | FileCheck %s --check-prefix=EBV8M_MAINLINE
// RUN: %clang -target arm -march=armv8m.main -mbig-endian %s -### -c 2>&1 | FileCheck %s --check-prefix=EBV8M_MAINLINE
// V8M_MAINLINE: "-cc1"{{.*}} "-triple" "thumbv8m.main-{{.*}} "-target-cpu" "generic"
// EBV8M_MAINLINE: "-cc1"{{.*}} "-triple" "thumbebv8m.main-{{.*}} "-target-cpu" "generic"

// ================== Check that a bogus architecture gives an error
// RUN: not %clang --target=arm -march=armbogusv6 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BOGUS %s
// CHECK-BOGUS: error: unsupported argument 'armbogusv6' to option '-march='
// RUN: not %clang --target=arm---eabihf -march=armbogusv7 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BOGUS-HF %s
// CHECK-BOGUS-HF: error: unsupported argument 'armbogusv7' to option '-march='
// RUN: not %clang --target=arm -march=armv6bogus -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BOGUS2 %s
// CHECK-BOGUS2: error: unsupported argument 'armv6bogus' to option '-march='
// RUN: not %clang --target=arm -march=bogus -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BOGUS3 %s
// CHECK-BOGUS3: error: unsupported argument 'bogus' to option '-march='

// ================== Check that a bogus CPU gives an error
// RUN: not %clang -target arm -mcpu=bogus -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BOGUS-CPU %s
// RUN: not %clang -target armv8-apple-darwin -arch arm64 -mcpu=bogus -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BOGUS-CPU %s
// CHECK-BOGUS-CPU: error: unsupported argument 'bogus' to option '-mcpu='
// RUN: not %clang -target armv8-apple-darwin -arch arm64 -mtune=bogus -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BOGUS-TUNE %s
// CHECK-BOGUS-TUNE: error: unsupported argument 'bogus' to option '-mtune='

// ================== Check default Architecture on each ARM11 CPU
// RUN: %clang -target arm-linux-gnueabi -mcpu=arm1136j-s -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV6 %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=arm1136jf-s -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV6 %s
// CHECK-CPUV6: "-cc1"{{.*}} "-triple" "armv6-

// RUN: %clang -target arm-linux-gnueabi -mcpu=mpcore -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV6K %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=mpcorenovfp -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV6K %s
// CHECK-CPUV6K: "-cc1"{{.*}} "-triple" "armv6k-

// RUN: %clang -target arm-linux-gnueabi -mcpu=arm1176jz-s -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV6KZ %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=arm1176jzf-s -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV6KZ %s
// CHECK-CPUV6KZ: "-cc1"{{.*}} "-triple" "armv6kz-

// RUN: %clang -target arm-linux-gnueabi -mcpu=arm1156t2-s -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV6T2 %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=arm1156t2f-s -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV6T2 %s
// CHECK-CPUV6T2: "-cc1"{{.*}} "-triple" "armv6t2-

// ================== Check default Architecture on each Cortex CPU
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a5 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a7 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a8 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a9 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a12 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a15 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a17 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a5 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a7 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a8 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a9 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a12 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a15 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a17 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A %s
// CHECK-CPUV7A: "-cc1"{{.*}} "-triple" "armv7-

// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-a5 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A %s
// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-a7 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A %s
// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-a8 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A %s
// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-a9 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A %s
// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-a12 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A %s
// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-a15 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A %s
// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-a17 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a5 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a7 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a8 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a9 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a12 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a15 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a17 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A %s
// CHECK-BE-CPUV7A: "-cc1"{{.*}} "-triple" "armebv7-

// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a5 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a7 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a8 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a9 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a12 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a15 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a17 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a5 -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a7 -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a8 -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a9 -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a12 -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a15 -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a17 -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7A-THUMB %s
// CHECK-CPUV7A-THUMB: "-cc1"{{.*}} "-triple" "thumbv7-

// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-a5 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A-THUMB %s
// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-a7 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A-THUMB %s
// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-a8 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A-THUMB %s
// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-a9 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A-THUMB %s
// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-a12 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A-THUMB %s
// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-a15 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A-THUMB %s
// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-a17 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a5 -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a7 -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a8 -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a9 -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a12 -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a15 -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-a17 -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7A-THUMB %s
// CHECK-BE-CPUV7A-THUMB: "-cc1"{{.*}} "-triple" "thumbebv7-

// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-m0 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV6M %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-m0plus -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV6M %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-m1 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV6M %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=sc000 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV6M %s
// CHECK-CPUV6M: "-cc1"{{.*}} "-triple" "thumbv6m-

// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-m3 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7M %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-m3 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7M %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=sc300 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7M %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=sc300 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7M %s
// CHECK-CPUV7M: "-cc1"{{.*}} "-triple" "thumbv7m-

// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-m3 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7M %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-m3 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7M %s
// CHECK-BE-CPUV7M: "-cc1"{{.*}} "-triple" "thumbebv7m-{{.*}}

// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-m4 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7EM %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-m7 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7EM %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-m4 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7EM %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-m7 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7EM %s
// CHECK-CPUV7EM: "-cc1"{{.*}} "-triple" "thumbv7em-

// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-m4 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7EM %s
// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-m7 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7EM %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-m4 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7EM %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-m7 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7EM %s
// CHECK-BE-CPUV7EM: "-cc1"{{.*}} "-triple" "thumbebv7em-

// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r4 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7R %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r4f -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7R %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r5 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7R %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r7 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7R %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r8 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7R %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r4 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7R %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r4f -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7R %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r5 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7R %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r7 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7R %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r8 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7R %s
// CHECK-CPUV7R: "-cc1"{{.*}} "-triple" "armv7r-

// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-r4 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7R %s
// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-r4f -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7R %s
// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-r5 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7R %s
// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-r7 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7R %s
// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-r8 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7R %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r4 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7R %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r4f -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7R %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r5 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7R %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r7 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7R %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r8 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7R %s
// CHECK-BE-CPUV7R: "-cc1"{{.*}} "-triple" "armebv7r-

// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r4 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7R-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r4f -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7R-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r5 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7R-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r7 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7R-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r8 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7R-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r4 -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7R-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r4f -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7R-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r5 -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7R-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r7 -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7R-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r8 -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV7R-THUMB %s
// CHECK-CPUV7R-THUMB: "-cc1"{{.*}} "-triple" "thumbv7r-

// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-r4 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7R-THUMB %s
// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-r4f -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7R-THUMB %s
// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-r5 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7R-THUMB %s
// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-r7 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7R-THUMB %s
// RUN: %clang -target armeb-linux-gnueabi -mcpu=cortex-r8 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7R-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r4 -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7R-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r4f -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7R-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r5 -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7R-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r7 -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7R-THUMB %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r8 -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV7R-THUMB %s
// CHECK-BE-CPUV7R-THUMB: "-cc1"{{.*}} "-triple" "thumbebv7r-

// RUN: %clang -target arm -mcpu=cortex-a32 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A %s
// RUN: %clang -target arm -mcpu=cortex-a35 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A %s
// RUN: %clang -target arm -mcpu=cortex-a53 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A %s
// RUN: %clang -target arm -mcpu=cortex-a57 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A %s
// RUN: %clang -target arm -mcpu=cortex-a72 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A %s
// RUN: %clang -target arm -mcpu=cortex-a73 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A %s
// RUN: %clang -target arm -mcpu=cortex-a32 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A %s
// RUN: %clang -target arm -mcpu=cortex-a35 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A %s
// RUN: %clang -target arm -mcpu=cortex-a53 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A %s
// RUN: %clang -target arm -mcpu=cortex-a57 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A %s
// RUN: %clang -target arm -mcpu=cortex-a72 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A %s
// RUN: %clang -target arm -mcpu=cortex-a73 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A %s
//
// RUN: %clang -target arm -mcpu=exynos-m3 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A %s
// RUN: %clang -target arm -mcpu=exynos-m3 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A %s
// CHECK-CPUV8A: "-cc1"{{.*}} "-triple" "armv8a-

// RUN: %clang -target arm -mcpu=cortex-a55 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A %s
// RUN: %clang -target arm -mcpu=cortex-a75 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A %s
// RUN: %clang -target arm -mcpu=cortex-a76 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A %s
// RUN: %clang -target arm -mcpu=cortex-a76ae -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A %s
// RUN: %clang -target arm -mcpu=cortex-a77 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A %s
// RUN: %clang -target arm -mcpu=cortex-a55 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A %s
// RUN: %clang -target arm -mcpu=cortex-a75 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A %s
// RUN: %clang -target arm -mcpu=cortex-a76 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A %s
// RUN: %clang -target arm -mcpu=cortex-a76ae -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A %s
// RUN: %clang -target arm -mcpu=cortex-a77 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A %s
//
// RUN: %clang -target arm -mcpu=exynos-m4 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A %s
// RUN: %clang -target arm -mcpu=exynos-m4 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A %s
// RUN: %clang -target arm -mcpu=exynos-m5 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A %s
// RUN: %clang -target arm -mcpu=exynos-m5 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A %s
// CHECK-CPUV82A: "-cc1"{{.*}} "-triple" "armv8.2a-

// RUN: %clang -target armeb -mcpu=cortex-a32 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A %s
// RUN: %clang -target armeb -mcpu=cortex-a35 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A %s
// RUN: %clang -target armeb -mcpu=cortex-a53 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A %s
// RUN: %clang -target armeb -mcpu=cortex-a57 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A %s
// RUN: %clang -target armeb -mcpu=cortex-a72 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A %s
// RUN: %clang -target armeb -mcpu=cortex-a73 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A %s
// RUN: %clang -target arm -mcpu=cortex-a32 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A %s
// RUN: %clang -target arm -mcpu=cortex-a35 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A %s
// RUN: %clang -target arm -mcpu=cortex-a53 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A %s
// RUN: %clang -target arm -mcpu=cortex-a57 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A %s
// RUN: %clang -target arm -mcpu=cortex-a72 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A %s
// RUN: %clang -target arm -mcpu=cortex-a73 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A %s
//
// RUN: %clang -target armeb -mcpu=exynos-m3 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A %s
// RUN: %clang -target arm -mcpu=exynos-m3 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A %s
// CHECK-BE-CPUV8A: "-cc1"{{.*}} "-triple" "armebv8a-

// RUN: %clang -target armeb -mcpu=cortex-a55 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A %s
// RUN: %clang -target armeb -mcpu=cortex-a75 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A %s
// RUN: %clang -target armeb -mcpu=cortex-a76 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A %s
// RUN: %clang -target armeb -mcpu=cortex-a76ae -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A %s
// RUN: %clang -target armeb -mcpu=cortex-a77 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A %s
// RUN: %clang -target arm -mcpu=cortex-a55 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A %s
// RUN: %clang -target arm -mcpu=cortex-a75 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A %s
// RUN: %clang -target arm -mcpu=cortex-a76 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A %s
// RUN: %clang -target arm -mcpu=cortex-a76ae -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A %s
// RUN: %clang -target arm -mcpu=cortex-a77 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A %s
//
// RUN: %clang -target armeb -mcpu=exynos-m4 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A %s
// RUN: %clang -target arm -mcpu=exynos-m4 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A %s
// RUN: %clang -target armeb -mcpu=exynos-m5 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A %s
// RUN: %clang -target arm -mcpu=exynos-m5 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A %s
// CHECK-BE-CPUV82A: "-cc1"{{.*}} "-triple" "armebv8.2a-

// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-r52 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8R %s
// CHECK-CPUV8R: "-cc1"{{.*}} "-triple" "armv8r-

// RUN: %clang -target arm -mcpu=cortex-a32 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a35 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a53 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a57 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a72 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a73 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a32 -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a35 -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a53 -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a57 -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a72 -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a73 -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A-THUMB %s
//
// RUN: %clang -target arm -mcpu=exynos-m3 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A-THUMB %s
// RUN: %clang -target arm -mcpu=exynos-m3 -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8A-THUMB %s
// CHECK-CPUV8A-THUMB: "-cc1"{{.*}} "-triple" "thumbv8a-

// RUN: %clang -target arm -mcpu=cortex-a55 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a75 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a76 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a76ae -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a77 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a55 -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a75 -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a76 -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a76ae -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a77 -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A-THUMB %s
//
// RUN: %clang -target arm -mcpu=exynos-m4 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A-THUMB %s
// RUN: %clang -target arm -mcpu=exynos-m4 -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A-THUMB %s
// RUN: %clang -target arm -mcpu=exynos-m5 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A-THUMB %s
// RUN: %clang -target arm -mcpu=exynos-m5 -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV82A-THUMB %s
// CHECK-CPUV82A-THUMB: "-cc1"{{.*}} "-triple" "thumbv8.2a-

// RUN: %clang -target armeb -mcpu=cortex-a32 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A-THUMB %s
// RUN: %clang -target armeb -mcpu=cortex-a35 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A-THUMB %s
// RUN: %clang -target armeb -mcpu=cortex-a53 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A-THUMB %s
// RUN: %clang -target armeb -mcpu=cortex-a57 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A-THUMB %s
// RUN: %clang -target armeb -mcpu=cortex-a72 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A-THUMB %s
// RUN: %clang -target armeb -mcpu=cortex-a73 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a32 -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a35 -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a53 -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a57 -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a72 -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a73 -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A-THUMB %s
//
// RUN: %clang -target armeb -mcpu=exynos-m3 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A-THUMB %s
// RUN: %clang -target arm -mcpu=exynos-m3 -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV8A-THUMB %s
// CHECK-BE-CPUV8A-THUMB: "-cc1"{{.*}} "-triple" "thumbebv8a-

// RUN: %clang -target armeb -mcpu=cortex-a55 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A-THUMB %s
// RUN: %clang -target armeb -mcpu=cortex-a75 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A-THUMB %s
// RUN: %clang -target armeb -mcpu=cortex-a76 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A-THUMB %s
// RUN: %clang -target armeb -mcpu=cortex-a76ae -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A-THUMB %s
// RUN: %clang -target armeb -mcpu=cortex-a77 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a55 -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a75 -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a76 -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a76ae -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A-THUMB %s
// RUN: %clang -target arm -mcpu=cortex-a77 -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A-THUMB %s
//
// RUN: %clang -target armeb -mcpu=exynos-m4 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A-THUMB %s
// RUN: %clang -target arm -mcpu=exynos-m4 -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A-THUMB %s
// RUN: %clang -target armeb -mcpu=exynos-m5 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A-THUMB %s
// RUN: %clang -target arm -mcpu=exynos-m5 -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV82A-THUMB %s
// CHECK-BE-CPUV82A-THUMB: "-cc1"{{.*}} "-triple" "thumbebv8.2a-

// RUN: %clang -target armv8a-arm-none-eabi -mcpu=cortex-a73 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-A73 %s
// RUN: %clang -target armv8a-arm-none-eabi -mcpu=cortex-a73 -mfpu=crypto-neon-fp-armv8 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-A73-MFPU %s
// RUN: %clang -target armv8a-arm-none-eabi -mcpu=cortex-a73 -mfloat-abi=soft -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-A73-SOFT %s
// CHECK-CORTEX-A73: "-cc1"{{.*}} "-triple" "armv8a-{{.*}} "-target-cpu" "cortex-a73"
// CHECK-CORTEX-A73-MFPU: "-cc1"{{.*}} "-target-feature" "+fp-armv8"
// CHECK-CORTEX-A73-MFPU: "-target-feature" "+sha2"
// CHECK-CORTEX-A73-MFPU: "-target-feature" "+aes"
// CHECK-CORTEX-A73-SOFT: "-target-feature" "+soft-float"
// CHECK-CORTEX-A73-SOFT: "-target-feature" "+soft-float-abi"

// RUN: %clang -target armv8a-arm-none-eabi -mcpu=cortex-a75 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-A75 %s
// RUN: %clang -target armv8a-arm-none-eabi -mcpu=cortex-a75 -mfpu=crypto-neon-fp-armv8 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-A75-MFPU %s
// RUN: %clang -target armv8a-arm-none-eabi -mcpu=cortex-a75 -mfloat-abi=soft -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-A75-SOFT %s
// CHECK-CORTEX-A75: "-cc1"{{.*}} "-triple" "armv8.2a-{{.*}} "-target-cpu" "cortex-a75"
// CHECK-CORTEX-A75-MFPU: "-cc1"{{.*}} "-target-feature" "+fp-armv8"
// CHECK-CORTEX-A75-MFPU: "-target-feature" "+sha2"
// CHECK-CORTEX-A75-MFPU: "-target-feature" "+aes"
// CHECK-CORTEX-A75-SOFT: "-target-feature" "+soft-float"
// CHECK-CORTEX-A75-SOFT: "-target-feature" "+soft-float-abi"

// RUN: %clang -target armv8a-arm-none-eabi -mcpu=cortex-a76 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-A76 %s
// RUN: %clang -target armv8a-arm-none-eabi -mcpu=cortex-a76 -mfpu=crypto-neon-fp-armv8 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-A76-MFPU %s
// RUN: %clang -target armv8a-arm-none-eabi -mcpu=cortex-a76 -mfloat-abi=soft -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-A76-SOFT %s
// CHECK-CORTEX-A76: "-cc1"{{.*}} "-triple" "armv8.2a-{{.*}} "-target-cpu" "cortex-a76"
// CHECK-CORTEX-A76-MFPU: "-cc1"{{.*}} "-target-feature" "+fp-armv8"
// CHECK-CORTEX-A76-MFPU: "-target-feature" "+sha2"
// CHECK-CORTEX-A76-MFPU: -target-feature" "+aes"
// CHECK-CORTEX-A76-SOFT: "-target-feature" "+soft-float"
// CHECK-CORTEX-A76-SOFT: "-target-feature" "+soft-float-abi"

// RUN: %clang -target armv8a-arm-none-eabi -mcpu=cortex-a76ae -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-A76AE %s
// RUN: %clang -target armv8a-arm-none-eabi -mcpu=cortex-a76ae -mfpu=crypto-neon-fp-armv8 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-A76AE-MFPU %s
// RUN: %clang -target armv8a-arm-none-eabi -mcpu=cortex-a76ae -mfloat-abi=soft -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-A76AE-SOFT %s
// CHECK-CORTEX-A76AE: "-cc1"{{.*}} "-triple" "armv8.2a-{{.*}} "-target-cpu" "cortex-a76ae"
// CHECK-CORTEX-A76AE-MFPU: "-cc1"{{.*}} "-target-feature" "+fp-armv8"
// CHECK-CORTEX-A76AE-MFPU: "-target-feature" "+sha2"
// CHECK-CORTEX-A76AE-MFPU: -target-feature" "+aes"
// CHECK-CORTEX-A76AE-SOFT: "-target-feature" "+soft-float"
// CHECK-CORTEX-A76AE-SOFT: "-target-feature" "+soft-float-abi"

// RUN: %clang -target arm -mcpu=neoverse-v1 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV84A %s
// RUN: %clang -target arm -mcpu=neoverse-v1 -mlittle-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV84A %s
// CHECK-CPUV84A: "-cc1"{{.*}} "-triple" "armv8.4a-{{.*}}

// RUN: %clang -target armeb -mcpu=neoverse-v1 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV84A %s
// RUN: %clang -target arm -mcpu=neoverse-v1 -mbig-endian -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV84A %s
// CHECK-BE-CPUV84A: "-cc1"{{.*}} "-triple" "armebv8.4a-{{.*}}

// RUN: %clang -target arm -mcpu=neoverse-v1 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV84A-THUMB %s
// RUN: %clang -target arm -mcpu=neoverse-v1 -mlittle-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV84A-THUMB %s
// CHECK-CPUV84A-THUMB: "-cc1"{{.*}} "-triple" "thumbv8.4a-{{.*}}

// RUN: %clang -target armeb -mcpu=neoverse-v1 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV84A-THUMB %s
// RUN: %clang -target arm -mcpu=neoverse-v1 -mbig-endian -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-BE-CPUV84A-THUMB %s
// CHECK-BE-CPUV84A-THUMB: "-cc1"{{.*}} "-triple" "thumbebv8.4a-{{.*}}

// RUN: %clang -target armv8a-arm-none-eabi -mcpu=cortex-x1 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-X1 %s
// RUN: %clang -target armv8a-arm-none-eabi -mcpu=cortex-x1 -mfpu=crypto-neon-fp-armv8 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-X1-MFPU %s
// CHECK-CORTEX-X1: "-cc1"{{.*}} "-triple" "armv8.2a-{{.*}} "-target-cpu" "cortex-x1"
// CHECK-CORTEX-X1-MFPU: "-cc1"{{.*}} "-target-feature" "+fp-armv8"
// CHECK-CORTEX-X1-MFPU: "-target-feature" "+sha2"
// CHECK-CORTEX-X1-MFPU: "-target-feature" "+aes"

// RUN: %clang -target armv8a-arm-none-eabi -mcpu=cortex-x1c -mfpu=crypto-neon-fp-armv8 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-CORTEX-X1C-MFPU %s
// CHECK-CORTEX-CORTEX-X1C: "-cc1"{{.*}} "-triple" "armv8.2a-{{.*}} "-target-cpu" "cortex-x1c"
// CHECK-CORTEX-CORTEX-X1C-MFPU: "-cc1"{{.*}} "-target-feature" "+fp-armv8"
// CHECK-CORTEX-CORTEX-X1C-MFPU: "-target-feature" "+crypto"

// RUN: %clang -target armv8a-arm-none-eabi -mcpu=cortex-a78 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-A78 %s
// RUN: %clang -target armv8a-arm-none-eabi -mcpu=cortex-a78 -mfpu=crypto-neon-fp-armv8 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-A78-MFPU %s
// CHECK-CORTEX-A78: "-cc1"{{.*}} "-triple" "armv8.2a-{{.*}} "-target-cpu" "cortex-a78"
// CHECK-CORTEX-A78-MFPU: "-cc1"{{.*}} "-target-feature" "+fp-armv8"
// CHECK-CORTEX-A78-MFPU: "-target-feature" "+sha2"
// CHECK-CORTEX-A78-MFPU: "-target-feature" "+aes"

// RUN: %clang -target armv8a-arm-none-eabi -mcpu=cortex-a78c -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-A78C %s
// RUN: %clang -target armv8a-arm-none-eabi -mcpu=cortex-a78c -mfpu=crypto-neon-fp-armv8 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-A78C-MFPU %s
// CHECK-CORTEX-A78C: "-cc1"{{.*}} "-triple" "armv8.2a-{{.*}} "-target-cpu" "cortex-a78c"
// CHECK-CORTEX-A78C-MFPU: "-cc1"{{.*}} "-target-feature" "+fp-armv8"
// CHECK-CORTEX-A78C-MFPU: "-target-feature" "+sha2"
// CHECK-CORTEX-A78C-MFPU: "-target-feature" "+aes"

// RUN: %clang -target armv8a-arm-none-eabi -mcpu=cortex-a78ae -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-A78AE %s
// RUN: %clang -target armv8a-arm-none-eabi -mcpu=cortex-a78ae -mfpu=crypto-neon-fp-armv8 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-A78AE-MFPU %s
// CHECK-CORTEX-A78AE: "-cc1"{{.*}} "-triple" "armv8.2a-{{.*}} "-target-cpu" "cortex-a78ae"
// CHECK-CORTEX-A78AE-MFPU: "-cc1"{{.*}} "-target-feature" "+fp-armv8"
// CHECK-CORTEX-A78AE-MFPU: "-target-feature" "+sha2"
// CHECK-CORTEX-A78AE-MFPU: "-target-feature" "+aes"

// RUN: %clang -target armv8a-arm-none-eabi -mcpu=cortex-a510 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-A510 %s
// RUN: %clang -target armv8a-arm-none-eabi -mcpu=cortex-a510 -mfpu=crypto-neon-fp-armv8 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-A510-MFPU %s
// CHECK-CORTEX-A510: "-cc1"{{.*}} "-triple" "armv9a-{{.*}} "-target-cpu" "cortex-a510"
// CHECK-CORTEX-A510-NOT: "-target-feature" "{{[+-]}}sm4"
// CHECK-CORTEX-A510-NOT: "-target-feature" "{{[+-]}}sha3"
// CHECK-CORTEX-A510: "-target-feature" "-aes"
// CHECK-CORTEX-A510-SAME: {{$}}
// CHECK-CORTEX-A510-MFPU: "-cc1"{{.*}} "-target-feature" "+fp-armv8"
// CHECK-CORTEX-A510-MFPU: "-target-feature" "+sha2"
// CHECK-CORTEX-A510-MFPU: "-target-feature" "+aes"

// RUN: %clang -target armv8a-arm-none-eabi -mcpu=cortex-a710 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-A710 %s
// RUN: %clang -target armv8a-arm-none-eabi -mcpu=cortex-a710 -mfpu=crypto-neon-fp-armv8 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-A710-MFPU %s
// CHECK-CORTEX-A710: "-cc1"{{.*}} "-triple" "armv9a-{{.*}} "-target-cpu" "cortex-a710"
// CHECK-CORTEX-A710-NOT: "-target-feature" "{{[+-]}}sm4"
// CHECK-CORTEX-A710-NOT: "-target-feature" "{{[+-]}}sha3"
// CHECK-CORTEX-A710: "-target-feature" "-aes"
// CHECK-CORTEX-A710-SAME: {{$}}
// CHECK-CORTEX-A710-MFPU: "-cc1"{{.*}} "-target-feature" "+fp-armv8"
// CHECK-CORTEX-A710-MFPU: "-target-feature" "+sha2"
// CHECK-CORTEX-A710-MFPU: "-target-feature" "+aes"

// RUN: %clang -target arm -mcpu=cortex-m23 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CPUV8MBASE %s
// CHECK-CPUV8MBASE:  "-cc1"{{.*}} "-triple" "thumbv8m.base-

// RUN: %clang -target arm -mcpu=cortex-m33 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-M33 %s
// RUN: %clang -target arm -mcpu=star-mc1 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-STAR-MC1 %s
// RUN: %clang -target arm -mcpu=cortex-m35p -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-M35P %s
// CHECK-CORTEX-M33:  "-cc1"{{.*}} "-triple" "thumbv8m.main-{{.*}} "-target-cpu" "cortex-m33"
// CHECK-STAR-MC1:  "-cc1"{{.*}} "-triple" "thumbv8m.main-{{.*}} "-target-cpu" "star-mc1"
// CHECK-CORTEX-M35P:  "-cc1"{{.*}} "-triple" "thumbv8m.main-{{.*}} "-target-cpu" "cortex-m35p"

// RUN: %clang -target arm -mcpu=cortex-m55 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-M55 %s
// CHECK-CORTEX-M55:  "-cc1"{{.*}} "-triple" "thumbv8.1m.main-{{.*}} "-target-cpu" "cortex-m55"

// RUN: %clang -target arm -mcpu=cortex-m85 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-M85 %s
// CHECK-CORTEX-M85:  "-cc1"{{.*}} "-triple" "thumbv8.1m.main-{{.*}} "-target-cpu" "cortex-m85"

// RUN: %clang -target arm -mcpu=cortex-m52 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CORTEX-M52 %s
// CHECK-CORTEX-M52:  "-cc1"{{.*}} "-triple" "thumbv8.1m.main-{{.*}} "-target-cpu" "cortex-m52"

// RUN: %clang -target arm -mcpu=neoverse-n2 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-NEOVERSE-N2 %s
// CHECK-NEOVERSE-N2: "-cc1"{{.*}} "-triple" "armv9a-{{.*}}" "-target-cpu" "neoverse-n2"

// ================== Check whether -mcpu accepts mixed-case values.
// RUN: %clang -target arm-linux-gnueabi -mcpu=Cortex-a5 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CASE-INSENSITIVE-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=cortex-A7 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CASE-INSENSITIVE-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=CORTEX-a8 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CASE-INSENSITIVE-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=Cortex-A9 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CASE-INSENSITIVE-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=corteX-A12 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CASE-INSENSITIVE-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=CorteX-a15 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CASE-INSENSITIVE-CPUV7A %s
// RUN: %clang -target arm-linux-gnueabi -mcpu=CorteX-A17 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CASE-INSENSITIVE-CPUV7A %s
// CHECK-CASE-INSENSITIVE-CPUV7A: "-cc1"{{.*}} "-triple" "armv7-

// ================== Check whether -march accepts mixed-case values.
// RUN: %clang -target arm -march=Armv5 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CASE-INSENSITIVE-V5 %s
// RUN: %clang -target arm -march=ARMV5 -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CASE-INSENSITIVE-V5 %s
// CHECK-CASE-INSENSITIVE-V5: "-cc1"{{.*}} "-triple" "armv5-{{.*}} "-target-cpu" "arm10tdmi"

// RUN: %clang -target arm -march=Armv6t2 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CASE-INSENSITIVE-V6T2-THUMB %s
// RUN: %clang -target arm -march=ARMV6T2 -mthumb -### -c %s 2>&1 | FileCheck -check-prefix=CHECK-CASE-INSENSITIVE-V6T2-THUMB %s
// CHECK-CASE-INSENSITIVE-V6T2-THUMB: "-cc1"{{.*}} "-triple" "thumbv6t2-{{.*}} "-target-cpu" "arm1156t2-s"

// ================== Check that the correct PROCESSOR features are added when used -mcpu=PROCESSOR+FEATURESLIST
// RUN: %clang -### --target=arm-arm-none-eabi -march=armv7-a -mcpu=cortex-a8+nocrc -c %s 2>&1 | FileCheck -check-prefix=A8FEATURES %s
// A8FEATURES: "-target-feature" "+dsp"
