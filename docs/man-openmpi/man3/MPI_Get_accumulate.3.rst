.. _mpi_get_accumulate:

MPI_Get_accumulate
==================

.. include_body

:ref:`MPI_Get_accumulate`, :ref:`MPI_Rget_accumulate` - Combines the contents of the
origin buffer with that of a target buffer and returns the target buffer
value.

.. The following directive tells the man page generation script to
   generate multiple bindings for this file.
.. mpi-bindings: MPI_Get_accumulate, MPI_Rget_accumulate

.. The following file was automatically generated
.. include:: ./bindings/mpi_get_accumulate.rst

INPUT PARAMETERS
----------------

* ``origin_addr`` : Initial address of buffer (choice).
* ``origin_count`` : Number of entries in buffer (nonnegative integer).
* ``origin_datatype`` : Data type of each buffer entry (handle).
* ``result_addr`` : Initial address of result buffer (choice).
* ``result_count`` : Number of entries in result buffer (nonnegative
   integer).
* ``result_datatype`` : Data type of each result buffer entry (handle).
* ``target_rank`` : Rank of target (nonnegative integer).
* ``target_disp`` : Displacement from start of window to beginning of
   target buffer (nonnegative integer).
* ``target_count`` : Number of entries in target buffer (nonnegative
   integer).
* ``target_datatype`` : Data type of each entry in target buffer (handle).
* ``op`` : Reduce operation (handle).
* ``win`` : Window object (handle).

OUTPUT PARAMETER
----------------

* :ref:`MPI_Rget_accumulate`: RMA request
* ``ierror`` : Fortran only: Error status (integer).

DESCRIPTION
-----------

:ref:`MPI_Get_accumulate` is a function used for one-sided MPI communication
that adds the contents of the origin buffer (as defined by origin_addr,
origin_count, and origin_datatype) to the buffer specified by the
arguments target_count and target_datatype, at offset target_disp, in
the target window specified by target_rank and win, using the operation
op. :ref:`MPI_Get_accumulate` returns in the result buffer result_addr the
contents of the target buffer before the accumulation.

Any of the predefined operations for :ref:`MPI_Reduce`, as well as MPI_NO_OP,
can be used. User-defined functions cannot be used. For example, if op
is MPI_SUM, each element of the origin buffer is added to the
corresponding element in the target, replacing the former value in the
target.

Each datatype argument must be a predefined data type or a derived data
type, where all basic components are of the same predefined data type.
Both datatype arguments must be constructed from the same predefined
data type. The operation op applies to elements of that predefined type.
The target_datatype argument must not specify overlapping entries, and
the target buffer must fit in the target window.

A new predefined operation, MPI_REPLACE, is defined. It corresponds to
the associative function f(a, b) =b; that is, the current value in the
target memory is replaced by the value supplied by the origin.

A new predefined operation, MPI_NO_OP, is defined. It corresponds to the
assiciative function f(a, b) = a; that is the current value in the
target memory is returned in the result buffer at the origin and no
operation is performed on the target buffer.

:ref:`MPI_Rget_accumulate` is similar to :ref:`MPI_Get_accumulate`, except that it
allocates a communication request object and associates it with the
request handle (the argument request) that can be used to wait or test
for completion. The completion of an :ref:`MPI_Rget_accumulate` operation
indicates that the data is available in the result buffer and the origin
buffer is free to be updated. It does not indicate that the operation
has been completed at the target window.


NOTES
-----

The generic functionality of :ref:`MPI_Get_accumulate` might limit the
performance of fetch-and-increment or fetch-and-add calls that might be
supported by special hardware operations. :ref:`MPI_Fetch_and_op` thus allows
for a fast implementation of a commonly used subset of the functionality
of :ref:`MPI_Get_accumulate`.

:ref:`MPI_Get` is a special case of :ref:`MPI_Get_accumulate`, with the operation
MPI_NO_OP. Note, however, that :ref:`MPI_Get` and :ref:`MPI_Get_accumulate` have
different constraints on concurrent updates.

It is the user's responsibility to guarantee that, when using the
accumulate functions, the target displacement argument is such that
accesses to the window are properly aligned according to the data type
arguments in the call to the :ref:`MPI_Get_accumulate` function.

ERRORS
------

.. include:: ./ERRORS.rst

.. seealso:: :ref:`MPI_Put` :ref:`MPI_Reduce`
