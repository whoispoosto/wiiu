# wiiu
WiiU Gamepad fun

# Flash Dumper
```
make
python3 tools/flash_dump.py image.bin <output file name> <serial port>
```

This will take quite a bit of time. Each Home LED toggle (on -> off, off -> on) represents two writes. With a chunk size of 2048, 2048 reads are required, or 1024 Home LED toggles. However, the script should finish in under 10 minutes.

sha256sum: `aaab1f92857640d54350ffcdb67fa2ed4c2e356d2906e966a1385dde74eb2c8e`

# TODO
* Better UART debugging -- maybe a basic on-board shell?
* Cleaner Python scripting interface -- again, maybe a basic (host) shell
* ~~Flash dumper~~
* Flash programmer
* Flash reverse-engineering
* WiFI flashing
* DOOM
