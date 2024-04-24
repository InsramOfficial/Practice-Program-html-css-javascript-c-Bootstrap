#include <iostream>
#include <string>
#include <openssl/des.h>

std::string desEncryptToHex(const std::string& plaintext, const std::string& key) {
    DES_cblock desKey;
    DES_key_schedule keySchedule;

    if (key.size() != 8) {
        std::cerr << "Error: Invalid key size. Key should be 8 characters long." << std::endl;
        return "";
    }

    // Initialize the key
    std::copy(key.begin(), key.end(), desKey);
    DES_set_odd_parity(&desKey);
    DES_set_key_checked(&desKey, &keySchedule);

    DES_cblock inputBlock, outputBlock;
    std::string ciphertext;

    // Encrypt each 8-byte block of the plain text
    for (size_t i = 0; i < plaintext.size(); i += 8) {
        std::copy(plaintext.begin() + i, plaintext.begin() + i + 8, inputBlock);

        DES_ecb_encrypt(&inputBlock, &outputBlock, &keySchedule, DES_ENCRYPT);

        ciphertext.append(reinterpret_cast<const char*>(outputBlock));
    }

    // Convert ciphertext to hexadecimal
    std::string hexRepresentation;
    for (char c : ciphertext) {
        hexRepresentation += "0123456789ABCDEF"[static_cast<unsigned char>(c) >> 4];
        hexRepresentation += "0123456789ABCDEF"[static_cast<unsigned char>(c) & 0xf];
    }

    return hexRepresentation;
}

int main() {
    std::string plaintext = "Hello, World!";  // Replace with your own plain text
    std::string key = "myKey123";  // Replace with your own key

    // Encrypt plain text using DES and obtain hexadecimal representation of ciphertext
    std::string hexCiphertext = desEncryptToHex(plaintext, key);

    std::cout << "Hex Ciphertext: " << hexCiphertext << std::endl;

    return 0;
}

