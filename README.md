# Magnify

## A magnifier to investigate raw and deconvoluted waveforms.

### Usage

Magnify is run from its source.

```
/path/to/magnify/magnify.sh /path/to/file.root [frame]
```

The `frame` names the type of output of the signal processing to display.  
This is likely either `wiener` or `gauss`.  Old prototype produced only a single `decon`.

The `magnify.sh` shell script wraps up a few ROOT commands.  To run them manually do:

```
cd scripts/
root -l loadClasses.C 'Magnify.C("path/to/rootfile")'
```

An (old prototype) example ROOT file of waveforms can be found at http://www.phy.bnl.gov/xqian/talks/wire-cell/2D_display_3455_0_0.root

If one omits the file name, a dialog will open to let user select the file:
```
cd scripts/
root -l loadClasses.C Magnify.C
```

### Screenshot

![screenshot](data/screenshot.png?raw=true "Screenshot")
