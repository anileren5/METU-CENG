# Import necessary libraries
import socket
import sys 
from common import *

# Get fine-tuning parameters
arg_count = len(sys.argv)
if (arg_count != 3): # You need to give two parameters as command line arguments.
    print("Please give the necessary fine tuning parameters")
    exit()
WINDOW_SIZE = int(sys.argv[1])
NUMBER_OF_TRIAL_FOR_ADRESS_TRANSFER = int(sys.argv[2])

SERVER_ADDRESS = ("172.17.0.2", 20001) # IP address of the server

# Specify the window boundaries
WINDOW_BASE  = 0
WINDOWS_END = WINDOW_BASE + WINDOW_SIZE - 1 # WINDOW_END is inclusive

# Read files and form pure packets
FILES = []
for i in range(10):
    FILES.append(ServerFile("S", i))
    FILES.append(ServerFile("L", i))

# Form eventual packets
EVENTUAL_PACKETS = [] # This is a mixture of eventual packets belonging to small and large files
eventual_packet_no = 0
for FILE in FILES:
    for purePacket in FILE.packets:
        EVENTUAL_PACKETS.append(makeEventualPacket(purePacket, eventual_packet_no))
        eventual_packet_no += 1

PACKET_COUNT = len(EVENTUAL_PACKETS)
RECEIVED_ACKS = [False] * PACKET_COUNT # Keep track of acknowledged eventual packets

# Create a UDP socket at server side
UDPServerSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
UDPServerSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
UDPServerSocket.bind(SERVER_ADDRESS)

# Get the adress of the client
CLIENT_ADDRESS = getClientAddress(UDPServerSocket, NUMBER_OF_TRIAL_FOR_ADRESS_TRANSFER)

def server():
    global WINDOW_SIZE, WINDOW_BASE, WINDOWS_END, FILES, EVENTUAL_PACKETS, PACKET_COUNT, RECEIVED_ACKS, UDPServerSocket, CLIENT_ADDRESS

    # Do this loop until no window is left
    while WINDOW_BASE != PACKET_COUNT:
        # Send all the eventual packets to the client quickly (that is, send them without waiting for an acknowledgement)
        for i in range(WINDOW_BASE, WINDOWS_END+1):
            UDPServerSocket.sendto(EVENTUAL_PACKETS[i], CLIENT_ADDRESS)
        
        # Collect received acknowledgement (if any)
        for i in range(WINDOW_BASE, WINDOWS_END+1):
            try:
                UDPServerSocket.settimeout(0.001)
                ACKPacket, _ = UDPServerSocket.recvfrom(40)
                packet_no_header, isCorrupt = AckPacketParser(ACKPacket)
                if isCorrupt == True:
                    continue
                else:
                    RECEIVED_ACKS[packet_no_header] = True
            except socket.timeout:
                continue

        # Resend the packets which are not acknowledge until no unacknowledge eventual packet is left
        while isWindowSlidable(RECEIVED_ACKS, WINDOW_BASE, WINDOWS_END) == False:
            missingACKs = 0
            for i in range(WINDOW_BASE, WINDOWS_END+1):
                if (RECEIVED_ACKS[i] == False):
                    missingACKs += 1
                    UDPServerSocket.sendto(EVENTUAL_PACKETS[i], CLIENT_ADDRESS)
            
            for i in range(missingACKs):
                try:
                    UDPServerSocket.settimeout(0.001)
                    ACKPacket, _ = UDPServerSocket.recvfrom(40)
                    packet_no_header, isCorrupt = AckPacketParser(ACKPacket)
                    if isCorrupt == True:
                        continue
                    else:
                        RECEIVED_ACKS[packet_no_header] = True            
                except socket.timeout:
                    continue

        # Slide the window (actually, it is better to say jump to next window)
        WINDOW_BASE += WINDOW_SIZE
        WINDOWS_END += WINDOW_SIZE

    # Send the termination message to client 
    UDPServerSocket.sendto(makeEventualPacket(FILES[0].packets[0], 9999), CLIENT_ADDRESS)

# Run the server
server()

