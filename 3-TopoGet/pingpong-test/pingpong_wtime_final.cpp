#include <stdio.h>
#include "mpi.h"
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#define TEST_TIME 1000
//save the usecond time


int main(int argc, char **argv) {
    int my_rank;
    int p;
    int source;
    int dest;
    int tag = 50;
    char message[1000];
	char node[MPI_MAX_PROCESSOR_NAME];
    double **result_data;
	int len;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
	int pos = 0;

    result_data=new double*[p+1];
    for(int j=0;j<p;j++)
        result_data[j]=new double[p+1];

    //set every node
    for (pos = 0; pos < p; pos++) {
        //when node == pos
        if (my_rank == pos) {
            //send message from pos node to other nodes

            for (dest = 0; dest < p; dest++) {

                if (dest == pos)               
		{
		    	result_data[pos][pos]=0;
			continue;
		}

                sprintf(message, "test source %03d node to target %03d node", my_rank, dest);


                //get the send time
               
		result_data[my_rank][dest]=MPI_Wtime();

                int num = TEST_TIME;
                while (num--) {
                    MPI_Send(message, strlen(message) + 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

                    //printf("%s\n", message);

                    MPI_Recv(message, 1000, MPI_CHAR, dest, tag, MPI_COMM_WORLD, &status);
                }
                // sendtime - currentime = cost time
                result_data[my_rank][dest] = (MPI_Wtime() - result_data[my_rank][dest])/1000;

               // printf("4%s\n", message);
               // printf("time :%lf\n", result_data[my_rank][dest]);

            }


            MPI_Barrier(MPI_COMM_WORLD);


        } else {

            int num = TEST_TIME;
            while (num--) {
                //recv message from pos node
                MPI_Recv(message, 1000, MPI_CHAR, pos, tag, MPI_COMM_WORLD, &status);
                //printf("2%s\n", message);

                //return message to pos node
                MPI_Send(message, strlen(message) + 1, MPI_CHAR, pos, tag, MPI_COMM_WORLD);
                //printf("3%s\n", message);
            }
            MPI_Barrier(MPI_COMM_WORLD);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    char *result;
    result=new char[(p+1)*50];
    memset(result, 0, (p+1)*50 * sizeof(char));
	int i = 0;
	char Nodeinfo[100];
	MPI_Get_processor_name(node,&len);
	sprintf(Nodeinfo,"%8s\n",node);
	int nodelen=strlen(Nodeinfo);
	strcat(result,Nodeinfo);
	
    for (i = 0; i < p; ++i) {

        char tresult[100];
        sprintf(tresult, "node %03d to node %03d cost time: %09lf  s\n", my_rank, i, result_data[my_rank][i]);
        strcat(result, tresult);

    }

	int j = 0;
    if (my_rank == 0) 
    {
            char *result_for_file = (char *) malloc((strlen(result)) * p);

            strcpy(result_for_file, result);

            for ( i = 1; i < p; i++)
                MPI_Recv(result_for_file + i * strlen(result), strlen(result), MPI_CHAR, i, tag, MPI_COMM_WORLD, &status);

            FILE *fp = fopen("TimeCostDataFile", "w+");
            FILE *fp2 = fopen("nodefile", "w+");

            //fputs(result_for_file, fp);
            //printf("%s", result_for_file);
            
            int len=(strlen(result_for_file)-nodelen*p)/p/p;
            int resultLen=strlen(result);
    //        printf("%d\n",len);
        for( i=0;i<p;i++)
        {
            for(j=0;j<nodelen;j++)
                fprintf(fp2,"%c",result_for_file[i*resultLen+j]);
            for( j=0;j<p;j++)
            {
                int pos=(i*p+j)*len;
                pos+=nodelen*(i+1);
            

                int k = 30;
                for( k=32;k<=40;k++)
                {
                    fprintf(fp,"%c",result_for_file[pos+k]);
                }
                fprintf(fp," ");
            }
            fputc('\n', fp);
        }
        fclose(fp);
        fclose(fp2);
        free(result_for_file);
    } 
	else 
	{
        MPI_Send(result, strlen(result), MPI_CHAR, 0, tag, MPI_COMM_WORLD);
    }

   
    delete []result;
    for(int j=0;j<p;j++)
        delete []result_data[j];

    delete []result_data;

    MPI_Finalize();
}
