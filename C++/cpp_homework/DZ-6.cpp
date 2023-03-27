//
// Created by Иван Черемисенов on 27.03.2023.
//

#include <functional>
#include <exception>
#include <list>
#include <iostream>
#include <vector>
#include <algorithm>

template<typename T>
class Set {
 public:
	Set();
	Set(T * items, int size);
	Set(const Set & other);

	void add(T item);
	void add(T * items, int size);
	void del(T item);
	bool in(T item) const;
	void print() const;

	Set operator * (const Set & other) const;
	Set operator + (const Set & other) const;

 private:
	void rehash_if_needed();
	size_t hash(T key) const;

	size_t m_bucket_size;
	size_t m_items_count;
	float m_max_load_factor;

	std::list<T> * m_buckets;
};

template <typename T>
Set<T>::Set() :
	m_bucket_size(1),
	m_items_count(0),
	m_max_load_factor(1),
	m_buckets(new std::list<T>[1])
{

}

template <typename T>
void Set<T>::rehash_if_needed()
{
	if (static_cast<double> (m_items_count) / m_bucket_size <= m_max_load_factor)
		return;

	m_bucket_size *= 2;

	auto * new_buckets = new std::list<T>[m_bucket_size];

	for (size_t i = 0; i < m_bucket_size / 2; i++) {
		std::list<T> & bucket = m_buckets[i];
		for (auto & b_el : bucket) {
			new_buckets[hash(b_el)].push_back(b_el);
		}
	}

	delete[] m_buckets;
	m_buckets = new_buckets;
}

template <typename T>
size_t Set<T>::hash(T key) const
{
	return std::hash<T>()(key) % m_bucket_size;
}


template <typename T>
bool Set<T>::in(T item) const
{
	for (auto & it : m_buckets[hash(item)])
		if (it == item)
			return true;
	return false;
}

template <typename T>
void Set<T>::add(T item)
{
	rehash_if_needed();

	if (in(item))
		return;

	m_buckets[hash(item)].push_back(item);
	m_items_count++;
}

template <typename T>
void Set<T>::del(T item)
{
	auto & bucket = m_buckets[hash(item)];

	for (auto it = bucket.begin(); it != bucket.end(); it++)
		if (*it == item) {
			bucket.erase(it);
			m_items_count--;
			return;
		}
}

template <typename T>
void Set<T>::add(T* items, int size)
{
	for (size_t i = 0; i < size; i++)
		add(items[i]);
}

template <typename T>
Set<T>::Set(T* items, int size) :
	m_bucket_size(1),
	m_items_count(0),
	m_max_load_factor(1),
	m_buckets(new std::list<T>[1])
{
	add(items, size);
}

template <typename T>
void Set<T>::print() const
{
	std::vector<T> items;

	for (size_t i = 0; i < m_bucket_size; i++) {
		for (auto & item: m_buckets[i])
			items.push_back(item);
	}

	std::sort(items.begin(), items.end());
	for (auto & item: items) {
		std::cout << item << " ";
	}
	std::cout << std::endl;
}

template <typename T>
Set<T> Set<T>::operator*(const Set& other) const
{
	Set<T> temp;

	for (size_t i = 0; i < m_bucket_size; i++) {
		for (auto & item: m_buckets[i])
			if (other.in(item))
				temp.add(item);
	}

	return temp;
}

template <typename T>
Set<T> Set<T>::operator+(const Set& other) const
{
	Set<T> temp(other);

	for (size_t i = 0; i < m_bucket_size; i++) {
		for (auto & item: m_buckets[i])
			temp.add(item);
	}

	return temp;
}

template <typename T>
Set<T>::Set(const Set& other) :
	m_bucket_size(other.m_bucket_size),
	m_items_count(other.m_items_count),
	m_max_load_factor(other.m_max_load_factor),
	m_buckets(new std::list<T>[other.m_bucket_size])
{
	for (int i = 0; i < m_bucket_size; i++)
		m_buckets[i] = other.m_buckets[i];
}
