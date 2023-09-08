#include<bits/stdc++.h>
#include<windows.h>
using namespace std;

// function to flip a bit
char flipBit(char ch)
{
    if(ch == '1'){
       return '0';
    }
    else{
       return '1';
    }
}

// function to add hamming bits to a data string
string addHammingCheckBits(string data, int r)
{
    // add the given string
    string new_data = "";
    int cur = 0, tmp = 1;
    for(int i = 0; i < data.size() + r; i++){
        if(i == tmp - 1){
           new_data += "2";
           tmp *= 2;
        }
        else{
            new_data += data[cur];
            cur++;
        }
    }

    // calculate hamming bits and add
    for(int i = 1; i <= data.size() + r; i *= 2){
        int hamming_bit = 0;

        for(int j = i + 1; j <= data.size() + r; j++){
            if(j & i){
               if(new_data[j - 1] == '1'){
                 hamming_bit ^= 1;
               }
            }
        }

        if(hamming_bit == 1){
           new_data[i - 1] = '1';
        }
        else{
           new_data[i - 1] = '0';
        }
    }

    return new_data;
}

// xor subtraction between dividend and divisor
string xorSubtraction(string dividend, string divisor)
{
    string ans = "";

    for(int i = 0; i < divisor.size(); i++){
        if(dividend[i] == divisor[i]){
           ans += "0";
        }
        else{
           ans += "1";
        }
    }

    return ans;
}

// get remainder or CRC check bits
string getCRCCheckBits(string dividend, string divisor)
{
    int len = divisor.size();
    string tmp_string = dividend.substr(0, len);
    int cur_pos = len;

    while(cur_pos < dividend.size()){
        if(tmp_string[0] == '1'){
           // xor subtraction possible
           tmp_string = xorSubtraction(tmp_string, divisor);
           tmp_string = tmp_string.substr(1) + dividend[cur_pos];
        }

        else{
           // adding another bit from dividend
           tmp_string = tmp_string.substr(1) + dividend[cur_pos];
        }

        cur_pos++;
    }

    if(tmp_string[0] == '1'){
       tmp_string = xorSubtraction(tmp_string, divisor);
    }

    tmp_string = tmp_string.substr(1);
    return tmp_string;
}

// generate error bits
string getErrorsInDataBits(string data_bits, double p)
{
    // error generator
    mt19937 generator(static_cast<unsigned int>(time(nullptr)));
    uniform_real_distribution<double> distribution(0.0, 1.0);

    for(int i = 0; i < data_bits.size(); i++){
        double randomValue = distribution(generator);
        // if randomValue < p, flip the bit
        if(randomValue < p){
           data_bits[i] = flipBit(data_bits[i]);
        }
    }
    return data_bits;
}

// Error detection using CRC
bool detectErrorUsingCRC(string dividend, string divisor)
{
    int len = divisor.size();
    string tmp_string = dividend.substr(0, len);
    int cur_pos = len;

    while(cur_pos < dividend.size()){
        if(tmp_string[0] == '1'){
           tmp_string = xorSubtraction(tmp_string, divisor);
           tmp_string = tmp_string.substr(1) + dividend[cur_pos];
        }

        else{
           tmp_string = tmp_string.substr(1) + dividend[cur_pos];
        }

        cur_pos++;
    }

    if(tmp_string[0] == '1'){
       tmp_string = xorSubtraction(tmp_string, divisor);
    }

    // if remainder is not 0, return true
    for(int i = 0; i < tmp_string.size(); i++){
        if(tmp_string[i] != '0'){
           return true;
        }
    }

    return false;
}

// correct the error using hamming bits and get the original data string
string getDataBitsWithoutHammingBits(string data_hamming)
{
    string incorrectBits = "";
    for(int i = 1; i <= data_hamming.size(); i *= 2){
        int hamming_bit = 0;

        for(int j = i + 1; j <= data_hamming.size(); j++){
            if(j & i){
               if(data_hamming[j - 1] == '1'){
                 hamming_bit ^= 1;
               }
            }
        }

        if((hamming_bit == 1 && data_hamming[i - 1] == '1') || (hamming_bit == 0 && data_hamming[i - 1] == '0')){
           // data is correct
           incorrectBits += "0";
        }
        else{
           // data is not correct
           incorrectBits += "1";
        }
    }

    // calculate the position of the wrong bit
    int pos = 1, wrong_bit_pos = 0;
    for(int i = 0; i < incorrectBits.size(); i++){
        if(incorrectBits[i] == '1'){
           wrong_bit_pos += pos;
        }
        pos *= 2;
    }

    // if wrong bit position in range, correct it
    if(wrong_bit_pos > 0 && wrong_bit_pos <= data_hamming.size()){
       data_hamming[wrong_bit_pos - 1] = flipBit(data_hamming[wrong_bit_pos - 1]);
    }

    // copying the new string removing the hamming bits
    string new_data = "";
    pos = 1;
    for(int i = 0; i < data_hamming.size(); i++){
        if(i == pos - 1){
           pos *= 2;
           continue;
        }
        new_data += data_hamming[i];
    }

    return new_data;
}

int main()
{
    // take input
    string data_string;
    cout << "enter data string: ";
    getline(cin, data_string);

    int m;
    cout << "enter number of data bytes in a row (m): ";
    cin >> m;

    double p;
    cout << "enter probability (p): ";
    cin >> p;

    string generator_polynomial;
    cout << "enter generator polynomial: ";
    cin >> generator_polynomial;

    cout << endl << endl;

    // data padding
    int rem = data_string.size() % m;

    for(int i = 0; i < ((m - rem) % m); i++){
        data_string += "~";
    }
    cout << "data string after padding: " << data_string << endl << endl;

    // creating data blocks
    int total_rows = data_string.size() / m;
    string data_rows[total_rows];

    for(int i = 0; i < total_rows; i++){
        data_rows[i] = "";
    }

    int row_number = 0, entry_number = 0;

    for(int i = 0; i < data_string.size(); i++){
        entry_number++;

        int ascii_value = (int)data_string[i];
        bitset<8> binaryRepresentation(ascii_value);
        string binary_string = binaryRepresentation.to_string();

        data_rows[row_number] += binary_string;
        if(entry_number == m){
           row_number++;
           entry_number = 0;
        }
    }

    // printing data blocks
    cout << "data block (ascii code of m characters per row):" << endl;
    for(int i = 0; i < total_rows; i++){
        cout << data_rows[i] << endl;
    }
    cout << endl;

    // calculating hamming bits
    int hamming_r_bits = 0, tmp = 1, data_row_size = m * 8;

    while(tmp < (data_row_size + hamming_r_bits + 1)){
        hamming_r_bits++;
        tmp *= 2;
    }

    // adding hamming bits
    string data_row_with_hamming_bits[total_rows];

    cout << "data block after adding check bits:" << endl;
    for(int i = 0; i < total_rows; i++){
        data_row_with_hamming_bits[i] = addHammingCheckBits(data_rows[i], hamming_r_bits);
        tmp = 1;

        for(int j = 0; j < data_row_size + hamming_r_bits; j++){
            if(j == tmp - 1){
               SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
               tmp *= 2;
            }
            else{
               SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }
            cout << data_row_with_hamming_bits[i][j];
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        cout << endl;
    }
    cout << endl;

    // serializing data column wise
    string column_wise_serialized_data = "";
    for(int j = 0; j < data_row_size + hamming_r_bits; j++){
        for(int i = 0; i < total_rows; i++){
            column_wise_serialized_data += data_row_with_hamming_bits[i][j];
        }
    }

    cout << "data bits after column-wise serialization:" << endl;
    cout << column_wise_serialized_data << endl << endl;

    // adding CRC checksums
    int data_bits_without_CRC_length = (data_row_size + hamming_r_bits) * total_rows;
    int data_bits_with_CRC_length = data_bits_without_CRC_length + generator_polynomial.size() - 1;
    string data_bits_with_CRC_bits = column_wise_serialized_data.substr(0);

    for(int i = 0; i < generator_polynomial.size() - 1; i++){
        data_bits_with_CRC_bits += "0";
    }

    string CRC_r_bits = getCRCCheckBits(data_bits_with_CRC_bits, generator_polynomial);
    for(int i = data_bits_without_CRC_length; i < data_bits_with_CRC_length; i++){
        data_bits_with_CRC_bits[i] = CRC_r_bits[i - data_bits_without_CRC_length];
    }

    // printing data with CRC checksums
    cout << "data bits after appending CRC checksum (sent frame):" << endl;
    for(int i = 0; i < data_bits_with_CRC_length; i++){
        if(i < data_bits_without_CRC_length){
           SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        }
        else{
           SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
        }
        cout << data_bits_with_CRC_bits[i];
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    cout << endl << endl;

    // adding errors
    string data_bits_with_error = getErrorsInDataBits(data_bits_with_CRC_bits, p);

    // printing received data frames
    cout << "received frame:" << endl;
    for(int i = 0; i < data_bits_with_error.size(); i++){
        if(data_bits_with_error[i] == data_bits_with_CRC_bits[i]){
           SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        }
        else{
           SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
        }
        cout << data_bits_with_error[i];
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    cout << endl << endl;

    // detect error using CRC checksums
    bool isError = detectErrorUsingCRC(data_bits_with_error, generator_polynomial);

    cout << "result of CRC checksum matching: ";
    if(isError){
       cout << "error detected" << endl << endl;
    }
    else{
       cout << "no error detected" << endl << endl;
    }

    // removing CRC bits
    string data_bits_received_without_CRC = data_bits_with_error.substr(0, data_bits_without_CRC_length);
    string data_rows_received[total_rows];

    for(int i = 0; i < total_rows; i++){
        data_rows_received[i] = "";
    }

    // getting error positions
    set<int> error_bits_position;
    int cur_pos = 0;

    while(cur_pos < data_bits_without_CRC_length){
        for(int i = 0; i < total_rows; i++){
            if(data_bits_received_without_CRC[cur_pos] != data_bits_with_CRC_bits[cur_pos]){
               error_bits_position.insert(cur_pos);
            }
            data_rows_received[i] += data_bits_received_without_CRC[cur_pos];
            cur_pos++;
        }
    }

    // printing received data bits without CRC bits
    cout << "data block after removing CRC checksum bits:" << endl;
    for(int i = 0; i < total_rows; i++){
        for(int j = 0; j < data_row_size + hamming_r_bits; j++){
            int position = j * total_rows + i;
            if(error_bits_position.find(position) != error_bits_position.end()){
               SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
            }
            else{
               SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }
            cout << data_rows_received[i][j];
        }
        cout << endl;
    }
    cout << endl;

    // removing hamming bits and correcting data bits if possible
    string data_rows_received_without_hamming_bits[total_rows];
    for(int i = 0; i < total_rows; i++){
        data_rows_received_without_hamming_bits[i] = getDataBitsWithoutHammingBits(data_rows_received[i]);
    }

    // printing data bits without hamming bits
    cout << "data block after removing check bits:" << endl;
    for(int i = 0; i < total_rows; i++){
        cout << data_rows_received_without_hamming_bits[i] << endl;
    }
    cout << endl;

    // generate output frame
    string output_frame = "";
    for(int i = 0; i < total_rows; i++){
        for(int j = 0; j < m; j++){
            string binary_string = data_rows_received_without_hamming_bits[i].substr(j * 8, 8);
            bitset<8> binary_number(binary_string);
            int decimal_number = static_cast<int>(binary_number.to_ulong());
            char ascii_character = static_cast<char>(decimal_number);
            output_frame += ascii_character;
        }
    }

    // print output frame
    cout << "output frame: " << output_frame << endl;

    return 0;
}
