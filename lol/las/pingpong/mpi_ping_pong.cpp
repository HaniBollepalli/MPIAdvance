#include <mpi.h>
#include <iostream>

int main (int argc, char* argv[]) {

  if (argc < 2) {
    std::cerr<<"usage: mpirun "<<argv[0]<<" <value>"<<std::endl;
    return -1;
  }
	//initializing variables
	int value;

    //MPI initialization
	MPI_Init(NULL, NULL);

    //getting number of processes into nbprocess
	int nbprocess;
	MPI_Comm_size(MPI_COMM_WORLD, &nbprocess);

    //getting the rank of process
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	if (rank == 0){
	//reading the input values into corresponding variables
    	sscanf(argv[1], "%i", &value);
    //sending the value to process 1
    	MPI_Send(&value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    	
    //Receive the modified value 
    	MPI_Recv(&value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
    		
    //print out value
    	std::cout<<value<<std::endl;
    	}
    	
    else if (rank == 1) {
		//Receive the data sent by process 0
		MPI_Recv(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
		
		value = value + 2;
		
		//Sending back the modified value to process 0
		MPI_Send(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		
		}
	
	MPI_Finalize();
    return 0;
}
