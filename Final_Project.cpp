#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"
#include <iomanip>
#include <fstream>
#include <iostream>

#define k 8
#define p 12
#define sample_count 32000
#define N 5
#define M 8
#define T 160

int W[12]={1,3,7,13,19,22,25,33,42,50,56,61};
int length,obs[T],result[3];
int pi[N]={1,0,0,0,0};
long double sample[sample_count],test[320],Ri[p+1],Ai[p+1],Ci[p+1],temp[p+1][p+1],D[k];
long double centroid[k][p];
long double A[N][N],B[N][M],alpha[T][N],beta[T][N],gamma[T][N],delta[T][N],psi[T][N],xi[T][N][N],prob_d[10],prob_o[4];
long double p_star_old,p_star_new;

void initialise_centroids();
void read_data();
void process_digit_models();
void process_operator_models();
void read_file(char *,char *);
void dcshift();
void normalise();
void process_data();
void initialise_arrays();
void calculate_Ci();
long generate_observation();
long double solution1();
void predict_digit(int);
void predict_operator();
void final_result();

using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	//long i;
	//initialise_centroids();
	//for(i=0;i<=1;i++)
	//{
		read_data();
		dcshift();
		normalise();
		process_data();
		//process_digit_models();
		//predict_digit(i);
	//}
	//read_data();
	//dcshift();
	//normalise();
	//process_data();
	//process_operator_models();
	//predict_operator();
	//final_result();
	while(1);
	return 0;
}

void initialise_centroids()
{
	long i=0,j=0;
	long double x;
	ifstream infile;
	infile.open("Centroids.txt");
	while(infile >> x)
	{
		centroid[i][j++]=x;
		if(j%p==0)
		{
			i++;
			j=0;
		}
	}
	infile.close();
}

void read_data()
{
	long i=0,j=0;
	long double x;
	char command[1000]="D://IITG//Speech_Processing//Recording_Module//Recording_Module.exe 2 D://IITG//Speech_Processing//Final_Project//Final_Project//test.wav D://IITG//Speech_Processing//Final_Project//Final_Project//test.txt";
	system(command);
	ifstream infile;
	infile.open("D://IITG//Speech_Processing//Final_Project//Final_Project//test.txt");
	while(infile >> x)
		sample[i++]=x;
	infile.close();
}

void dcshift()
{
	long i;
	long double avg,sum=0;
	for(i=0;i<sample_count;i++)
		sum+=sample[i];
	avg=sum/sample_count;
	for(i=0;i<sample_count;i++)
		sample[i]-=avg;
}

void normalise()
{
	long i;
	long double max=sample[0];
	long double min=sample[0];
	for(i=0;i<sample_count;i++)
	{
		if(sample[i]>max)
			max=sample[i];
	}
	for(i=0;i<sample_count;i++)
	{
		if(sample[i]<min)
			min=sample[i];
	}
	for(i=0;i<sample_count;i++)
	{
		if(sample[i]>=0)
			sample[i]=sample[i]*5000/max;
		else
			sample[i]=sample[i]*5000/min*(-1);
	}
}

void process_data()
{
	length=0;
	long i,j=0,start,end,mark;
	for(i=0;i<sample_count;i++)
	{
		if(sample[i]>300.00000)
		{
			start=i;
			break;
		}
	}
	start-=1500;
	for(i=sample_count-1;i>=0;i--)
	{
		if(sample[i]>500.00000)
		{
			end=i;
			break;
		}
	}
	end+=1000;
	mark=start;
	while(mark<=end)
	{
		for(i=mark;i<320+mark;i++)
			test[j++]=sample[i];
		initialise_arrays();
		calculate_Ci();
		for(i=1;i<=p;i++)
			printf("%lf\t",Ci[i]);
		printf("\n");
		printf("\n");
		//obs[length]=generate_observation();
		j=0;
		mark+=80;
		length++;
	}
}

void process_digit_models()
{
	long i;
	for(i=0;i<10;i++)
	{
		switch(i)
		{
			case 0:
					read_file("A0.txt","B0.txt");
					prob_d[i]=solution1();
					break;
			case 1:
					read_file("A1.txt","B1.txt");
					prob_d[i]=solution1();
					break;
			case 2:
					read_file("A2.txt","B2.txt");
					prob_d[i]=solution1();
					break;
			case 3:
					read_file("A3.txt","B3.txt");
					prob_d[i]=solution1();
					break;
			case 4:
					read_file("A4.txt","B4.txt");
					prob_d[i]=solution1();
					break;
			case 5:
					read_file("A5.txt","B5.txt");
					prob_d[i]=solution1();
					break;
			case 6:
					read_file("A6.txt","B6.txt");
					prob_d[i]=solution1();
					break;
			case 7:
					read_file("A7.txt","B7.txt");
					prob_d[i]=solution1();
					break;
			case 8:
					read_file("A8.txt","B8.txt");
					prob_d[i]=solution1();
					break;
			case 9:
					read_file("A9.txt","B9.txt");
					prob_d[i]=solution1();
					break;
		}
	}
}

void process_operator_models()
{
	long i;
	for(i=0;i<4;i++)
	{
		switch(i)
		{
			case 0:
					read_file("A_plus.txt","B_plus.txt");
					prob_o[i]=solution1();
					break;
			case 1:
					read_file("A_minus.txt","B_minus.txt");
					prob_o[i]=solution1();
					break;
			case 2:
					read_file("A_multiply.txt","B_mutiply.txt");
					prob_o[i]=solution1();
					break;
			case 3:
					read_file("A_divide.txt","B_divide.txt");
					prob_o[i]=solution1();
					break;
		}
	}
}

void read_file(char str1[10], char str2[10])
{
	long i=0,j=0;
	long double x;
	ifstream infile;
	infile.open(str1);
	while(infile >> x)
	{
		A[i][j++]=x;
		if(j%N==0)
		{
			i++;
			j=0;
		}
	}
	infile.close();
	i=j=0;
	infile.open(str2);
	while(infile >> x)
	{
		B[i][j++]=x;
		if(j%M==0)
		{
			i++;
			j=0;
		}
	}
	infile.close();
}

void initialise_arrays()
{
	long i;
	for(i=0;i<=p;i++)
	{
		Ri[i]=0.0;
		Ai[i]=0.0;
		Ci[i]=0.0;
		for(j=0;j<=p;j++)
			temp[i][j]=0.0;
	}
}

void calculate_Ci()
{
	long i,j,l;
	long double E,K,sum;
	for(i=0;i<=p;i++)
	{
		for(j=0;j<=(319-i);j++)
				Ri[i]+=test[j]*test[j+i];
	}
	K=Ri[1]/Ri[0];
	temp[1][1]=K;
	E=(1-(K*K))*Ri[0];
	for(i=2;i<=p;i++)
	{
		K=Ri[i];
		for(j=1;j<=i-1;j++)
			K-=(temp[i-1][j])*(Ri[i-j]);
		K=K/E;
		temp[i][i]=K;
		for(j=1;j<=i-1;j++)
			temp[i][j]=temp[i-1][j]-(K*temp[i-1][i-j]);
		E=(1-(K*K))*E;
		if(i==p)
		{
			for(l=1;l<=p;l++)
				Ai[l]=temp[p][l]; 
		}
	} 
	for(i=1;i<=p;i++)
	{
		sum=0.0;
		for(j=1;j<=i-1;j++)
			sum+=j*Ci[j]*Ai[i-j]/i;
		Ci[i]=Ai[i]+sum;
	}	
}	

long generate_observation()
{
	long i,j,index=0;
	long double min,d;
	for(i=0;i<k;i++)
	{
		d=0;
		for(j=0;j<p;j++)
			d+=W[j]*(centroid[i][j]-Ci[j+1])*(centroid[i][j]-Ci[j+1]);
		D[i]=d;
	}
	min=D[0];
	for(i=0;i<k;i++)
	{
		if(D[i]<=min)
		{
			min=D[i];
			index=i;
		}
	}
	return index;
}

long double solution1()
{
	long i,j,t;
	long double sum;
	for(i=0;i<N;i++)
		alpha[0][i]=pi[i]*B[i][obs[0]];
	for(t=0;t<length-1;t++)
	{
		for(j=0;j<N;j++)
		{
			sum=0;
			for(i=0;i<N;i++)
				sum+=alpha[t][i]*A[i][j];
			alpha[t+1][j]=sum*B[j][obs[t+1]];
		}
	}
	sum=0;
	for(i=0;i<N;i++)
		sum+=alpha[length-1][i];
	return sum;
}

void predict_digit(int pos)
{
	long i,index=0;
	long double max=prob_d[0];
	for(i=0;i<10;i++)
	{
		if(prob_d[i]>max)
		{
			max=prob_d[i];
			index=i;
		}
	}
	result[pos]=index;
}

void predict_operator()
{
	long i,index=0;
	long double max=prob_o[0];
	for(i=0;i<4;i++)
	{
		if(prob_o[i]>max)
		{
			max=prob_o[i];
			index=i;
		}
	}
	result[2]=index;
}

void final_result()
{
	switch(result[2])
	{
		case 0:
				printf("%ld",result[0]+result[1]);
				break;
		case 1:
				printf("%ld",result[0]-result[1]);
				break;
		case 2:
				printf("%ld",result[0]*result[1]);
				break;
		case 3:
				if(!result[1])
					printf("Cannot divide by zero\n");
				else
					printf("%ld",result[0]/result[1]);
				break;
	}
}