import sys, serial


if len(sys.argv) < 4:
    print("usage: uartload.py <file in> <file out> <serialport>")
    sys.exit(-1)


try:
    fin = open(sys.argv[1], 'rb')
except FileNotFoundError:
    print("could not open input file")
    sys.exit(-1)

try:
    fout = open(sys.argv[2], 'wb')
except FileNotFoundError:
    print("could not open output file")
    sys.exit(-1)

try:
    ser = serial.Serial(sys.argv[3], 115200, timeout=None)
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

fout.write(ser.read(0x400000))

fout.close()
ser.close()

print("done!")