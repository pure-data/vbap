# vbap

A Pure Data port of the VBAP object for Max/MSP

Originally written by Ville Pulkki  
Helsinki University of Technology  
Laboratory of Acoustics and Audio Signal Processing

http://www.acoustics.hut.fi/research/cat/vbap/

Algorithm and Max object
1998-2010 Ville Pulkki  
2002, Juha Vehviläinen  
2003, Olaf Matthes  

Pure Data port
2006-2010, Hans-Christoph Steiner  
2007, Frank Barknecht  
2020, Dan Wilcox  

## Description

This is a library for Pure Data for sound spatialization using the vector base
amplitude panning (VBAP) method. VBAP is an amplitude panning method to position
virtual sources in arbitrary 2-D or 3-D loudspeaker setups. In amplitude panning
the same sound signal is applied to a number of loudspeakers with appropriate
non-zero amplitudes. With 2-D setups VBAP is a reformulation of the existing
pair-wise panning method. However, differing from earlier solutions it can be
generalized for 3-D loudspeaker setups as a triplet-wise panning method. A sound
signal is then applied to one, two, or three loudspeakers simultaneously. VBAP
has certain advantages compared to earlier virtual source positioning methods in
arbitrary layouts. Previous methods either used all loudspeakers to produce
virtual sources, which results in some artefacts, or they used loudspeaker
triplets with a non-generalizable 2-D user interface.

The directional qualities of virtual sources generated with VBAP can be stated
as follows. Directional coordinates used for this purpose are the angle between
a position vector and the median plane (θcc), and the angle between a projection
of a position vector to the median plane and frontal direction (Φcc). The
perceived θcc direction of a virtual source coincides well with the VBAP panning
direction when a loudspeaker set is near the median plane. When the loudspeaker
set is moved towards a side of a listener, the perceived θcc direction is biased
towards the median plane. The perceived Φcc direction of an amplitude-panned
virtual source is individual and cannot be predicted with any panning law.

## Build

This is a pure C external and can be compiled simply with:

    make

## Usage

To load this external in Pd, include the directory containing the following
files in your Pd search paths:

* vbap.pd_*, rvbap.pd-*, define_loudspeakers.pd_*: compiled external objects
* vbap-help.pd, rvbap-help.pd, define_loudspeakers-help.pd: help files
