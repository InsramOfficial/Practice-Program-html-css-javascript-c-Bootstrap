#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

string TextToHex(const string& text) {
    ostringstream oss;
    oss << hex << setfill('0');
    for (size_t i = 0; i < text.length(); ++i) {
        oss << setw(2) << static_cast<int>(static_cast<unsigned char>(text[i]));
    }
    return oss.str();
}

string PadTo16Digits(const string& hex) {
    if (hex.length() >= 16) {
        return hex;
    }
    
    string paddedHex = hex;
    const string alphabetHex = "ABCDEF";
    while (paddedHex.length() < 16) {
        paddedHex += alphabetHex[paddedHex.length() % alphabetHex.length()];
    }
    
    return paddedHex;
}

int main() {
    string text;
    cout << "Enter a text string: ";
    getline(cin, text);

    string hex = TextToHex(text);
    cout << "Hexadecimal: " << hex << endl;

    string paddedHex = PadTo16Digits(hex);
    cout << "Padded Hexadecimal: " << paddedHex << endl;

    return 0;
}

