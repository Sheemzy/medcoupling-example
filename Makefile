##
## Makefile dédié à la compilation de programmes utilisant med/medcoupling et mpi depuis un env spack
## Vous pouvez changer les variables *_PREFIX_DIR pour spécifier l'emplacement
## où ces libs sont installées sur votre système
##

EXECUTABLE=main

HDF5_PREFIX_DIR=$(shell spack location -i hdf5)
MEDCOUPLING_PREFIX_DIR=$(shell spack location -i salome-medcoupling)
OPENMPI_PREFIX_DIR=$(shell spack location -i openmpi)
MED_PREFIX_DIR=$(shell spack location -i salome-med)

SOURCE_FILES=example.cpp

INCLUDES=-I$(HDF5_PREFIX_DIR)/include \
         -I$(MEDCOUPLING_PREFIX_DIR)/include \
         -I$(OPENMPI_PREFIX_DIR)/include \
         -I$(MED_PREFIX_DIR)/include

LIB_DIRS=-L$(HDF5_PREFIX_DIR)/lib \
         -L$(MEDCOUPLING_PREFIX_DIR)/lib \
         -L$(OPENMPI_PREFIX_DIR)/lib \
         -L$(MED_PREFIX_DIR)/lib

LIBS=-lmpi -lhdf5 -lmedcoupling -linterpkernel -lparamedmem -lmedloader -lparamedloader

RPATH_FLAGS=-Wl,-rpath,$(HDF5_PREFIX_DIR)/lib:$(MEDCOUPLING_PREFIX_DIR)/lib:$(OPENMPI_PREFIX_DIR)/lib:$(MED_PREFIX_DIR)/lib

all:
        mpicxx $(SOURCE_FILES) $(INCLUDES) $(LIB_DIRS) $(LIBS) $(RPATH_FLAGS) -o $(EXECUTABLE)

clean:
        rm -f $(EXECUTABLE)
