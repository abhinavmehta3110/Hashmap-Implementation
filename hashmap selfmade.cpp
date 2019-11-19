#include<iostream>
#include<string>
using namespace std;

template<typename V>
class mapNode
{
	public:
	string key;
	V value;
	mapNode<V>* next;
	
	mapNode(string key, V value)
	{
		this->value = value;
		this->key = key;
		next = NULL;
	}
	~mapNode()
	{
		cout << "destructor on key : " << key << endl;
		delete next;
	}
};

template<typename V>
class ourmap
{
	public:
	mapNode<V>** bucket;
	int count;
	int numBuckets;
	
	ourmap()
	{
		bucket = new mapNode<V>*[5];
		for(int i = 0; i < 5; i++)
		{
			bucket[i] = NULL;
		}
		count = 0;
		numBuckets = 5;
	}
	~ourmap()
	{
		for (int i = 0; i < numBuckets; i++)  // for loop important to delete linked list at every index of array
		{
			delete bucket[i];
		}
		delete [] bucket;
	}
	int getSize()
	{
		return count;
	}
	
	private:
	int hash(string key)
	{
		int ans = 0; 			// ans denotes hashcode
		int len = key.length();
		int multiplier = 1;
		for( int i = len - 1 ; i >= 0 ; i-- )
		{
			ans += multiplier * key[i];
			ans = ans % numBuckets;
			multiplier *= 37;		//we can use any prime number here at place of 37
			multiplier = multiplier % numBuckets;
		}
		return ans % numBuckets;
	}
	
	void rehash()
	{
		mapNode<V>** temp = bucket;
		bucket = new mapNode<V>*[2 * numBuckets];
		for(int i = 0; i < 2 * numBuckets; i++)
		{
			bucket[i] = NULL;
		}
		int oldNumBuckets = numBuckets;
		count = 0;
		numBuckets *= 2;
		for(int i = 0; i < oldNumBuckets; i++)
		{
			mapNode<V> * head = temp[i];
			while(head != NULL)
			{
				insert(head -> key , head -> value);
				head = head -> next;
			}
		}
		delete [] temp;
	}
	
	public:
	void insert(string key, V value)
	{
		int index = hash(key);
		mapNode<V>* head = bucket[index];
		while(head != NULL)
		{
			if(head->key == key)
			{
				head->value = value;
				return;
			}
			head = head->next;
		}
		head = bucket[index];
		mapNode<V>* newNode = new mapNode<V>(key, value);
		newNode->next = head;
		bucket[index] = newNode;
		count++;
		
		double loadFactor = (double)count / numBuckets;
		if(loadFactor > 0.7)
		{
			rehash();
		}
	}
	
	V find(string key)
	{
		int index = hash(key);
		mapNode<V>* head = bucket[index];
		while(head != NULL)
		{
			if(head->key == key)
			{
				return head->value;
			}
			head = head->next;
		}
		return 0;
	}
	
	V remove(string key)
	{
		int index = hash(key);
		mapNode<V> * head = bucket[index];
		mapNode<V> * prev = NULL;
		while(head != NULL)
		{
			if(head->key == key)
			{
				if(prev == NULL)
				{
					bucket[index] = head->next;
				}
				else
				{
					prev->next = head->next;
				}
				
				head->next = NULL;
				V ans = head->value;
				delete head;
				count--;
				return ans;
			}
			prev = head;
			head = head->next;
		}
		return 0;
	}
	double getLoadFactor()
	{
		return (double)count / numBuckets;
	}
	
};


int main()
{
	ourmap<int> m;
	string s = "abc";
	for(int i = 0; i < 10; i++)
	{
		string s1 = s + char(i + '0');
		m.insert(s1, i + 1);
		cout << m.getLoadFactor() << endl;
	}
	cout << m.getSize() << endl;
	m.remove("abc1");
	m.remove("abc4");
	cout << m.getSize() << endl;
	for(int i = 0; i < 10; i++)
	{
		string s1 = s + char(i + '0');
		cout << s1 << " " << m.find(s1) << endl;
	}
	m.insert("abc0", 5);
	cout << m.getSize() << endl;
	m.insert("abc34", 35) ;
	cout << m.getSize() << endl; 
	
}
