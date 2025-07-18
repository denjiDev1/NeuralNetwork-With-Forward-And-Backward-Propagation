#include <iostream>
#include <mutex>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <cmath>
#include <stack>
using namespace std;

int * neuro = new int;
double ** finaltemp;
double* ans ;
double* array2;
sem_t sems;

void* funs(void * arg)
{
    sem_wait(&sems);
    int* temp = (int*)arg;
    int i = *temp; 
        for(int j =0 ; j< *neuro ; j++)
        {
            ans[j] += array2[i] * finaltemp[i][j];
        }      
    sem_post(&sems);
    pthread_exit(NULL);
}

int main(int argc, char* arg[])
{
    char * param = arg[1];
    sem_init(&sems, 0, 1);
    string st="";
    int count = 0;
    int arr[2];
    int xtr;

    for(int i=0;i<100;i++)
    {
        if(param[i]!=',')
        {
            st+=param[i];
        }

        else if(param[i] == ',')
        {
            arr[count] = stoi(st);
            count++;
            st = "";
        }
        if(count==2)
        {
            xtr = i+1;
            break;
        }

    }

    int neurons_perlayer = arr[0];
    int index = arr[1];

    double * temp = new double [neurons_perlayer * neurons_perlayer];

    string str1 = "";
    for(int i=xtr,j=0;param[i]!= '\0';i++)
    {
        if(param[i]!=',')
        {
            str1+=param[i];
        }
        if(param[i]==',')
        {
            temp[j] = stod(str1);
            str1 = "";
            j++;
        }
    }

    finaltemp = new double *[neurons_perlayer];
    for(int i=0;i<neurons_perlayer;i++)
    {
        finaltemp[i] = new double[neurons_perlayer];
    }



    int aise = 0;
    for(int i=0;i<neurons_perlayer;i++)
    {
        for(int j=0;j<neurons_perlayer;j++)
        {
            finaltemp[i][j] = temp[aise];
            aise++;
        }
    }
       // sleep(index);
    cout <<index << " : ";
    *neuro = neurons_perlayer;
    
    string pipename ="pipe"; 
    pipename +=to_string(index-1);

    ans = new double[neurons_perlayer]{0};
    array2 = new double[neurons_perlayer];

    int cc2=0;
    char buff[256];
    string val="";
    int rd = open(pipename.c_str(),O_RDWR);
    read(rd,buff,256);
    write(rd,buff,256);
    for(int i =0 ; buff[i]!='\0' ; i++ )
    {
        if(buff[i]!=',')
        {
            val+=buff[i];
        }
        else if(buff[i] == ',')
        {
            array2[cc2] = stod(val);
            val="";
        }

    }

    pthread_t * threads = new pthread_t[neurons_perlayer];
    for(int i =0 ; i < neurons_perlayer ; i++)
    {
        int * j = new int;
        *j = i;
        pthread_create(&threads[i] , NULL , funs ,(void*)j );
         
    }

    
    string pipe_name = "pipe";
    pipe_name+=to_string(index);

    //int v = mkfifo(pipe_name.c_str(),0666);
    int mode = open(pipe_name.c_str(),O_RDWR);

    string arrs = "";
    for(int i=0;i<neurons_perlayer ; i++)
    {
        arrs += to_string(ans[i]);
        arrs+=',';
    }
    arrs+='\0';
    write(mode,arrs.c_str(),arrs.length());

    char buffer[256];
    int rd2 = open("lindex",O_RDWR);
    read(rd2,buffer,256);
    write(rd2,buffer,256);
    string lind ="";

    lind +=buffer[0];
    lind+='\0';

    int last;
    //cout << lind << to_string(index)<<endl;
    
    if(stoi(lind) == index )
    {
        double x1 = (pow(ans[0],2) + ans[0] + 1)/2.0;
        double x2 = (pow(ans[0],2) - ans[0])/2.0;

        string finaloutput = "finaloutput";
        int sc = mkfifo("finaloutput",0666);
        int modes = open(finaloutput.c_str(),O_RDWR);
        string arrsh  = to_string( ans[0]);
        arrsh += ',';
        arrsh += to_string(x1);
        arrsh += ',';
        arrsh += to_string(x2);
        arrsh +=',';
        arrsh+='\0';
        //cout << arrsh << endl;
        write(modes,arrsh.c_str(),arrsh.length());
        string finaloutput1 = "pipe0";
        char tempt[256];
        int modes1 = open(finaloutput1.c_str(),O_RDWR);
        read(modes1,tempt,256);
        write(modes1,arrsh.c_str(),arrsh.length());

       
    }
          cout << arrs <<endl;

        sleep(stoi(lind)+1);    
        char buff2[256];
        int rde = open("finaloutput",O_RDWR     );
        read(rde,buff2,256);
        write(rde,buff2,256);
        cout <<index << " output : ";
        for(int i =0 ; buff2[i]!='\0' ; i++)
        {
            if(buff2[i] == ',')
            {
                break;
            }
            cout << buff2[i];
        }
        cout << endl;
            write(rd2,buffer,256);

        return 0;
}