#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <random>

#include "hash.hpp"

using namespace std;

const char _PASSPORT_DATA_DELIMITER = '\0';

const string PASSPORT_DATA_MANES[12] = {
    "Крутое имя 1",
    "Лучшее имя 1",
    "Хорошее имя",
    "Не самое лучшее имя",
    "Лучшее имя 2",
    "Имя №1 в России",
    "Самое редкое имя",
    "Андрей",
    "Последовательность заметили?",
    "Крутое имя 2",
    "А она есть...",
    "Плохое имя",
};

string _get_rand_name()
{
    return PASSPORT_DATA_MANES[rand() % 12];
}

string _get_rand_date_of_birth()
{
    stringstream ss;
    ss << setw(2) << setfill('0') << to_string(rand() % 30 + 1);
    ss << '.';
    ss << setw(2) << setfill('0') << to_string(rand() % 12 + 1);
    ss << '.';
    ss << to_string(1970 + rand() % 50 + 1);
    return ss.str();
}

string _get_rand_passport_number()
{
    stringstream ss;
    ss << setw(2) << setfill('0') << to_string(rand() % 100 + 1);
    ss << ' ';
    ss << setw(2) << setfill('0') << to_string(rand() % 100 + 1);
    ss << ' ';
    ss << setw(6) << setfill('0') << to_string(rand() % 1000000 + 1);
    return ss.str();
}

class PassportData
{

protected:
    string name;
    string date_of_birth;
    string passport_number;

public:
    PassportData() : name(_get_rand_name()),
                     date_of_birth(_get_rand_date_of_birth()),
                     passport_number(_get_rand_passport_number()) {}
    PassportData(
        string name,
        string date_of_birth,
        string passport_number) : name(name),
                                  date_of_birth(date_of_birth),
                                  passport_number(passport_number) {}

    PassportData(const PassportData &other) : name(other.name),
                                              date_of_birth(other.date_of_birth),
                                              passport_number(other.passport_number) {}

    ~PassportData(){};

    string get_name() const { return name; }
    void set_name(string v) { name = v; }

    string get_date_of_birth() const { return date_of_birth; }
    void set_date_of_birth(string v) { date_of_birth = v; }

    string get_passport_number() const { return passport_number; }
    void set_passport_number(string v) { passport_number = v; }

    friend ostream &operator<<(ostream &, const PassportData &);
    friend istream &operator>>(istream &, PassportData &);

    friend ofstream &operator<<(ofstream &, const PassportData &);
    friend ifstream &operator>>(ifstream &, PassportData &);
};

ostream &operator<<(ostream &out, const PassportData &data)
{
    return (out << "PassportData(name: \"" << data.name
                << "\", date_of_birth: \"" << data.date_of_birth
                << "\", passport_number: \"" << data.passport_number << "\")");
}

istream &operator>>(istream &in, PassportData &data)
{
    string buf;

    cout << "Input name: ";
    in.sync();
    getline(in, buf, '\n');
    data.name = buf;

    cout << "Input date of birth: ";
    in.sync();
    getline(in, buf, '\n');
    data.date_of_birth = buf;

    cout << "Input passport number: ";
    in.sync();
    getline(in, buf, '\n');
    data.passport_number = buf;

    return in;
}

ofstream &operator<<(ofstream &out, const PassportData &data)
{
    out << data.name << _PASSPORT_DATA_DELIMITER;
    out << data.date_of_birth << _PASSPORT_DATA_DELIMITER;
    out << data.passport_number << _PASSPORT_DATA_DELIMITER;
    return out;
}

ifstream &operator>>(ifstream &in, PassportData &data)
{

    string buf;

    in.sync();
    getline(in, buf, _PASSPORT_DATA_DELIMITER);
    data.name = buf;

    in.sync();
    getline(in, buf, _PASSPORT_DATA_DELIMITER);
    data.date_of_birth = buf;

    in.sync();
    getline(in, buf, _PASSPORT_DATA_DELIMITER);
    data.passport_number = buf;

    return in;
}

ofstream &operator<<(ofstream &out, const HashEntry<string, PassportData> &entry)
{
    out << entry.key << _PASSPORT_DATA_DELIMITER;
    return (out << entry.value);
}

ifstream &operator>>(ifstream &in, HashEntry<string, PassportData> &entry)
{
    string buf;

    in.sync();
    getline(in, buf, _PASSPORT_DATA_DELIMITER);
    entry.key = buf;

    return (in >> entry.value);
}