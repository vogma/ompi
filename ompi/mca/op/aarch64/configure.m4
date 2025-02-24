# -*- shell-script -*-
#
# Copyright (c) 2019-2020 The University of Tennessee and The University
#                         of Tennessee Research Foundation.  All rights
#                         reserved.
# Copyright (c) 2024      NVIDIA Corporation.  All rights reserved.
#
# $COPYRIGHT$
#
# Additional copyrights may follow
#
# $HEADER$
#

# MCA_ompi_op_arm_CONFIG([action-if-can-compile],
#		         [action-if-cant-compile])
# ------------------------------------------------
AC_DEFUN([MCA_ompi_op_aarch64_CONFIG],[
    AC_CONFIG_FILES([ompi/mca/op/aarch64/Makefile])

    AS_VAR_PUSHDEF([op_sve_check], [ompi_cv_op_sve_check])
    op_sve_support=0
    op_sve_set_flags=0
    modified_cflags=$CFLAGS
    OPAL_VAR_SCOPE_PUSH([op_sve_cflags_save])
    MCA_BUILD_OP_SVE_FLAGS="$CFLAGS"

    case "${host}" in
        aarch64*|arm64*)
            op_aarch64_check="yes";;
        *)
            op_aarch64_check="no";;
    esac
    AS_IF([test "$op_aarch64_check" = "yes"],
          [AC_LANG_PUSH([C])

            AS_IF([echo "$CFLAGS" | grep -qv -- '\+sve'],
                [modified_cflags="`echo $CFLAGS | sed 's/\(-march=[[^ ]]*\)/\1+sve/'`"])

           #
           # Check for NEON support
           #
           AC_CACHE_CHECK([for NEON support], op_cv_neon_support, 
                [
                  AC_LINK_IFELSE(
                      [AC_LANG_PROGRAM([[
#if defined(__aarch64__) && defined(__ARM_NEON)
#include <arm_neon.h>
#else
#error "No support for __aarch64__"
#endif
                                       ]],
                                       [[
#if defined(__aarch64__) && defined(__ARM_NEON)
    int32x4_t vA;
    vA = vmovq_n_s32(0)
#endif
                                       ]])],
                      [op_cv_neon_support=yes],
                      [op_cv_neon_support=no])])

           #
           # Check for NEON FP support
           #
           AC_CACHE_CHECK([for NEON FP support], op_cv_neon_fp_support,
                [AS_IF([test "$op_cv_neon_support" = "yes"],
                        [
                          AC_LINK_IFELSE(
                              [AC_LANG_PROGRAM([[
#if defined(__aarch64__) && defined(__ARM_NEON) && (defined(__ARM_NEON_FP) || defined(__ARM_FP))
#include <arm_neon.h>
#else
#error "No support for __aarch64__ or NEON FP"
#endif
                                             ]],
                                             [[
#if defined(__aarch64__) && defined(__ARM_NEON) && (defined(__ARM_NEON_FP) || defined(__ARM_FP))
    float32x4_t vA;
    vA = vmovq_n_f32(0)
#endif
                                             ]])],
                            [op_cv_neon_fp_support=yes],
                            [op_cv_neon_fp_support=no])])])

           #
           # Check for SVE support
           #
           AC_CACHE_CHECK([for SVE support], op_sve_check, AS_VAR_SET(op_sve_check, yes))
                AS_IF([test $op_sve_support -eq 0],
                 [AC_MSG_CHECKING([for SVE support (no additional flags)])


                    AC_LINK_IFELSE(
                              [AC_LANG_PROGRAM([[
                                #if defined(__aarch64__) && defined(__ARM_FEATURE_SVE)
                                    #include <arm_sve.h>
                                #else
                                    #error "No support for __aarch64__ or SVE"
                                #endif
                                             ]],
                                             [[
                                #if defined(__aarch64__) && defined(_ARM_FEATURE_SVE)
                                    svfloat32_t vA;
                                    vA = svdup_n_f32(0)
                                #endif
                                             ]])],
                      [op_sve_support=1
                       AC_MSG_RESULT([yes])],
                      [AC_MSG_RESULT([no])])

                  AS_IF([test $op_sve_support -eq 0],
                        [AC_MSG_CHECKING([for SVE support (with +sve)])
                            op_sve_cflags_save="$CFLAGS"
                            CFLAGS="$modified_cflags"
                            AC_LINK_IFELSE(
                              [AC_LANG_PROGRAM([[
                                #if defined(__aarch64__) && defined(__ARM_FEATURE_SVE)
                                    #include <arm_sve.h>
                                #else
                                    #error "No support for __aarch64__ or SVE"
                                #endif
                                             ]],
                                             [[
                                #if defined(__aarch64__) && defined(_ARM_FEATURE_SVE)
                                    svfloat32_t vA;
                                    vA = svdup_n_f32(0)
                                #endif
                                             ]])],
                        [op_sve_support=1
                        op_sve_set_flags=1
                        MCA_BUILD_OP_SVE_FLAGS="$modified_cflags"
                        AC_MSG_RESULT([yes])],
                        [AC_MSG_RESULT([no])])
                         CFLAGS="$op_sve_cflags_save"
                        ])
                        ])
          ])

    AM_CONDITIONAL([MCA_BUILD_ompi_op_has_neon_support],
                   [test "$op_cv_neon_support" = "yes"])
    AM_CONDITIONAL([MCA_BUILD_ompi_op_has_neon_fp_support],
                   [test "$op_cv_neon_fp_support" = "yes"])
    AM_CONDITIONAL([MCA_BUILD_ompi_op_has_sve_support],
                   [test "$op_sve_support" = "yes"])
    AC_SUBST(MCA_BUILD_ompi_op_has_neon_support)
    AC_SUBST(MCA_BUILD_ompi_op_has_neon_fp_support)
    AC_SUBST(MCA_BUILD_ompi_op_has_sve_support)
    AC_SUBST(MCA_BUILD_OP_SVE_FLAGS)


    AS_IF([test "$op_cv_neon_support" = "yes"],
          [AC_DEFINE([OMPI_MCA_OP_HAVE_NEON], [1],[NEON supported in the current build])])
    AS_IF([test "$op_cv_neon_fp_support" = "yes"],
          [AC_DEFINE([OMPI_MCA_OP_HAVE_NEON_FP], [1],[NEON FP supported in the current build])])
    AS_IF([test "$op_cv_sve_support" = "yes"],
          [AC_DEFINE([OMPI_MCA_OP_HAVE_SVE], [1],[SVE supported in the current build])])

    # If we have at least support for Neon
    AS_IF([test "$op_cv_neon_support" = "yes"],
          [$1],
          [$2])

    AC_MSG_NOTICE([Value of MCA_BUILD_OP_SVE_FLAGS: $MCA_BUILD_OP_SVE_FLAGS])

])dnl
