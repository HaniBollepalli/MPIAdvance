#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <mpi.h>

#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif

using namespace std;  
int main (int argc, char* argv[]) {
  const clock_t begin_time = clock();
	if (argc < 6) 
  {
    std::cerr<<"Usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <scheduling> <granularity>"<<std::endl;
    return -1;
  }
  
  int a=atoi(argv[2]);
  int b=atoi(argv[3]);
  int n=atoi(argv[4]);
  //float result1=(b-a)/n;
  

  int intensity=atoi(argv[5]);
  float tmp=(float)(b-a)/(float)n;
  int function_call=atoi(argv[1]);
  
  
   MPI_Init(&argc, &argv);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;

    MPI_Get_processor_name(processor_name, &name_len);
    float global_sum=0.0;

	int number=0;
//:cout<<"world"<<world_rank<<endl;	
	
			double t1 = MPI_Wtime();
		
	if(world_rank==0)
		
		{ 
		
		int chunk=n/(world_size-1);
		
		//cout<<"chunk"<<chunk<<"xx";
		float result;
			for(int i=1;i<world_size;i++)
			{
				int start=(i-1)*chunk;
				float param[7];
				int end=start+(chunk);
				if(end>n|| i==world_size-1)
				{
					end=n;
					
				}
		
				param[0]=start;
				param[1]=end;
				param[2]=tmp;
				param[3]=intensity;
				param[4]=function_call;
				param[5]=a;
				param[6]=b;
				
				
				MPI_Send(&param, 7, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
		
			}
		
		
		for (int i = 1; i < world_size; i++ ) 
    {	

			 float status;
             MPI_Recv(&result,1, MPI_FLOAT, i,0,  MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
		//cout<<"result _received"<<i<<","<<result<<endl; 
			 global_sum+=(float)result;
			
		}

        global_sum=tmp*global_sum;
if(world_rank==0)
{
          cout<<global_sum<<"\n";
cerr<<MPI_Wtime()-t1;
}


}

		else
			
			{


  


	float *parameter=(float *)malloc(7 * sizeof(int));   ;
			
			MPI_Recv(parameter,7, MPI_FLOAT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

	int start=(int)parameter[0];
	int end=(int)parameter[1];
	int a=(int)parameter[5];
	int b=(int)parameter[6];
	float tmp=parameter[2];
	int intensity=(int)parameter[3];


	



	
    float integral=0.0;
	for(int i=start;i < end;i++)  
	  {

		
		  float x=(float)a+((float)i+0.5)*(float)(tmp);
		 
		  switch(atoi(argv[1]))
		  {  
			  case 1:integral+=f1(x,intensity);
	       			         break; 
			  case 2:integral+=f2(x,intensity);
					 break;  
			  case 3:integral+=f3(x,intensity);
					  break;
			  case 4:integral+=f4(x,intensity);
				      break;
		  }
	  }
		
    MPI_Send(&integral, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
	  	 

 
			}




// Finalize the MPI environment.
   MPI_Finalize();
//cout<<global_sum<<"\n";  
//cerr << float( clock () - begin_time )/1000;



  //return 0;
}
