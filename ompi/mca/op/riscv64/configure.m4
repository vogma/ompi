# -*- shell-script -*-
#
# $COPYRIGHT$
#
# Additional copyrights may follow
#
# $HEADER$
#

# MCA_ompi_op_riscv64_CONFIG([action-if-can-compile],
#		         [action-if-cant-compile])
# ------------------------------------------------
AC_DEFUN([MCA_ompi_op_riscv64_CONFIG],[
    AC_CONFIG_FILES([ompi/mca/op/riscv64/Makefile])

    MCA_BUILD_OP_RVV_FLAGS=""
    op_rvv_support=0

    OPAL_VAR_SCOPE_PUSH([op_rvv_cflags_save])

    case "${host}" in
        riscv*|riscv64*)
            op_rvv_check="yes";;
        *)
            op_rvv_check="no";;
    esac
            AS_IF([test "$op_rvv_check" = "yes"],
          [AC_LANG_PUSH([C])

           #
           # Check for RVV support
           #
           AS_IF([test "$op_rvv_check" = "yes"],
                 [AC_MSG_CHECKING([for RVV support (no additional flags)])
                  AC_LINK_IFELSE(
                             [AC_LANG_PROGRAM([[
#if defined(__linux__) && defined(__riscv_v_intrinsic) && defined(__riscv_v)
#include <riscv_vector.h>
#include <stdint.h>
#else
#error "Not a linux platform or no support for RISC-V vector extension"
#endif
                                       ]],
                                       [[
#if defined(__riscv) && defined(__riscv_v_intrinsic) && defined(__riscv_v)
    size_t vlmax_e8m1 = __riscv_vsetvlmax_e8m1();
    vint8m1_t offset_vec = __riscv_vmv_v_x_i8m1(0, vlmax_e8m1);
#endif
                                       ]])],
                             [op_rvv_support=1
                              AC_MSG_RESULT([yes])],
                             [AC_MSG_RESULT([no])])

                  AS_IF([test $op_rvv_support -eq 0],
                        [AC_MSG_CHECKING([for RVV support (with -march=rv64gcv)])
                         op_rvv_cflags_save="$CFLAGS"
                         CFLAGS="$CFLAGS -march=rv64gcv"
                          AC_LINK_IFELSE(
                             [AC_LANG_PROGRAM([[
#if defined(__linux__) && defined(__riscv_v_intrinsic) && defined(__riscv_v)
#include <riscv_vector.h>
#include <stdint.h>
#else
#error "Not a linux platform or no support for RISC-V vector extension"
#endif
                                       ]],
                                       [[
#if defined(__riscv) && defined(__riscv_v_intrinsic) && defined(__riscv_v)
    size_t vlmax_e8m1 = __riscv_vsetvlmax_e8m1();
    vint8m1_t offset_vec = __riscv_vmv_v_x_i8m1(0, vlmax_e8m1);
#endif
                                       ]])],
                             [op_rvv_support=1
                              MCA_BUILD_OP_RVV_FLAGS="-march=rv64gcv"
                              AC_MSG_RESULT([yes])],
                             [AC_MSG_RESULT([no])])
                         CFLAGS="$op_rvv_cflags_save"
                        ])
                               AC_LANG_POP([C])
            ])
])
    AM_CONDITIONAL([MCA_BUILD_ompi_op_has_rvv_support],
                   [test "$op_rvv_support" = "1"])

    AC_SUBST(MCA_BUILD_OP_RVV_FLAGS)
    AC_SUBST(MCA_BUILD_ompi_op_has_rvv_support)

    OPAL_VAR_SCOPE_POP

    AS_IF([test "$op_rvv_support" = "1"],
          [AC_DEFINE([OMPI_MCA_OP_HAVE_RVV], [1],[RVV supported in the current build])])


    # Enable this component if we have RISC-V Vector support
    AS_IF([test "$op_rvv_support" = "1"],
          [$1],
          [$2])
])dnl
