#pragma once
#include <list>

using namespace std;

template<class TValue, class TKey = int>
class HashEntidad {
	TKey key;
	TValue value;
public:
	HashEntidad(TKey key, TValue value) : key(key), value(value) {}

	TKey& getKey() { return key; }
	TValue& getValue() { return value; }
};

template<class TValue, class TKey = int>
class Algorithms;

template<class TValue, class TKey = int>
class HashTable {
	HashEntidad<TValue, TKey>** table;
	int num_elementos;
	int max_size;
	friend class Algorithms<TValue, TKey>;
public:
	HashTable(int max_size = 100) {
		this->max_size = max_size;
		table = new HashEntidad<TValue, TKey>*[max_size];
		for (int i = 0; i < max_size; ++i) {
			table[i] = nullptr;
		}
		num_elementos = 0;
	}

	void insertar(TKey key, TValue value) {
		int hash = create_key(key);
		table[hash] = new HashEntidad<TValue, TKey>(key, value);
		++num_elementos;
	}

	bool exists(TKey key) {
		int base, step, hash;
		base = key % max_size;
		hash = base;
		step = 0;
		while (table[hash] != nullptr) {
			if (table[hash]->getKey() == key) return true;
			hash = (base + step) % max_size;
			++step;
			if (step >= max_size) break;
		}
		return false;
	}

	int size() {
		return max_size;
	}

	int size_actual() {
		return num_elementos;
	}

private:
	int create_key(TKey key) {
		int base, step, hash;
		if (num_elementos == max_size) return -1;
		base = key % max_size;
		hash = base;
		step = 0;
		while (table[hash] != nullptr) {
			hash = (base + step) % max_size;
			++step;
		}
		return hash;
	}
};

template<class TValue, class TKey>
class Algorithms {
	typedef HashTable<TValue, TKey> Table;
	typedef HashEntidad<TValue, TKey> Entidad;
public:
	static void for_each(Table* ht, function<void(TValue&)> pred) {
		for (int i = 0; i < ht->size_actual(); ++i) {
			pred(ht->table[i]->getValue());
		}
	}

	static Entidad* find_if(Table* ht, function<bool(TValue&)> pred) {
		for (int i = 0; i < ht->size_actual(); ++i) {
			if (pred(ht->table[i]->getValue())) {
				return ht->table[i];
			}
		}
		return nullptr;
	}

	static Entidad* search_n(Table* ht, size_t count, function<bool(TValue&)> pred) {
		Entidad* first = nullptr;
		size_t contador = 0;
		for (int i = 0; i < ht->size_actual(); ++i) {
			if (pred(ht->table[i]->getValue())) {
				if (contador == 0) first = ht->table[i];
				++contador;
				if (contador >= count) return first;
				continue;
			}
			contador = 0;
		}
		return nullptr;
	}

	static void stable_sort(Table* ht, list<TValue>& output, function<bool(TValue&, TValue&)> pred) {
		list<TValue> sorted;
		HashTable<bool> already_sorted(ht->size());
		int pos = 0;
		while (ht->size_actual() != sorted.size()) {
			if (already_sorted.exists(ht->table[pos]->getKey())) {
				pos = (pos + 1) % ht->size_actual();
				continue;
			}
			int posMenor = pos;
			for (int j = 0; j < ht->size_actual(); ++j) {
				if (!already_sorted.exists(ht->table[j]->getKey()) && pred(ht->table[j]->getValue(), ht->table[posMenor]->getValue())) {
					posMenor = j;
				}
			}
			sorted.push_back(ht->table[posMenor]->getValue());
			already_sorted.insertar(ht->table[posMenor]->getKey(), true);
			pos = (pos + 1) % ht->size_actual();
		}
		output.assign(sorted.begin(), sorted.end());
	}


	static typename list<TValue>::iterator partition(Table* ht, list<TValue>& output, function<bool(TValue&)> pred) {
		list<TValue> falso;
		for (int i = 0; i < ht->size_actual(); ++i) {
			if (pred(ht->table[i]->getValue())) {
				output.push_back(ht->table[i]->getValue());
				continue;
			}
			falso.push_back(ht->table[i]->getValue());
		}
		auto it = next(output.end(), -1);
		output.splice(output.end(), falso);
		return next(it);
	}
};