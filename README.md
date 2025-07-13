# wiiu
WiiU Gamepad fun

# Flash Dumper
```
make
python3 tools/flash_dump.py image.bin <output file name> <serial port>
```

This will take quite a bit of time. Each Home LED toggle (on -> off, off -> on) represents two writes. With a chunk size of 2048, 2048 reads are required, or 1024 Home LED toggles. However, the script should finish in under 10 minutes.

The `sha256sum` of your dump could differ depending on what firmware version(s) your Gamepad has. My partition 0 is `ec8f5da15b7129088f2b45b55b89f3f57c29ade440551801e18941b3f4088828`, and my partition 1 is `c1dcb66b56843e5de75bb2a451641e325dc5e596ba741cc82a5b7b151df1c977`. 

The best way to confirm a proper dump is to look for the [firmware partition table](https://kuribo64.net/wup/doku.php?id=flash_layout&s[]=lvc#firmware_partitions) at the start the file.

# Credits
Thank you to [Arisotura](https://github.com/Arisotura) for all her work in reverse-engineering the Gamepad. None of this would have been remotely possible without her help. The `lib/wup` library, `melon.ld`, and the `Makefile` are all taken from [here](https://github.com/Arisotura/melonpad), with some modifications.

# TODO
* Better UART debugging -- maybe a basic on-board shell?
* Cleaner Python scripting interface -- again, maybe a basic (host) shell
* ~~Flash dumper~~
* Flash programmer
* Uploading new firmware over WiFi
* DOOM
