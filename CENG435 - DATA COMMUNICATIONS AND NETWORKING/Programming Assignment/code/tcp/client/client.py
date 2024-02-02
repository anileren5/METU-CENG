# Import necessary libraries
import socket
import time
import os

HOST = "172.17.0.2" # IP Address of the server container
PORT = 43542        # Port of the server container
BUFFER_SIZE = 1024  # Client reads BUFFER_SIZE amount of bytes in each recv call

def receive(s, type, index):
    # Receive the file size as a header
    file_size = int(s.recv(1024).decode('utf-8'))

    # Send a confirmation message to the server
    s.sendall("SIZE_RECEIVED".encode('utf-8'))
    
    with open(f"./objects/{type}-{index}.obj", 'wb') as file:
        # Receive the file data
        received_size = 0
        while received_size < file_size:
            remaining_size = file_size - received_size
            chunk_size = BUFFER_SIZE if remaining_size > BUFFER_SIZE else remaining_size
            file_data = s.recv(chunk_size)
            file.write(file_data)
            received_size += len(file_data)
            
def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        # Connect to the server
        s.connect((HOST, PORT))

        # Get 10 large objects and 10 small objects one by one
        for i in range(10):
            # Receive 1 large object in chunks of 1024 bytes
            receive(s, "large", i)

            # Receive 1 small object in chunks of 1024 bytes
            receive(s, "small", i)


if __name__ == "__main__":
    start = time.perf_counter() # Start the timer for experiments
    main() 
    end = time.perf_counter()   # Stop the timer for experiments
    elapsed = end - start
    print(elapsed, flush=True)
    os.system("./diff_check.sh") # Check if all the files (objects) are received correctly

