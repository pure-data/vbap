# Makefile to build class 'vbap' for Pure Data.
# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build
# settings and rules.

# library name
lib.name = vbap

# input source file (class name == source file basename)
class.sources = rvbap.c define_loudspeakers.c
vbap.class.sources = vbap.c define_loudspeakers.c

# all extra files to be included in binary distribution of the library
datafiles = define_loudspeakers.h max2pd.h vbap.h \
            README.md LICENSE.txt CHANGELOG.txt \
            $(wildcard *.pd) $(wildcard examples/*.pd)

# include Makefile.pdlibbuilder, allow override using PDLIBBUILDER_DIR variable
PDLIBBUILDER_DIR=.
include $(PDLIBBUILDER_DIR)/Makefile.pdlibbuilder
