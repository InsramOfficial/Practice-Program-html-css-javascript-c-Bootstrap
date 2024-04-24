#include <iostream>
#include <bitset>

using namespace std;

// Initial permutation table for DES
const int initial_permutation[64] = {
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};

// Expansion table for the Feistel function
const int expansion_table[48] = {
    32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11,
    12, 13, 12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20, 21, 20, 21,
    22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
};

// Permutation table for the Feistel function
const int permutation_table[32] = {
    16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25
};

// S-boxes for the Feistel function
const int s_boxes[8][4][16] = {
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
    },
    // S2
    {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
    },
    // S3
    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
    },
    // S4
    {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
    },
    // S5
    {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
    },
    // S6
    {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
    },
    // S7
    {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
    },
    // S8
    {
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
    }
};

// Permutation table for the final permutation
const int final_permutation[64] = {
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25
};

// Initial permutation for the key
const int initial_key_permutation[56] = {
    57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2,
    59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36, 63, 55, 47, 39,
    31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37,
    29, 21, 13, 5, 28, 20, 12, 4
};

// Key permutation for each round
const int round_key_permutation[48] = {
    14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4,
    26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40,
    51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

// Left shift table for key schedule
const int key_left_shifts[16] = {
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

// Convert a string to a binary representation
string stringToBinary(string text) {
    string binary = "";
    for (char c : text) {
        bitset<8> binaryChar(c);
        binary += binaryChar.to_string();
    }
    return binary;
}

// Convert a binary string to a hexadecimal string
string binaryToHex(string binary) {
    string hex = "";
    for (int i = 0; i < binary.length(); i += 4) {
        string fourBits = binary.substr(i, 4);
        int decimal = stoi(fourBits, nullptr, 2);
        char hexChar = decimal < 10 ? '0' + decimal : 'A' + (decimal - 10);
        hex += hexChar;
    }
    return hex;
}

// Convert a hexadecimal string to a binary string
string hexToBinary(string hex) {
    string binary = "";
    for (char c : hex) {
        int decimal = isdigit(c) ? c - '0' : c - 'A' + 10;
        string fourBits = bitset<4>(decimal).to_string();
        binary += fourBits;
    }
    return binary;
}

// Apply the initial permutation on the input
string initialPermutation(string input) {
    string permuted = "";
    for (int i = 0; i < 64; i++) {
        int newIndex = initial_permutation[i] - 1;
        permuted += input[newIndex];
    }
    return permuted;
}

// Apply the final permutation on the output
string finalPermutation(string output) {
    string permuted = "";
    for (int i = 0; i < 64; i++) {
        int newIndex = final_permutation[i] - 1;
        permuted += output[newIndex];
    }
    return permuted;
}

// Apply the expansion permutation on the right half of the input
string expansionPermutation(string right) {
    string expanded = "";
    for (int i = 0; i < 48; i++) {
        int newIndex = expansion_table[i] - 1;
        expanded += right[newIndex];
    }
    return expanded;
}

// Apply the S-box substitution on the given 6-bit input
string sBoxSubstitution(string input, int sBoxIndex) {
    int row = stoi(input.substr(0, 1) + input.substr(5, 1), nullptr, 2);
    int column = stoi(input.substr(1, 4), nullptr, 2);
    int value = s_boxes[sBoxIndex][row][column];
    return bitset<4>(value).to_string();
}

// Apply the Feistel function on the given 32-bit right half and 48-bit subkey
string feistelFunction(string right, string subkey) {
    string expanded = expansionPermutation(right);
    string xored = "";
    for (int i = 0; i < 48; i++) {
        if (expanded[i] == subkey[i]) {
            xored += '0';
        } else {
            xored += '1';
        }
    }
    string substituted = "";
    for (int i = 0; i < 8; i++) {
        string sBoxInput = xored.substr(i * 6, 6);
        substituted += sBoxSubstitution(sBoxInput, i);
    }
    string permuted = "";
    for (int i = 0; i < 32; i++) {
        int newIndex = permutation_table[i] - 1;
        permuted += substituted[newIndex];
    }
    return permuted;
}

// Generate the round keys from the initial key
string generateRoundKeys(string initialKey) {
    string roundKeys = "";
    string key = "";
    for (int i = 0; i < 56; i++) {
        int newIndex = initial_key_permutation[i] - 1;
        key += initialKey[newIndex];
    }
    for (int i = 0; i < 16; i++) {
        // Perform circular left shift on the halves
        string left = key.substr(0, 28);
        string right = key.substr(28, 28);
        left = left.substr(key_left_shifts[i]) + left.substr(0, key_left_shifts[i]);
        right = right.substr(key_left_shifts[i]) + right.substr(0, key_left_shifts[i]);
        key = left + right;
        // Apply permutation to generate round key
        string roundKey = "";
        for (int j = 0; j < 48; j++) {
            int newIndex = round_key_permutation[j] - 1;
            roundKey += key[newIndex];
        }
        roundKeys += roundKey;
    }
    return roundKeys;
}

// Encrypt the given plaintext using the given round keys
string encrypt(string plaintext, string roundKeys) {
    string binaryPlaintext = stringToBinary(plaintext);
    string binaryKey = hexToBinary(roundKeys);
    string initialPermutated = initialPermutation(binaryPlaintext);
    string left = initialPermutated.substr(0, 32);
    string right = initialPermutated.substr(32, 32);
    for (int i = 0; i < 16; i++) {
        string nextLeft = right;
        string roundKey = binaryKey.substr(i * 48, 48);
        string feistelOutput = feistelFunction(right, roundKey);
        for (int j = 0; j < 32; j++) {
            if (left[j] == feistelOutput[j]) {
                right[j] = '0';
            } else {
                right[j] = '1';
            }
        }
        left = nextLeft;
    }
    string finalOutput = right + left;
    string encrypted = finalPermutation(finalOutput);
    string hex = binaryToHex(encrypted);
    return hex;
}

// Decrypt the given ciphertext using the given round keys
string decrypt(string ciphertext, string roundKeys) {
    string binaryCiphertext = hexToBinary(ciphertext);
    string binaryKey = hexToBinary(roundKeys);
    string initialPermutated = initialPermutation(binaryCiphertext);
    string left = initialPermutated.substr(0, 32);
    string right = initialPermutated.substr(32, 32);
    for (int i = 15; i >= 0; i--) {
        string nextRight = left;
        string roundKey = binaryKey.substr(i * 48, 48);
        string feistelOutput = feistelFunction(left, roundKey);
        for (int j = 0; j < 32; j++) {
            if (right[j] == feistelOutput[j]) {
                left[j] = '0';
            } else {
                left[j] = '1';
            }
        }
        right = nextRight;
    }
    string finalOutput = right + left;
    string decrypted = finalPermutation(finalOutput);
    string hex = binaryToHex(decrypted);
    return hex;
}

// Main function
int main() {
    string plaintext = "0123456789ABCDEF";
    string key = "133457799BBCDFF1";
    string roundKeys = generateRoundKeys(key);
    string ciphertext = encrypt(plaintext, roundKeys);
    string decrypted = decrypt(ciphertext, roundKeys);
    cout << "Plaintext: " << plaintext << endl;
    cout << "Ciphertext: " << ciphertext << endl;
    cout << "Decrypted: " << decrypted << endl;
    return 0;
}

