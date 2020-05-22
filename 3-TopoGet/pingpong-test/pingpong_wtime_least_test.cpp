#include <stdio.h>
#include "mpi.h"
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>

#define TEST_TIME 1000
#define NODE_TEST_TIME 100
#define PROCESS_PER_NODE 20
#define RESULT_MESSAGE_LEN 50

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

    clock_t begin = clock();

    int node_num=ceil((double)p/PROCESS_PER_NODE);

    result_data=new double*[p+1];
    for(int j=0;j<p;j++)
    {   
         result_data[j]=new double[p+1];
         memset(result_data[j],0,sizeof(double)*(p+1));
    }
//    printf("test\n");
    for(pos=0;pos<p;pos+=PROCESS_PER_NODE)
    {
        //set every node the first process
        if (my_rank % PROCESS_PER_NODE == 0 && my_rank == pos) {
            //send message from pos node to other nodes

            for (dest = pos; dest < p; dest+=PROCESS_PER_NODE) {


                sprintf(message, "test source %04d node to target %04d node", my_rank, dest);

                if(dest==my_rank)
                    continue;

                //get the send time           
                result_data[my_rank][dest]=MPI_Wtime();

                int num,tnum;
                num=TEST_TIME;
                tnum=TEST_TIME;

                while (num--) {
                    MPI_Send(message, strlen(message) + 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

                    //printf("%s\n", message);

                    MPI_Recv(message, 1000, MPI_CHAR, dest, tag, MPI_COMM_WORLD, &status);
                }
                // sendtime - currentime = cost time
                result_data[my_rank][dest] = (MPI_Wtime() - result_data[my_rank][dest])/tnum;

                // printf("4%s\n", message);
//                printf("myrank: %d dest: %d time :%lf\n", my_rank,dest,result_data[my_rank][dest]);

            }
            MPI_Barrier(MPI_COMM_WORLD);
        } 
        else if(my_rank % PROCESS_PER_NODE == 0 && my_rank > pos) {


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
        else
            MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    char *result;
    result=new char[(p+1)*RESULT_MESSAGE_LEN];
    memset(result, 0, (p+1)*RESULT_MESSAGE_LEN * sizeof(char));
	int i = 0;
	char Nodeinfo[100];
	MPI_Get_processor_name(node,&len);
	sprintf(Nodeinfo,"%8s\n",node);
	int nodelen=strlen(Nodeinfo);
	strcat(result,Nodeinfo);
	
    if(my_rank % PROCESS_PER_NODE == 0)
    {
        for (i = 0; i < p; i+=PROCESS_PER_NODE) {

            char tresult[100];
            sprintf(tresult, "node %04d to node %04d cost time: %09lf  s\n", my_rank, i, result_data[my_rank][i]);
            //printf("myrank: %d time :%lf\n", my_rank,result_data[my_rank][i]);
            strcat(result, tresult);
        }
    }
    

	int j = 0;
    if (my_rank == 0) 
    {
        char *result_for_file = (char *) malloc((strlen(result)) * p);

        strcpy(result_for_file, result);

        for ( i = PROCESS_PER_NODE; i < p; i+=PROCESS_PER_NODE)
            MPI_Recv(result_for_file + (i/PROCESS_PER_NODE) * strlen(result), strlen(result), MPI_CHAR, i, tag, MPI_COMM_WORLD, &status);
        
//        printf("%s\n",result_for_file);

        FILE *fp = fopen("TimeNodeCostDataFile", "w+");
        FILE *fp2 = fopen("nodefile", "w+");

        //fputs(result_for_file, fp);
        //printf("%s", result_for_file);
        
        int len=(strlen(result_for_file)-nodelen*node_num)/node_num/node_num;
        int resultLen=strlen(result);
    //        printf("%d\n",len);
        for( i=0;i<node_num;i++)
        {
            for(j=0;j<nodelen;j++)
                fprintf(fp2,"%c",result_for_file[i*resultLen+j]);
            for( j=0;j<node_num;j++)
            {
                int pos=(i*node_num+j)*len;
                pos+=nodelen*(i+1);
            

                int k = 30;
                for( k=34;k<=42;k++)
                {
                    fprintf(fp,"%c",result_for_file[pos+k]);
                }
                fprintf(fp," ");
            }
            fputc('\n', fp);
        }

        fflush(fp);
        fflush(fp2);
        fclose(fp);
        fclose(fp2);
        free(result_for_file);
    } 
	else if(my_rank % PROCESS_PER_NODE == 0 && my_rank > 0)
	{
        MPI_Send(result, strlen(result), MPI_CHAR, 0, tag, MPI_COMM_WORLD);
    }
   
    if(my_rank == 0)
    {
        FILE *fp = fopen("TimeNodeCostDataFile", "r");
        for(int m=0;m<node_num;m++)
        {
            for(int k=0;k<node_num;k++)
            {
                fscanf(fp,"%lf",&result_data[m][k]);
            }
        }
        fclose(fp);
        
        for(int m=0;m<node_num;m++)
        {
            for(int n=0;n<m;n++)
            {
                result_data[m][n]=result_data[n][m];
            }
        }

        fp = fopen("TimeNodeCostDataFile", "w+");
        for(int m=0;m<node_num;m++)
        {
            for(int n=0;n<node_num;n++)
            {
                fprintf(fp,"%lf ",result_data[m][n]);
            }
            fprintf(fp,"\n");
        }
        fflush(fp);
        fclose(fp);

        double node_node_time=0;
        int node_node_num=0;

        for(int m=0;m<node_num;m++)
        {
            for(int n=0;n<node_num;n++)
            {
                if(m!=n)
                {
                    node_node_num++;
                    node_node_time+=result_data[m][n];
                }      
            }
        }

        if(node_node_num==0)
            node_node_time=0;
        else
            node_node_time=node_node_time/node_node_num;

        fp = fopen("TimeAverageCostDataFile", "w+");
        fprintf(fp,"%lf\n",node_node_time);
        fflush(fp);
        fclose(fp);

        clock_t results = clock();
        printf("total time: %lf\n",(double)(results - begin)/ CLOCKS_PER_SEC);


    }

    delete []result;
    for(int j=0;j<p;j++)
        delete []result_data[j];

    delete []result_data;

    MPI_Finalize();
}
