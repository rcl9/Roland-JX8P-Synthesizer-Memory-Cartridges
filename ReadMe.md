# Memory Cartridges for the Roland JX8P Synthesizer (from 1985)

This repository documents two designs for a programmable RAM cartridge for the Roland JX8P synthesizer which I had designed and built in late 1985. Prior to this design my only option would have been to buy an expensive Roland MC-16C Memory Cartridge which held just 16 patches. With my new design, the single plug-in cartridge could handle 256 patches and be programmed by an external computer.

Please also refer to my [Roland JX8P Synthesizer Patch Collection](https://github.com/rcl9/Roland-JX8P-Synthesizer-Patch-Collection).

Each RAM cartridge would allow for 32 patches selectable from 8 different banks. The binary images would be saved to/from the cartridges from an external Z80 computer that had a custom Roland-style connector and interface.

<div style="text-align:center">
<img src="/Images/1985_12 - Roland JX8P synth memory cartridges.jpg" alt="" style="width:70%; height:auto;">
</div>

<div style="text-align:center">
<img src="/Images/1985_12 - Roland JX8P synth memory cartridges %232.jpg" alt="" style="width:70%; height:auto;">
</div>

The 8K 6264 CMOS RAM chips were backed up from a 3V coin battery which was controlled by a Dallas DS1210 that converted the CMOS RAM into non-volatile memory.

The first variation of the design used a DIP switch array to choose the patch bank. The second, much more friendly and useful version, used a de-bounced push button to toggle through the 8 banks. It also contained a single digit LED display to show the current patch bank.

<img src="/Schematics/JX8P 256 Voice RAM Cartridge %231 - August 29 1985_Page_1.jpg" alt="" style="width:48%; height:auto;">   <img src="/Schematics/JX8P 256 Voice RAM Cartridge %232 - December 30 1985.jpg" alt="" style="width:48%; height:auto;">

An EPROM version was also created to store the same collection of patches in ROM.

Reading and writing the binary images to the RAM cartridges was done via my personally designed "Colossus Control Computer" and its 28 pin Roland-compatible connector interface:

<div style="text-align:center">
<img src="/Images/1985_03 - Colossus Control Computer.jpg" alt="" style="width:70%; height:auto;">
</div>

## Binary-Image to SysEx File Converter

The *RCL's Patch Converter* directory contains *convert.cpp* which is a simple C++ utility program I had written  to convert a 2048 byte JX8P binary memory image into corresponding SysEx files. 

The patches can be output as a single .syx file (+ a MIDI message to write each patch into the JX8P memory), or as separate .syx files (with no MIDI message to write the patch into the JX8P memory).
