.. _mpi_alltoall:


MPI_Alltoall
============

.. include_body

:ref:`MPI_Alltoall`, :ref:`MPI_Ialltoall`, :ref:`MPI_Alltoall_init` - All processes send
data to all processes

.. The following directive tells the man page generation script to
   generate multiple bindings for this file.
.. mpi-bindings: MPI_Alltoall, MPI_Ialltoall, MPI_Alltoall_init

.. The following file was automatically generated
.. include:: ./bindings/mpi_alltoall.rst

INPUT PARAMETERS
----------------
* ``sendbuf``: Starting address of send buffer (choice).
* ``sendcount``: Number of elements to send to each process (integer).
* ``sendtype``: Datatype of send buffer elements (handle).
* ``recvcount``: Number of elements to receive from each process (integer).
* ``recvtype``: Datatype of receive buffer elements (handle).
* ``comm``: Communicator over which data is to be exchanged (handle).
* ``info``: Info (handle, persistent only)

OUTPUT PARAMETERS
-----------------
* ``recvbuf``: Starting address of receive buffer (choice).
* ``request``: Request (handle, non-blocking and persistent only).
* ``ierror``: Fortran only: Error status (integer).

DESCRIPTION
-----------

:ref:`MPI_Alltoall` is a collective operation in which all processes send the
same amount of data to each other, and receive the same amount of data
from each other. The operation of this routine can be represented as
follows, where each process performs 2n (n being the number of processes
in communicator *comm*) independent point-to-point communications
(including communication with itself).

::

   	MPI_Comm_size(comm, &n);
   	for (i = 0, i < n; i++)
   	    MPI_Send(sendbuf + i * sendcount * extent(sendtype),
   	        sendcount, sendtype, i, ..., comm);
   	for (i = 0, i < n; i++)
   	    MPI_Recv(recvbuf + i * recvcount * extent(recvtype),
   	        recvcount, recvtype, i, ..., comm);

Each process breaks up its local *sendbuf* into n blocks - each
containing *sendcount* elements of type *sendtype* - and divides its
*recvbuf* similarly according to *recvcount* and *recvtype*. Process j
sends the k-th block of its local *sendbuf* to process k, which places
the data in the j-th block of its local *recvbuf*. The amount of data
sent must be equal to the amount of data received, pairwise, between
every pair of processes.

WHEN COMMUNICATOR IS AN INTER-COMMUNICATOR

When the communicator is an inter-communicator, the gather operation
occurs in two phases. The data is gathered from all the members of the
first group and received by all the members of the second group. Then
the data is gathered from all the members of the second group and
received by all the members of the first. The operation exhibits a
symmetric, full-duplex behavior.

The first group defines the root process. The root process uses MPI_ROOT
as the value of *root*. All other processes in the first group use
``MPI_PROC_NULL`` as the value of *root*. All processes in the second group
use the rank of the root process in the first group as the value of
*root*.

When the communicator is an intra-communicator, these groups are the
same, and the operation occurs in a single phase.


USE OF IN-PLACE OPTION
----------------------

When the communicator is an intracommunicator, you can perform an
all-to-all operation in-place (the output buffer is used as the input
buffer). Use the variable MPI_IN_PLACE as the value of *sendbuf*. In
this case, *sendcount* and *sendtype* are ignored. The input data of
each process is assumed to be in the area where that process would
receive its own contribution to the receive buffer.


NOTES
-----

All arguments on all processes are significant. The *comm* argument, in
particular, must describe the same communicator on all processes.

There are two MPI library functions that are more general than
:ref:`MPI_Alltoall`. :ref:`MPI_Alltoallv` allows all-to-all communication to and from
buffers that need not be contiguous; different processes may send and
receive different amounts of data. :ref:`MPI_Alltoallw` expands :ref:`MPI_Alltoallv`'s
functionality to allow the exchange of data with different datatypes.


ERRORS
------

.. include:: ./ERRORS.rst

.. seealso::
   * :ref:`MPI_Alltoallv`
   * :ref:`MPI_Alltoallw`
