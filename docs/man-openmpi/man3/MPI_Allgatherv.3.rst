.. _mpi_allgatherv:


MPI_Allgatherv
==============

.. include_body

:ref:`MPI_Allgatherv`, :ref:`MPI_Iallgatherv`, :ref:`MPI_Allgatherv_init` - Gathers data
from all processes and delivers it to all. Each process may contribute a
different amount of data.

.. The following directive tells the man page generation script to
   generate multiple bindings for this file.
.. mpi-bindings: MPI_Allgatherv, MPI_Iallgatherv, MPI_Allgatherv_init

.. The following file was automatically generated
.. include:: ./bindings/mpi_allgatherv.rst

INPUT PARAMETERS
----------------
* ``sendbuf``: Starting address of send buffer (choice).
* ``sendcount``: Number of elements in send buffer (integer).
* ``sendtype``: Datatype of send buffer elements (handle).
* ``recvcount``: Integer array (of length group size) containing the number of elements that are received from each process.
* ``displs``: Integer array (of length group size). Entry i specifies the displacement (relative to recvbuf) at which to place the incoming data from process i.
* ``recvtype``: Datatype of receive buffer elements (handle).
* ``comm``: Communicator (handle).
* ``info``: Info (handle, persistent only).

OUTPUT PARAMETERS
-----------------
* ``recvbuf``: Address of receive buffer (choice).
* ``request``: Request (handle, non-blocking and persistent only).
* ``ierror``: Fortran only: Error status (integer).

DESCRIPTION
-----------

:ref:`MPI_Allgatherv` is similar to :ref:`MPI_Allgather` in that all processes gather
data from all other processes, except that each process can send a
different amount of data. The block of data sent from the jth process is
received by every process and placed in the jth block of the buffer
*recvbuf.*

The type signature associated with sendcount, sendtype, at process j
must be equal to the type signature associated with recvcounts[j],
recvtype at any other process.

The outcome is as if all processes executed calls to

.. code-block:: c

   MPI_Gatherv(sendbuf, sendcount, sendtype, recvbuf, recvcount,
               displs, recvtype, root, comm);

   // for root = 0, ..., n-1.

The rules for correct usage of :ref:`MPI_Allgatherv`
are easily found from the corresponding rules for :ref:`MPI_Gatherv`.


USE OF IN-PLACE OPTION
----------------------

When the communicator is an intracommunicator, you can perform an
all-gather operation in-place (the output buffer is used as the input
buffer). Use the variable MPI_IN_PLACE as the value of *sendbuf*. In
this case, *sendcount* and *sendtype* are ignored. The input data of
each process is assumed to be in the area where that process would
receive its own contribution to the receive buffer. Specifically, the
outcome of a call to :ref:`MPI_Allgatherv` that used the in-place option is
identical to the case in which all processes executed *n* calls to

.. code-block:: c

      MPI_Gatherv(MPI_IN_PLACE, 0, MPI_DATATYPE_NULL, recvbuf,
                  recvcounts, displs, recvtype, root, comm);

      // for root = 0, ..., n-1.

Note that MPI_IN_PLACE is a special kind of value; it has the same
restrictions on its use as MPI_BOTTOM.

Because the in-place option converts the receive buffer into a
send-and-receive buffer, a Fortran binding that includes INTENT must
mark these as INOUT, not OUT.


WHEN COMMUNICATOR IS AN INTER-COMMUNICATOR
------------------------------------------

When the communicator is an inter-communicator, the gather operation
occurs in two phases. The data is gathered from all the members of the
first group, concatenated, and received by all the members of the second
group. Then the data is gathered from all the members of the second
group, concatenated, and received by all the members of the first. The
send buffer arguments in the one group must be consistent with the
receive buffer arguments in the other group, and vice versa. The
operation must exhibit symmetric, full-duplex behavior.


ERRORS
------

.. include:: ./ERRORS.rst

.. seealso::
   * :ref:`MPI_Gatherv`
   * :ref:`MPI_Allgather`
