.. _mpi_neighbor_alltoallw:


MPI_Neighbor_alltoallw
======================

.. include_body

:ref:`MPI_Neighbor_alltoallw`, :ref:`MPI_Ineighbor_alltoallw`,
:ref:`MPI_Neighbor_alltoallw_init` |mdash| All processes send data of different
types to, and receive data of different types from, all processes

.. The following directive tells the man page generation script to
   generate multiple bindings for this file.
.. mpi-bindings: MPI_Neighbor_alltoallw, MPI_Ineighbor_alltoallw, MPI_Neighbor_alltoallw_init

.. The following file was automatically generated
.. include:: ./bindings/mpi_neighbor_alltoallw.rst

INPUT PARAMETERS
----------------
* ``sendbuf``: Starting address of send buffer.
* ``sendcounts``: Integer array, where entry i specifies the number of elements to send to neighbor i.
* ``sdispls``: Integer array, where entry i specifies the displacement (in bytes, offset from *sendbuf*) from which to send data to neighbor i.
* ``sendtypes``: Datatype array, where entry i specifies the datatype to use when sending data to neighbor i.
* ``recvcounts``: Integer array, where entry j specifies the number of elements to receive from neighbor j.
* ``rdispls``: Integer array, where entry j specifies the displacement (in bytes, offset from *recvbuf*) to which data from neighbor j should be written.
* ``recvtypes``: Datatype array, where entry j specifies the datatype to use when receiving data from neighbor j.
* ``comm``: Communicator over which data is to be exchanged.
* ``info``: Info (handle, persistent only).

OUTPUT PARAMETERS
-----------------
* ``recvbuf``: Address of receive buffer.
* ``request``: Request (handle, non-blocking and persistent only).
* ``ierror``: Fortran only: Error status.

DESCRIPTION
-----------

:ref:`MPI_Neighbor_alltoallw` is a generalized collective operation in which
all processes send data to and receive data from all neighbors. It adds
flexibility to :ref:`MPI_Neighbor_alltoallv` by allowing the user to specify
the datatype of individual data blocks (in addition to displacement and
element count). Its operation can be thought of in the following way,
where each process performs 2n (n being the number of neighbors in the
topology of communicator *comm*) independent point-to-point
communications. The neighbors and buffer layout are determined by the
topology of *comm*.

.. code-block:: c

           MPI_Cart_get(comm, maxdims, dims, periods, coords);
           for (dim = 0, i = 0 ; dim < dims ; ++dim) {
               MPI_Cart_shift(comm, dim, 1, &r0, &r1);
               MPI_Isend(sendbuf + sdispls[i]  * extent(sendtype),
                         sendcount, sendtypes[i], r0, ..., comm, ...);
               MPI_Irecv(recvbuf + rdispls[i] * extent(recvtype),
                         recvcount, recvtypes[i], r0, ..., comm, ...);
               ++i;
               MPI_Isend(sendbuf + sdispls[i] * extent(sendtype),
                         sendcount, sendtypes[i], r1, ..., comm, &req[i]);
               MPI_Irecv(recvbuf + rdispls[i] * extent(recvtype),
                         recvcount, recvtypes[i], r1, ..., comm, ...);
               ++i;
           }

           MPI_Wait_all (...);

     	   MPI_Comm_size(comm, &n);
   	   for (i = 0, i < n; i++)
   	     MPI_Send(sendbuf + sdispls[i], sendcounts[i],
   	          sendtypes[i], i, ..., comm);
   	   for (i = 0, i < n; i++)
	     MPI_Recv(recvbuf + rdispls[i], recvcounts[i],
   	          recvtypes[i], i, ..., comm);

Process j sends the k-th block of its local *sendbuf* to neighbor k,
which places the data in the j-th block of its local *recvbuf*.

When a pair of processes exchanges data, each may pass different element
count and datatype arguments so long as the sender specifies the same
amount of data to send (in bytes) as the receiver expects to receive.

Note that process i may send a different amount of data to process j
than it receives from process j. Also, a process may send entirely
different amounts and types of data to different processes in the
communicator.


NOTES
-----

The MPI_IN_PLACE option for *sendbuf* is not meaningful for this
operation

The specification of counts, types, and displacements should not cause
any location to be written more than once.

All arguments on all processes are significant. The *comm* argument, in
particular, must describe the same communicator on all processes.

The offsets of *sdispls* and *rdispls* are measured in bytes. Compare
this to :ref:`MPI_Neighbor_alltoallv`, where these offsets are measured in
units of *sendtype* and *recvtype*, respectively.


ERRORS
------

.. include:: ./ERRORS.rst

.. seealso::
   * :ref:`MPI_Neighbor_alltoall`
   * :ref:`MPI_Neighbor_alltoallv`
   * :ref:`MPI_Cart_create`
   * :ref:`MPI_Graph_create`
   * :ref:`MPI_Dist_graph_create`
