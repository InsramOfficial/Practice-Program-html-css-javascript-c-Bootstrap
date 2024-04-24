#include <iostream>
#include <string>
#include <math.h>
#include <cctype>
#include <windows.h>

using namespace std;

int bin_to_dec(string bin)
{
    int dec = 0, incr = 0, sum = 0;
    for (int i = (bin.length() - 1); i >= 0; i--)
    {
        dec = bin[i] - '0';
        sum = sum + (int)(dec * (pow(2, incr)));
        incr++;
    }
    return sum;
}

string dec_to_bin(int dec)
{
    int i = dec;
    string str = "";
    char binary[4];
    while (1)
    {
        int quotient = i / 2;
        int remainder = i % 2;
        i = quotient;
        itoa(remainder, binary, 10);
        str = str + binary;
        if (quotient == 0)
            break;
    }
    string fin_str = "";
    for (int i = (str.length() - 1); i >= 0; i--)
    {
        fin_str = fin_str + str[i];
    }
    return fin_str;
}

string process_string(string substring, int s[][4][16], int p)
{
    string row, col;
    for (int i = 0; i < 6; i++)
    {
        if (i == 0 || i == 5)
            row = row + substring[i];
        else
        {
            col = col + substring[i];
        }
    }
    int r = bin_to_dec(row);
    int c = bin_to_dec(col);
    int s_box_pos = s[p][r][c];
    string fbit_bin_str = "0000";
    int len = dec_to_bin(s_box_pos).length();
    string bin = dec_to_bin(s_box_pos);

    for (int i = 0; i < len; i++)
    {
        fbit_bin_str[i + (4 - len)] = bin[i];
    }
    return fbit_bin_str;
}

string fifty_six_bit_key(string key, int pc1[8][7])
{
    int i, j, k, bin_key_index = 0, index = 0;
    string fs_bin_key;

    for (k = 0; k < 56; k++)
        fs_bin_key = fs_bin_key + '0';
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 7; j++)
        {
            index = pc1[i][j] - 1;
            fs_bin_key[bin_key_index] = key[index];
            bin_key_index++;
        }
    }
    return fs_bin_key;
}

string rol_keys(string key, int rotating_schedule[16], int i)
{
    int count = rotating_schedule[i], l, j, k;
    string rol_key;
    for (k = 0; k < 28; k++)
        rol_key = rol_key + '0';
    while (count > 0)
    {
        for (l = 0; l < 28; l++)
        {
            if (l == 0)
                j = 27;
            else
            {
                j = l - 1;
            }
            rol_key[j] = key[l];
        }
        key = rol_key;
        count--;
    }
    return rol_key;
}

string fourty_eight_bit_key(string C, string D, int pc2[6][8])
{
    int fe_bin_key_index = 0, concat_key_index = 0, i, j, k;
    string fe_bin_key, concat_keys = C + D;
    for (i = 0; i < 48; i++)
        fe_bin_key = fe_bin_key + '0';
    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 8; j++)
        {
            concat_key_index = pc2[i][j] - 1;
            fe_bin_key[fe_bin_key_index] = concat_keys[concat_key_index];
            fe_bin_key_index++;
        }
    }
    return fe_bin_key;
}

string initial_permuted_msg(string msg, int ip[8][8])
{
    int ip_msg_index = 0, msg_index = 0, i, j;
    string ip_msg;
    for (i = 0; i < 64; i++)
        ip_msg = ip_msg + '0';
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            msg_index = ip[i][j] - 1;
            ip_msg[ip_msg_index] = msg[msg_index];
            ip_msg_index++;
        }
    }
    return ip_msg;
}

string expanded_msg(string R, int ep[8][6])
{
    int ep_msg_index = 0, R_index = 0, i, j;
    string ep_msg;
    for (i = 0; i < 48; i++)
        ep_msg = ep_msg + '0';
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 6; j++)
        {
            R_index = ep[i][j] - 1;
            ep_msg[ep_msg_index] = R[R_index];
            ep_msg_index++;
        }
    }
    return ep_msg;
}

string xored_msg(string ep_msg, string fe_key)
{
    int xor_val, i, len = ep_msg.length();
    char holder[len];
    string xored_msg;

    for (i = 0; i < len; i++)
    {
        xor_val = ((fe_key[i] - '0') ^ (ep_msg[i] - '0'));
        itoa(xor_val, holder, 10);
        xored_msg = xored_msg + holder;
    }
    return xored_msg;
}

string substitution_function(string xored_message, int sbox[][4][16])
{
    string thirty_two_bit_msg;
    int index = 0, len = 6;
    for (int i = 0; i < 48; i += 6)
    {
        thirty_two_bit_msg = thirty_two_bit_msg + process_string(xored_message.substr(i, len), sbox, index);
        index++;
    }
    return thirty_two_bit_msg;
}

string permuted_message(string thirty_two_bit_msg, int pf[4][8])
{
    int index = 0, thirty_two_bit_msg_index = 0, i, j;
    string permuted_msg;
    for (i = 0; i < 32; i++)
        permuted_msg = permuted_msg + '0';
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 8; j++)
        {
            thirty_two_bit_msg_index = pf[i][j] - 1;
            permuted_msg[index] = thirty_two_bit_msg[thirty_two_bit_msg_index];
            index++;
        }
    }
    return permuted_msg;
}

string inverse_ip_msg(string reversed_msg, int inv_ip[8][8])
{
    int inv_msg_index = 0, index = 0, i, j;
    string inv_msg = "";
    for (i = 0; i < 64; i++)
        inv_msg = inv_msg + '0';
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            index = inv_ip[i][j] - 1;
            inv_msg[inv_msg_index] = reversed_msg[index];
            inv_msg_index++;
        }
    }
    return inv_msg;
}

string hex_conversion(string de_msg, string conv_table[23], string hex[23])
{
    int len = 4;
    string substring = "", hex_message = "";
    for (int i = 0; i < 64; i += 4)
    {
        int find = 0;
        substring = de_msg.substr(i, len);
        while (1)
        {
            if (conv_table[find] == substring)
                break;
            find++;
        }
        hex_message = hex_message + hex[find];
    }
    return hex_message;
}

string text_to_hex(string text)
{
    string hex_message = "";
    for (int i = 0; i < text.length(); i++)
    {
        int ascii_val = int(text[i]);
        string hex_val = dec_to_bin(ascii_val);
        hex_message += hex_val;
    }
    return hex_message;
}

string des_process(string message, string key, bool encrypt)
{
    string conv_table[23] = {
        "0000",
        "0001",
        "0010",
        "0011",
        "0100",
        "0101",
        "0110",
        "0111",
        "1000",
        "1001",
        "pad",
        "pad",
        "pad",
        "pad",
        "pad",
        "pad",
        "pad",
        "1010",
        "1011",
        "1100",
        "1101",
        "1110",
        "1111"};

    string hex[23] = {
        "0",
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "pad",
        "pad",
        "pad",
        "pad",
        "pad",
        "pad",
        "pad",
        "A",
        "B",
        "C",
        "D",
        "E",
        "F"};

    int ip[8][8] = {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47,39, 31, 23, 15, 7};

    int inv_ip[8][8] = {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25};

    // Perform text to hex conversion
    string hex_message = text_to_hex(message);

    // Add padding if needed
    int padding_length = 0;
    if (hex_message.length() % 64 != 0)
    {
        padding_length = 64 - (hex_message.length() % 64);
        for (int i = 0; i < padding_length; i++)
            hex_message += "0";
    }

    // Perform DES operations on each 64-bit block
    string encrypted_message = "";
    for (int i = 0; i < hex_message.length(); i += 64)
    {
        string block = hex_message.substr(i, 64);

        // Perform DES encryption or decryption
        if (encrypt)
        {
            // Perform encryption operations
            // ...
        }
        else
        {
            // Perform decryption operations
            // ...
        }

        // Append the encrypted or decrypted block to the result
        encrypted_message += block;
    }

    // Convert the result back to text
    string decrypted_text = hex_conversion(encrypted_message, conv_table, hex);

    // Remove padding if added
    if (padding_length > 0)
        decrypted_text = decrypted_text.substr(0, decrypted_text.length() - padding_length);

    return decrypted_text;
}
string bin_to_hex(string binary)
{
    string hex_message = "";
    for (int i = 0; i < binary.length(); i += 4)
    {
        string substring = binary.substr(i, 4);
        int decimal_val = bin_to_dec(substring);
        if (decimal_val < 10)
            hex_message += to_string(decimal_val);
        else
            hex_message += char(decimal_val - 10 + 'A');
    }
    return hex_message;
}
int main()
{
    string message, key;
    cout << "Enter the message: ";
    getline(cin, message);
    cout << "Enter the key: ";
    getline(cin, key);

    // Perform DES encryption
    string encrypted_text = des_process(message, key, true);
    cout << "Encrypted message: " << encrypted_text << endl;

    // Perform DES decryption
    string decrypted_text = des_process(encrypted_text, key, false);
    cout << "Decrypted message: " << decrypted_text << endl;

    return 0;
}

