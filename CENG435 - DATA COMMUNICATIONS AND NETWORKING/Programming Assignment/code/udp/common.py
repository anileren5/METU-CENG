import struct
import hashlib
import socket

CHUNK_SIZE = 1024

# This function checks if all the files are received and it is used by the client as a precaution for the missing termination message
def areAllFilesReceived(SMALL_FILES, LARGE_FILES):
    flag = True
    FILES = SMALL_FILES + LARGE_FILES
    for FILE in FILES:
        for packet in FILE.packets:
            if packet == None:
                flag = False   
                break
    return flag

# This function checks if all the acks are received in a window
def isWindowSlidable(RECEIVED_ACKS, WINDOW_BASE, WINDOWS_END):
    isSlidable = True
    for i in range(WINDOW_BASE, WINDOWS_END+1):
        if RECEIVED_ACKS[i] == False:
            isSlidable = False
            break
    return isSlidable

# This function finds the missing ack packets in a window
def getMissingACKs(RECEIVED_ACKS, WINDOW_BASE, WINDOWS_END):
    missingACKs = []
    for i in range(WINDOW_BASE, WINDOWS_END+1):
        if RECEIVED_ACKS[i] == False:
            isSlidable = False
            missingACKs.append(i)
    return missingACKs

# This function is used by the client to tell its address to server
def sendClientAddress(UDPClientSocket, SERVER_ADDRESS, N):
    for i in range(N):
        UDPClientSocket.sendto("ALOHA".encode(),SERVER_ADDRESS)

# This function is used to get the adress of the client by the server. It is used at the beginning of the transfer
def getClientAddress(UDPServerSocket, N):
    start = True
    client_address = None
    for i in range(N):
        try:
            if start:
                _, client_address = UDPServerSocket.recvfrom(5)
                start = False
            else:
                UDPServerSocket.settimeout(0.01)
                _, client_address = UDPServerSocket.recvfrom(5)
        except socket.timeout:
            continue
    if N == 1:
        try:
            UDPServerSocket.settimeout(0.01)
            _, client_address = UDPServerSocket.recvfrom(1024)
        except socket.timeout:
            pass
    return client_address

# This function calculates checksum of a data
def calculate_checksum(data):
    return hashlib.sha256(data).digest()

# This function checks if the checksum in the header is equal to checksum calculated over the received packet
def check_checksum(data, checksum):
    return hashlib.sha256(data).digest() == checksum

# This function adds padding 0s at the end of a chunk if its size is smaller than CHUNK_SIZE
def add_padding(chunk):
    if (len(chunk) == CHUNK_SIZE):
        return chunk
    padding_size = CHUNK_SIZE - len(chunk)
    return chunk + b'0' * padding_size

# This function decomposes a pure packet into its subcomponents
def purePacketParser(purePacket): # This decomposes the packets in an ServerFileObjects
    type_header, file_no_header, packet_no_header, length_header = struct.unpack("!QQQQ", purePacket[:32])
    chunk_without_padding = purePacket[32:32+length_header]
    return type_header, file_no_header, packet_no_header, length_header, chunk_without_padding

# This function creates an eventual packet with the specified eventual packet number (sequence number) using a pure packet
def makeEventualPacket(purePacket, sequence_number): # This makes an eventual packet (sequence_number + pure packet + checksum)
    sequence_number_header = struct.pack('!Q', sequence_number)
    checksum = calculate_checksum(sequence_number_header + purePacket)
    eventualPacket = sequence_number_header + purePacket + checksum
    return eventualPacket

# This function decomposes an eventual packet into its subcomponents
def eventualPacketParser(eventualPacket):
    sequence_number_header = struct.unpack('!Q', eventualPacket[:8])
    sequence_number_header = sequence_number_header[0]
    purePacket = eventualPacket[:-32][8:]
    checksum = eventualPacket[-32:]
    type_header, file_no_header, packet_no_header, length_header, chunk_without_padding = purePacketParser(purePacket)
    calculated_checksum = calculate_checksum(eventualPacket[:-32])
    isCorrupt = calculated_checksum != checksum
    return sequence_number_header, type_header, file_no_header, packet_no_header, length_header, chunk_without_padding, isCorrupt

# This function creates an acknowledgement packet with the given eventual packet number
def makeACKPacket(eventualPacketNo):
    packet_no_header = struct.pack('!Q', eventualPacketNo)
    checksum = calculate_checksum(packet_no_header)
    return packet_no_header + checksum

# This function decomposes an acknowledgement packet into its subcomponents
def AckPacketParser(ackPacket):
    packet_no_header = struct.unpack('!Q', ackPacket[:8])
    packet_no_header = packet_no_header[0]
    checksum = ackPacket[8:]
    calculated_checksum = calculate_checksum(ackPacket[:8])
    isCorrupt = checksum != calculated_checksum
    return packet_no_header, isCorrupt

# This is a class for the server to keep track of the files it will send
class ServerFile:
    def __init__(self, type, file_no): 
        self.path = f"./server_objects/{'small' if type == 'S' else 'large'}-{file_no}.obj"
        self.packets = [] # Note that these packets are not eventual packets to be sent !!! They even do not include a checksum.
        self.number_of_packets = 0

        with open(self.path, "rb") as file:
            packet_no = 0
            while True:
                chunk = file.read(CHUNK_SIZE)
                if not chunk:
                    break

                # Form a pure packet
                type_header = struct.pack('!Q', 0 if type == 'S' else 1) # 0 means small 1 means large   
                file_no_header = struct.pack('!Q', file_no)
                packet_no_header = struct.pack('!Q', packet_no)
                length_header = struct.pack('!Q', len(chunk))
                padded_chunk = add_padding(chunk) # If the last chunk is smaller than CHUNK_SIZE then add padding 0s to the end

                packet = type_header + file_no_header + packet_no_header + length_header + padded_chunk # This is a pure packet

                self.packets.append(packet)
                self.number_of_packets += 1
                packet_no += 1

# This is a class for the server to keep track of the files it will send
class ClientFile:
    def __init__(self, type, file_no, number_of_packets): # type is either small or large
        self.type = type
        self.file_no = file_no
        self.path = f"./client_objects/{type}-{file_no}.obj"
        self.number_of_packets = number_of_packets
        self.packets = [None] *  number_of_packets
        self.isPacketReceived = [False] * number_of_packets

    # Writes the packets to corresponding path    
    def write(self):
        bytes = b''
        for packet in self.packets:
            bytes += packet
        with open(self.path, "wb") as file:
            file.write(bytes)

    def empty(self):
        self.packets = [None] *  self.number_of_packets
        self.isPacketReceived = [False] * self.number_of_packets