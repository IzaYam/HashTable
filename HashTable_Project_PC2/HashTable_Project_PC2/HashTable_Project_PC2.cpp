#include <iostream>
#include <string>
#include <functional>
#include <fstream>
#include <sstream>
#include "Solution.hpp"

using namespace std;

struct Registro {
	float customer_ID = 0.0f;
	float call = 0.0f;
	float complains = 0.0f;
	float subscripcion = 0.0f;
	float charge = 0.0f;
	float seconds_use = 0.0f;
	float frequency_use = 0.0f;
	float frequency_SMS = 0.0f;
	float distinct_num = 0.0f;
	float age_group = 0.0f;
	float tariff = 0.0f;
	float status = 0.0f;
	float churn = 0.0f;
	float value = 0.0f;

	Registro() {}

	friend ostream& operator<< (ostream& out, const Registro& r) {
		out << r.customer_ID << "\t" << r.call << "\t" << r.complains << "\t" << r.subscripcion << "\t" << r.charge << "\t" << r.seconds_use << "\t" << r.frequency_use << "\t" << r.frequency_SMS << "\t" << r.distinct_num << "\t" << r.age_group << "\t" << r.tariff << "\t" << r.status << "\t" << r.churn << "\t" << r.value;
		return out;
	}
};

class DataSet {
	HashTable<Registro*>* registros;

public:
	DataSet() {
		registros = new HashTable<Registro*>(3150);
	}

	~DataSet() {
		if (registros != nullptr) {
			delete registros;
		}
	}

	void leer(string nombre) {
		fstream file(nombre, ios::in);
		if (!file.is_open()) return;
		string stringFila, stringElemento;
		getline(file, stringFila); //omitir el header
		while (getline(file, stringFila)) {
			Registro* registro = new Registro();
			stringstream am(stringFila);
			getline(am, stringElemento, ',');
			registro->customer_ID = stof(stringElemento);
			getline(am, stringElemento, ',');
			registro->call = stof(stringElemento);
			getline(am, stringElemento, ',');
			registro->complains = stof(stringElemento);
			getline(am, stringElemento, ',');
			registro->subscripcion = stof(stringElemento);
			getline(am, stringElemento, ',');
			registro->charge = stof(stringElemento);
			getline(am, stringElemento, ',');
			registro->seconds_use = stof(stringElemento);
			getline(am, stringElemento, ',');
			registro->frequency_use = stof(stringElemento);
			getline(am, stringElemento, ',');
			registro->frequency_SMS = stof(stringElemento);
			getline(am, stringElemento, ',');
			registro->distinct_num = stof(stringElemento);
			getline(am, stringElemento, ',');
			registro->age_group = stof(stringElemento);
			getline(am, stringElemento, ',');
			registro->tariff = stof(stringElemento);
			getline(am, stringElemento, ',');
			registro->status = stof(stringElemento);
			getline(am, stringElemento, ',');
			registro->churn = stof(stringElemento);
			getline(am, stringElemento, ',');
			registro->value = stof(stringElemento);
			registros->insertar(int(registro->customer_ID), registro);
		}
		file.close();
	}

	void for_each_test() {
		float sum = 0;
		Algorithms<Registro*>::for_each(registros, [&](Registro*& a) {
			sum += a->subscripcion;
			});
		cout << sum / registros->size_actual();
	}

	void find_if_test() {
		auto it = Algorithms<Registro*>::find_if(registros, [](Registro*& a) {
			return a->call == 25;
			});
		cout << *(*it).getValue();
	}

	void search_n_test() {
		auto it = Algorithms<Registro*>::search_n(registros, 2, [](Registro*& a) {
			return a->call == 7;
			});
		cout << (it != nullptr ? "se encontro" : "no se encontro");
	}

	void stable_sort_test() {
		list<Registro*> output;
		Algorithms<Registro*>::stable_sort(registros, output, [](Registro*& a, Registro*& b) {
			return a->subscripcion < b->subscripcion;
			});

		for (Registro*& elem : output) {
			cout << *elem << "\n";
		}
	}

	void partition_test() {
		list<Registro*> output;
		auto fin = Algorithms<Registro*>::partition(registros, output, [](Registro*& a) {
			return a->tariff == 1;
			});
		cout << *(*fin) << endl;
	}
};

int main() {
	DataSet dataset;
	dataset.leer("dataset.csv");
	short op;
	while (true) {
		cout << "\nMenu\n";
		cout << "1) For each\n";
		cout << "2) Find if\n";
		cout << "3) search_n\n";
		cout << "4) Stable Sort\n";
		cout << "5) Partition\n";
		cout << "Opcion: "; cin >> op;
		switch (op) {
		case 1:
			dataset.for_each_test();
			break;
		case 2:
			dataset.find_if_test();
			break;
		case 3:
			dataset.search_n_test();
			break;
		case 4:
			dataset.stable_sort_test();
			break;
		case 5:
			dataset.partition_test();
			break;
		default: cout << "error";
			break;
		}
	}
}