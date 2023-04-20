#pragma once

#include <iostream>
#include <fstream>
#include <random>

using namespace std;

template <class T>
int hash_k(T v); // UnimplementedError

int hash_k(string v)
{
    int k = 0;
    for (int i = 0; i < v.length(); i++)
        k += (int)(v[i]);
    return k;
}

template <class K, class V>
struct HashEntry
{
    K key;
    V value;

    HashEntry(){};
    ~HashEntry(){};

    friend ofstream &operator<<(ofstream &, const HashEntry<K, V> &);
    friend ifstream &operator>>(ifstream &, HashEntry<K, V> &);
};

template <class K, class V>
class HashTable;

template <class K, class V>
class HashNode
{
    friend HashTable<K, V>;

    HashEntry<K, V> *entry = nullptr;
    HashNode<K, V> *next = nullptr;

    HashNode() {}
    ~HashNode()
    {
        if (entry != nullptr)
            delete entry;
        if (next != nullptr)
            delete next;
    }

public:
    friend ofstream &operator<<(ofstream &out, const HashNode<K, V> &node)
    {
        return (out << *node.entry);
    }

    friend ifstream &operator>>(ifstream &in, HashNode<K, V> &node)
    {
        if (node.entry == nullptr)
            node.entry = new HashEntry<K, V>();
        return (in >> *node.entry);
    }
};

template <class K, class V>
class HashTable
{
    const double A = rand() % 1000 * 0.001;

    size_t size;
    HashNode<K, V> *table;

    size_t _collision_count = 0;

public:
    HashTable(size_t size) : size(size)
    {
        table = new HashNode<K, V>[size];
        for (int i = 0; i < size; i++)
            table[i] = HashNode<K, V>();
    }
    ~HashTable()
    {
        delete[] table;
        cout << "Collision count: " << _collision_count << "; Table size: " << size << '.' << endl;
    }

    int hash(const K &key)
    {
        const int k = hash_k(key);
        return size * (k * A - (int)(k * A));
    }

    void set(const K &key, const V &value)
    {
        int h = hash(key);

        // cout << "SET (" << h << "): " << key << " : " << value << endl;

        HashNode<K, V> *node = &(table[h]);
        bool flag = 0;

        for (; node != nullptr && !flag; node = node->next)
            if (node->entry != nullptr)
            {
                if (node->entry->key == key)
                {
                    node->entry->value = value;
                    flag = 1;
                }
                else if (node->next == nullptr)
                {
                    node->next = new HashNode<K, V>();
                    _collision_count++;
                }
            }
            else
            {
                node->entry = new HashEntry<K, V>();
                node->entry->key = key;
                node->entry->value = value;
                flag = 1;
            }
    }

    HashEntry<K, V> *get(K key)
    {
        int h = hash(key);
        bool found, end;
        HashNode<K, V> *node = &(table[h]);

        do
        {
            found = (node->entry != nullptr && node->entry->key == key);
            if (!found)
                if (node->next == nullptr)
                    end = 1;
                else
                    node = node->next;
        } while (!(found || end));

        if (found)
            return node->entry;
        else
            return nullptr;
    }

    HashEntry<K, V> *pop(K key)
    {
        int h = hash(key);
        bool found, end;
        HashNode<K, V> *node = &(table[h]);
        HashNode<K, V> *prev = nullptr;

        do
        {
            found = (node->entry != nullptr && node->entry->key == key);
            if (!found)
                if (node->next == nullptr)
                    end = 1;
                else
                {
                    prev = node;
                    node = node->next;
                }
        } while (!(found || end));

        if (found)
        {
            // unpack node
            prev->next = node->next;
            HashEntry<K, V> *entry = node->entry;
            node->entry = nullptr;
            node->next = nullptr;
            // нет необходимости удалять пустой узел, он все равно будет заполнен set'ом при соответствии хешей.
            return entry;
        }
        else
            return nullptr;
    }

    void print()
    {
        for (int i = 0; i < size; i++)
        {
            HashNode<K, V> *node = &(table[i]);
            if (node->entry != nullptr)
            {
                for (; node != nullptr; node = node->next)
                    if (node->entry != nullptr)
                        cout << i << ". " << node->entry->key << ": " << node->entry->value << "; ";
                cout << endl;
            }
            else
                cout << "Empty str" << endl;
        }
    }

    bool save(const string filename) const
    {
        ofstream file(filename, ios::binary);
        if (!file)
        {
            file.close();
            return false;
        }
        for (int i = 0; i < size; i++)
        {
            for (HashNode<K, V> *node = &(table[i]); node != nullptr; node = node->next)
                if (node->entry != nullptr)
                    file << *node;
            file << '\n';
        }

        return true;
    }

    bool load(const string filename)
    {
        ifstream file(filename, ios::binary);
        if (!file)
        {
            file.close();
            return false;
        }
        int i = 0;
        while (i < size && !file.eof() && file.peek() != EOF)
        {
            HashNode<K, V> *node = new HashNode<K, V>();
            HashNode<K, V> *p = node;
            while (file.peek() != '\n')
            {
                if (p->entry != nullptr)
                {
                    p->next = new HashNode<K, V>();
                    p = p->next;
                }
                file >> *p;
            }
            file.get();
            table[i] = *node;
            i++;
        }

        file.close();
        return true;
    }
};
