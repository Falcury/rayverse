Disassembly of RAYMAN.EXE (original Rayman 1, DOS)

Files:
- readme.txt (this file)
- RAYMAN.EXE (the unpacked executable)
- RAYMAN.idb (the disassembly database, as of 21 July 2017)
- doc/cdtracks.txt (list of the audio tracks present on the Rayman CD)
- doc/levelnumbers.txt (list of level numbers)
- doc/rayman.cfg.txt (brief description of the file format of RAYMAN.CFG)
- doc/rayman.cfg.grammar (grammar file for opening/editor RAYMAN.CFG with Hexinator)


The IDA database can be opened with IDA 5.0.
You can download the freeware version of IDA Pro here:
https://www.hex-rays.com/products/ida/support/download_freeware.shtml


Note that the disassembly at this point is far from complete.
All suggestions are welcome.

Forum topic (Betilla's Gardens):
http://z15.invisionfree.com/Tehrunescape455/index.php?showtopic=1574


I used the following steps to generate the raw disassembly database from RAYMAN.EXE:
- Unpack the packed RAYMAN.EXE; I used the program PMWUNLIT.EXE (version 1.20) for this.
- Use IDA Free 4.1 to load the executable (version 5.0 does not have a loader for the Linear Executable format, but version 4.1 does)
- Open the database in IDA Free 5.0.

This procedure has been discussed here: 
http://www.vogons.org/viewtopic.php?t=30579