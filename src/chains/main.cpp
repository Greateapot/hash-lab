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

        for (int i = 0; i < size; i++)
        {
            PassportData data = PassportData();
            table->set(data.get_date_of_birth(), data);
        }

        table->print();

        HashEntry<string, PassportData> *entry1 = table->get("24.04.2002");

        if (entry1 != nullptr)
            cout << entry1->value << endl;
        else
            cout << "Not found" << endl;

        HashEntry<string, PassportData> *entry2 = table->pop("30.10.2008");

        if (entry2 != nullptr)
            cout << entry2->value << endl;
        else
            cout << "Not found" << endl;
        
        table->print();

        HashEntry<string, PassportData> *entry3 = table->get("30.10.2008");

        if (entry3 != nullptr)
            cout << entry3->value << endl;
        else
            cout << "Not found" << endl;

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