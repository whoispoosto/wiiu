import sys, serial, time

ARG_PROGRAM_NAME = 0
ARG_INPUT_BINARY = 1
ARG_PARTITION_0 = 2
ARG_PARTITION_1 = 3
ARG_SERIAL_PORT = 4
TOTAL_ARG_COUNT = 5

PARTITION_SIZE = 0x400000
BAUD_RATE = 115200

partitions = []

if len(sys.argv) < TOTAL_ARG_COUNT:
    print(f"usage: {sys.argv[ARG_PROGRAM_NAME]} image.bin <partition 0 output file> <partition 1 output file> <serial port>")
    sys.exit(-1)

try:
    fin = open(sys.argv[ARG_INPUT_BINARY], 'rb')
except FileNotFoundError:
    print("Could not open input binary")
    sys.exit(-1)

try:
    partitions.append(open(sys.argv[ARG_PARTITION_0], 'wb'))
except FileNotFoundError:
    print("Could not open partition 0 file")
    sys.exit(-1)

try:
    partitions.append(open(sys.argv[ARG_PARTITION_1], 'wb'))
except FileNotFoundError:
    print("Could not open partition 1 file")
    sys.exit(-1)

try:
    ser = serial.Serial(sys.argv[ARG_SERIAL_PORT], BAUD_RATE, timeout=None)
except serial.serialutil.SerialException:
    print("Could not open serial port")

    fin.close()
    
    for p in partitions:
        p.close()

    sys.exit(-1)

# Get input binary length
fin.seek(0, 2)
flen = fin.tell()
fin.seek(0, 0)

print("Binary length: {} bytes".format(flen))

print("Sending binary length...")
ser.write(flen.to_bytes(4, byteorder='little'))

print("Sending binary...")
ser.write(fin.read(flen))
fin.close()

start_time = time.time()

for (i, partition) in enumerate(partitions):
    print(f"Reading partition {i}...")
    partition.write(ser.read(PARTITION_SIZE))
    partition.close()

ser.close()

end_time = time.time()
delta_time = end_time - start_time

print(f"Done in {delta_time} seconds!")