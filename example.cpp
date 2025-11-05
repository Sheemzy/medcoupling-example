#include <iostream>
#include <string>

#include "CommInterface.hxx"
#include "ComponentTopology.hxx"
#include "MEDFileMesh.hxx"
#include "MPIProcessorGroup.hxx"
#include "ParaDataArray.hxx"
#include "ParaFIELD.hxx"
#include "ParaMEDFileMesh.hxx"
#include "ParaMEDLoader.hxx"
#include "ParaMESH.hxx"
#include "ProcessorGroup.hxx"
#include "med.h"
#include "mpi.h"

using namespace MEDCoupling;

static ParaMESH read_mesh(const std::string& path,
                          MPIProcessorGroup& proc_group)
{
    const MEDFileMesh* file_mesh = MEDFileMesh::New(path.c_str());
    MEDCouplingMesh* mesh = file_mesh->getMeshAtLevel(0);
    file_mesh->decrRef();
    ParaMESH read_mesh(dynamic_cast<MEDCouplingPointSet*>(mesh), proc_group,
                       "mesh");
    mesh->decrRef();
    return read_mesh;
}

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    CommInterface comm_interface;
    MPIProcessorGroup world_pg(comm_interface);
    // world_pg.size() = comm_interface.commSize(...)
    // source:
    // https://github.com/SalomePlatform/medcoupling/blob/master/src/ParaMEDMEM/MPIProcessorGroup.cxx#L65

    int rank = world_pg.myRank();
    int world_size = world_pg.size();

    const std::string source_file_path = "mesh.med";
    ParaMESH source_mesh = read_mesh(source_file_path, world_pg);

    if (rank == 0)
        std::cout << "isStructured: " << source_mesh.isStructured()
                  << std::endl; // false

    // On doit obligatoirement avoir nb_components >= world_pg.size()
    // source:
    // https://github.com/SalomePlatform/medcoupling/blob/master/src/ParaMEDMEM/ComponentTopology.cxx#L29
    const size_t nb_components = world_size;
    ComponentTopology ct(nb_components, &world_pg);

    // La création d'un parafield avec ct.nb_blocks != 1 et world_pg->size() !=
    // 1 sur un maillage non structuré est interdite source:
    // https://github.com/SalomePlatform/medcoupling/blob/master/src/ParaMEDMEM/ParaFIELD.cxx#L75
    ParaFIELD field(ON_NODES, ONE_TIME, &source_mesh,
                    ct); // MEDCoupling::KERNEL_EXCEPTION

    // on pourrait ici charger nos données dans le parafield a partir d'un
    // fichier externe/une fonction mathématique

    world_pg.release();
    MPI_Finalize();
    return 0;
}
