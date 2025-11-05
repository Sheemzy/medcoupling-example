# Minimal not working example for ParaFIELD over multiple MPI processes

1. Clone the repository:
```shell
git clone https://github.com/Sheemzy/medcoupling-example.git && cd medcoupling-example
```

2. If you are not using a spack environment with:
- med >= 4.1.1
- medcoupling >= 9.13.0
- hdf5 >= 1.10.11
- openmpi >= 5.0.8

(which is very likely to happen)

Modify the Makefile's *_PREFIX_DIR variables to your own installation prefix of these 4 libs.

3. Compile the program using the Makefile, or a MPI compiler:
```shell
make
```

4. Run the program using MPI:
```shell
mpirun -n 3 ./main
```
