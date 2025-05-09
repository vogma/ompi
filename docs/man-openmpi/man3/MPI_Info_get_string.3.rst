.. _mpi_info_get_string:


MPI_Info_get_string
===================

.. include_body

:ref:`MPI_Info_get_string` |mdash| Retrieves the value associated with a key in
an info object.

.. The following file was automatically generated
.. include:: ./bindings/mpi_info_get_string.rst

INPUT PARAMETERS
----------------
* ``info``: Info object (handle).
* ``key``: Key (string).

OUTPUT PARAMETER
----------------
* ``buflen On entry, length of value arg. On return, set to required size to``: hold value string (integer).
* ``value``: Value (string).
* ``flag``: Returns true if key defined, false if not (boolean).
* ``ierror``: Fortran only: Error status (integer).

DESCRIPTION
-----------

:ref:`MPI_Info_get_string` retrieves the value associated with *key* from
*info*, if any. If such a key exists in info, it sets *flag* to true and
returns the value in *value*, otherwise it sets flag to false and leaves
value unchanged. *buflen* on input is the size of the provided buffer,
for the output of buflen it is the size of the buffer needed to store
the value string. If the buflen passed into the function is less than
the actual size needed to store the value string (including null
terminator in C), the value is truncated. On return, the value of
*buflen* will be set to the required buffer size to hold the value
string. If buflen is set to 0, value is not changed. In C, *buflen*
includes the required space for the null terminator. In C, this function
returns a null terminated string in all cases where the *buflen* input
value is greater than 0.

If *key* is larger than MPI_MAX_INFO_KEY, the call is erroneous.


ERRORS
------

.. include:: ./ERRORS.rst

.. seealso::
   * :ref:`MPI_Info_create`
   * :ref:`MPI_Info_delete`
   * :ref:`MPI_Info_dup`
   * :ref:`MPI_Info_free`
   * :ref:`MPI_Info_get_nkeys`
   * :ref:`MPI_Info_get_nthkey`
   * :ref:`MPI_Info_set`
