import sys, serial


if len(sys.argv) < 3:
    print("usage: uartload.py <file> <serialport>")
    sys.exit(-1)


try:
    fin = open(sys.argv[1], 'rb')
except FileNotFoundError:
    print("could not open input file")
    sys.exit(-1)

try:
    ser = serial.Serial(sys.argv[2], 115200, timeout=1)
except serial.serialutil.SerialException:
    print("could not open serial port")
    fin.close()
    sys.exit(-1)

fin.seek(0, 2)
flen = fin.tell()
fin.seek(0, 0)

print("binary length: {} bytes".format(flen))

# send payload length
print("sending payload length...")
ser.write(flen.to_bytes(4, byteorder='little'))

# send binary
print("sending binary...")
ser.write(fin.read(flen))
fin.close()

print("reading...")

try:
    while True:
        if ser.in_waiting > 0:  # optional, avoids blocking
            line = ser.readline().decode('utf-8', errors='replace').strip()
            print(f"Received: {line}")
except KeyboardInterrupt:
    print("Interrupted by user")
finally:
    ser.close()

print("done!")