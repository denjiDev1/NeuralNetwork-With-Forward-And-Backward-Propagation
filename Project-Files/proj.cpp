#include <iostream>
#include <mutex>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <stack>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

stack<int> backtrack;
int priority = 0;
int input_neurons = 2;
int neurons_perlayer = 8;
int layers = 7;
int cc = 0;

sem_t semaphore;

double overall[6][8][8] = {
-0.2, 0.3, -0.4, 0.5, 0.6, -0.7, 0.8, -0.9,
0.1, 0.9, -0.3, 0.2, -0.5, 0.4, 0.6, -0.8,
-0.7, 0.5, 0.8, -0.2, -0.3, -0.6, 0.1, 0.4,
-0.2, 0.3, -0.4, 0.5, 0.6, -0.7, 0.8, -0.9,
0.1, 0.9, -0.3, 0.2, -0.5, 0.4, 0.6, -0.8,
-0.7, 0.5, 0.8, -0.2, -0.3, -0.6, 0.1, 0.4,
-0.2, 0.3, -0.4, 0.5, 0.6, -0.7, 0.8, -0.9,
0.1, 0.9, -0.3, 0.2, -0.5, 0.4, 0.6, -0.8,
0.2, -0.3, 0.4, -0.5, -0.6, 0.7, -0.8, 0.9,
-0.1, -0.9, 0.3, -0.2, 0.5, -0.4, -0.6, 0.8,
0.7, -0.5, -0.8, 0.2, 0.3, 0.6, -0.1, -0.4,
0.2, -0.3, 0.4, -0.5, -0.6, 0.7, -0.8, 0.9,
-0.1, -0.9, 0.3, -0.2, 0.5, -0.4, -0.6, 0.8,
0.7, -0.5, -0.8, 0.2, 0.3, 0.6, -0.1, -0.4,
0.2, -0.3, 0.4, -0.5, -0.6, 0.7, -0.8, 0.9,
-0.1, -0.9, 0.3, -0.2, 0.5, -0.4, -0.6, 0.8,
0.3, -0.4, 0.5, -0.6, -0.7, 0.8, -0.9, 0.1,
-0.2, -0.9, 0.4, -0.3, 0.5, -0.6, -0.8, 0.1,
0.6, -0.5, -0.7, 0.2, 0.4, 0.8, -0.1, -0.3,
0.3, -0.4, 0.5, -0.6, -0.7, 0.8, -0.9, 0.1,
-0.2, -0.9, 0.4, -0.3, 0.5, -0.6, -0.8, 0.1,
0.6, -0.5, -0.7, 0.2, 0.4, 0.8, -0.1, -0.3,
0.3, -0.4, 0.5, -0.6, -0.7, 0.8, -0.9, 0.1,
-0.2, -0.9, 0.4, -0.3, 0.5, -0.6, -0.8, 0.1,
0.4, -0.5, 0.6, -0.7, -0.8, 0.9, -0.1, 0.2,
-0.3, -0.8, 0.5, -0.4, 0.6, -0.7, -0.9, 0.2,
0.5, -0.4, -0.6, 0.3, 0.2, 0.8, -0.2, -0.1,
0.4, -0.5, 0.6, -0.7, -0.8, 0.9, -0.1, 0.2,
-0.3, -0.8, 0.5, -0.4, 0.6, -0.7, -0.9, 0.2,
0.5, -0.4, -0.6, 0.3, 0.2, 0.8, -0.2, -0.1,
0.4, -0.5, 0.6, -0.7, -0.8, 0.9, -0.1, 0.2,
-0.3, -0.8, 0.5, -0.4, 0.6, -0.7, -0.9, 0.2,
0.5, -0.6, 0.7, -0.8, -0.9, 0.1, -0.2, 0.3,
-0.4, -0.7, 0.6, -0.5, 0.8, -0.6, -0.2, 0.1,
0.4, -0.3, -0.5, 0.1, 0.6, 0.7, -0.3, -0.2,
0.5, -0.6, 0.7, -0.8, -0.9, 0.1, -0.2, 0.3,
-0.4, -0.7, 0.6, -0.5, 0.8, -0.6, -0.2, 0.1,
0.4, -0.3, -0.5, 0.1, 0.6, 0.7, -0.3, -0.2,
0.5, -0.6, 0.7, -0.8, -0.9, 0.1, -0.2, 0.3,
-0.4, -0.7, 0.6, -0.5, 0.8, -0.6, -0.2, 0.1,
-0.1,   0,   0,    0,   0,    0 ,  0,    0,
0.2,    0,   0,   0,    0,     0,   0,   0,
0.3,0,0,0,0,0,0,0,
0.4,0,0,0,0,0,0,0,
0.5,0,0,0,0,0,0,0,
-0.6,0,0,0,0,0,0,0,
-0.7,0,0,0,0,0,0,0,
0.8 , 0,0,0,0,0,0,0,

};

double output_weights[8] = {-0.1,
0.2,
0.3,
0.4,
0.5,
-0.6,
-0.7,
0.8};


void* func(void * args)
{
    
    int* index = (int*)args;
    char* param = new char [1000]{'\0'};
    double ** temp = new double * [neurons_perlayer];
    for(int i =0 ; i < neurons_perlayer ; i++)
    {
        temp[i] = new double[neurons_perlayer];
    }
    int var =0;

    string st = to_string(neurons_perlayer);
    for(int i =0 ; st[i]!='\0';i++  )
    {
        param[var] = st[i];
        var++;
    }
    param[var] = ',';
    var++;

    st = to_string(*index);
    for(int i =0 ; st[i]!='\0';i++  )
    {
        param[var] = st[i];
        var++;
    }
    param[var] = ',';
    var++;


    for(int i =0 ; i < neurons_perlayer ; i++)
    {
        for(int j =0 ; j < neurons_perlayer ; j++)
        {
            temp[i][j] = overall[(*index)-1][i][j];
            string sc = to_string(temp[i][j]);
            for(int k =0 ; sc[k]!='\0' ; k++)
            {
               param[var]= sc[k];
               var++;   
            }
            param[var] = ',';
            var++;
        }
    }
    for(int i =0 ; param[i]!='\0' ; i++)
    {
        cout << param[i];
    }
    int inds = *index;
    char* b = new char[8]{'.','/','h','f','i','l','e'}; 
    char* argss[] = { b , param,NULL};
    int tt = priority;
    priority = *index;
    cc++;
    backtrack.push(cc);
    execvp("./hfile" ,argss );
    exit(0);

}




int main()
{
    cout << "\n******* Neural Network Architecture *******\n\n";
    double* input_layer = new double [neurons_perlayer];

    double h0[2][8] = {0.1, -0.2, 0.3, 0.1, -0.2, 0.3, 0.1, -0.2,-0.4, 0.5, 0.6, -0.4, 0.5, 0.6, -0.4, 0.5};

    double * inputs = new double[input_neurons];
    for(int i =0 ; i < input_neurons ; i++)
    {
        cout <<"Enter Value of Neuron "<<i+1<<" : ";
        cin >> inputs[i];
    }


    for(int kk =0 ; kk < 2 ; kk++)
    {

        for(int j =0 ; j< neurons_perlayer ; j++)
        {
            input_layer[j] = 0;
        }

        for(int i =0 ; i < input_neurons ; i++)
        {
            for(int j =0 ; j< neurons_perlayer ; j++)
            {
                input_layer[j] += inputs[i] * h0[i][j];
            }       
        }
        
        string pipe_name = "pipe";
        pipe_name+=to_string(0);

        string lindex = to_string(layers-1);
        lindex += ',';
        int lind = mkfifo("lindex",0666);
        int model = open("lindex",O_RDWR);
        write(model,lindex.c_str(),lindex.length());


        int v = mkfifo(pipe_name.c_str(),0666);
        int mode = open(pipe_name.c_str(),O_RDWR);

        string arr = "";
        for(int i=0;i<neurons_perlayer ; i++)
        {
            arr += to_string(input_layer[i]);
            arr+=',';
        }
        arr+='\0';
        write(mode,arr.c_str(),arr.length());
        cout << "\n\n***** Forward Pass "<<kk+1<<" ***** \n\n";
            
            for(int i=1;i<layers;i++)
            {
                int * j = new int;
                *j = i;
                int pid = fork();
                if(pid == 0)
                {
                    sleep(i);
                    string pip = "pipe";
                    pip+=to_string(i);
                    int v = mkfifo(pip.c_str(),0666);

                    func((void*)j);
                    exit(0);
                }
            
            }
                while (wait(NULL) > 0){};
            
        
        sleep(1);

        double* arrayoutput = new double[neurons_perlayer];
        int cc2=0;
        char buff[256];
        string val="";    
        int rde = open("pipe0",O_RDWR  | O_NONBLOCK   );
        read(rde,buff,256);
        write(rde,buff,256);
        for(int i =0 ; buff[i]!='\0' ; i++ )
        {
            if(buff[i]!=',' && buff[i] !=' ')
            {
                val+=buff[i];
            }
            else if(buff[i] == ',')
            {
                arrayoutput[cc2] = stod(val);
                cc2++;
                val="";
            }
        }
        cout <<"0 output : "<<arrayoutput[0] <<endl;
        cout <<"x1       : "<<arrayoutput[1] << "\n" <<"x2       : "<< arrayoutput[2] <<endl;

        for(int i =0 ; i< input_neurons ; i++)
        {
            inputs[i] = arrayoutput[i+1];
        }   

    }

}