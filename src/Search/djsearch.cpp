#include"djsearch.h"
#include"djhelper.h"
#include<iostream>
#include<sstream>
#include<unistd.h>
#include<fcntl.h>
#include<cstdio>
#include<cstring>
#include<fstream>
#define max_search 20
using namespace std;
string search_list[max_search];
int search_list_num=0;
int search_result[max_page];
int result_num;
int avail_content[max_page];
int indegree_list[max_page];
char con_path[max_page][100];
int CMD_TYPE;

ifstream f_index,f_indegree,f_content[max_page];

// main search enginee
void search_main_loop(){
	welcome();
	if(init_base()<0) return;
	string ipt;
	while(1){
		init_search();
		cin>>ipt;
		if("DJMAN"==ipt)
		{
			print_manual();
			continue;
		}
		else if("Q"==ipt)	break;
		read_input(ipt);
		int check=search_engine();
		if(check==-1)
			printf(WARN"error searching"CLR);
		else if(check==0)
			printf("No Result\n");
		else{
			suc_search_dealer();
			CLEAR();
			continue;
		}
		while(1){
			cout<<"输入1以继续:";
			string i;
			cin>>i;
			if(i=="1")
			break;
		}
		CLEAR();
	}
	END_DEALER();
}

// read input key words
void read_input(string ipt){
	CMD_TYPE=CMD_NONE;
	int i=0;
	while(i<ipt.length()){
		int j=0;
		string temp="";
		while(ipt[j+i]!='&'&&ipt[i+j]!='|'&&j+i<ipt.length())
			{
				temp+=ipt[j+i];
				j++;
			}
			search_list[search_list_num]=temp;
			search_list_num++;
			if(j+i<ipt.length()){
				if(ipt[i+j]=='&')	CMD_TYPE=CMD_AND;
				else if(ipt[i+j]=='|') CMD_TYPE=CMD_OR;
			}
			i+=j+1;
	}
}

//search engine main function
void init_search(){
	result_num=0;
	search_list_num=0;
	memset(search_result,0,sizeof(search_result));
	printf("\033[0m");
	cout<<"搜索内容||Q";
	printf("=> ");
}

//initialization
int init_base(){
	memset(search_result,0,sizeof(search_result));
	memset(avail_content,0,sizeof(avail_content));
	if(access(index_path,R_OK)==0)
		f_index.open(index_path,ios::in);
	else{
		printf(WARN"CANNOT OPEN index.txt"CLR);
		return -1;
	}
	if(access(indegree_path,R_OK)==0)
		f_indegree.open(indegree_path,ios::in);
	else{
		printf(WARN"CANNOT OPEN indegree.txt"CLR);
		return -1;
	}
	for(int i=0;i<max_page;i++){
		char tp[100];
		sprintf(tp,"%s%d.txt",content_path,i);
		strcpy(con_path[i],tp);
		if(access(tp,R_OK)==0){
			f_content[i].open(tp,ios::in);
			avail_content[i]=1;
			f_content[i].close();
		}
	}
	string buf;
	while(getline(f_indegree,buf)){
			int id,num;
			sscanf(buf.c_str(),"%d %d",&id,&num);
			indegree_list[id]=num;
		}
	f_indegree.close();
	f_index.close();
	return 0;
}


int search_engine(){
	switch(CMD_TYPE){
		case CMD_AND: return CMD_AND_ENGINE();break;
		case CMD_OR:	return CMD_OR_ENGINE();break;
		case CMD_NONE:return CMD_NONE_ENGINE();break;
		default: printf(WARN"ERROR in search_enging"CLR);break;
	}
	return -1;
}

//Bool Retrieve: AND
int CMD_AND_ENGINE(){
	int res[max_page]={1};
	for(int i=0;i<max_page;i++)
		res[i]=1;
	for(int cur_string=0;cur_string<search_list_num;cur_string++){
		int re[max_page]={0};
		memset(re,0,sizeof(re));
		search_string(search_list[cur_string],re);
		for(int i=0;i<max_page;i++)
			if(res[i]==1){
				if(re[i]==0)
					res[i]=0;
			}
	}
	int count=0;
	for(int i=0;i<max_page;i++){
		if(res[i]==1){
			search_result[i]=1;
			count++;
		}
	}
	return count;
}


//Bool Retrieve: OR
int CMD_OR_ENGINE(){
	int res[max_page]={1};
	for(int i=0;i<max_page;i++)
		res[i]=1;
		for(int cur_string=0;cur_string<search_list_num;cur_string++){
		int re[max_page]={0};
		memset(re,0,sizeof(re));
		search_string(search_list[cur_string],re);
		for(int i=0;i<max_page;i++)
				if(re[i]==1)
					res[i]=1;
	}
	int count=0;
	for(int i=0;i<max_page;i++){
		if(res[i]==1){
			search_result[i]=1;
			count++;
		}
	}
	return count;
}

// Retrieve: NONE
int CMD_NONE_ENGINE(){
	int res[max_page];
	memset(res,0,sizeof(res));
	string wtf=search_list[0];
		search_string(wtf,res);
	int count=0;
	for(int i=0;i<max_page;i++){
		if(res[i]==1){
			search_result[i]=1;
			count++;
		}
	}
	return count;
}

// results
void suc_search_dealer(){
		printf(RED"\033[1m");
		cout<<"搜索结果";
		printf(CLR""GRN);
		int re[max_page];
		int curr=0;
		for(int i=0;i<max_page;i++)
			if(search_result[i]){
					re[curr]=i;
					curr++;
				}
		mysort(curr,&re[0],&indegree_list[0]);
		for(int j=curr-1;j>=0;j--){
					int i=re[j];
					printf(THK"\033[34m%d:"CLR,i);
					f_content[i].open(con_path[i]);
					string buf;
					getline(f_content[i],buf);
					printf("%s.....\tindegree=%d"CLR,buf.c_str(),indegree_list[i]);
					f_content[i].close();
				}
		printf(CLR);
		char buf[20];
		while(1){
			printf(PUP);
				cout<<"请输入要查看的文章编号，输入-1以进行下一次搜索:";
			printf("\033[0m");
			scanf("%s",buf);
			int wtf;
			sscanf(buf,"%d",&wtf);
			if(wtf==-1)
				break;
			else if(search_result[wtf]!=1)
				printf(WARN"Page %d not in list\n"CLR,wtf);
			else{
				f_content[wtf].open(con_path[wtf]);
				string buf;
				while(getline(f_content[wtf],buf))
					cout<<buf<<endl;
			}
		}
}

void END_DEALER(){

}

// Search
void search_string(string tar,int *p){
	f_index.open(index_path,ios::in);
	string buf;
	while(getline(f_index,buf)){
		int eq=1;
		for(int i=0;i<tar.length();i++)
			if(buf[i]!=tar[i]){
				eq=0;break;
			}
		if(eq==1&&buf[tar.length()]==' '){
			string num_list=buf.substr(tar.length()+1,buf.length()-tar.length()-1);
			const char *list=num_list.c_str();
			char tp[10];
			int tp_l=0,cur=0;
			while(cur<num_list.length()){
				tp_l=0;
				while(list[cur+tp_l]!=' '&&cur+tp_l<num_list.length()){
					tp[tp_l]=list[cur+tp_l];
					tp_l++;
				}
				tp[tp_l]='\0';
				int i;
				sscanf(tp,"%d",&i);
				p[i]=1;
				cur+=tp_l+1;
			}
		}
	}
	f_index.close();
}
