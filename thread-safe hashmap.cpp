
#include<set>
#include<map>
#include <exception>
#include <mutex>
#include <condition_variable>
#include<list>
#include<iomanip>
#include<cmath>
#include<string>
#include<vector>
#include<queue>
#include<stack>
#include<complex>
#include<sstream>
#include<iostream>
#include<fstream>
#include<algorithm>
#include<numeric>
#include<utility>
#include<functional>
#include<stdio.h>
#include<assert.h>
#include<memory.h>

#define all(v)              ((v).begin()), ((v).end())
#define sz(v)               ((int)((v).size()))
#define clr(v, d)           memset(v, d, sizeof(v))
#define rep(i, v)       for(int i=0;i<sz(v);++i)
#define lp(i, n)        for(int i=0;i<(int)(n);++i)
#define lpi(i, j, n)    for(int i=(j);i<(int)(n);++i)
#define lpd(i, j, n)    for(int i=(j);i>=(int)(n);--i)
using namespace std;
typedef pair<long, long> pii;
typedef long long ll;
struct s
{
	long long x, y;
};
bool cmp(s a, s b) { return a.x<b.x; }
long long gcd(int a, int b) { return b == 0 ? a : gcd(b, a%b); }
long mod(long a, long b) { return (a%b + b) % b; }


/*
*  wrapper for items stored in the map
*/
template<typename K, typename V>
class HashItem {

private:
	K key;
	V value;
	HashItem * nextItem;

public:
	HashItem(K key, V value) {
		this->key = key;
		this->value = value;
		this->nextItem = nullptr;
	}

	/*
	* copy constructor
	*/
	HashItem(const HashItem & item) {
		this->key = item.getKey();
		this->value = item.getValue();
		this->nextItem = nullptr;
	}

	void setNext(HashItem<K, V> * item) {
		this->nextItem = item;
	}

	HashItem * getNext() {
		return nextItem;
	}

	K getKey() {
		return key;
	}

	V getValue() {
		return value;
	}

	void setValue(V value) {
		this->value = value;
	}


};


/*
* template HashMap for storing items
* default hash function HF = std::hash<K>
*/
template <typename K, typename V, typename HF = std::hash<K>>
class HashMap {
public:

private:
	std::size_t mapSize;
	std::size_t numOfItems;
	HF hashFunc;
	HashItem<K, V> ** hMap;
	mutable std::mutex mtx;
	mutable std::mutex mtxForWait;

	//block the thread
	std::condition_variable condVar;


	/*
	* constructor
	* @mSize specifies the bucket size og the map
	*/
	HashMap(std::size_t mSize) {
		// lock initialization for single thread
		// mutex bwba to see which thread is comming like semafore
		//lock_guard Enter the  only one thread to Enter the mutex 
		std::lock_guard<std::mutex>lock(mtx);
		if (mSize < 1)
			throw std::exception("Empty map ");

		mapSize = mSize;
		numOfItems = 0;
		// initialize
		// pointer of pointer to everyone in string 
		hMap = new HashItem<K, V> *[mapSize]();
	}

	/*
	* for simplicity no copy constructor
	* anyway we want test how different threads
	* use same instance of the map
	*/
	//Told the compiler There is no Copy constructor & assignment operator  
	//HashMap(const HashMap & hmap) = delete;

	/*
	* inserts item
	* replaces old value with the new one when item already exists
	* @key key of the item
	* @value value of the item
	*/
	void insert(const K & key, const V & value) {
		std::lock_guard<std::mutex>lock(mtx);
		insertHelper(this->hMap, this->mapSize, numOfItems, key, value);
		condVar.notify_all();
	}

	/*
	* erases item with key when such item exists
	* @key of item to erase
	*/
	void erase(const K & key) {
		std::lock_guard<std::mutex>lock(mtx);
		// calculate the bucket where item must be inserted
		std::size_t hVal = hashFunc(key) % mapSize;
		HashItem<K, V> * prev = nullptr;
		HashItem<K, V> * item = hMap[hVal];

		while ((item != nullptr) && (item->getKey() != key)) {
			prev = item;
			item = item->getNext();
		}
		// no item found with the given key
		if (item == nullptr) {
			return;
		}
		else {
			if (prev == nullptr) {
				// item found is the first item in the bucket
				hMap[hVal] = item->getNext();
			}
			else {
				// item found in one of the entries in the bucket
				prev->setNext(item->getNext());
			}
			delete item;
			numOfItems--;
		}
		condVar.notify_all();
	}

	/*
	* get element with the given key by reference
	* @key is the key of item that has to be found
	* @value is the holder where the value of item with key will be copied
	*/
	bool getItem(const K & key, V & value) const {
		std::lock_guard<std::mutex>lock(mtx);
		// calculate the bucket where item must be inserted
		std::size_t hVal = hashFunc(key) % mapSize;
		HashItem<K, V> * item = hMap[hVal];

		while ((item != nullptr) && (item->getKey() != key))
			item = item->getNext();
		// item not found
		if (item == nullptr) {
			return false;
		}

		value = item->getValue();
		return true;
	}


	/*
	* get element with the given key by reference
	* @key is the key of item that has to be found
	* shows an example of thread waitung for some condition
	* @value is the holder where the value of item with key will be copied
	* check key or value if it is inserted or not
	*/
	bool getWithWait(const K & key, V & value) {
		std::unique_lock<std::mutex>ulock(mtxForWait);
		condVar.wait(ulock, [this] {return !this->empty(); });
		// calculate the bucket where item must be inserted
		std::size_t hVal = hashFunc(key) % mapSize;
		HashItem<K, V> * item = hMap[hVal];

		while ((item != nullptr) && (item->getKey() != key))
			item = item->getNext();
		// item not found
		if (item == nullptr) {
			return false;
		}

		value = item->getValue();
		return true;
	}


	/*
	* resizes the map
	* creates new map on heap
	* copies the elements into new map
	* @newSize specifies new bucket size
	*/
	void resize(std::size_t newSize) {
		std::lock_guard<std::mutex>lock(mtx);
		if (newSize < 1)
			throw std::exception("Number of buckets must be greater than zero.");

		resizeHelper(newSize);
		condVar.notify_all();
	}

	/*
	* outputs all items of the map
	*/
	void outputMap() const {
		std::lock_guard<std::mutex>lock(mtx);
		if (numOfItems == 0) {
			std::cout << "Map is empty." << std::endl << std::endl;
			return;
		}
		std::cout << "Map contains " << numOfItems << " items." << std::endl;
		for (std::size_t i = 0; i < mapSize; i++) {
			HashItem<K, V> * item = hMap[i];
			while (item != nullptr) {
				std::cout << "Bucket: " << std::setw(3) << i << ", key: " << std::setw(3) << item->getKey() << ", value:" << std::setw(3) << item->getValue() << std::endl;
				item = item->getNext();
			}
		}
		std::cout << std::endl;
	}

	/*
	* returns true when map has no items
	*/
	bool empty() const {
		std::lock_guard<std::mutex>lock(mtx);
		return numOfItems == 0;
	}

	void clear() {
		std::lock_guard<std::mutex>lock(mtx);
		deleteMap(hMap, mapSize);
		numOfItems = 0;
		hMap = new HashItem<K, V> *[mapSize]();
	}

	/*
	* returns number of items stored in the map
	*/
	std::size_t size() const {
		std::lock_guard<std::mutex>lock(mtx);
		return numOfItems;
	}

	/*
	* returns number of buckets
	*/
	std::size_t bucket_count() const {
		std::lock_guard<std::mutex>lock(mtx);
		return mapSize;
	}

	/*
	* desctructor
	*/
	~HashMap() {
		std::lock_guard<std::mutex>lock(mtx);
		deleteMap(hMap, mapSize);
	}



	/*
	* help method for inserting key, value item into the map hm
	* mapSize specifies the size of the map, items - the number
	* of stored items, will be incremented when insertion is completed
	* @hm HashMap
	* @mSize specifies number of buckets
	* @items holds the number of items in hm, will be incremented when insertion successful
	* @key - key of item to insert
	* @value - value of item to insert
	*/
	void insertHelper(HashItem<K, V> ** hm, const std::size_t & mSize, std::size_t & items, const K & key, const V & value) {
		//get the value of key 
		std::size_t hVal = hashFunc(key) % mSize;

		HashItem<K, V> * prev = nullptr;
		//get the item
		HashItem<K, V> * item = hm[hVal];

		//put the prev first then contiue the items 
		while ((item != nullptr) && (item->getKey() != key)) {
			prev = item;
			item = item->getNext();
		}

		// inserting new item
		if (item == nullptr) {
			item = new HashItem<K, V>(key, value);
			items++;
			if (prev == nullptr) {
				// insert new value as first item in the bucket
				hm[hVal] = item;
			}
			else {
				// append new item on previous in the same bucket
				prev->setNext(item);
			}
		}
		else {
			// replace existing value
			item->setValue(value);
		}
	}

	/*
	* help method to resize the map
	* @newSize specifies new number of buckets
	*/
	void resizeHelper(std::size_t newSize) {
		HashItem<K, V> ** newMap = new HashItem<K, V> *[newSize]();
		std::size_t items = 0;
		for (std::size_t i = 0; i < mapSize; i++) {
			HashItem<K, V> * item = hMap[i];
			while (item != nullptr) {
				insertHelper(newMap, newSize, items, item->getKey(), item->getValue());
				item = item->getNext();
			}
		}

		deleteMap(hMap, mapSize);
		hMap = newMap;
		mapSize = newSize;
		numOfItems = items;
		newMap = nullptr;

	}

	/*
	* help function for deleting the map hm
	* @hm HashMap
	* @mSize number of buckets in hm
	*/
	void deleteMap(HashItem<K, V> ** hm, std::size_t mSize) {
		// delete all nodes
		for (std::size_t i = 0; i < mSize; ++i) {
			HashItem<K, V> * item = hm[i];
			while (item != nullptr) {
				HashItem<K, V> * prev = item;
				item = item->getNext();
				delete prev;
			}
			hm[i] = nullptr;
		}
		// delete the map
		delete[] hm;
	}
};

int main()
{

	HashItem<int, int  >;
	t::template HashItem<int, int>(); // call a function template

	this->template HashItem<int>(); // call a function template

	// new object from HashMap
	// object = size say 5 
	// object from HashItem
	// object HashMap try insert object from HashItem

	//calling fuction output 
	// 
}
