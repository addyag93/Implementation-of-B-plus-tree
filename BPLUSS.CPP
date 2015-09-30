/* Program to implement B+tree INSERTION and DELETION
	NAME:	Aditya Gupta(04)
		Jewel pruthi(18)
		Abhishek Kumar(03)
   Compatible with turbo c++
*/
#include<iostream.h>
#include<conio.h>
#include<math.h>
#include<process.h>
typedef enum{false=0,true}bool;    //for true and false
double order;
class Bnode                //  A node
{
	int n;             // Number of keys at Present
	int *keys;         // keys in the node
	bool leaf;         // Whether leaf or not
	Bnode **p;         // Pointers to child node
	Bnode *parent;     // Node Parent

	public:
	Bnode(bool type,Bnode *p1,int size)   //Constructor
	{
		leaf=type;
		parent=p1;
		keys=new int[size-1];
		p=new Bnode *[size];
		n=0;
	}
	Bnode(Bnode  *&obj)
	{
	  leaf=obj->leaf;
	  parent=obj->parent;
	  keys=obj->keys;
	  p=obj->p;
	  n=obj->n;
	}
	int insertnonfull(int k); 	    // To insert a key in non full node
	void display_node();                // Display node
						// Keys,Whether leaf or not

	friend class Btree;
};

class Btree
{
	public:
	Bnode *root;
	Btree()
	{
		root=NULL;
		order=0;
	}

	void insert(int k);     // To insert a key in the tree
	void delete_key(int k); // To delete a key in the tree
	void splitnode(Bnode *snode,Bnode *P,int k); //To split the node that is full
	void redistribute(Bnode *m,Bnode *sibling,int pointer); //To Redistribute keys between nodes during deletion
	void merge(Bnode *m,Bnode *sibling,int pointer); //To Merge nodes during deletion
	void merge_non_leaf(Bnode *m,Bnode *sibling,int pointer);//Merging of non-leaf nodes
	void traverse1();     //Tree travsesal from bottom to top
	void traverse(Bnode *root);
	int search(int key);  // To search a key in the tree
	friend class Bnode;

};
	void Btree::traverse1()
	{
		traverse(root);
	}
	void Btree::traverse(Bnode *root)
	{
		if(root->leaf == false)
		{
			for(int i=0;i<root->n+1;i++)
				traverse(root->p[i]);
		}
		cout<<endl;
		for(int i=0;i<root->n;i++)
		{
			if(root->leaf ==true)
				{
					cout<<"Node:" ;
				}
					cout<<root->keys[i]<<" ";
		}

	}
	int Btree::search(int k)
	{
		Bnode *m=root;
		cout<<"---ROOT:----"<<endl;
		m->display_node();

		while(m->leaf!=true)
		{
			if(m->keys[0]>k)
			{
				m=m->p[0];
				cout<<"Pointer value is:"<<0<<endl;
				m->display_node();
			}
			else if(m->n==1 && m->keys[0]<=k)
			{
				m=m->p[1];
				cout<<"Pointer value is :"<<1<<endl;
				m->display_node();
			}
			else
			{
				for(int i=1;i<=m->n;i++)
				{
					if(m->keys[i-1]<=k)
					{
					  if(m->keys[i]>k)
					   {
						m=m->p[i];
						m->display_node();
						cout<<"Pointer value is :"<<i<<endl;
						break;
					   }
					   else if(i==m->n)
					   {
						m=m->p[i];
						cout<<"Pointer value is :"<<i<<endl;
						m->display_node();
						break;
					   }
					}
				}
			}
		}
		int flag=0;
		for(int i=0;i<m->n;i++)
		{
			if(m->keys[i]==k)
			{
				flag=1;
				cout<<"SEARCH FINISHED"<<endl;
				break;
			}
		}
		return flag;
	}
	void Bnode::display_node()
	{
		cout<<"Keys in this node"<<n<<endl;
		for(int i=0;i<n;i++)
		{
			cout<<"|"<<keys[i]<<"|";
		}
		if(leaf==1)
			cout<<"L"<<endl; //means leaf node
		else
			cout<<"NL"<<endl;  //means non leaf node

	}
	int Bnode::insertnonfull(int k)
	{
	int i=n-1;
	p[n+1]=p[n];
	while(i>=0 &&keys[i]>k)
	{
		keys[i+1]=keys[i];
		p[i+1]=p[i];
		i--;
	}
	keys[i+1]=k;
	p[i+1]=p[i];
	n=n+1;
	return i+1;
	}
	void Btree::splitnode(Bnode *snode,Bnode *P,int k)
	{
		Bnode *z = new Bnode(true,NULL,order);
		Bnode *temp= new Bnode(true,NULL,order+1);
		for(int j=0;j<snode->n;j++)
		{
			temp->keys[j]=snode->keys[j];
			temp->p[j]=snode->p[j];
			temp->n+=1;
		}
		temp->insertnonfull(k);
		double x1=double((order-1)/2.0);
		double leftnode=ceil(x1);
		double rightnode=order-leftnode;
		if(snode->leaf==true)
		{
			for(int j=0;j<leftnode;j++)
			{
				snode->keys[j]=temp->keys[j];
			}
			snode->n=leftnode;
			for(j=0;j<rightnode;j++)
			{
				z->keys[j]=temp->keys[j+leftnode];
			}
			z->n=rightnode;
			snode->p[order-1]=z;
		}

		Bnode *N=new Bnode(false,NULL,order);;
		int x;
		if(P->n<order-1)
		{
			int loc=P->insertnonfull(z->keys[0]);
			P->p[loc]=snode;
			P->p[loc+1]=z;
			z->parent=snode->parent;
			P->display_node();
		}
		else if(P->n==order-1)
		{
			Bnode *temp2 =new Bnode(false,P->parent,order+1); //similar to P
			for(int j=0;j<P->n;j++)
			{
				temp2->keys[j]=P->keys[j];
				temp2->p[j]=P->p[j];
				temp2->n++;
			}
			temp2->p[j]=P->p[j];
			temp2->p[j]->display_node();
			int loc= temp2->insertnonfull(z->keys[0]);
			temp2->p[loc]=snode;
			temp2->p[loc+1]=z;
			leftnode=ceil((order/2.0)-1);
			rightnode=leftnode+1;
			if(loc>leftnode)
			{
				snode->parent=N;
				z->parent=N;
			}
			else if(loc<leftnode)
			{
				snode->parent=P;
				z->parent=P;
			}
			while(temp2!=NULL)
			{
				for(int j=0;j<leftnode;j++)
				{
					P->keys[j]=temp2->keys[j];
					P->p[j]=temp2->p[j];
					P->p[j]->parent=P;

				}
				P->n=leftnode;
				P->p[j]=temp2->p[leftnode];
				P->p[j]->parent=P;

				int m;
				for(m=0,j=rightnode;j<temp2->n;j++,m++)
				{
					N->keys[m]=temp2->keys[j];
					N->p[m]=temp2->p[j];
					N->p[m]->parent=N;
					N->n++;
				}
				x=temp2->keys[leftnode];
				N->p[m]=temp2->p[j];
				N->p[m]->parent=N;
				temp2->p[leftnode]->parent=P;
				temp2->p[leftnode+1]->parent=N;
				if(P->parent==NULL)
				{
					P->parent=new Bnode(false,NULL,order);
					root=P->parent;
					int location=root->insertnonfull(x);
					root->p[location]=P;
					P->parent=root;
					N->parent=root;
					root->p[location+1]=N;
					root->display_node();
					break;
				}
				else if(P->parent->n<order-1)
				{
					int loc=P->parent->insertnonfull(x);
					P->parent->p[loc]=P;
					P->parent->p[loc+1]=N;
					N->parent=P->parent;
					break;
				}
				else if(P->parent->n==order-1)
				{
					temp2->n=0;
					temp2->parent=P->parent->parent;
					for(int j=0;j<P->parent->n;j++)
					{
						temp2->keys[j]=P->parent->keys[j];
						temp2->p[j]=P->parent->p[j];
						temp2->n++;
					}
					temp2->p[j]=P->parent->p[j];
					int location=temp2->insertnonfull(x);
					temp2->p[location]=P;
					Bnode *Nduplicate=N;
					temp2->p[location+1]=Nduplicate;
					N=NULL;
					N=new Bnode(false,NULL,order);
					P=P->parent;
				}
			}//end of while

		}//end of else if
	}

	void Btree::insert(int k)
	{
		if(root==NULL)
		{
			root=new Bnode(true,NULL,order);
			root->keys[0]=k;
			root->n=1;
			root->display_node();
		}
		else if(root->leaf==true)
		{
		      if(root->n<order-1)
			{
				root->insertnonfull(k);
				root->display_node();
			}
			else
			{
				Bnode *newnode=new Bnode(false,NULL,order);
				root->parent=newnode;
				splitnode(root,newnode,k);
				root=newnode;
				root->parent=NULL;
				root->leaf=false;

			}
		}
		else
		{
			Bnode *m=root;
			while(m->leaf!=true)
			{
				if(m->keys[0]>k)
				{
					m=m->p[0];
				}
				else if(m->n==1 && m->keys[0]<=k)
				{
					m=m->p[1];
				}
				else
				{
					for(int i=1;i<=m->n;i++)
					{
						if(m->keys[i-1]<=k)
						{
							if( m->keys[i]>k)
							{
								m=m->p[i];
								break;
							}
							else if(i==m->n)
							{
								m=m->p[i];
								break;
							}
						}//end of if
					}//end of for loop
				}// end of else
			}//end of while
			if(m->n==order-1)
			{
				splitnode(m,m->parent,k);
			}
			else
			{
				m->insertnonfull(k);
			}
	    }//end of else of insert
	}//end of insert
	void Btree::redistribute(Bnode *m,Bnode *sibling,int pointer)
	{
		Bnode *temp=new Bnode(false,NULL,order+1);
		if(m->leaf==true)
		{
			for(int j=0;j<m->n;j++)
			{
				temp->keys[j]=m->keys[j];
				temp->p[j]=m->p[j];
				temp->n+=1;
			}
			j=0;
			for(int k=temp->n;j<sibling->n;k++,j++)
			{
				temp->keys[k]=sibling->keys[j];
				temp->p[k]=sibling->p[j];
				temp->n+=1;
			}
			double leftnode =ceil((order-1)/2.0);
			double rightnode=order-leftnode;
			for(j=0;j<leftnode;j++)
			{
				m->keys[j]=temp->keys[j];
			}
			m->n=leftnode;
			for(j=0;j<rightnode;j++)
			{
				sibling->keys[j]=temp->keys[j+rightnode];
			}
			sibling->n=rightnode;
			m->parent->keys[pointer]=sibling->keys[0];
		}//end of if
		else if(m->leaf!=true)
		{
			for(int j=0;j<m->n;j++)
			{
				temp->keys[j]=m->keys[j];
				temp->p[j]=m->p[j];
				temp->n+=1;
			}
			temp->p[j]=m->p[j];
			temp->keys[j]=m->parent->keys[pointer];
			temp->n++;
			j=temp->n;
			for(int k=0;k<sibling->n;k++,j++)
			{
				temp->keys[j]=sibling->keys[k];
				temp->p[j]=sibling->p[k];
				temp->n+=1;
			}
			temp->p[temp->n]=sibling->p[k];
			double leftnode =ceil((order)/2.0)-1;
			double rightnode=leftnode+1;
			for(j=0;j<leftnode;j++)
			{
				m->keys[j]=temp->keys[j];
				m->p[j]=temp->p[j];
			}
			m->p[leftnode]=temp->p[leftnode];
			m->p[j]->parent=m;
			m->n=leftnode;
			m->parent->keys[pointer]=temp->keys[leftnode];
			sibling->n=0;
			for(k=0,j=rightnode;j<temp->n;j++,k++)
			{
				sibling->keys[k]=temp->keys[j];
				sibling->p[k]=temp->p[j];
				sibling->n++;
			}
			sibling->p[sibling->n]=temp->p[j];
			sibling->p[sibling->n]->parent=sibling;
		}//end of else if
	}//end of redistribute
	void Btree::merge_non_leaf(Bnode *m,Bnode *sibling,int pointer)
	{
		m->keys[m->n]=m->parent->keys[pointer];
		m->parent->n--;
		m->n++;
		for(int j=0,k=m->n;j<sibling->n;j++,k++)
		{
			m->keys[k]=sibling->keys[j];
			m->p[k]=sibling->p[j];
			sibling->p[j]->parent=m;  //parent also changes
			m->n++;
		}
		m->p[m->n]=sibling->p[j];
		sibling->p[j]->parent=m;
		if(m->parent==root)
		{
			if(root->n==0)
			{
				root=m;
				sibling->parent=root->parent=m->parent=NULL;
			}
		}
		else
		{
			Bnode *Prnt=m->parent;
			int i=pointer;
			while(i<Prnt->n)
			{
				Prnt->keys[i]=Prnt->keys[i+1];
				Prnt->p[i+1]=Prnt->p[i+2];
			}
		}
	}//end of merge_non_leaf
	void Btree::merge(Bnode *m,Bnode *sibling,int pointer)
	{
		double condition_underflow=ceil(order/2.0)-1.0;  //for non leaf
		if(m->leaf==true)
		{
			for(int j=0,k=m->n;j<sibling->n;j++,k++)
			{
				m->keys[k]=sibling->keys[j];
				m->p[k]=sibling->p[j];
				m->n+=1;
			}
			int i=pointer;
			m->parent->n--;
			while(i<m->parent->n)
			{
				m->parent->keys[i]=m->parent->keys[i+1];
				m->parent->p[i+1]=m->parent->p[i+2];
			}
			m=m->parent;
		}// end of  if
		while(m!=NULL)
		{
			if(m->n<condition_underflow)
			{
				int i=0;
				while(i<order)
				{
					if(m->parent->p[i]==m)
					{
						pointer=i;
						break;
					}
					else
						i++;
				}// end of while
				if(pointer<m->parent->n)
					sibling=m->parent->p[pointer+1];
				else
					sibling=m->parent->p[pointer-1];
				if(sibling->n>m->n+1)
				{
					if(pointer<m->parent->n)
						redistribute(m,sibling,pointer);
					else
						redistribute(sibling,m,pointer-1);
				}// end of if
				else if(sibling->n==m->n+1)
				{
					if(pointer<m->parent->n)
					{
						merge_non_leaf(m,sibling,pointer);
						m=m->parent;
					}
					else
					{
						merge_non_leaf(sibling,m,pointer-1);
						m=sibling->parent;
					}
				}
			}// end of if(in while)
			else
			{
				break;
			}
		}//end of while
	}// end of merge
	void Btree::delete_key(int k)
	{
		if(root==NULL)
		{
			cout<<"Tree is empty.No key to delete"<<endl;
		}
		else if(root->leaf==true)
		{
			int found=0,i=0;
			while(i<root->n)
			{
				if(k==root->keys[i])
				{
					found=1;
					break;
				}
				else
					i++;
			}
			if(found==1)
			{
				if(i!=root->n-1)
				{
					root->n--;
					while(i<root->n)
					{
						root->keys[i]=root->keys[i+1];
						i++;
					}
				}
				else
					root->n--;
			}// end of if
			else
				cout<<"KEY Not Found";
		}// end of else if
		else
		{
			int pointer=0;
			Bnode *m=root;
			while(m->leaf!=true)
			{
				if(m->keys[0]>k)
				{
					m=m->p[0];
					pointer=0;
				}
				else if(m->n==1 && m->keys[0]<=k)
				{
					m=m->p[1];
					pointer=1;
				}
				else
				{
					for(int i=1;i<=m->n;i++)
					{
						if(m->keys[i-1]<=k)
						{
							if( m->keys[i]>k)
							{
								m=m->p[i];
								pointer=i;
								break;
							}
							else if(i==m->n)
							{
								m=m->p[i];
								pointer=i;
								break;
							}
						}
					}// end of for loop
				}// end of else
			}// end of while
			int found=0;
			int i=0;
			while(i<m->n)
			{
				if(k==m->keys[i])
				{
					found=1;
					break;
				}
				else
					i++;
			}// end of while
			if(found==1)
			{
				if(i!=m->n-1)
				{
					m->n--;
					while(i<m->n)
					{
						m->keys[i]=m->keys[i+1];
						i++;
					}
				}
				else
					m->n--;
				double condition_underflow=ceil((order-1)/2.0);
				Bnode *sibling;
				if(m->n<condition_underflow)
				{
					if(pointer<m->parent->n)
						sibling=m->parent->p[pointer+1];
					else
						sibling=m->parent->p[pointer-1];
					if(sibling->n>m->n+1)
					{
						if(pointer<m->parent->n)
							redistribute(m,sibling,pointer);
						else
							redistribute(sibling,m,pointer-1);
					}// end of if
					else if(sibling->n==m->n+1)
					{
						if(pointer<m->parent->n)
							merge(m,sibling,pointer);
						else
							merge(sibling,m,pointer-1);
					}  // end of else
				}// end of if
			}//end of if(found==1)

			else
				cout<<"KEY Not Found";
		}// end of  else
	}//end of delete_key(int key)
	void main()
	{
		clrscr();
		int n=0,search_key;
		char ch;
		
		while(1)
		{
			cout<<"Enter the order of B+ Tree\n";
			cin>>n;
			if (n<3)
				cout<<"order less than three not allowed...Please enter it again"<<endl;
			else
				break;
		}
		Btree tree;
		order=n;
		while(1)
		{
			cout<<"\n1->Insert in tree";
			cout<<"\n2->Search in tree";
			cout<<"\n3->Display tree";
			cout<<"\n4->Delete in tree";
			cout<<"\n5->Exit"<<endl;
			cin>>ch;
			if(ch=='1')
			{
				do
				{
					cout<<"Enter the search key to be insert in tree\n";
					cin>>search_key;
					tree.insert(search_key);
					cout<<"press Y to enter more else press N\n";
					cin>>ch;
				}while(ch=='y'||ch=='Y');
			}
			else if(ch=='2')
			{
				int searchkey;
				cout<<"\nEnter the key you want to search\n";
				cin>>searchkey;
				int flag=tree.search(searchkey);
				if(flag==1)
					cout<<"Key Present"<<endl;
				else
					cout<<"Key Not Present"<<endl;
			}
			else if(ch=='3')
			{
			       tree.traverse1();
			}
			else if(ch=='4')
			{
				do
				{       int search_key;
					char ch;
					cout<<"Enter the search key to be delete in tree\n";
					cin>>search_key;
					tree.delete_key(search_key);
					cout<<"press Y to delete more else press N\n";
					cin>>ch;
				}while(ch=='y'||ch=='Y');
			}
			else if(ch=='5')
			{
				exit(0);
			}
			else
			{
				cout<<"Incorrect choice";
			}
		} // end of while

	} //end of void main()
