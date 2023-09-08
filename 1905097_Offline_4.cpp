#include<bits/stdc++.h>
#include<windows.h>
using namespace std;

string addHammingCheckBits(string data, int r)
{
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

string xorSubtraction(string dividend, string divisor)
{
    string ans = "";
//    cout << "Dividend: " << dividend << " Divisor: " << divisor << endl;

    for(int i = 0; i < divisor.size(); i++){
        if(dividend[i] == divisor[i]){
           ans += "0";
        }
        else{
           ans += "1";
        }
    }
//    cout << "Ans: " << ans << endl;

    return ans;
}

string getCRCCheckBits(string dividend, string divisor)
{
    int len = divisor.size();
    string tmp_string = dividend.substr(0, len);
    int cur_pos = len;

    while(cur_pos < dividend.size()){
        if(tmp_string[0] == '1'){
//           cout << "In 1: " << tmp_string << endl;
           tmp_string = xorSubtraction(tmp_string, divisor);
           tmp_string = tmp_string.substr(1) + dividend[cur_pos];
        }

        else{
//           cout << "In 0: " << tmp_string << endl;
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

string getErrorsInDataBits(string data_bits, double p)
{
    return data_bits;
}

bool detectErrorUsingCRC(string dividend, string divisor)
{
    int len = divisor.size();
    string tmp_string = dividend.substr(0, len);
    int cur_pos = len;

    while(cur_pos < dividend.size()){
        if(tmp_string[0] == '1'){
//           cout << "In 1: " << tmp_string << endl;
           tmp_string = xorSubtraction(tmp_string, divisor);
           tmp_string = tmp_string.substr(1) + dividend[cur_pos];
        }

        else{
//           cout << "In 0: " << tmp_string << endl;
           tmp_string = tmp_string.substr(1) + dividend[cur_pos];
        }

        cur_pos++;
    }

    if(tmp_string[0] == '1'){
       tmp_string = xorSubtraction(tmp_string, divisor);
    }

    for(int i = 0; i < tmp_string.size(); i++){
        if(tmp_string[i] != '0'){
           return true;
        }
    }

    return false;
}

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
//    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    for(int i = 0; i < total_rows; i++){
        cout << data_rows[i] << endl;
    }
    cout << endl;

    int hamming_r_bits = 0, tmp = 1, data_row_size = m * 8;

    while(tmp < (data_row_size + hamming_r_bits + 1)){
        hamming_r_bits++;
        tmp *= 2;
    }
//    cout << hamming_r_bits << endl;

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

    string column_wise_serialized_data = "";
    for(int j = 0; j < data_row_size + hamming_r_bits; j++){
        for(int i = 0; i < total_rows; i++){
            column_wise_serialized_data += data_row_with_hamming_bits[i][j];
        }
    }

    cout << "data bits after column-wise serialization:" << endl;
    cout << column_wise_serialized_data << endl << endl;

    int data_bits_without_CRC_length = (data_row_size + hamming_r_bits) * total_rows;
    int data_bits_with_CRC_length = data_bits_without_CRC_length + generator_polynomial.size() - 1;
    string data_bits_with_CRC_bits = column_wise_serialized_data.substr(0);

    for(int i = 0; i < generator_polynomial.size() - 1; i++){
        data_bits_with_CRC_bits += "0";
    }

    string CRC_r_bits = getCRCCheckBits(data_bits_with_CRC_bits, generator_polynomial);
//    cout << CRC_r_bits << endl;
    for(int i = data_bits_without_CRC_length; i < data_bits_with_CRC_length; i++){
        data_bits_with_CRC_bits[i] = CRC_r_bits[i - data_bits_without_CRC_length];
    }

    cout << "data bits after appending CRC checksum <sent frame>:" << endl;
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

    string data_bits_with_error = getErrorsInDataBits(data_bits_with_CRC_bits, p);
    data_bits_with_error[18] = '0';
    data_bits_with_error[27] = '0';
    data_bits_with_error[40] = '1';
    data_bits_with_error[52] = '1';
    data_bits_with_error[69] = '0';
    data_bits_with_error[109] = '1';
    data_bits_with_error[139] = '0';
    data_bits_with_error[155] = '1';
    data_bits_with_error[159] = '0';

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

    bool isError = detectErrorUsingCRC(data_bits_with_error, generator_polynomial);

    cout << "result of CRC checksum matching: ";
    if(isError){
       cout << "error detected" << endl << endl;
    }
    else{
       cout << "no error detected" << endl << endl;
    }

    string data_bits_received_without_CRC = data_bits_with_error.substr(0, data_bits_without_CRC_length);
    string data_rows_received[total_rows];

    for(int i = 0; i < total_rows; i++){
        data_rows_received[i] = "";
    }

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

    return 0;
}
