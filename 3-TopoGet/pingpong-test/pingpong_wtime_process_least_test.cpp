#include <stdio.h>
#include "mpi.h"
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

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

    clock_t end1;
    clock_t end2;
    clock_t end3;
    clock_t end4;

    int begin=my_rank-my_rank%PROCESS_PER_NODE;
 //   int end=(begin+PROCESS_PER_NODE < p) ? begin+PROCESS_PER_NODE : p;
    int end=begin+PROCESS_PER_NODE;

    clock_t start = clock();

    int vp=0;
    if(p%20==0)
        vp=p;
    else
        vp=(p/20+1)*20;

    result_data=new double*[vp+1];
    for(int j=0;j<p;j++)
    {   
         result_data[j]=new double[vp+1];
         memset(result_data[j],0,sizeof(double)*(vp+1));
    }

    printf("my_rank: %d begin: %d end: %d\n",my_rank,begin,end);

    for(pos=begin;pos<end;pos++)
    {
        //set every node the first process
        if (my_rank == pos) {
            //send message from pos node to other nodes

            for (dest = begin; dest < end; dest++) {

                if(dest == my_rank)
                    continue;

                if(dest>=p)
                    continue;

                sprintf(message, "test source %04d node to target %04d node", my_rank, dest);

                //get the send time           
                result_data[my_rank][dest]=MPI_Wtime();

                int num,tnum;
                num=NODE_TEST_TIME;
                tnum=NODE_TEST_TIME;

                double firstTime=0.0;

                while (num--) {
                    MPI_Send(message, strlen(message) + 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

                    //printf("%s\n", message);

                    MPI_Recv(message, 1000, MPI_CHAR, dest, tag, MPI_COMM_WORLD, &status);
                    if(num == NODE_TEST_TIME -1)
                    {
                        firstTime=MPI_Wtime()-result_data[my_rank][dest];
                    }

                }
                // sendtime - currentime = cost time
                result_data[my_rank][dest] = (MPI_Wtime() - result_data[my_rank][dest])/(tnum-1);

                // printf("4%s\n", message);
                //printf("time :%lf\n", result_data[my_rank][dest]);
                printf("myrank: %d dest: %d time :%lf\n", my_rank,dest,result_data[my_rank][dest]);

            }
        } 
        else{

            if(pos<p)
            {
                int num = NODE_TEST_TIME;
                while (num--) {
                    //recv message from pos node
                    MPI_Recv(message, 1000, MPI_CHAR, pos, tag, MPI_COMM_WORLD, &status);
                    //printf("2%s\n", message);

                    //return message to pos node
                    MPI_Send(message, strlen(message) + 1, MPI_CHAR, pos, tag, MPI_COMM_WORLD);
                    //printf("3%s\n", message);
                }
            }
            
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    printf("go %d\n",my_rank);
    MPI_Barrier(MPI_COMM_WORLD);
    end1 = clock();
    printf("1total time: %lf\n",(double)(end1-start)/ CLOCKS_PER_SEC);

    char *result;
    result=new char[(p+1)*RESULT_MESSAGE_LEN];
    memset(result, 0, (p+1)*RESULT_MESSAGE_LEN * sizeof(char));
	int i = 0;
	char Nodeinfo[100];
	MPI_Get_processor_name(node,&len);
	sprintf(Nodeinfo,"%8s\n",node);
	int nodelen=strlen(Nodeinfo);
	strcat(result,Nodeinfo);
	
    for (i = begin; i < begin+PROCESS_PER_NODE; ++i) {

        char tresult[100];
        sprintf(tresult, "node %04d to node %04d cost time: %09lf  s\n", my_rank, i, result_data[my_rank][i]);
        //printf("myrank: %d time :%lf\n", my_rank,result_data[my_rank][i]);
        strcat(result, tresult);

    }

	int j = 0;
    if (my_rank == 0) 
    {
        char *result_for_file = (char *) malloc((strlen(result)) * p);

        strcpy(result_for_file, result);

        for ( i = 1; i < p; i++)
            MPI_Recv(result_for_file + i * strlen(result), strlen(result), MPI_CHAR, i, tag, MPI_COMM_WORLD, &status);

        end2 = clock();
        printf("2total time: %lf\n",(double)(end2-start)/ CLOCKS_PER_SEC);
//        printf("%s\n",result_for_file);
//        printf("node len: %d\n",nodelen);
        
        FILE *fp = fopen("TimeProcessCostDataFile", "w+");
        FILE *fp2 = fopen("nodefile", "w+");

        //fputs(result_for_file, fp);
        //printf("%s", result_for_file);
        
        int len=(strlen(result_for_file)-nodelen*p)/p/PROCESS_PER_NODE;
        int resultLen=strlen(result);
//        printf("%d\n",len);
        for( i=0;i<p;i++)
        {
            for(j=0;j<nodelen;j++)
                fprintf(fp2,"%c",result_for_file[i*resultLen+j]);
            for( j=0;j<PROCESS_PER_NODE;j++)
            {
                int pos=i*resultLen+j*len+nodelen;
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

        end3 = clock();   
        printf("3total time: %lf\n",(double)(end3-start)/ CLOCKS_PER_SEC);  

        fclose(fp);
        fclose(fp2);
        free(result_for_file);
    } 
	else// if(my_rank % PROCESS_PER_NODE == 0 && my_rank > 0)
	{
        MPI_Send(result, strlen(result), MPI_CHAR, 0, tag, MPI_COMM_WORLD);
    }
   
    if(my_rank == 0)
    {
        FILE *fp = fopen("TimeProcessCostDataFile", "r");
        for(int m=0;m<p;m++)
        {
            for(int k=0;k<PROCESS_PER_NODE;k++)
            {
                fscanf(fp,"%lf",&result_data[m][k]);
            }
        }
        fclose(fp);
/*
        fp = fopen("TimeProcessCostDataFile", "w+");
        for(int m=0;m<p;m++)
        {
            for(int n=0;n<p;n++)
            {
                fprintf(fp,"%lf ",result_data[m][n]);
            }
            fprintf(fp,"\n");
        }
        fclose(fp);
*/
        double cpu_cpu_time=0;
        int cpu_cpu_num=0;
        double process_process_time=0;
        int process_process_num=0;

        for(int m=0;m<p;m++)
        {
            for(int n=0;n<PROCESS_PER_NODE;n++)
            {
                if(n/(PROCESS_PER_NODE/2)==m%PROCESS_PER_NODE/(PROCESS_PER_NODE/2))
                {
                    cpu_cpu_num++;
                    cpu_cpu_time+=result_data[m][n];
                }    
                else
                {
                    process_process_num++;
                    process_process_time+=result_data[m][n];
                }    
            }
        }

        cpu_cpu_time=cpu_cpu_time/cpu_cpu_num;
        process_process_time=process_process_time/process_process_num;

        fp = fopen("TimeAverageCostDataFile", "a+");
        fprintf(fp,"%lf %lf\n",cpu_cpu_time,process_process_time);
        fflush(fp);
        fclose(fp);

        fp=fopen("TimeNodeCostDataFile","r");
        FILE *fp2=fopen("TimeCostDataFile","w+");

        int node_max_num=0;
        if(p%PROCESS_PER_NODE==0)
            node_max_num=p/PROCESS_PER_NODE;
        else
            node_max_num=p/PROCESS_PER_NODE+1;

        double **node_data=new double*[node_max_num];
        for(int m=0;m<node_max_num;m++)
            node_data[m]=new double[node_max_num];

        for(int m=0;m<node_max_num;m++)
        {
            for(int n=0;n<node_max_num;n++)
            {
                fscanf(fp,"%lf" , &node_data[m][n]);
            }
        }

        for(int m=0;m<p;m++)
        {
            for(int n=0;n<PROCESS_PER_NODE;n++)
            {
                if(n+m/PROCESS_PER_NODE*PROCESS_PER_NODE>=p)
                    continue;
                result_data[m][n+m/PROCESS_PER_NODE*PROCESS_PER_NODE]=result_data[m][n];
            }
        }

        for(int m=0;m<p;m++)
        {
            for(int n=m+1;n<p;n++)
            {
                if(m/20==n/20)
                    continue;
                result_data[m][n]=node_data[m/PROCESS_PER_NODE][n/PROCESS_PER_NODE];
//                printf("%d %d %d %d\n",m,n,m/PROCESS_PER_NODE,n/PROCESS_PER_NODE);
//                printf("%lf\n",result_data[m][n]);
                result_data[n][m]=result_data[m][n];
            }
        }

        for(int m=0;m<p;m++)
        {
            for(int n=0;n<p;n++)
            {
                fprintf(fp2,"%lf ",result_data[m][n]);
            }
            fprintf(fp2,"\n");
        }   

        fflush(fp2);
        fclose(fp);
        fclose(fp2);

        end4 = clock();   

        

        printf("4total time: %lf\n",(double)(end4-start)/ CLOCKS_PER_SEC);

        for(int m=0;m<node_max_num;m++)
            delete []node_data[m];
        delete []node_data;
    }

    delete []result;
    for(int j=0;j<p;j++)
        delete []result_data[j];

    delete []result_data;

    MPI_Finalize();
}
