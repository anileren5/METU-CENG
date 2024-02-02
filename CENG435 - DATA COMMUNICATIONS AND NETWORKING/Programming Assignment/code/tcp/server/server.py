# Import necessary librariries
import socket
import os

HOST = "172.17.0.2"  # IP address of the server container
PORT = 43542         # Port of the server container
BUFFER_SIZE = 1024   # Server sends the file chunks with size of BUFFER_SIZE

def send(conn, object_file_path):
    with open(object_file_path, 'rb') as file:
        # Send the file size as a header
        file_size = os.path.getsize(object_file_path)
        conn.sendall(str(file_size).encode('utf-8'))

        # Wait for a confirmation message from the client
        confirmation = conn.recv(BUFFER_SIZE).decode('utf-8')
        if confirmation == "SIZE_RECEIVED": # If the server make sure that client has the size information, it starts to send chunks
            # Send the file data
            file_data = file.read(BUFFER_SIZE)
            while file_data:
                conn.send(file_data)
                file_data = file.read(BUFFER_SIZE)

    
def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind((HOST, PORT)) # Bind to socket corresponding to PORT.
        s.listen() 
        conn, addr = s.accept() # Wait for incoming TCP connections.

        # Send 10 large objects and 10 small objects one by one
        with conn:
            for i in range(10):
                # Send 1 large object in chunks of 1024 bytes
                send(conn, f"./objects/large-{i}.obj")

                # Send 1 small object in chunks of 1024 bytes
                send(conn, f"./objects/small-{i}.obj")

if __name__ == "__main__":
    main()


