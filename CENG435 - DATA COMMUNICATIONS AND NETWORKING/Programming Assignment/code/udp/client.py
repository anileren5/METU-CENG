# Import necessary libraries
import socket
import time
import sys 
import os
from common import * 

# Get fine-tuning parameters
arg_count = len(sys.argv)
if (arg_count != 2): # You need to give one parameter as command line arguments.
    print("Please give the necessary fine tuning parameters")
    exit()
NUMBER_OF_TRIAL_FOR_ADRESS_TRANSFER = int(sys.argv[1])

SERVER_ADDRESS = ("172.17.0.2", 20001) # IP adress of the server container


EVENTUAL_PACKET_COUNT = 9870 # This is an hardcoded value based upon the example objects. If you try the code with the objects of different sizes, this value must be updated. So, please use the objects of the same size given in the homework
EVENTUAL_PACKET_SIZE = 1096 # This is the size of an eventual packet

# Create file instances to store incoming packets
SMALL_FILES = []
LARGE_FILES = []
for i in range(10):
    SMALL_FILES.append(ClientFile("small", i, 10))
for i in range(10):
    LARGE_FILES.append(ClientFile("large", i, 977))

# Create a UDP socket at client side
UDPClientSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

# Tell the client's adress to the server
sendClientAddress(UDPClientSocket, SERVER_ADDRESS, NUMBER_OF_TRIAL_FOR_ADRESS_TRANSFER)

def client():
    
    while True:
        try:
            UDPClientSocket.settimeout(0.1) # This is a precaution for missing termination message

            # Receive an eventual packet and parse it into their subcomponents
            eventualPacket, _ = UDPClientSocket.recvfrom(EVENTUAL_PACKET_SIZE)
            sequence_number_header, type_header, file_no_header, packet_no_header, length_header, chunk_without_padding, isCorrupt = eventualPacketParser(eventualPacket)

            if isCorrupt == True: # If the file is corrupted, ignore it
                continue 
            if sequence_number_header == 9999: # If this is an termination packet, terminate the client
                break
            
            if (type_header == 0): # Small
                SMALL_FILES[file_no_header].packets[packet_no_header] = chunk_without_padding # Save received packet
                SMALL_FILES[file_no_header].isPacketReceived[packet_no_header] = True         # Mark this packet as received
            elif (type_header == 1): # Large 
                LARGE_FILES[file_no_header].packets[packet_no_header] = chunk_without_padding # Save received packet
                LARGE_FILES[file_no_header].isPacketReceived[packet_no_header] = True         # Mark this packet as received

            UDPClientSocket.sendto(makeACKPacket(sequence_number_header), SERVER_ADDRESS) # Send acknowledgement packet to the server
        except socket.timeout:
            if (areAllFilesReceived(SMALL_FILES, LARGE_FILES)):
                break

    # Write the received packets to files in the corresponding paths
    for FILE in SMALL_FILES:
        FILE.write()
    for FILE in LARGE_FILES:
        FILE.write()

start_time = time.perf_counter() # Start the timer for experiments
client()
end_time = time.perf_counter()  # Stop the timer for experiments
elapsed_time = end_time - start_time
print(elapsed_time)
os.system("./diff_check.sh") # Check if all the files (objects) are received correctly

    
