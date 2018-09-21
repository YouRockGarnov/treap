#include <vector>
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <exception>
#include <memory>
#include <vector>

using namespace std;
using ll = long long;
const ll INF = 10e+8;

template<typename T>
class TreapManager;

template<typename T>
class Treap {
    friend TreapManager<T>;

public:
    Treap(const T& val_, const ll& y_) {
        y = y_;
        val = val_;
    }

    Treap() {
        y = rand() % INF;
    }

    ll key() {
        return L != nullptr ? L->count : 0;
    }

private:
    Treap<T>* L = nullptr;
    Treap<T>* R = nullptr;

    T val;
    ll y;
    ll count = 1;
};

template<typename T>
class TreapManager {
public:
    using treapPair = pair<Treap<T>*, Treap<T>* >;
    friend Treap<T>;

    TreapManager() {
        srand(time(NULL));
    }

    static void Insert(ll pos, Treap<T>* node, Treap<T>*& t) {
        if (t == nullptr) {
            t = node;
            return;
        }

        treapPair tp = Split(pos, t);
        tp.first = Merge(tp.first, node);
        t = Merge(tp.first, tp.second);
    }

    static void Remove(ll key, Treap<T>*& t) {
        if (t == nullptr)
            return;

        if (t->key() == key) {
            t = Merge(t->L, t->R);
            return;
        }

        if (t->key() > key) {
            Remove(key, t->L);
        } else {
            Remove(key - t->key() - 1, t->R);
        }

        Update(t);
    }

    static void Set(ll key, T val, Treap<T>*& t) {
        if (t == nullptr)
            return;

        if (t->key() == key) {
            t->val = val;
            return;
        }

        if (t->key() > key) {
            Set(key, val, t->L);
        } else {
            Set(key - t->key() - 1, val, t->R);
        }

        Update(t);
    }

    static T At(ll key, Treap<T>*& t) {
        if (t == nullptr || t == NULL)
            throw std::range_error("");

        if (t->key() == key) {
            return t->val;
        }

        if (t->key() > key) {
            return At(key, t->L);
        } else {
            return At(key - t->key() - 1, t->R);
        }

        Update(t);
    }

	static void Show(Treap<T>*& t) {
		if (t == nullptr)
			return;

		Show(t->L);
		cout << t->val << ' ';
		Show(t->R);
	}

private:
    static treapPair Split(const T& key, Treap<T>*& t) {
        if (t == nullptr)
            return { nullptr, nullptr };

        if (key > t->key()) {
            treapPair p = Split(key - t->key() - 1, t->R);

            t->R = p.first;

            Update(t);
            return { t, p.second };

        } else {
            treapPair p = Split(key, t->L);

            t->L = p.second;

            Update(t);
            return{ p.first, t };
        }
    }

    static void Update(Treap<T>*& t) {
        if (t == nullptr)
            return;

        ll Lcount = 0, Rcount = 0;

        if (t->L != nullptr) {
            Lcount = t->L->count;
        }
        if (t->R != nullptr)
            Rcount = t->R->count;

        t->count = Lcount + Rcount + 1;
    }

    static Treap<T>* Merge(Treap<T>*& L, Treap<T>*& R) {
        if (L == nullptr)
            return R;
        if (R == nullptr)
            return L;

        if (L->y > R->y) {
            L->R = Merge(L->R, R);
            Update(L);
            return L;
        } else {
            R->L = Merge(L, R->L);
            Update(R);
            return R;
        }
    }
};

template<typename T>
class Vector {
public:
    void push_back(const T& val) {
        insert(sz + 1, val);
    }

    void insert(int index, const T& val) {
		if (sz == 0) {
			treap = new Treap<T>(val, rand() % INF);
			sz++;
			return;
		}

        sz++;

        if (index > sz)
            throw std::range_error("");

        Treap<T>* node = new Treap<T>(val, rand() % INF);

        treapManager.Insert(index, node, treap);
    }

    void remove(int index) {
        treapManager.Remove(index, treap);
        sz--;
    }

    void set(int index, const T& val) {
        treapManager.Set(index, val, treap);
    }

    T at(int index) {
        return treapManager.At(index, treap);
    }

    size_t size() {
        return sz;
    }

	void show() {
		if (sz > 0) {
			treapManager.Show(treap);
			std::cout << std::endl;
		}
	}
private:
    size_t sz = 0;
    TreapManager<T> treapManager = TreapManager<T>();
    Treap<T>* treap;
};

//for debuging or checking
int check() {
    Vector<int> vec;
    std::vector<int> stdvec;

    srand(time(NULL));
    for (int i = 0; i < 10000; ++i) {
/*		std::cout << "////////////" << std::endl << "MY: ";
		vec.show();

		std::cout << std::endl << "STD: ";
		for (auto elem : stdvec) {
			std::cout << elem;
		}
		std::cout << std::endl;*/

        int choice = rand() % 5;

        switch (choice){
            case 0:{
                int pushing = rand() % 10000;
                vec.push_back(pushing);
                stdvec.push_back(pushing);

				break;
            }

            case 1:{
                int pushing = rand() % 10000;

                if (stdvec.empty()){
                    vec.push_back(pushing);
                    stdvec.push_back(pushing);
					continue;
                }

                int index = rand() % stdvec.size();

                vec.insert(index, pushing);

                vector<int>::iterator iter = stdvec.begin();
                for (int j = 0; j < index; ++j) {
                    ++iter;
                }

                stdvec.insert(iter, pushing);

				break;
            }

            case 2:{
                if (stdvec.size() == 0)
                    continue;

                int index = rand() % stdvec.size();

                vec.remove(index);

                vector<int>::iterator iter = stdvec.begin();
                for (int j = 0; j < index; ++j) {
                    ++iter;
                }

                stdvec.erase(iter);

				break;
            }

            case 3:{
                if (stdvec.size() == 0)
                    continue;

                int index = rand() % stdvec.size();
                int val = rand() % 10000;

                vec.set(index, val);
                stdvec[index] = val;

				break;
            }

            case 4:{
                if (stdvec.size() == 0)
                    continue;

                int index = rand() % stdvec.size();

                if (stdvec[index] != vec.at(index))
                    std::cout<<"Error: " << stdvec[index] << " != " << vec.at(index);

				break;
            }
        }
    }

    system("pause");
}
