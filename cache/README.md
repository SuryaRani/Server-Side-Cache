Program: httpproxy.c

How to Run:
./httpproxy [client_port] [server_port] 

Optional arguments
./httpproxy [client_port] [server_port] -c [cache_size] -u -m [max_file_size]

Limitations: 
The program works on my machine for caching a small file, FIFO and LRU updating but for some reason times out on the pipeline tests.

Could be caused by persistent connection problems but I could not figure it out in time.

I am only closing my client connection when I do not recieve any bytes from the client connection itself in the beggining of my handle_connection function.
