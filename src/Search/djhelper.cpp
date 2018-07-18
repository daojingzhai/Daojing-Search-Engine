#include"djhelper.h"
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include<fstream>
#include<string>
#include<iostream>
using namespace std;
void welcome()
{
	CLEAR();
	printf(THK""RED"WELCOME TO Daojing Search Engine"CLR);
	printf(THK""GRN"START SEARCH OR ENTER \033[31mMANUAL "GRN"TO READ THE USR MANUAL, ENTER \033[31mQ "GRN"TO EXIT\n");
}

// manual
void print_manual()
{
		fstream rdme;
		rdme.open(README_path);
		string buf;
		while(getline(rdme,buf))
			cout<<buf<<endl;
		while(1){
			cout<<"输入1以继续:";
			string i;
			cin>>i;
			if(i=="1")
			break;
		}

}

int Partition(int low, int high,int *p,int *value){
    int pivotpos = low;
    int pivot = value[p[low]];
		int temp=p[low];
    for (int i=low+1; i<=high; i++) {
        if (value[p[i]]<pivot) {
            pivotpos++;
            if (pivotpos!=i) {
                int temp = p[pivotpos];
                p[pivotpos] = p[i];
                p[i] = temp;
            }
        }
    }
    p[low] = p[pivotpos];
    p[pivotpos] = temp;
    return pivotpos;
}

void QuickSort(int left, int right,int *p,int *value){
    if (left<right) {
        int pivotpos = Partition(left, right,p,value);
        QuickSort(left, pivotpos-1,p,value);
        QuickSort(pivotpos+1, right,p,value);
    }
    return;
}

// Quicksort algorithm
void mysort(int num,int *p,int *value){
		QuickSort(0,num-1,p,value);
}
