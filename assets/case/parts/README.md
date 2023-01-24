# 🦋 701c.org / Case / Parts

Find more details on reproduction parts at [https://701c.org/projects/parts](https://701c.org/projects/parts).

> **Note**: From time to time there may be limited quantities of these parts available in the [701c.org Shop](https://701c.org/shop).

#

## Background

Clips, slides, port covers, and battery doors; these things can get easily damaged when the plastic begins to deteriorate or go missing entirely if they’re not fully attached. Here is a list of high-quality models I’ve made to replace the common components on the 701C body. 

As many of these parts are very detailed, I’ve included the settings I use below for high-resolution 3D SLA printers along with suggested resins.

#

## PCMCIA Cover with Popout

Download the STLs:
   * For the main cover [here](https://github.com/hrushka/701c.org/tree/main/src/ds2434/ds2434.ino).
   * For the popout [here](https://github.com/hrushka/701c.org/tree/main/src/ds2434/ds2434.ino).

### Example Printer Settings (SLA)

| Property      | Value |
| ----------- | ----------- |
| Printer      | Elegoo Saturn 2 8k |
| Resin MIX   | 60% ABS-Like / 40% F69 (Flexible) |
| Main Cover Angle | 48.743° |
| Main Cover Supports | Light |
| Popout Angle | Face down on build plate |
| Popout Supports | None, no raft |
| Layer Height | 25µm |
| Exposure Time | 5s |
| Bottom Exposure Time | 50s |
| Ambient Temperature | 31°C |

Note that there are two different techniques for printing the main cover and the popout. I have found that printing directly on the build plate has a higher chance of success when printing the small clips on the side of the popout, but that I get greater dimentsional accuracy when printing at the calculated angle for the main cover.

**See the note below about calculating the correct angle for your print.**

#

## RAM Door Cover

Download the STL [here](https://github.com/hrushka/701c.org/tree/main/src/ds2434/ds2434.ino).

### Example Printer Settings (SLA)

| Property      | Value |
| ----------- | ----------- |
| Printer      | Elegoo Saturn 2 8k |
| Resin MIX   | 60% ABS-Like / 40% F69 (Flexible) |
| Print Angle | Face down on build plate |
| Print Supports | None, no raft |
| Layer Height | 25µm |
| Exposure Time | 5s |
| Bottom Exposure Time | 50s |
| Ambient Temperature | 31°C |

I've had the best results with this print by printing directly on the build plate with the **clips facing up**. The base layers may come out a little thick on the sides, but light sanding with 1000 grit paper will make quick work of any overexposed layers.

#

## Floppy Disk (FDD) Port Cover

Download the STL [here](https://github.com/hrushka/701c.org/tree/main/src/ds2434/ds2434.ino).

### Example Printer Settings (SLA)

| Property      | Value |
| ----------- | ----------- |
| Printer      | Elegoo Saturn 2 8k |
| Resin MIX   | **100% F69 (Flexible)** |
| Print Angle | Face down on build plate |
| Print Supports | 2 small light supports for the "clip" overhang MAY be needed |
| Layer Height | **50µm** |
| Exposure Time | **7.6s** |
| Bottom Exposure Time | 50s |
| Ambient Temperature | 31°C |

You may get a clean print without supports, but I add two custom light supports to the overhang of the clip and then trim them after washing and before curing.

#

## Things to know...

1. Make sure your resin is mixed well and allow the resin to pre-heat in the enclosure or the tank for 30 minutes before printing. I have found that when using flexible resins like F69, temperature is incredibly important to prevent the resin from becoming too viscous.
   
2. If your printer is different or you print at a different layer height you must **recalculate the angle of the print**. I suggest using an angle calculator like the one provided by [RC87](https://www.rc87.blog/angle-calculator/).