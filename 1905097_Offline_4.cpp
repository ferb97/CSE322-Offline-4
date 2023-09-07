#include<bits/stdc++.h>
using namespace std;

int main()
{
    string data_string;
    cout << "enter data string: ";
    getline(cin, data_string);
//    cout << data_string << endl;

    int m;
    cout << "enter number of data bytes in a row <m>: ";
    cin >> m;
//    cout << m << endl;

    double p;
    cout << "enter probability <p>: ";
    cin >> p;
//    cout << p << endl;

    string generator_polynomial;
    cout << "enter generator polynomial: ";
    cin >> generator_polynomial;
//    cout << generator_polynomial << endl;

    cout << endl << endl;
    // data padding
    int rem = data_string.size() % m;

    for(int i = 0; i < ((m - rem) % m); i++){
        data_string += "~";
    }
//    cout << data_string.size() <<endl;
    cout << "data string after padding: " << data_string << endl << endl;

    int total_rows = data_string.size() / m;
    string data_rows[total_rows];

    for(int i = 0; i < total_rows; i++){
        data_rows[i] = "";
    }

    int row_number = 0, entry_number = 0;

    for(int i = 0; i < data_string.size(); i++){
        entry_number++;
        int ascii_value = (int)data_string[i];
//        cout << ascii_value << endl;
        bitset<8> binaryRepresentation(ascii_value);
        string binary_string = binaryRepresentation.to_string();
//        cout << binary_string << endl;
        data_rows[row_number] += binary_string;
        if(entry_number == m){
           row_number++;
           entry_number = 0;
        }
    }

    cout << "data block <ascii code of m characters per row>:" << endl;
    for(int i = 0; i < total_rows; i++){
        cout << data_rows[i] << endl;
    }

    return 0;
}
