# Magnify

## A magnifier to investigate raw and deconvoluted waveforms.

### Usage

```
cd scripts/
root -l loadClasses.C 'Magnify.C("path/to/rootfile")'
```

An example ROOT file of waveforms can be found at http://www.phy.bnl.gov/xqian/talks/wire-cell/2D_display_3455_0_0.root

If one omits the file name, a dialog will open to let user select the file:
```
cd scripts/
root -l loadClasses.C Magnify.C
```

### Screenshot

![screenshot](data/screeshot.png?raw=true "Screenshot")