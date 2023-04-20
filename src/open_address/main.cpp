#include <iostream>
#include <ctime>

#include "hash.hpp"
#include "passport_data.hpp"

using namespace std;

int main()
{
    srand((int)time(0));

    const size_t size = 100;
    const string filename = "hash-table.hex";

    {
        HashTable<string, PassportData> *table = new HashTable<string, PassportData>(size);

        string entry1_test_key,
            entry2_test_key;

        for (int i = 0; i < size; i++)
        {
            PassportData data = PassportData();
            if (i == 0)
                entry1_test_key = data.get_date_of_birth();
            else if (i == 1)
                entry2_test_key = data.get_date_of_birth();
            table->set(data.get_date_of_birth(), data);
        }

        table->print();

        HashEntry<string, PassportData> *entry1 = table->get(entry1_test_key);

        if (entry1 != nullptr)
            cout << entry1->value << ' ' << table->hash(entry1_test_key) << endl;
        else
            cout << "1. Not found: " << entry1_test_key << ' ' << table->hash(entry1_test_key) << endl;

        HashEntry<string, PassportData> *entry2 = table->pop(entry2_test_key);

        if (entry2 != nullptr)
            cout << entry2->value << ' ' << table->hash(entry2_test_key) << endl;
        else
            cout << "2. Not found: " << entry2_test_key << ' ' << table->hash(entry2_test_key) << endl;

        table->print();

        HashEntry<string, PassportData> *entry3 = table->get(entry2_test_key);

        if (entry3 != nullptr)
            cout << entry3->value << ' ' << table->hash(entry2_test_key) << endl;
        else
            cout << "3. Not found: " << entry2_test_key << ' ' << table->hash(entry2_test_key) << endl;

        table->save(filename);

        delete table;
    }

    cout << "\n\n\n";

    {
        HashTable<string, PassportData> *table = new HashTable<string, PassportData>(size);

        table->load(filename);

        table->print();

        delete table;
    }
}