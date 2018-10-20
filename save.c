#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <math.h>
#include <fcntl.h>
#include "parser.h"
#include <string.h>

#define Number_of_patterns 4
#define Number_of_inputs  2
#define Number_of_hidden 2
#define Number_of_outputs 1

#define rando() ((double)rand()/((double)RAND_MAX+1))

int main() 
{
    int i;
    int j;
    int k;
    int p;
    int np;
    int op;
    int ranpat[Number_of_patterns + 1];
    int NumberLoop;
    int NumberPattern = Number_of_patterns;
    int NumberInput = Number_of_inputs;
    int NumberHidden = Number_of_hidden;
    int NumberOutput = Number_of_outputs;

    double Input[Number_of_patterns+1][Number_of_inputs+1] = {{0, 0, 0}, {0, 0, 0}, {0, 1, 0}, {0, 0, 1}, {0, 1, 1}};
    double Target[Number_of_patterns+1][Number_of_outputs+1] = {{0, 0}, {0, 0}, {0, 1}, {0, 1}, {0, 0}};

    double HiddenCompute[Number_of_patterns+1][Number_of_hidden+1];

    double WeightIn2Hidden[Number_of_inputs+1][Number_of_hidden+1];

    double Hidden[Number_of_patterns+1][Number_of_hidden+1];

    double OutputCompute[Number_of_patterns+1][Number_of_outputs+1];

    double WeightHidden2Out[Number_of_hidden+1][Number_of_outputs+1];

    double Output[Number_of_patterns+1][Number_of_outputs+1];

    double DeltaOutput[Number_of_outputs+1];

    double BackpropagationArray[Number_of_hidden+1];

    double DeltaHidden[Number_of_hidden+1];

    double DWeightIn2Hidden[Number_of_inputs+1][Number_of_hidden+1];

    double DWeightHidden2Out[Number_of_hidden+1][Number_of_outputs+1];

    double Error;

    double eta = 0.5;	
    double alpha = 0.9;	
    double InitWeights = 0.5;

    char *s1 = Read_file("weights1.txt");
    char *s2 = Read_file("weights2.txt");

    char key = 'a';
    while (key != 'y' && key != 'n')
    {
        printf("Do you want to load saved weights ? press y or n\n");
        scanf("%c", &key);
    }

    if(key == 'n')
    {
        for( j = 1 ; j <= NumberHidden ; j++)
        {
            for( i = 0 ; i <= NumberInput ; i++)
            {
                DWeightIn2Hidden[i][j] = 0.0 ;
                WeightIn2Hidden[i][j] = 2.0 * ( rando() - 0.5 ) * InitWeights ;
            }
        }


        for( k = 1 ; k <= NumberOutput ; k ++)
        {
            for( j = 0 ; j <= NumberHidden ; j++)
            {
                DWeightHidden2Out[j][k] = 0.0 ;              
                WeightHidden2Out[j][k] = 2.0 * ( rando() - 0.5 ) * InitWeights ;
            }
        }
    }
    else
    {
        for( j = 1 ; j <= NumberHidden ; j++)
        {
            for( i = 0 ; i <= NumberInput ; i++)
            {
                DWeightIn2Hidden[i][j] = 0.0 ;
                WeightIn2Hidden[i][j] = Read_string(s1, i, j);
            }
        }


        for( k = 1 ; k <= NumberOutput ; k ++)
        {
            for( j = 0 ; j <= NumberHidden ; j++)
            {
                DWeightHidden2Out[j][k] = 0.0 ;
                WeightHidden2Out[j][k] = Read_string(s2, j, k);
            }
        }
    }
    for( NumberLoop = 1 ; NumberLoop < 100000 ; NumberLoop++)
    {


        for( p = 1 ; p <= NumberPattern ; p++) 
        {
            ranpat[p] = p ;
        }
        for( p = 1 ; p <= NumberPattern ; p++)
        {
            np = p + rando() * ( NumberPattern + 1 - p ) ;
            op = ranpat[p] ; ranpat[p] = ranpat[np] ; ranpat[np] = op ;
        }

        Error = 0.0;

        for( np = 1 ; np <= NumberPattern ; np++)
        {

            p = ranpat[np];


            for( j = 1 ; j <= NumberHidden ; j++)
            {
                HiddenCompute[p][j] = WeightIn2Hidden[0][j];
                for( i = 1 ; i <= NumberInput ; i++)
                {
                    HiddenCompute[p][j] += Input[p][i] * WeightIn2Hidden[i][j];
                }
                Hidden[p][j] = 1.0/(1.0 + exp(-HiddenCompute[p][j]));
            }


            for( k = 1 ; k <= NumberOutput ; k++)
            {
                OutputCompute[p][k] = WeightHidden2Out[0][k];


                for( j = 1 ; j <= NumberHidden ; j++ )
                {
                    OutputCompute[p][k] += Hidden[p][j] * WeightHidden2Out[j][k];
                }


                Output[p][k] = 1.0/(1.0 + exp(-OutputCompute[p][k]));
                Error += 0.5 * (Target[p][k] - Output[p][k]) * (Target[p][k] - Output[p][k]);
                DeltaOutput[k] = (Target[p][k] - Output[p][k]) * Output[p][k] * (1.0 - Output[p][k]);
            }


            for( j = 1 ; j <= NumberHidden ; j++)
            {
                BackpropagationArray[j] = 0.0 ;

                for( k = 1 ; k <= NumberOutput ; k++)
                {
                    BackpropagationArray[j] += WeightHidden2Out[j][k] * DeltaOutput[k];
                }

                DeltaHidden[j] = BackpropagationArray[j] * Hidden[p][j] * (1.0 - Hidden[p][j]);
                DWeightIn2Hidden[0][j] = eta * DeltaHidden[j] + alpha * DWeightIn2Hidden[0][j];
                WeightIn2Hidden[0][j] += DWeightIn2Hidden[0][j];

                for( i = 1 ; i <= NumberInput ; i++)
                { 
                    DWeightIn2Hidden[i][j] = eta * Input[p][i] * DeltaHidden[j] + alpha * DWeightIn2Hidden[i][j];
                    WeightIn2Hidden[i][j] += DWeightIn2Hidden[i][j];
                }

            }


            for( k = 1 ; k <= NumberOutput ; k ++)
            {
                DWeightHidden2Out[0][k] = eta * DeltaOutput[k] + alpha * DWeightHidden2Out[0][k];
                WeightHidden2Out[0][k] += DWeightHidden2Out[0][k];
                for( j = 1 ; j <= NumberHidden ; j++ )
                {
                    DWeightHidden2Out[j][k] = eta * Hidden[p][j] * DeltaOutput[k] + alpha * DWeightHidden2Out[j][k];
                    WeightHidden2Out[j][k] += DWeightHidden2Out[j][k];
                }
            }


        }
        if( NumberLoop%200 == 0 )
        {
            printf("%d\n", NumberLoop);
        }
        if( Error < 0.0001 )
        {
            break;
        }
    }

    FILE *f1 = fopen("weights1.txt", "w");

    for (j = 1 ; j <= NumberHidden ; j++)
    {
        fprintf(f1, "(");
        for (i = 0 ; i <= NumberInput ; i++)
        {
            fprintf(f1, "%f|", WeightIn2Hidden[i][j]);
        }
        fprintf(f1, ")");
    }

    fclose(f1);

    FILE *f2 = fopen("weights2.txt", "w");

    for (k = 1 ; k <= NumberOutput ; k++)
    {
        fprintf(f2, "(");
        for (j = 0 ; j <= NumberHidden ; j++)
        {
            fprintf(f2, "%f|", WeightHidden2Out[j][k]);
        }
        fprintf(f2, ")");
    }

    fclose(f2);

    for( p = 1 ; p <= NumberPattern ; p++)
    {
        printf("Inputs = ");
        for( i = 1 ; i <= NumberInput ; i++ )
        {
            printf("%f\t", Input[p][i]);
        }
        printf("\n");
        for( k = 1 ; k <= NumberOutput ; k++ )
        {
            printf("Target : %f\t Output : %f\n", Target[p][k], Output[p][k]) ;
        }
    }

    return 0 ;
}
