#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<math.h>

int number_of_pages=0,size,option,lo_segspace,holecount=0,Activeprocess=0,allocated=0,allocatedpages=0;
int failedsegalloc=0,failedpagealloc=0;
int failedsegalloc_p[100];
int pagetable[5000];

void lineparser(char *args);

int segmentallocator(char *cmd1,char *cmd2,char *cmd3,char *cmd4,char *cmd5);
void printstate();
void printsegmentlist();
struct hole * holeallocate(int sizeneeded);
void holecoalesc();
int segmentdeallocator(char *cmd);
void printholelist();


int addtoholelist(int start,int size);
int deletehole(int position);

// page memory manager functions
int pageallocator(char *cmd1,char *cmd2,char *cmd3,char *cmd4,char *cmd5);
void printpagelist();
int pagedeallocator(char *cmd);

struct segmentlist
{
	// process list for segmentation
	int size;
	int pid;
	int textsize;
	int datasize;
	int heapsize;
	int textstart;
	int datastart;
	int heapstart;
	int allocated;
	struct segmentlist *next;
};

struct pagelist
{
	// process list for paging
  int bytes;
  int pid;
  int pagenum;
  int page[1000];
  int frame[1000];
  int used[1000];
 struct pagelist *next;

};

struct hole
{
	int hole_base;
	int hole_size;
	struct hole *next;
};

struct hole *first =NULL;
struct segmentlist *first_p=NULL;
struct pagelist *firstpage=NULL;
	


int main(int argc, char *argv[])
{
	int fd,bytecount,i=0,p,z=0,count;
	char *data=(char *) malloc(1000*sizeof(char));
	char *memdata=(char *) malloc(10*sizeof(char));
	char *args[50],*memargs[2]={0,0};

	 for(p=0;p<50;p++)
	 {
	 	args[p]=0;
	 }


   fd=open(argv[1],O_RDONLY);

   bytecount=read(fd,data,1000);
   data[bytecount]='\0';
   //printf("%s\n",data);

   data=strtok(data,"\n");

   while(data!=0)
   {
     args[i]=data;
    // printf("lines are %s\n",args[i]);
     i++;
     data=strtok(0,"\n");
   }
    //printf("lines are %s\n",args[0]);
    count=i;
    //printf("%d\n",count);
    memdata=args[0];
    memdata=strtok(memdata," ");
    
  while(memdata!=0)
   {
      memargs[z]=memdata;
      z++;
      memdata=strtok(0," ");
   } 

    size =atoi(memargs[0]);
    option=atoi(memargs[1]);

   // printf("memory size %d\n",size);
   // printf("memory option %d\n",option);

    if (option==1)
	{
		// segmentation memory allocator
			// initially when no process is allocated 
			first =(struct hole *)malloc(sizeof(struct hole));
			struct hole *h =(struct hole *)malloc(sizeof(struct hole));
			h->hole_base=0;
			h->hole_size=size;
			holecount++;

			first=h;
			first->next=(struct hole *)malloc(sizeof(struct hole));
			first->next=NULL;

	}
	else if(option==2)
	{
		// paging memory allocator
		   number_of_pages= size/32; // as page size is 32 byte

			//printf("number of pages%d\n",number_of_pages);

			for(i=0;i<number_of_pages;i++)
			{
				pagetable[i]=-1;
			}
	}
	else
	{
		printf("invalid option\n");
		exit(0);
	}

    for(i=1;i<=count-1;i++)
    {
    	lineparser(args[i]);
    }


	return 0;
}

void lineparser(char *args)
{

	//printf("executed\n");
	//printf("%s\n",args);


	char *cmd[10]={0,0,0,0,0,0,0,0,0,0};
	int l=0;

	args=strtok(args," ");
	while(args!=0)
	{
		cmd[l]=args;
		//printf("%s\n",cmd[l]);
		l++;
		args=strtok(0," ");

	}

	if(strcmp(cmd[0],"A")==0)
	{
		//printf("Allocator\n");
		if (option==1)
		{ 
		  segmentallocator(cmd[1],cmd[2],cmd[3],cmd[4],cmd[5]);
		}
		else if(option == 2)
		{
			pageallocator(cmd[1],cmd[2],cmd[3],cmd[4],cmd[5]);
		}
	}
	else if(strcmp(cmd[0],"D")==0)
	{
		//printf("Deallocator\n");
		if (option==1)
		{   
		  segmentdeallocator(cmd[1]);
		}
		else if(option==2)
		{
			pagedeallocator(cmd[1]);
		}
	}
	else if(strcmp(cmd[0],"P")==0)
	{
		//printf("print memory state\n");
		 printstate();
	}
	else
	{
		printf("Not valid\n");
	}
}

int segmentallocator(char *cmd1, char *cmd2, char *cmd3,char *cmd4, char *cmd5)
{

	int segsize,pid,text,data,heap,textseg,dataseg,heapseg,seg_need[100],l;

	struct segmentlist *s,*temp;
	struct hole *href,*htemp;
	s=(struct segmentlist *)malloc(sizeof(struct segmentlist));

      	segsize=atoi(cmd1);
		pid=atoi(cmd2);
	    seg_need[0]=atoi(cmd3);
		seg_need[1]=atoi(cmd4);
		seg_need[2]=atoi(cmd5);

	if(segsize==seg_need[0]+seg_need[1]+seg_need[2])
	{		
		if((size-allocated) > segsize)
		{
				// hole allocate for all segments
			htemp=holeallocate(seg_need[0]);
			s->textstart=htemp->hole_size;
			s->textsize=seg_need[0];

			htemp=holeallocate(seg_need[1]);
			s->datastart=htemp->hole_size;
			s->datasize=seg_need[1];

			htemp=holeallocate(seg_need[2]);
			s->heapstart=htemp->hole_size;
			s->heapsize=seg_need[2];

			s->size=segsize;
			s->pid=pid;
			s->allocated=s->textsize+s->datasize+s->heapsize;
            allocated+=s->allocated;
			Activeprocess++;

			if(first_p == NULL)
			{
         		 first_p=s;         
         		 first_p->next=NULL;
          		return 0;
			}
			else
			{
				temp= first_p;
			
				while(temp->next != NULL)
				{
					temp=temp->next;
				}		
			
				temp->next=s;	
				s->next=NULL;
			}	
		}
		else
		{
			failedsegalloc++;
		}

			

	}
	else
	{
		printf("Segmentation error\n");
	}

	return 0;
}


void printstate()
{
	int p=0,m;
	

	if(option==1)
	{
		printf("Memory size=%d bytes, Allocated bytes=%d, free=%d\n",size,allocated,size-allocated);
		printf("There are currently %d holes and %d process\n",holecount,Activeprocess);
		if(holecount>0)
		{
			printf("Hole List:\n");
			//printf("hole %d: start location =%d size=%d \n",p,first->hole_base,first->hole_size);
			printholelist();
		}

		if(Activeprocess>0)
		{
			printf("Process List:\n");
			printsegmentlist();        
		}
		else
		{
			printf("no Active process\n");
		}

		printf("Failed allocations[No memory]= %d\n",failedsegalloc);
	}
	else
	{
		printf("Memory size=%d bytes, total pages=%d\n",size,number_of_pages);
		printf("allocatedpages=%d, free pages=%d\n",allocatedpages,(number_of_pages - allocatedpages));
		printf(" There are currently %d active process\n",Activeprocess);
		
		if((number_of_pages-allocatedpages) > 0)
		{
			printf("Free pagelist:");
			for(p=0;p<number_of_pages;p++)
			{
				if(pagetable[p]!=0)
				{
					printf("%d,",p);
				}
			}
		}
		
		if(Activeprocess>0)
		{
			printf("\nProcess List:\n");
			printpagelist();
		}
		
		printf("\nFailed allocations[No memory]=%d\n",failedpagealloc);
	}
}


struct hole * holeallocate(int sizeneeded)
{
	struct hole *href,*temp;
		
	int l;

	// first fit
     href=first;

	for(l=0;l<holecount;l++)
	{
		do
		{
          if(href->hole_size >= sizeneeded)
          {
            href->hole_size = href->hole_size - sizeneeded;
            goto label;
          }
		}while(href->next != NULL);
	}
   label:

	    return href;
    
}

void printsegmentlist()
{
	struct segmentlist *stemp;

	stemp=first_p;


		while(stemp->next != NULL)
		{
			
			printf("process id=%d, size=%d, allocation=%d\n ",stemp->pid,stemp->size,stemp->allocated);
			printf("\ttext start=%d, size=%d\n",stemp->textstart,stemp->textsize);
			printf("\tdata start=%d, size=%d\n",stemp->datastart,stemp->datasize);
			printf("\theap start=%d, size=%d\n",stemp->heapstart,stemp->heapsize);
			//printf("%d %d %d %d %d %d %d %d\n",stemp->size,stemp->pid,stemp->textsize,stemp->textstart,stemp->heapsize,stemp->heapstart,stemp->datasize,stemp->datastart);		
			stemp=stemp->next;
		}
			//printf("%d %d %d %d %d %d %d %d\n",stemp->size,stemp->pid,stemp->textsize,stemp->textstart,stemp->heapsize,stemp->heapstart,stemp->datasize,stemp->datastart);		
			
			printf("process id=%d, size=%d, allocation=%d\n ",stemp->pid,stemp->size,stemp->allocated);
			printf("\ttext start=%d, size=%d\n",stemp->textstart,stemp->textsize);
			printf("\tdata start=%d, size=%d\n",stemp->datastart,stemp->datasize);
			printf("\theap start=%d, size=%d\n",stemp->heapstart,stemp->heapsize);		
}

int segmentdeallocator(char *cmd)
{
	int pid,j,pos=-1;
	pid=atoi(cmd);
	struct segmentlist *s,*s1;
	s=first_p;
	
 	// process to be removed from list

		for(j=0;j<Activeprocess;j++)
	 	{
       		if(s->pid==pid)
       		{
       	  		pos=j+1;
       			//printf("proces to be deallocated is located at %d\n",pos);
       			break;
      		 }
       		s=s->next;
		}

		if(pos==-1)
		{
			// if there is no such process that is allocated
			return 0;
		}

	if(pos==Activeprocess)
	{
		
		
		if(first_p->next==NULL)
		{
			// if there are two process and first process to be deleted
			s=first_p->next;
			allocated-=first_p->allocated;
			addtoholelist(first_p->textstart,first_p->textsize);
 			addtoholelist(first_p->datastart,first_p->datasize);
 			addtoholelist(first_p->heapstart,first_p->heapsize);
			free(first_p);
			first_p=s;
			Activeprocess--;
			goto end; //tbr
		}
		else
		{
			// if there are more process and process at end to be deleted
 			s=first_p;
 			while(s->next !=NULL)
 			{
     		  s1=s;
     		  s=s->next;
 			}

 			s1->next= NULL;
 			allocated-=s->allocated;
 			addtoholelist(s->textstart,s->textsize);
 			addtoholelist(s->datastart,s->datasize);
 			addtoholelist(s->heapstart,s->heapsize);
 			free(s);
 			Activeprocess--;
 			return 0;
		}

	}
	else if(pos==1)
	{
		// more process are there but process at first to be deleted
		s=first_p->next;
		allocated-=first_p->allocated;
		addtoholelist(first_p->textstart,first_p->textsize);
 		addtoholelist(first_p->datastart,first_p->datasize);
 		addtoholelist(first_p->heapstart,first_p->heapsize);
		free(first_p);
		first_p=s;
		Activeprocess--;
		return 0;
	}
	else
	{
		// if there more process and somewhere in middle it is to be deleted

		s=first_p;

		for(j=2;j<=pos;j++)
		{
			s1=s;
			s=s->next;
		}
		
		  s1->next=s->next;
		  s->next=NULL;
		  allocated-=s->allocated;
		    addtoholelist(s->textstart,s->textsize);
 			addtoholelist(s->datastart,s->datasize);
 			addtoholelist(s->heapstart,s->heapsize);
		  free(s);
		  Activeprocess--;

		  return 0;
	}
	
	
	end:
	   return 0;
}

void printholelist()
{
	struct hole *htemp;
	if(holecount>0)
	{
		int h=1;
			htemp=first;


		while(htemp->next != NULL)
		{
			
			printf("\thole=%d: startlocation=%d, size=%d\n",h,htemp->hole_base,htemp->hole_size);
			
			//printf("%d %d %d %d %d %d %d %d\n",stemp->size,stemp->pid,stemp->textsize,stemp->textstart,stemp->heapsize,stemp->heapstart,stemp->datasize,stemp->datastart);		
			htemp=htemp->next;
			h++;
		}

		printf("\thole=%d, startlocation=%d, size=%d\n",h,htemp->hole_base,htemp->hole_size);

	}

}

int addtoholelist(int start,int size)
{
	int v,coal=1;

	int s,l;

		struct hole *htemp,*h;
			h=(struct hole *)malloc(sizeof(struct hole));
			h->hole_base=start;
			h->hole_size=size;



		if(first==NULL)
		{
			first=h;
			holecount++;
			first->next=NULL;
			return 0;
		}
		else
		{
			htemp=first;
			
			for(v=0;v<holecount;v++)
			{
				s=htemp->hole_base;
			    l=htemp->hole_size+htemp->hole_base;

				if(start == l)
				{	
					if(coal==1)
					{  // added to the size of hole on top
						htemp->hole_size= htemp->hole_size+size;
					}	
					coal++; // there is one more hole where it can be added			
 					
				}
				else if((start + size) == s)
				{
					if(coal==1)
					{
						htemp->hole_base=start;
                    	htemp->hole_size=htemp->hole_size+size;
                    	
					}
					coal++;
                                    
				}

				htemp=htemp->next;
			}
				if(coal==1)
				{
					htemp=first;
					holecount++;

				while(htemp->next != NULL)
					htemp=htemp->next;

				htemp->next=h;
				h->next=NULL;

				}
				
		}

		
		if(coal > 2)
		{
			holecoalesc();

		}
		

		return 0;
}

void holecoalesc()
{

	// This will add if there are any contiguos holes
	int l,m;
	struct hole *h,*h1;
	h1=(struct hole *) malloc(sizeof(struct hole));

						
			h1=first;
			h=h1->next;


	for(l=0;l<holecount-1;l++)
	{
			
		for(m=l+1;m<holecount;m++)
		{
			//printf("m %d",m);

			if((h1->hole_base+h1->hole_size)== h->hole_base)
				{
					// combine holes
					h1->hole_size = h1-> hole_size+ h->hole_size;
					deletehole(m+1);
					//holecount--;

					break;

				}
				else if((h->hole_base+h->hole_size)== h1->hole_base)
				{
					// combine holes

					h1->hole_base=h->hole_base;
					h1->hole_size=h1->hole_size + h->hole_size;
					deletehole(m+1);
					break;
				}
				
				if(h->next !=NULL)
						h=h->next;
			
		}

		h1=h1->next;
	}

}
int deletehole(int position)
{

	int j;
	struct hole *h,*h1;
	h=(struct hole *) malloc(sizeof (struct hole));
	h=first;

	//printholelist();
	//printf("pos%d\n",position);
	//printf("holecount%d\n",holecount);

	if(position == holecount)
	{
		if(first->next==NULL)
		{
			h=first->next;
			free(first);
			holecount--;
			h=first->next;
			return 0;
		}
		else
		{
			
			h=first;
			while(h->next != NULL)
			{
				h1=h;
				h=h->next;
			}

			
			h1->next=NULL;
			free(h);
			holecount--;
			return 0;
		}

	}
	else if(position ==1)
	{
		h=first->next;
		free(first);
		first=h;
		holecount--;
		return 0;
	}
	else
	{
		h=first;
		for(j=2;j<=position;j++)
		{
			h1=h;
			h=h->next;
		}
		h1->next=h->next;
		h->next=NULL;
		free(h);
		holecount--;
		return 0;
	}

}



int pageallocator(char *cmd1,char *cmd2,char *cmd3,char *cmd4,char *cmd5)
{

		int processsize,pid,page_p,page_rem,m,n;
	
		struct pagelist *ptemp;
		struct pagelist *p=(struct pagelist *)malloc(sizeof(struct pagelist));
		processsize=atoi(cmd1);
		pid=atoi(cmd2);

			page_p=processsize/32;
		page_rem=processsize%32;
			

		if(page_rem > 0)
		{
			page_p=page_p+1;
			//printf("pages required for this process %d\n",page_p);
		}

		if((number_of_pages-allocatedpages)>page_p)
		{
			Activeprocess++;
			p->pid=pid;
			p->bytes=processsize;
			p->pagenum=page_p;

			if(allocatedpages==0)
			{
				allocatedpages+=page_p;
				for(m=0;m<page_p;m++)
					{
						p->page[m]=m;
						p->frame[m]=m;
						pagetable[m]=0;
						p->used[m]=32;
					}

					if(page_rem!=0)
					{
						p->used[page_p-1]=page_rem;
					}
			}
			else if(allocatedpages > 0)
			{
				for(m=0;m<page_p;m++)
				{
					p->page[m]=m;
					p->used[m]=32;
					for(n=0;n<number_of_pages;n++)
					{
						if(pagetable[n] != 0)
						{
							p->frame[m]=n;
					    	pagetable[n]=0;
					    	goto k;
						}
					
					}					
				k:
					allocatedpages++;
				}
				if(page_rem!=0)
					{
						//printf("%d\n",page_rem);
						p->used[page_p-1]=page_rem;
					}

			}

			if(firstpage==NULL)
			{
				firstpage=p;
				firstpage->next=NULL;
				return 0;
			}
			else
			{
				ptemp=firstpage;

				while(ptemp->next != NULL)
				{
					ptemp=ptemp->next;
				}
				ptemp->next=p;
				p->next=NULL;

			}

		}
		else
		{
			failedpagealloc++;
		}

    	return 0;
}

void printpagelist()
{
	struct pagelist *ptemp;
	int p=1,m;
	ptemp=firstpage;


	while(ptemp->next != NULL)
	{
		printf("processid=%d, size=%d, number of pages=%d\n",ptemp->pid,ptemp->bytes,ptemp->pagenum);

		for(m=0;m<ptemp->pagenum;m++)
		{
			printf(" virt page %d -> phys page %d used: %d bytes\n",ptemp->page[m],ptemp->frame[m],ptemp->used[m]);
		}
		ptemp=ptemp->next;
		p++;
	}

	printf("processid=%d, size=%d, number of pages=%d\n",ptemp->pid,ptemp->bytes,ptemp->pagenum);

		for(m=0;m<ptemp->pagenum;m++)
		{
			printf(" virt page %d -> phys page %d used: %d bytes\n",ptemp->page[m],ptemp->frame[m],ptemp->used[m]);
		}
}

int pagedeallocator(char *cmd)
{
	int pid,j=0,pos=-1,frame_to_d[1000],k=0;
	pid=atoi(cmd);

	//printf("process to be deallocated%d\n",pid);

	for(k=0;k<1000;k++)
	{
			frame_to_d[k]=0;
	}

	struct pagelist *ptemp,*p,*p1;
	p=firstpage;

	

	for(j=0;j<Activeprocess;j++)
	{
		if(p->pid==pid)
		{
			pos=j+1;
			//printf("%d position",pos);
			break;
		}
		p=p->next;
	}

	if(pos == -1)
	{
		// no such process to be deallocated as it is not allocated
		//printf("process %d\n",pid);
		return 0;
	}

	if(pos==Activeprocess)
	{
		if(firstpage->next==NULL)
		{
			//if there is one process and that process to be deleted
			p=firstpage->next;
			allocatedpages-=firstpage->pagenum;

			for(j=0;j<firstpage->pagenum;j++)
			{
				// it will store the used pages in physical memory
					frame_to_d[j]=firstpage->frame[j];
			}
			
			for(k=0;k<firstpage->pagenum;k++)
			{
				for(j=0;j<number_of_pages;j++)
				{
                    if(j==frame_to_d[k])
                    {
                    	pagetable[j]=-1;
                    	break;
                    }

				}

			}
			
			free(firstpage);
			firstpage=p;
			Activeprocess--;
			return 0;
		}
		else
		{
			// if there are more process and process at end to be deleted
			p=firstpage;
			while(p->next != NULL)
			{
				p1=p;
				p=p->next;
			}

			p1->next=NULL;
			allocatedpages-=p->pagenum;


			for(j=0;j<p->pagenum;j++)
			{
				// it will store the used pages in physical memory
					frame_to_d[j]=p->frame[j];
			}
			
			for(k=0;k<p->pagenum;k++)
			{
				for(j=0;j<number_of_pages;j++)
				{

                    if(frame_to_d[k]==j)
                    {
                    	pagetable[j]=-1;
                    	break;
                    }
				}
			}
			free(p);
			Activeprocess--;
		}
	}
	else if(pos==1)
	{
		p=firstpage->next;
			allocatedpages-=firstpage->pagenum;

			for(j=0;j<firstpage->pagenum;j++)
			{
				// it will store the used pages in physical memory
					frame_to_d[j]=firstpage->frame[j];
			}
			
			for(k=0;k<firstpage->pagenum;k++)
			{
				for(j=0;j<number_of_pages;j++)
				{
                    if(frame_to_d[k]==j)
                    {
                    	pagetable[j]=-1;
                    	break;
                    }

				}

			}
			
			free(firstpage);
			firstpage=p;
			Activeprocess--;

	}
	else
	{
		// if there are more process and some where in middle it is to be deleted

		p=firstpage;

		for(j=2;j<=pos;j++)
		{
			p1=p;
			p=p->next;
		}

		p1->next=p->next;
		p->next=NULL;
		allocatedpages-=p->pagenum;
		for(j=0;j<p->pagenum;j++)
			{
				// it will store the used pages in physical memory
					frame_to_d[j]=p->frame[j];
			}
			
			for(k=0;k<p->pagenum;k++)
			{
				for(j=0;j<number_of_pages;j++)
				{

                    if(frame_to_d[k]==j)
                    {
                    	pagetable[j]=-1;
                    	break;
                    }
				}
			}
			free(p);
			Activeprocess--;

	}

	return 0;
}


       