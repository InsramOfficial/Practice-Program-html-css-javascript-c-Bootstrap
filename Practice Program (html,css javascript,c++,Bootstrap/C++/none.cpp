#include <iostream>
#include <string>

using namespace std;

string shift_left_once(string key) {
  string shifted_key = "";
  for (int i = 1; i < key.length(); i++) {
    shifted_key += key[i];
  }
  shifted_key += key[0];
  return shifted_key;
}

string shift_left_twice(string key) {
  return shift_left_once(shift_left_once(key));
}

const string permutation_p = "1 5 9 13 2 6 10 14 3 7 11 15 4 8 12 16";

string permute(string message, const string& permutation) {
  string permuted_message = "";
  for (int i = 0; i < permutation.length(); i++) {
    permuted_message += message[permutation[i] - 1];
  }
  return permuted_message;
}

string substitute(string message) {
  // TODO: Implement the DES substitution function.
}

const string permutation_pi = "2 14 11 13 8 1 15 5 6 12 9 0 3 7 10 4";

string generate_round_key(string key, int round) {
  string shifted_key = shift_left_once(key);
  if (round % 2 == 0) {
    shifted_key = shift_left_twice(shifted_key);
  }
  return shifted_key;
}

string encrypt_round(string plain_text, string round_key) {
  string permuted_left = permute(plain_text, permutation_p);
  string xored_message = xor(permuted_left, round_key);
  string substituted_message = substitute(xored_message);
  string permuted_right = permute(substituted_message, permutation_pi);
  return permuted_right;
}

string encrypt(string plain_text, string key) {
  string cipher_text = "";
  for (int i = 0; i < 16; i++) {
    cipher_text += encrypt_round(plain_text, generate_round_key(key, i));
  }
  return cipher_text;
}

int main() {
  string plain_text, key;
  cout << "Enter the plain text: ";
  cin >> plain_text;
  cout << "Enter the key: ";
  cin >> key;

  string cipher_text = encrypt(plain_text, key);
  cout << "Cipher text: " << cipher_text << endl;

  return 0;
}
