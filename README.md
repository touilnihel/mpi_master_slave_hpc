Overview

This project implements a data distribution system using the master-slave model with point-to-point communication in MPI (Message Passing Interface). The master process distributes data to slave processes upon request, and the slaves process the data in parallel before sending the results back to the master.

Features

Dynamic workload distribution between the master and slave processes.

Parallel processing of data chunks.

Efficient point-to-point communication using MPI.

Flexible design allowing any process to act as the master.

Requirements

C compiler with OpenMPI package installed.

Compilation

To compile the program, use the following command:

mpicc -o mpi_master_slave mpi_master_slave.c

Execution

Run the program with at least two processes:

mpirun -np <num_processes> ./mpi_master_slave

Replace <num_processes> with the desired number of MPI processes (must be >= 2).

How It Works

Master Process

Stores the dataset and initializes data.

Randomly selects a master process.

Sends data chunks to requesting slave processes.

Receives processed data from slaves and stores it.

Sends termination signals when all chunks have been processed.

Slave Processes

Request data from the master.

Receive a data chunk and process it (square each element).

Send the processed data back to the master.

Repeat until a termination signal is received.

Constraints

Supports any number of processes (≥ 2).

Any process can be assigned as the master.

Works with variable data sizes and chunk sizes.

Uses only MPI point-to-point communication functions.

Example Output

Processed data by the master: 1 4 9 16 25 ...
