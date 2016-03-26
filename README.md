### Cirugía - A Surgical Tool for NES ROMs

#### Features
* Return CRC and SHA1 checksums for ROM data
* Parse NES ROM headers (iNES and NES 2.0)
* Modify all fields in headers and convert between versions
* Apply IPS patches
* Create IPS patches by comparing ROMs

#### Compiling
CLI:
```
make
```
GTK+:
```
make -f Makefile.gtk
```

#### Examples
Set ROM's header to version 2, mapper 23, submapper 15:  
```
cirugia -v 2 -m 23 -s 15 -o newfile.nes oldfile.nes
```

Apply an IPS patch:  
```
cirugia -a patchfile.ips -o newfile.nes oldfile.nes
```

Create an IPS patch:  
```
cirugia -p patchfile.ips -x hackedfile.nes -o newfile.nes oldfile.nes
```

```
Usage: cirugia [options] [FILE]

Options:
  -v        Set Version (1-2)
  -m        Set Mapper (0-4095)
  -s        Set Submapper (0-15)
  -i        Set Mirroring (0-2)
  -j        Set PRG RAM Present (0-1)
  -t        Set Trainer (0-1)
  -q        Set System Type (0-2)
  -r        Set TV System/Region (0-2)
  -k        Set VS PPU Chip (0-12)
  -l        Set VS PPU Mode (0-3)
  -b        Set PRG ROM size (0-4095)
  -f        Set PRG RAM size (0-14)
  -g        Set PRG NVRAM size (0-14)
  -c        Set CHR ROM size (0-4095)
  -d        Set CHR RAM size (0-14)
  -e        Set CHR NVRAM size (0-14)
  -a        Apply IPS Patch (filename)
  -p        IPS Output File (filename)
  -x        Diff ROM (filename)
  -o        Output File (filename)
```
