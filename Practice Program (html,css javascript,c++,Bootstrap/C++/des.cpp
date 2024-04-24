#include <iostream>
#include <string>
#include <cmath>
using namespace std;

// Function prototypes
string desEncrypt(string message, string key);
string desDecrypt(string cipher, string key);
void generate_keys(string key);


// Array to hold 16 keys
string round_keys[16];

// String to hold the plain text
string pt;

// Function to convert a number in decimal to binary
string convertDecimalToBinary(int decimal)
{
    string binary;
    while (decimal != 0)
    {
        binary = (decimal % 2 == 0 ? "0" : "1") + binary;
        decimal = decimal / 2;
    }
    while (binary.length() < 4)
    {
        binary = "0" + binary;
    }
    return binary;
}

// Function to convert a number in binary to decimal
int convertBinaryToDecimal(string binary)
{
    int decimal = 0;
    int counter = 0;
    int size = binary.length();
    for (int i = size - 1; i >= 0; i--)
    {
        if (binary[i] == '1')
        {
            decimal += pow(2, counter);
        }
        counter++;
    }
    return decimal;
}

// Function to do a circular left shift by 1
string shift_left_once(string key_chunk)
{
    string shifted = "";
    for (int i = 1; i < 28; i++)
    {
        shifted += key_chunk[i];
    }
    shifted += key_chunk[0];
    return shifted;
}

// Function to do a circular left shift by 2
string shift_left_twice(string key_chunk)
{
    string shifted = "";
    for (int i = 0; i < 2; i++)
    {
        for (int j = 1; j < 28; j++)
        {
            shifted += key_chunk[j];
        }
        shifted += key_chunk[0];
        key_chunk = shifted;
        shifted = "";
    }
    return key_chunk;
}

// Function to compute xor between two strings
string Xor(string a, string b)
{
    string result = "";
    int size = b.size();
    for (int i = 0; i < size; i++)
    {
        if (a[i] != b[i])
        {
            result += "1";
        }
        else
        {
            result += "0";
        }
    }
    return result;
}

// Function to generate the 16 keys.
void generate_keys(string key)
{
    // The PC1 table
    int pc1[56] = {
        57, 49, 41, 33, 25, 17, 9,
        1, 58, 50, 42, 34, 26, 18,
        10, 2, 59, 51, 43, 35, 27,
        19, 11, 3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,
        7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29,
        21, 13, 5, 28, 20, 12, 4
    };
    // The PC2 table
    int pc2[48] = {
        14, 17, 11, 24, 1, 5,
        3, 28, 15, 6, 21, 10,
        23, 19, 12, 4, 26, 8,
        16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53,
        46, 42, 50, 36, 29, 32
    };

    // 1. Compressing the key using the PC1 table
    string permuted_choice_1 = "";
    for (int i = 0; i < 56; i++)
    {
        permuted_choice_1 += key[pc1[i] - 1];
    }

    // Dividing the key into two equal halves
    string left = permuted_choice_1.substr(0, 28);
    string right = permuted_choice_1.substr(28, 28);

    for (int j = 0; j < 16; j++)
    {
        // 2.1. Performing circular left shift
        left = shift_left_once(left);
        right = shift_left_once(right);

        // Combining the two halves
        string combined_key = left + right;

        // 2.2. Compressing the key using the PC2 table
        string round_key = "";
        for (int i = 0; i < 48; i++)
        {
            round_key += combined_key[pc2[i] - 1];
        }

        // Saving each round key
        round_keys[j] = round_key;
    }
}

// Function to implement the XOR operation
string xorOperation(string a, string b)
{
    string result = "";
    int size = a.size();
    for (int i = 0; i < size; i++)
    {
        if (a[i] == b[i])
        {
            result += "0";
        }
        else
        {
            result += "1";
        }
    }
    return result;
}

// Function to implement the S-box substitution
string sBox(string input)
{
    // S-box table
    int s[8][4][16] = {
        {
            {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
            {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
            {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
            {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
        },
        {
            {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
            {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
            {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
            {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
        },
        {
            {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
            {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
            {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
            {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
        },
        {
            {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
            {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
            {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
            {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
        },
        {
            {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
            {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
            {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
            {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
        },
        {
            {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
            {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
            {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
            {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
        },
        {
            {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
            {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
            {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
            {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
        },
        {
            {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
            {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
            {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
            {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
        }
    };

    string output = "";
    for (int i = 0; i < 8; i++)
    {
        string row_str = input.substr(i * 6, 1) + input.substr(i * 6 + 5, 1);
        string col_str = input.substr(i * 6 + 1, 4);

        int row = convertBinaryToDecimal(row_str);
        int col = convertBinaryToDecimal(col_str);

        int val = s[i][row][col];
        output += convertDecimalToBinary(val);
    }
    return output;
}

// Function to perform the F function
string F(string right, string round_key)
{
    // 1. Expanding the right half from 32 to 48 bits
    int expansion_table[48] = {
        32, 1, 2, 3, 4, 5, 4, 5,
        6, 7, 8, 9, 8, 9, 10, 11,
        12, 13, 12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21, 20, 21,
        22, 23, 24, 25, 24, 25, 26, 27,
        28, 29, 28, 29, 30, 31, 32, 1
    };

    string expanded_right = "";
    for (int i = 0; i < 48; i++)
    {
        expanded_right += right[expansion_table[i] - 1];
    }

    // 2. Performing XOR operation on expanded right and round key
    string xored_right = xorOperation(expanded_right, round_key);

    // 3. Performing S-box substitution
    string substituted_right = sBox(xored_right);

    // 4. Applying a permutation to the substituted right
    int permutation_table[32] = {
        16, 7, 20, 21, 29, 12, 28, 17,
        1, 15, 23, 26, 5, 18, 31, 10,
        2, 8, 24, 14, 32, 27, 3, 9,
        19, 13, 30, 6, 22, 11, 4, 25
    };

    string permuted_right = "";
    for (int i = 0; i < 32; i++)
    {
        permuted_right += substituted_right[permutation_table[i] - 1];
    }

    return permuted_right;
}

int main()
{
    // Example usage of the DES algorithm

    string plaintext = "Hello World!";
    string key = "secretkey";

    // Encryption
    string ciphertext = desEncrypt(plaintext, key);
    cout << "Ciphertext: " << ciphertext << endl;

    // Decryption
    string decrypted_text = desDecrypt(ciphertext, key);
    cout << "Decrypted text: " << decrypted_text << endl;

    return 0;
}

