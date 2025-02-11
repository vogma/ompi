/*
 *
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include "ompi_config.h"

#ifdef HAVE_SYS_TYPES_H
#    include <sys/types.h>
#endif
#include "opal/util/output.h"

#include "stdio.h"

#include "ompi/mca/op/base/base.h"
#include "ompi/mca/op/op.h"
#include "ompi/mca/op/riscv64/op_riscv64.h"
#include "ompi/op/op.h"

#if defined(GENERATE_RVV_CODE)
#    include <riscv_vector.h>
#    define OMPI_OP_TYPE_PREPEND v
#    define OMPI_OP_OP_PREPEND   __riscv_
typedef float float32_t;
typedef double float64_t;
#else
#    error we should not reach this
#endif /* GENERATE_RVV_CODE */

/*
 * Concatenate preprocessor tokens A and B without expanding macro definitions
 * (however, if invoked from a macro, macro arguments are expanded).
 */
#define OP_CONCAT_NX(A, B) A##B

/*
 * Concatenate preprocessor tokens A and B after macro-expanding them.
 */
#define OP_CONCAT(A, B) OP_CONCAT_NX(A, B)

/*
 * Since all the functions in this file are essentially identical, we
 * use a macro to substitute in names and types.  The core operation
 * in all functions that use this macro is the same.
 *
 * This macro is for (out op in).
 *
 */
#if defined(GENERATE_RVV_CODE)
#    define OP_RVV_FUNC(name, type_name, type_size, type_cnt, type, op, lmul)                      \
        static void ompi_op_rvv_2buff_##name##_##type##type_size##_t(                              \
            const void *_in, void *_out, int *count, struct ompi_datatype_t **dtype,               \
            struct ompi_op_base_module_1_0_0_t *module)                                            \
        {                                                                                          \
            int cnt = *count;                                                                      \
                                                                                                   \
            type##type_size##_t *in = (type##type_size##_t *) _in;                                 \
            type##type_size##_t *out = (type##type_size##_t *) _out;                               \
                                                                                                   \
            int in_offset = 0;                                                                     \
            int out_offset = 0;                                                                    \
                                                                                                   \
            OP_CONCAT(OMPI_OP_TYPE_PREPEND, type##type_size####lmul##_t)                           \
            vec_src, vec_dst;                                                                      \
                                                                                                   \
            for (int vl; cnt > 0; cnt -= vl, in_offset += vl, out_offset += vl) {                  \
                vl = OP_CONCAT(OMPI_OP_OP_PREPEND, vsetvl_e##type_size####lmul)(cnt);              \
                                                                                                   \
                vec_src = OP_CONCAT(OMPI_OP_OP_PREPEND,                                            \
                                    vle##type_size##_v_##type_name####type_size####lmul)(          \
                    &in[in_offset], vl);                                                           \
                vec_dst = OP_CONCAT(OMPI_OP_OP_PREPEND,                                            \
                                    vle##type_size##_v_##type_name####type_size####lmul)(          \
                    &out[out_offset], vl);                                                         \
                                                                                                   \
                vec_dst = OP_CONCAT(OMPI_OP_OP_PREPEND,                                            \
                                    v##op##_vv_##type_name####type_size####lmul)(vec_src, vec_dst, \
                                                                                 vl);              \
                                                                                                   \
                OP_CONCAT(OMPI_OP_OP_PREPEND, vse##type_size##_v_##type_name####type_size####lmul) \
                (&out[out_offset], vec_dst, vl);                                                   \
            }                                                                                      \
        }
#endif /*GENERATE_RVV_CODE*/

/*************************************************************************
 * Max
 *************************************************************************/
#undef current_func
#define current_func(a, b) ((a) > (b) ? (a) : (b))
OP_RVV_FUNC(max, i, 8, 16, int, max, m4)
OP_RVV_FUNC(max, u, 8, 16, uint, maxu, m4)
OP_RVV_FUNC(max, i, 16, 8, int, max, m4)
OP_RVV_FUNC(max, u, 16, 8, uint, maxu, m4)
OP_RVV_FUNC(max, i, 32, 4, int, max, m4)
OP_RVV_FUNC(max, u, 32, 4, uint, maxu, m4)
OP_RVV_FUNC(max, i, 64, 2, int, max, m4)
OP_RVV_FUNC(max, u, 64, 2, uint, maxu, m4)

OP_RVV_FUNC(max, f, 32, 4, float, fmax, m4)
OP_RVV_FUNC(max, f, 64, 2, float, fmax, m4)

/*************************************************************************
 * Min
 *************************************************************************/
#undef current_func
#define current_func(a, b) ((a) < (b) ? (a) : (b))
OP_RVV_FUNC(min, i, 8, 16, int, min, m4)
OP_RVV_FUNC(min, u, 8, 16, uint, minu, m4)
OP_RVV_FUNC(min, i, 16, 8, int, min, m4)
OP_RVV_FUNC(min, u, 16, 8, uint, minu, m4)
OP_RVV_FUNC(min, i, 32, 4, int, min, m4)
OP_RVV_FUNC(min, u, 32, 4, uint, minu, m4)
OP_RVV_FUNC(min, i, 64, 2, int, min, m4)
OP_RVV_FUNC(min, u, 64, 2, uint, minu, m4)

OP_RVV_FUNC(min, f, 32, 4, float, fmin, m4)
OP_RVV_FUNC(min, f, 64, 2, float, fmin, m4)

/*************************************************************************
 * Sum
 ************************************************************************/
#undef current_func
#define current_func(a, b) ((a) + (b))
OP_RVV_FUNC(sum, i, 8, 16, int, add, m4)
OP_RVV_FUNC(sum, u, 8, 16, uint, add, m4)
OP_RVV_FUNC(sum, i, 16, 8, int, add, m4)
OP_RVV_FUNC(sum, u, 16, 8, uint, add, m4)
OP_RVV_FUNC(sum, i, 32, 4, int, add, m4)
OP_RVV_FUNC(sum, u, 32, 4, uint, add, m4)
OP_RVV_FUNC(sum, i, 64, 2, int, add, m4)
OP_RVV_FUNC(sum, u, 64, 2, uint, add, m4)

OP_RVV_FUNC(sum, f, 32, 4, float, fadd, m4)
OP_RVV_FUNC(sum, f, 64, 2, float, fadd, m4)

/*************************************************************************
 * Product
 *************************************************************************/
#undef current_func
#define current_func(a, b) ((a) * (b))
OP_RVV_FUNC(prod, i, 8, 16, int, mul, m4)
OP_RVV_FUNC(prod, u, 8, 16, uint, mul, m4)
OP_RVV_FUNC(prod, i, 16, 8, int, mul, m4)
OP_RVV_FUNC(prod, u, 16, 8, uint, mul, m4)
OP_RVV_FUNC(prod, i, 32, 4, int, mul, m4)
OP_RVV_FUNC(prod, u, 32, 4, uint, mul, m4)
OP_RVV_FUNC(prod, i, 64, 2, int, mul, m4)
OP_RVV_FUNC(prod, u, 64, 2, uint, mul, m4)

OP_RVV_FUNC(prod, f, 32, 4, float, fmul, m4)
OP_RVV_FUNC(prod, f, 64, 2, float, fmul, m4)

/*************************************************************************
 * Bitwise AND
 *************************************************************************/
#undef current_func
#define current_func(a, b) ((a) & (b))
OP_RVV_FUNC(band, i, 8, 16, int, and, m4)
OP_RVV_FUNC(band, u, 8, 16, uint, and, m4)
OP_RVV_FUNC(band, i, 16, 8, int, and, m4)
OP_RVV_FUNC(band, u, 16, 8, uint, and, m4)
OP_RVV_FUNC(band, i, 32, 4, int, and, m4)
OP_RVV_FUNC(band, u, 32, 4, uint, and, m4)
OP_RVV_FUNC(band, i, 64, 2, int, and, m4)
OP_RVV_FUNC(band, u, 64, 2, uint, and, m4)

/*************************************************************************
 * Bitwise OR
 *************************************************************************/
#undef current_func
#define current_func(a, b) ((a) | (b))
OP_RVV_FUNC(bor, i, 8, 16, int, or, m4)
OP_RVV_FUNC(bor, u, 8, 16, uint, or, m4)
OP_RVV_FUNC(bor, i, 16, 8, int, or, m4)
OP_RVV_FUNC(bor, u, 16, 8, uint, or, m4)
OP_RVV_FUNC(bor, i, 32, 4, int, or, m4)
OP_RVV_FUNC(bor, u, 32, 4, uint, or, m4)
OP_RVV_FUNC(bor, i, 64, 2, int, or, m4)
OP_RVV_FUNC(bor, u, 64, 2, uint, or, m4)

/*************************************************************************
 * Bitwise XOR
 *************************************************************************/
#undef current_func
#define current_func(a, b) ((a) ^ (b))
OP_RVV_FUNC(bxor, i, 8, 16, int, xor, m4)
OP_RVV_FUNC(bxor, u, 8, 16, uint, xor, m4)
OP_RVV_FUNC(bxor, i, 16, 8, int, xor, m4)
OP_RVV_FUNC(bxor, u, 16, 8, uint, xor, m4)
OP_RVV_FUNC(bxor, i, 32, 4, int, xor, m4)
OP_RVV_FUNC(bxor, u, 32, 4, uint, xor, m4)
OP_RVV_FUNC(bxor, i, 64, 2, int, xor, m4)
OP_RVV_FUNC(bxor, u, 64, 2, uint, xor, m4)

/*
 *  This is a three buffer (2 input and 1 output) version of the reduction
 *  routines, needed for some optimizations.
 */
#if defined(GENERATE_RVV_CODE)
#    define OP_RVV_FUNC_3BUFF(name, type_name, type_size, type_cnt, type, op, lmul)                \
        static void ompi_op_rvv_3buff_##name##_##type##type_size##_t(                              \
            const void *_in1, const void *_in2, void *_out, int *count,                            \
            struct ompi_datatype_t **dtype, struct ompi_op_base_module_1_0_0_t *module)            \
        {                                                                                          \
            int cnt = *count;                                                                      \
            type##type_size##_t *in1 = (type##type_size##_t *) _in1,                               \
                                *in2 = (type##type_size##_t *) _in2,                               \
                                *out = (type##type_size##_t *) _out;                               \
            int in_offset = 0;                                                                     \
            int out_offset = 0;                                                                    \
                                                                                                   \
            OP_CONCAT(OMPI_OP_TYPE_PREPEND, type##type_size####lmul##_t)                           \
            vec_src, vec_dst;                                                                      \
            for (int vl; cnt > 0; cnt -= vl, in_offset += vl, out_offset += vl) {                  \
                vl = OP_CONCAT(OMPI_OP_OP_PREPEND, vsetvl_e##type_size####lmul)(cnt);              \
                                                                                                   \
                vec_src = OP_CONCAT(OMPI_OP_OP_PREPEND,                                            \
                                    vle##type_size##_v_##type_name####type_size####lmul)(          \
                    &in1[in_offset], vl);                                                          \
                vec_dst = OP_CONCAT(OMPI_OP_OP_PREPEND,                                            \
                                    vle##type_size##_v_##type_name####type_size####lmul)(          \
                    &in2[out_offset], vl);                                                         \
                                                                                                   \
                vec_dst = OP_CONCAT(OMPI_OP_OP_PREPEND,                                            \
                                    v##op##_vv_##type_name####type_size####lmul)(vec_src, vec_dst, \
                                                                                 vl);              \
                                                                                                   \
                OP_CONCAT(OMPI_OP_OP_PREPEND, vse##type_size##_v_##type_name####type_size####lmul) \
                (&out[out_offset], vec_dst, vl);                                                   \
            }                                                                                      \
        }
#endif /* defined(GENERATE_RVV_CODE) */

/*************************************************************************
 * Max
 *************************************************************************/
#undef current_func
#define current_func(a, b) ((a) > (b) ? (a) : (b))
OP_RVV_FUNC_3BUFF(max, i, 8, 16, int, max, m4)
OP_RVV_FUNC_3BUFF(max, u, 8, 16, uint, maxu, m4)
OP_RVV_FUNC_3BUFF(max, i, 16, 8, int, max, m4)
OP_RVV_FUNC_3BUFF(max, u, 16, 8, uint, maxu, m4)
OP_RVV_FUNC_3BUFF(max, i, 32, 4, int, max, m4)
OP_RVV_FUNC_3BUFF(max, u, 32, 4, uint, maxu, m4)
OP_RVV_FUNC_3BUFF(max, i, 64, 2, int, max, m4)
OP_RVV_FUNC_3BUFF(max, u, 64, 2, uint, maxu, m4)

OP_RVV_FUNC_3BUFF(max, f, 32, 4, float, fmax, m4)
OP_RVV_FUNC_3BUFF(max, f, 64, 2, float, fmax, m4)

/*************************************************************************
 * Min
 *************************************************************************/
#undef current_func
#define current_func(a, b) ((a) < (b) ? (a) : (b))
OP_RVV_FUNC_3BUFF(min, i, 8, 16, int, min, m4)
OP_RVV_FUNC_3BUFF(min, u, 8, 16, uint, minu, m4)
OP_RVV_FUNC_3BUFF(min, i, 16, 8, int, min, m4)
OP_RVV_FUNC_3BUFF(min, u, 16, 8, uint, minu, m4)
OP_RVV_FUNC_3BUFF(min, i, 32, 4, int, min, m4)
OP_RVV_FUNC_3BUFF(min, u, 32, 4, uint, minu, m4)
OP_RVV_FUNC_3BUFF(min, i, 64, 2, int, min, m4)
OP_RVV_FUNC_3BUFF(min, u, 64, 2, uint, minu, m4)

OP_RVV_FUNC_3BUFF(min, f, 32, 4, float, fmin, m4)
OP_RVV_FUNC_3BUFF(min, f, 64, 2, float, fmin, m4)

/*************************************************************************
 * Sum
 ************************************************************************/
#undef current_func
#define current_func(a, b) ((a) + (b))
OP_RVV_FUNC_3BUFF(sum, i, 8, 16, int, add, m4)
OP_RVV_FUNC_3BUFF(sum, u, 8, 16, uint, add, m4)
OP_RVV_FUNC_3BUFF(sum, i, 16, 8, int, add, m4)
OP_RVV_FUNC_3BUFF(sum, u, 16, 8, uint, add, m4)
OP_RVV_FUNC_3BUFF(sum, i, 32, 4, int, add, m4)
OP_RVV_FUNC_3BUFF(sum, u, 32, 4, uint, add, m4)
OP_RVV_FUNC_3BUFF(sum, i, 64, 2, int, add, m4)
OP_RVV_FUNC_3BUFF(sum, u, 64, 2, uint, add, m4)

OP_RVV_FUNC_3BUFF(sum, f, 32, 4, float, fadd, m4)
OP_RVV_FUNC_3BUFF(sum, f, 64, 2, float, fadd, m4)

/*************************************************************************
 * Product
 *************************************************************************/
#undef current_func
#define current_func(a, b) ((a) * (b))
OP_RVV_FUNC_3BUFF(prod, i, 8, 16, int, mul, m4)
OP_RVV_FUNC_3BUFF(prod, u, 8, 16, uint, mul, m4)
OP_RVV_FUNC_3BUFF(prod, i, 16, 8, int, mul, m4)
OP_RVV_FUNC_3BUFF(prod, u, 16, 8, uint, mul, m4)
OP_RVV_FUNC_3BUFF(prod, i, 32, 4, int, mul, m4)
OP_RVV_FUNC_3BUFF(prod, u, 32, 4, uint, mul, m4)
OP_RVV_FUNC_3BUFF(prod, i, 64, 2, int, mul, m4)
OP_RVV_FUNC_3BUFF(prod, u, 64, 2, uint, mul, m4)

OP_RVV_FUNC_3BUFF(prod, f, 32, 4, float, fmul, m4)
OP_RVV_FUNC_3BUFF(prod, f, 64, 2, float, fmul, m4)

/*************************************************************************
 * Bitwise AND
 *************************************************************************/
#undef current_func
#define current_func(a, b) ((a) & (b))
OP_RVV_FUNC_3BUFF(band, i, 8, 16, int, and, m4)
OP_RVV_FUNC_3BUFF(band, u, 8, 16, uint, and, m4)
OP_RVV_FUNC_3BUFF(band, i, 16, 8, int, and, m4)
OP_RVV_FUNC_3BUFF(band, u, 16, 8, uint, and, m4)
OP_RVV_FUNC_3BUFF(band, i, 32, 4, int, and, m4)
OP_RVV_FUNC_3BUFF(band, u, 32, 4, uint, and, m4)
OP_RVV_FUNC_3BUFF(band, i, 64, 2, int, and, m4)
OP_RVV_FUNC_3BUFF(band, u, 64, 2, uint, and, m4)

/*************************************************************************
 * Bitwise OR
 *************************************************************************/
#undef current_func
#define current_func(a, b) ((a) | (b))
OP_RVV_FUNC_3BUFF(bor, i, 8, 16, int, or, m4)
OP_RVV_FUNC_3BUFF(bor, u, 8, 16, uint, or, m4)
OP_RVV_FUNC_3BUFF(bor, i, 16, 8, int, or, m4)
OP_RVV_FUNC_3BUFF(bor, u, 16, 8, uint, or, m4)
OP_RVV_FUNC_3BUFF(bor, i, 32, 4, int, or, m4)
OP_RVV_FUNC_3BUFF(bor, u, 32, 4, uint, or, m4)
OP_RVV_FUNC_3BUFF(bor, i, 64, 2, int, or, m4)
OP_RVV_FUNC_3BUFF(bor, u, 64, 2, uint, or, m4)

/*************************************************************************
 * Bitwise XOR
 *************************************************************************/
#undef current_func
#define current_func(a, b) ((a) ^ (b))
OP_RVV_FUNC_3BUFF(bxor, i, 8, 16, int, xor, m4)
OP_RVV_FUNC_3BUFF(bxor, u, 8, 16, uint, xor, m4)
OP_RVV_FUNC_3BUFF(bxor, i, 16, 8, int, xor, m4)
OP_RVV_FUNC_3BUFF(bxor, u, 16, 8, uint, xor, m4)
OP_RVV_FUNC_3BUFF(bxor, i, 32, 4, int, xor, m4)
OP_RVV_FUNC_3BUFF(bxor, u, 32, 4, uint, xor, m4)
OP_RVV_FUNC_3BUFF(bxor, i, 64, 2, int, xor, m4)
OP_RVV_FUNC_3BUFF(bxor, u, 64, 2, uint, xor, m4)

/** C integer ***********************************************************/
#define C_INTEGER(name, ftype)                                              \
    [OMPI_OP_BASE_TYPE_INT8_T] = ompi_op_rvv_##ftype##_##name##_int8_t,     \
    [OMPI_OP_BASE_TYPE_UINT8_T] = ompi_op_rvv_##ftype##_##name##_uint8_t,   \
    [OMPI_OP_BASE_TYPE_INT16_T] = ompi_op_rvv_##ftype##_##name##_int16_t,   \
    [OMPI_OP_BASE_TYPE_UINT16_T] = ompi_op_rvv_##ftype##_##name##_uint16_t, \
    [OMPI_OP_BASE_TYPE_INT32_T] = ompi_op_rvv_##ftype##_##name##_int32_t,   \
    [OMPI_OP_BASE_TYPE_UINT32_T] = ompi_op_rvv_##ftype##_##name##_uint32_t, \
    [OMPI_OP_BASE_TYPE_INT64_T] = ompi_op_rvv_##ftype##_##name##_int64_t,   \
    [OMPI_OP_BASE_TYPE_UINT64_T] = ompi_op_rvv_##ftype##_##name##_uint64_t

/** Floating point, including all the Fortran reals *********************/
#define FLOAT(name, ftype)  ompi_op_rvv_##ftype##_##name##_float32_t
#define DOUBLE(name, ftype) ompi_op_rvv_##ftype##_##name##_float64_t

#define FLOATING_POINT(name, ftype) \
    [OMPI_OP_BASE_TYPE_FLOAT] = FLOAT(name, ftype), [OMPI_OP_BASE_TYPE_DOUBLE] = DOUBLE(name, ftype)

/*
 * MPI_OP_NULL
 * All types
 */
#define FLAGS_NO_FLOAT (OMPI_OP_FLAGS_INTRINSIC | OMPI_OP_FLAGS_ASSOC | OMPI_OP_FLAGS_COMMUTE)
#define FLAGS                                                                  \
    (OMPI_OP_FLAGS_INTRINSIC | OMPI_OP_FLAGS_ASSOC | OMPI_OP_FLAGS_FLOAT_ASSOC \
     | OMPI_OP_FLAGS_COMMUTE)

ompi_op_base_handler_fn_t ompi_op_rvv_functions[OMPI_OP_BASE_FORTRAN_OP_MAX][OMPI_OP_BASE_TYPE_MAX] =
    {
        /* Corresponds to MPI_OP_NULL */
        [OMPI_OP_BASE_FORTRAN_NULL] = {
            /* Leaving this empty puts in NULL for all entries */
            NULL,
        },
        /* Corresponds to MPI_MAX */
        [OMPI_OP_BASE_FORTRAN_MAX] = {
            C_INTEGER(max, 2buff),
            FLOATING_POINT(max, 2buff),
        },
        /* Corresponds to MPI_MIN */
        [OMPI_OP_BASE_FORTRAN_MIN] = {
            C_INTEGER(min, 2buff),
            FLOATING_POINT(min, 2buff),
        },
        /* Corresponds to MPI_SUM */
        [OMPI_OP_BASE_FORTRAN_SUM] = {
            C_INTEGER(sum, 2buff),
            FLOATING_POINT(sum, 2buff),
        },
        /* Corresponds to MPI_PROD */
        [OMPI_OP_BASE_FORTRAN_PROD] = {
            C_INTEGER(prod, 2buff),
            FLOATING_POINT(prod, 2buff),
        },
        /* Corresponds to MPI_LAND */
        [OMPI_OP_BASE_FORTRAN_LAND] = {
            NULL,
        },
        /* Corresponds to MPI_BAND */
        [OMPI_OP_BASE_FORTRAN_BAND] = {
            C_INTEGER(band, 2buff),
        },
        /* Corresponds to MPI_LOR */
        [OMPI_OP_BASE_FORTRAN_LOR] = {
            NULL,
        },
        /* Corresponds to MPI_BOR */
        [OMPI_OP_BASE_FORTRAN_BOR] = {
            C_INTEGER(bor, 2buff),
        },
        /* Corresponds to MPI_LXOR */
        [OMPI_OP_BASE_FORTRAN_LXOR] = {
            NULL,
        },
        /* Corresponds to MPI_BXOR */
        [OMPI_OP_BASE_FORTRAN_BXOR] = {
            C_INTEGER(bxor, 2buff),
        },
        /* Corresponds to MPI_REPLACE */
        [OMPI_OP_BASE_FORTRAN_REPLACE] = {
            /* (MPI_ACCUMULATE is handled differently than the other
               reductions, so just zero out its function
               implementations here to ensure that users don't invoke
               MPI_REPLACE with any reduction operations other than
               ACCUMULATE) */
            NULL,
        },

};

ompi_op_base_3buff_handler_fn_t ompi_op_rvv_3buff_functions[OMPI_OP_BASE_FORTRAN_OP_MAX][OMPI_OP_BASE_TYPE_MAX] =
    {
        /* Corresponds to MPI_OP_NULL */
        [OMPI_OP_BASE_FORTRAN_NULL] = {
            /* Leaving this empty puts in NULL for all entries */
            NULL,
        },
        /* Corresponds to MPI_MAX */
        [OMPI_OP_BASE_FORTRAN_MAX] = {
            C_INTEGER(max, 3buff),
            FLOATING_POINT(max, 3buff),
        },
        /* Corresponds to MPI_MIN */
        [OMPI_OP_BASE_FORTRAN_MIN] = {
            C_INTEGER(min, 3buff),
            FLOATING_POINT(min, 3buff),
        },
        /* Corresponds to MPI_SUM */
        [OMPI_OP_BASE_FORTRAN_SUM] = {
            C_INTEGER(sum, 3buff),
            FLOATING_POINT(sum, 3buff),
        },
        /* Corresponds to MPI_PROD */
        [OMPI_OP_BASE_FORTRAN_PROD] = {
            C_INTEGER(prod, 3buff),
            FLOATING_POINT(prod, 3buff),
        },
        /* Corresponds to MPI_LAND */
        [OMPI_OP_BASE_FORTRAN_LAND] = {
            NULL,
        },
        /* Corresponds to MPI_BAND */
        [OMPI_OP_BASE_FORTRAN_BAND] = {
            C_INTEGER(band, 3buff),
        },
        /* Corresponds to MPI_LOR */
        [OMPI_OP_BASE_FORTRAN_LOR] = {
            NULL,
        },
        /* Corresponds to MPI_BOR */
        [OMPI_OP_BASE_FORTRAN_BOR] = {
            C_INTEGER(bor, 3buff),
        },
        /* Corresponds to MPI_LXOR */
        [OMPI_OP_BASE_FORTRAN_LXOR] = {
            NULL,
        },
        /* Corresponds to MPI_BXOR */
        [OMPI_OP_BASE_FORTRAN_BXOR] = {
            C_INTEGER(bxor, 3buff),
        },
        /* Corresponds to MPI_REPLACE */
        [OMPI_OP_BASE_FORTRAN_REPLACE] = {
            /* MPI_ACCUMULATE is handled differently than the other
               reductions, so just zero out its function
               implementations here to ensure that users don't invoke
               MPI_REPLACE with any reduction operations other than
               ACCUMULATE */
            NULL,
        },
};
