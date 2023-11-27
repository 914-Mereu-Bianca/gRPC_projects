This code provides the implementation of a gRPC synchronous server and of two clients, one synchronous and one asynchronous.
The purpose of the code is to determine how many requests are processed simultaneously in a gRPC synchronous server. The results show us that the maximum number is constrained by the system-imposed limit on the number of threads that can be created.

We can also edit the maximum number of threads that can run in parallel in the sync server to provide a better understanding of how the server works.

To run the following code create the 'build' directory, open it and run 'cmake ..' and 'cmake --build .'

Finally, open a terminal, navigate to the build directory and run the server with the command './gRPCServer', then open another terminal and run one of the clients './gRPCClientSync' or './gRPCClientAsync'