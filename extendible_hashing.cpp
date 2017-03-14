#include <bits/stdc++.h>
using namespace std;

int hash(int key, int depth)   // calculates the hash value based on last depth bits
{
	return (0xffffffff >> (32-depth)) & key;
}

int binary (int num)		//binary form of buckets
{
	if (num == 0)
		return 0;
	else
		return (num % 2) + 10 * binary(num / 2);
}

//buckets created as sets
class Bucket
{
	public:
		Bucket(int a, int b)               //a = capacity of bucket, b = local_depth
		{
			capacity = a;
			local_depth = b;
		}
	
		void insert(int key)			//insertion of a key in a bucket
		{
			S.insert(key);
		}
		
		void del(int key)				//delete a key in the bucket
		{
			S.erase(S.find(key));
		}
		
		bool is_present(int key)				//checks if a key is present in the bucket or not
		{
			set<int>::iterator iter;
			iter = S.find(key);
			if(iter != S.end())
				return true;
			else
				return false;
		}
		
		int find_pos(int key)				//find position of key in bucket
		{
			int i =0;
			set<int>::iterator iter;
			for(iter=S.begin(); iter!=S.end();++iter)
               {
               	if((*iter) == key)
               		return i;
               	i++;
               }
		}
		
		bool is_full()				//checks if the bucket is full
		{
			if(S.size() == capacity)
				return true;
			else
				return false;
		}
		
		bool is_empty()			//checks if bucket is empty
		{
			if(S.size() == 0)
				return true;
			else
				return false;
		}
		
		void clear()			//clears all elements of the set
		{
			S.clear();
		}
		
		vector<int>* disp()		//copies all the entries of a bucket in a vector
	    {
	    		vector<int> *elements=new vector<int>;
	    		set<int>::iterator iter;
	        	for(iter=S.begin(); iter!=S.end();++iter)
               elements->push_back(*iter);
               return elements;
         }
         
         void show(int num)		//display the contents of the bucket
         {
         		int i=0;
         		set<int>::iterator iter;
         		cout << "Main Bucket Contains:" << endl;
	        	for(iter=S.begin(); iter!=S.end();++iter)
               {
               	cout <<(*iter) << " ";
               	i++;
               	if(i==num)
               	{
               		cout << endl;
               		cout << "Overflow page Contains:"<<endl;
               	}
               }
               if(i == num)
               	cout << "No elements"<<endl;
               if(i<num)
               {
               	cout << endl;
               	cout << "Overflow page Contains:"<<endl;
               	cout << "No elements"<<endl;
         		}
         }
         
         int size()
         {
         		return S.size();
         	}
         	
		int local_depth;
		int capacity;
		int flag = 0;			//denotes if the bucket has an overflow page or not
			
	private:
		set<int> S;
};

class HashTable
{
	public:
		HashTable(int size,int gdepth)
		{
			global_depth = gdepth;
			capacity = size;
			hash_size = pow(2,global_depth);
			v.resize(hash_size);
			for(int i=0; i<hash_size;i++)
			{
				v[i] = (Bucket* )malloc(sizeof(Bucket));
				*(v[i]) = Bucket(capacity, global_depth);
			}
		}
		
		int global_depth;
				
		void insert(int key,int insert_type)
		{
			int hash_val = hash(key,global_depth);
			if(insert_type == 1)
			{
				if(!v[hash_val]->is_full()) 
					v[hash_val]->insert(key);
				else if(v[hash_val]->local_depth == global_depth)
				{
					if(v[hash_val]->flag == 0)
					{
						v[hash_val]->capacity = 2*capacity;
						v[hash_val]->insert(key);
						v[hash_val]->flag =1;
					}
					else 
					{
						vector<int>* vr = v[hash_val]->disp();
						vector<int> &to_rehash = *vr;
						v[hash_val]->clear();
						hash_size = hash_size*2;
						v.resize(hash_size);
						global_depth++;
						for(int i = (hash_size/2); i<hash_size; i++)
						{
							v[i] = v[i-(hash_size/2)];
						}
						v[hash_val]->local_depth ++;
						v[hash_val+(hash_size/2)] = (Bucket* )malloc(sizeof(Bucket));
						*(v[hash_val+(hash_size/2)]) = Bucket(capacity, global_depth);
						for (int i = 0; i<to_rehash.size(); i++)
						{
							hash_val = hash(to_rehash[i],global_depth);
							v[hash_val]->insert(to_rehash[i]);
						}
						hash_val = hash(key,global_depth);
						v[hash_val]->insert(key);
					}
				}
				else 
				{
					vector<int>* vr = v[hash_val]->disp();
					vector<int> &to_rehash = *vr;
					v[hash_val]->clear();
					if(hash_val < hash_size/2)
					{
						v[hash_val + (hash_size/2)] = (Bucket* )malloc(sizeof(Bucket));
						*(v[hash_val+(hash_size/2)]) = Bucket(capacity, global_depth);
						v[hash_val]->local_depth ++;
					}
					else
					{	
						v[hash_val] = (Bucket* )malloc(sizeof(Bucket));
						*(v[hash_val]) = Bucket(capacity, global_depth);
						v[hash_val - (hash_size/2)]->local_depth ++;
					}
					for (int i = 0; i<capacity; i++)
					{
						hash_val = hash(to_rehash[i],global_depth);
						v[hash_val]->insert(to_rehash[i]);
					}
					hash_val = hash(key,global_depth);
					v[hash_val]->insert(key);
				}
			}	
				
			if(insert_type == 0)
			{
				if(!v[hash_val]->is_full()) 
					v[hash_val]->insert(key);
				else if(v[hash_val]->local_depth == global_depth)
				{
					vector<int>* vr = v[hash_val]->disp();
					vector<int> &to_rehash = *vr;
					hash_size = hash_size*2;
					v.resize(hash_size);
					global_depth++;
					for(int i = (hash_size/2); i<hash_size; i++)
					{
						v[i] = v[i-(hash_size/2)];
					}
					v[hash_val]->local_depth ++;
					v[hash_val+(hash_size/2)] = (Bucket* )malloc(sizeof(Bucket));
					*(v[hash_val+(hash_size/2)]) = Bucket(capacity, global_depth);
					for (int i = 0; i<capacity; i++)
					{
						if(hash(to_rehash[i],global_depth) != hash(to_rehash[i],global_depth-1))
						{
							v[hash_val + (hash_size/2)]->insert(to_rehash[i]);
							v[hash_val]->del(to_rehash[i]);
						}
					}
					hash_val = hash(key,global_depth);
					v[hash_val]->insert(key);
				}
				else
				{
					vector<int>* vr = v[hash_val]->disp();
					vector<int> &to_rehash = *vr;
					v[hash_val]->clear();
					if(hash_val < hash_size/2)
					{
						v[hash_val + (hash_size/2)] = (Bucket* )malloc(sizeof(Bucket));
						*(v[hash_val+(hash_size/2)]) = Bucket(capacity, global_depth);
						v[hash_val]->local_depth ++;
					}
					else
					{	
						v[hash_val] = (Bucket* )malloc(sizeof(Bucket));
						*(v[hash_val]) = Bucket(capacity, global_depth);
						v[hash_val - (hash_size/2)]->local_depth ++;
					}
					for (int i = 0; i<capacity; i++)
					{
						hash_val = hash(to_rehash[i],global_depth);
						v[hash_val]->insert(to_rehash[i]);
					}
					hash_val = hash(key,global_depth);
					v[hash_val]->insert(key);
				}
			}	
		}
		
		
		void search(int key)
		{
			int hash_val = hash(key,global_depth);
			if(v[hash_val]->is_present(key))
			{
				cout << "Key is present in bucket no:" << hash_val;
				if(v[hash_val]->find_pos(key) < capacity)
					cout<< " in the main bucket"<<endl;
				else
				cout << " in the overflow page"<<endl;
			}
			else
				cout<<"Key is not present.";
		}
		
		void status()
		{
			for(int i =0; i<v.size(); i++)
			{
				if(i == hash(i,v[i]->local_depth))
				{
					cout << "Elements in bucket no: "<< i << "(" << binary(i) << ")" << " are "<< endl;
					v[i]->show(capacity);
					cout << endl;
				}
			}
		}
		
		void del(int key)
		{
			int hash_val=hash(key, global_depth);
			if(v[hash_val]->is_present(key))
			{
				v[hash_val]->del(key);
				
			}
			if(v[hash_val]->size() <= capacity)
				v[hash_val]->flag = 0;
		}
		
	private:
		vector<Bucket *> v;
		int hash_size;
		int capacity;
};

int get_key(FILE *fp)
{
	int temp;
	fscanf(fp, "%d",&temp);
	return temp;
}

int main(int argc, char **argv)
{
	char file_name[100];
	int gdepth = atoi(argv[1]);		//global depth of the hashtable created
	int bucket_size = atoi(argv[2]);	//bucket-size of the buckets
	int insert_type = atoi(argv[3]);	//type of handle (1: overflow pages allowed , 0:overflow pages not allowed)
	strcpy(file_name, argv[4]);		//input file that contains operations to be done

	HashTable table(bucket_size,gdepth);	
	
	FILE *fp = fopen(file_name,"r");

	char type_of_operation[100];
	char ch;
	ch = getc(fp);
	int key;
	while(ch!=EOF)
	{
		ungetc(ch,fp);
		fscanf(fp, "%s", type_of_operation);

		if(!strcmp(type_of_operation,"insert"))			//insert mode of handling 1 or 0 in an argument to insert
		{
			key = get_key(fp);
			table.insert(key,insert_type);
		}
		else if(!strcmp(type_of_operation,"search"))
		{
			key = get_key(fp);
			table.search(key);
			
		}
		else if(!strcmp(type_of_operation,"delete"))
		{
			key = get_key(fp);
			table.del(key);
		}
		else if(!strcmp(type_of_operation,"status"))
		{
			table.status();
		}
		table.insert(1,2);
		ch = getc(fp);
	}
	fclose(fp);
	return 0;
}
