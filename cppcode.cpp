#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include<iomanip>

const int PAGE_SIZE=256;
const int TLB_SIZE=16;


using namespace std;

int main()
{
    ifstream fin;    //takes the input file i.e address.txt for logical address
    fstream fout;    //stores the logical address and physical address data
    fin.open("address.txt",ios::in);
    fout.open("output.txt",ios::out);
    if(!fin)
    {
        cout<<"ERROR OPENING FILE\n FILE FAILED TO OPEN"<<endl;
        exit(1);
    }
    string value;
	long long page_no,offset,page_table,totalhits=0,fault=0,pages=0,pagetablehits =0;
	
	int qp=0;						//to maintain the queue position
	int physicaladdress=0,frame,logicaladdress;
	int tlb[TLB_SIZE][2];
	int pagetable[PAGE_SIZE];
    // initialising the tlb and pagetable with values -1
	memset(tlb,-1,TLB_SIZE*2*sizeof(tlb[0][0]));
	memset(pagetable,-1,sizeof(pagetable));
	
	int i,hit;
	//parsing through input file till the end of file
    while(fin.peek()!=EOF)
	{
        
        fin>>value;  // takes each address from input file
		
		pages++;  // to know how many translations happened
		logicaladdress=stoi(value);
		//get page number from logical address
		
		page_no=logicaladdress/int(PAGE_SIZE);
		//get offset number from logical address
		offset=logicaladdress % int(PAGE_SIZE);
        // logical address is assigined with value 
		
		frame=0,physicaladdress=0; // intialising frame number and physical address
		
		hit=0;			//1 if found in TLB
		
		//CHECK IN TLB
		
		for(i=0;i<TLB_SIZE;i++)
		{
			if(tlb[i][0]==page_no)     // TLB HIT
			{
				hit=1;
				totalhits++;
				frame=tlb[i][1];
				break;
			}
		}
		//if present in tlb
		if(hit!=1)
        {
            int f=0;
			// checking through the page table
			for(i=0;i<PAGE_SIZE;i++)
			{
				if(pagetable[i]==page_no)
				{
					frame=i;
					pagetablehits++;
					
					break;
				}
				if(pagetable[i]==-1)  // page faults occurs
				{
					f=1;
					fault++;
					break;
				}
			}
			if(f==1)        // updating the pagetable when page fault occurs
			{
				pagetable[i]=page_no;
				frame=i;
			}
			//replace in tlb using fifo
			tlb[qp][0]=page_no;
			tlb[qp][1]=i;
			qp++;
			qp=qp%(TLB_SIZE-1);	
        }
       
        // cout<<"VIRTUAL ADDRESS = "<<logicaladdress<<"\t\t";
        string out1 ="Virtual Address = ";
		string out2 ="Physical Address = ";
        physicaladdress=frame*PAGE_SIZE + offset;
		out1 = out1 + to_string(logicaladdress)+"\t";
		out2 = out2 + to_string(physicaladdress);
		string out = out1+out2;
        // cout<<"PHYSICAL ADDRESS = "<<physicaladdress<<endl;
		fout<<out<<endl;
    }
	
    double hitrate=(double)totalhits/pages*100;
	double pagetablehitsrate=(double)pagetablehits/pages*100;
	// cout<<"Number of Translated Addresses = "<<pages<<endl;
	// cout<<"Number of TLB Hits = "<<totalhits<<endl;
	// cout<<"Number of TLB Misses = "<<pages-totalhits<<endl;
	cout<<"TLB Hit Rate = "<<fixed<<setprecision(2)<<hitrate<<endl;
	cout<<"TLB Miss Rate = "<<fixed<<setprecision(2)<<100-hitrate<<endl;
    // cout<<"Number of Page Table Hits = "<<pagetablehits<<endl;
	cout<<"Page Table Hit Rate = "<<fixed<<setprecision(2)<<pagetablehitsrate<<endl;
    // cout<<"Number of Page Table Misses = "<<pages-pagetablehits<<endl;
	cout<<"Page Table Miss Rate = "<<fixed<<setprecision(2)<<100-pagetablehitsrate<<endl;
	cout<<"Page faults = "<<fault<<endl;
	cout<<"Page fault Rate = "<<fixed<<setprecision(2)<<(double)fault/pages *100<<endl;
	fin.close();
	fout.close();
	return 0;	

}