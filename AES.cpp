#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <locale>
#include <stdexcept> 
#include <random> 
using namespace std;
string padString(string input) {
    char paddingChar = ' ';
    int targetBytes = 16;
    int paddingNeeded = targetBytes - (input.length() % targetBytes);
    if (paddingNeeded != targetBytes) {
        input.append(paddingNeeded, paddingChar);
    }
    return input;
}
vector<string> circularLeftShift(vector<string> word) {
    string temp = word[0];
    for (size_t i = 0; i < word.size() - 1; ++i) {
        word[i] = word[i + 1];
    }
    word[word.size() - 1] = temp;
    return word;
}
vector<string> byteSubstitution(vector<string> word, const vector<unsigned char>& Sbox) {
    vector<string> result(word.size());
    for (size_t i = 0; i < word.size(); ++i) {
        int row = stoi(word[i].substr(0, 1), nullptr, 16);
        int col = stoi(word[i].substr(1, 1), nullptr, 16);
        unsigned char sboxValue = Sbox[row * 16 + col];
        stringstream ss;
        ss << hex << setw(2) << setfill('0') << (int)sboxValue;
        result[i] = ss.str();
    }
    return result;
}
vector<string> addRoundConstant(vector<string> word, int round) {
    vector<unsigned char> Rcon = {
        0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
    };
    stringstream ss;
    ss << hex << setw(2) << setfill('0') << (int)Rcon[round - 1];
    string rcStr = ss.str();
    int rc = stoi(rcStr, nullptr, 16);
    int w0 = stoi(word[0], nullptr, 16);
    int result = rc ^ w0;
    stringstream ssResult;
    ssResult << hex << setw(2) << setfill('0') << result;
    word[0] = ssResult.str();
    return word;
}
vector<string> xorWords(vector<string> word1, vector<string> word2) {
    vector<string> result(word1.size());
    for (size_t i = 0; i < word1.size(); ++i) {
        int w1 = stoi(word1[i], nullptr, 16);
        int w2 = stoi(word2[i], nullptr, 16);
        int xorResult = w1 ^ w2;
        stringstream ss;
        ss << hex << setw(2) << setfill('0') << xorResult;
        result[i] = ss.str();
    }
    return result;
}
string xorHexStrings(const string& hex1, const string& hex2) {
    try {
        int num1 = stoi(hex1, nullptr, 16);
        int num2 = stoi(hex2, nullptr, 16);
        int xorResult = num1 ^ num2;
        stringstream ss;
        ss << hex << setw(2) << setfill('0') << xorResult;
        return ss.str();
    }
    catch (const invalid_argument& e) {
        cerr << "Error: Invalid hexadecimal string in xorHexStrings: " << e.what() << endl;
        return ""; 
    }
}
string byteSubstitution(const string& byte, const vector<unsigned char>& Sbox) {
    int row = stoi(byte.substr(0, 1), nullptr, 16);
    int col = stoi(byte.substr(1, 1), nullptr, 16);
    unsigned char sboxValue = Sbox[row * 16 + col];
    stringstream ss;
    ss << hex << setw(2) << setfill('0') << (int)sboxValue;
    return ss.str();
}
void shiftRowLeft(vector<string>& row, int n) {
    rotate(row.begin(), row.begin() + n, row.end());
}
unsigned char GFMultiply(unsigned char a, unsigned char b) {
    unsigned char result = 0;
    for (int i = 0; i < 8; ++i) {
        if (b & 1) {
            result ^= a;
        }
        bool highBitSet = (a & 0x80);
        a <<= 1;
        if (highBitSet) {
            a ^= 0x1b;
        }
        b >>= 1;
    }
    return result;
}
void mixColumns(vector<vector<string>>& state) {
    vector<vector<unsigned char>> matrix(4, vector<unsigned char>(4));
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            matrix[row][col] = (unsigned char)stoi(state[row][col], nullptr, 16);
        }
    }
    for (int col = 0; col < 4; ++col) {
        unsigned char a = matrix[0][col];
        unsigned char b = matrix[1][col];
        unsigned char c = matrix[2][col];
        unsigned char d = matrix[3][col];
        matrix[0][col] = GFMultiply(0x02, a) ^ GFMultiply(0x03, b) ^ c ^ d;
        matrix[1][col] = a ^ GFMultiply(0x02, b) ^ GFMultiply(0x03, c) ^ d;
        matrix[2][col] = a ^ b ^ GFMultiply(0x02, c) ^ GFMultiply(0x03, d);
        matrix[3][col] = GFMultiply(0x03, a) ^ b ^ c ^ GFMultiply(0x02, d);
    }
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            stringstream ss;
            ss << hex << setw(2) << setfill('0') << (int)matrix[row][col];
            state[row][col] = ss.str();
        }
    }
}
void printMatrix(const vector<vector<string>>& matrix, const string& label) {
    cout << label << ":" << endl;
    for (int row = 0; row < matrix.size(); ++row) {
        for (int col = 0; col < matrix[0].size(); ++col) {
            cout << matrix[row][col] << " ";
        }
        cout << endl;
    }
}

string invByteSubstitution(const string& byte, const vector<unsigned char>& InvSbox) {
    int row = stoi(byte.substr(0, 1), nullptr, 16);
    int col = stoi(byte.substr(1, 1), nullptr, 16);
    unsigned char invSboxValue = InvSbox[row * 16 + col];
    stringstream ss;
    ss << hex << setw(2) << setfill('0') << (int)invSboxValue;
    return ss.str();
}
void invShiftRowLeft(vector<string>& row, int n) {
    rotate(row.rbegin(), row.rbegin() + n, row.rend());
}
void invMixColumns(vector<vector<string>>& state) {
    vector<vector<unsigned char>> matrix(4, vector<unsigned char>(4));
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            matrix[row][col] = (unsigned char)stoi(state[row][col], nullptr, 16);
        }
    }

    for (int col = 0; col < 4; ++col) {
        unsigned char a = matrix[0][col];
        unsigned char b = matrix[1][col];
        unsigned char c = matrix[2][col];
        unsigned char d = matrix[3][col];

        matrix[0][col] = GFMultiply(0x0e, a) ^ GFMultiply(0x0b, b) ^ GFMultiply(0x0d, c) ^ GFMultiply(0x09, d);
        matrix[1][col] = GFMultiply(0x09, a) ^ GFMultiply(0x0e, b) ^ GFMultiply(0x0b, c) ^ GFMultiply(0x0d, d);
        matrix[2][col] = GFMultiply(0x0d, a) ^ GFMultiply(0x09, b) ^ GFMultiply(0x0e, c) ^ GFMultiply(0x0b, d);
        matrix[3][col] = GFMultiply(0x0b, a) ^ GFMultiply(0x0d, b) ^ GFMultiply(0x09, c) ^ GFMultiply(0x0e, d);
    }

    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            stringstream ss;
            ss << hex << setw(2) << setfill('0') << (int)matrix[row][col];
            state[row][col] = ss.str();
        }
    }
}
int main() {
    setlocale(LC_ALL, "rus");
    vector<unsigned char> Sbox = {
      0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
      0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
      0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
      0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
      0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
      0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
      0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
      0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
      0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
      0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
      0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
      0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
      0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
      0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
      0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
      0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
    };
    vector<unsigned char> InvSbox = {
        0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
        0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
        0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
        0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
        0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
        0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
        0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
        0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
        0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
        0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
        0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
        0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
        0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
        0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
        0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
        0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
    };
    string myKey;
    const int numRows = 4;
    cout << "Введите ключ: ";
    getline(cin, myKey);
    myKey = padString(myKey);
    cout << "Ключ после дополнения (" << myKey.length() << " ASCII characters, 1 byte each):" << endl;
    cout << myKey << endl;
    string zeroKey = "";
    for (size_t i = 0; i < myKey.length(); ++i) {
        stringstream ss;
        ss << hex << setw(2) << setfill('0') << (int)(unsigned char)myKey[i];
        zeroKey += ss.str();
    }
    int numCols = zeroKey.length() / 2 / numRows;
    vector<vector<string>> matrix(numRows, vector<string>(numCols));
    for (int col = 0; col < numCols; ++col) {
        for (int row = 0; row < numRows; ++row) {
            int index = row + col * numRows;
            matrix[row][col] = zeroKey.substr(index * 2, 2);
        }
    }
    printMatrix(matrix, "Key Matrix");
    vector<vector<string>> keySchedule(numRows, vector<string>(numCols * 11));
    for (int col = 0; col < numCols; ++col) {
        for (int row = 0; row < numRows; ++row) {
            keySchedule[row][col] = matrix[row][col];
        }
    }
    for (int round = 1; round <= 10; ++round) {
        for (int col = numCols * round; col < numCols * (round + 1); ++col) {
            vector<string> w_i_minus_1;
            vector<string> w_i_minus_Nk;
            for (int row = 0; row < numRows; ++row) {
                w_i_minus_1.push_back(keySchedule[row][col - 1]);
                w_i_minus_Nk.push_back(keySchedule[row][col - numCols]);
            }

            vector<string> g_w_i_minus_1;
            if (col % numCols == 0) {
                g_w_i_minus_1 = circularLeftShift(w_i_minus_1);
                g_w_i_minus_1 = byteSubstitution(g_w_i_minus_1, Sbox);
                g_w_i_minus_1 = addRoundConstant(g_w_i_minus_1, round);
            }
            else {
                g_w_i_minus_1 = w_i_minus_1;
            }

            vector<string> w_i = xorWords(w_i_minus_Nk, g_w_i_minus_1);
            for (int row = 0; row < numRows; ++row) {
                keySchedule[row][col] = w_i[row];
            }
        }
    }
    string myMessage;
    cout << "Введите сообщение: ";
    getline(cin, myMessage);
    int messageLengthBytes = myMessage.length();
    int paddingNeeded = 16 - (messageLengthBytes % 16);
    if (paddingNeeded != 16) {
        myMessage.append(paddingNeeded, ' ');
    }
    cout << "Сообщение после дополнения (" << myMessage.length() << " ASCII characters, 1 byte each):" << endl;
    cout << myMessage << endl;
    string hexMessage = "";
    for (size_t i = 0; i < myMessage.length(); ++i) {
        stringstream ss;
        ss << hex << setw(2) << setfill('0') << (int)(unsigned char)myMessage[i];
        hexMessage += ss.str();
    }
    int numC = 4; 
    vector<vector<string>> matrixMes(numRows, vector<string>(numC));
    int blockCount = hexMessage.length() / 32;
    if (numCols != numC) {
        cerr << "Ошибка: Размерность матрицы секретного ключа (" << numCols << ") и матрицы сообщения (" << numC << ") не совпадают." << endl;
        return 1;
    }
    vector<vector<string>> IV(numRows, vector<string>(numCols));
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 255); 
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            int randomByte = distrib(gen);
            stringstream ss;
            ss << hex << setw(2) << setfill('0') << randomByte;
            IV[row][col] = ss.str();
        }
    }
    printMatrix(IV, "Initial IV");
    string finalCipherTextHex = "";
    vector<vector<string>> previousCiphertextBlock = IV;
    for (int blockIndex = 0; blockIndex < blockCount; ++blockIndex) {
        cout << endl << "--- Block " << blockIndex + 1 << " ---" << endl;
        for (int col = 0; col < numC; ++col) {
            for (int row = 0; row < numRows; ++row) {
                int index = row + col * numRows + blockIndex * 16;
                string byteStr = hexMessage.substr(index * 2, 2);
                matrixMes[row][col] = byteStr;
            }
        }
        printMatrix(matrixMes, "Message Matrix");
        vector<vector<string>> encryptedPreviousCiphertext(numRows, vector<string>(numCols));
        vector<vector<string>> state = previousCiphertextBlock; 
        printMatrix(state, "Previous Ciphertext Block before encryption");
        for (int round = 1; round <= 10; ++round) {
            cout << endl << "--- Round " << round << " ---" << endl;
            for (int row = 0; row < numRows; ++row) {
                for (int c = 0; c < numCols; ++c) {
                    state[row][c] = byteSubstitution(state[row][c], Sbox);
                }
            }
            printMatrix(state, "After Byte Substitution");
            for (int row = 0; row < numRows; ++row) {
                shiftRowLeft(state[row], row);
            }
            printMatrix(state, "After Shift Rows");
            if (round < 10) {
                mixColumns(state);
                printMatrix(state, "After Mix Columns");
            }
            vector<vector<string>> roundKey(numRows, vector<string>(numCols));
            for (int row = 0; row < numRows; ++row) {
                for (int c = 0; c < numCols; ++c) {
                    roundKey[row][c] = keySchedule[row][c + numCols * round];
                }
            }
            printMatrix(roundKey, "Round Key");
            for (int row = 0; row < numRows; ++row) {
                for (int c = 0; c < numCols; ++c) {
                    state[row][c] = xorHexStrings(state[row][c], roundKey[row][c]);
                }
            }
            printMatrix(state, "After Add Round Key");
        }
        encryptedPreviousCiphertext = state;
        printMatrix(encryptedPreviousCiphertext, "Encrypted Previous Ciphertext");
        vector<vector<string>> ciphertextBlock(numRows, vector<string>(numC));
        for (int row = 0; row < numRows; ++row) {
            for (int col = 0; col < numC; ++col) {
                ciphertextBlock[row][col] = xorHexStrings(matrixMes[row][col], encryptedPreviousCiphertext[row][col]);
            }
        }
        printMatrix(ciphertextBlock, "Ciphertext Block after XOR");
        string cipherTextBlockHex = "";
        for (int col = 0; col < numC; ++col) { 
            for (int row = 0; row < numRows; ++row) {
                cipherTextBlockHex += ciphertextBlock[row][col];
                cipherTextBlockHex += " ";
            }
        }
        finalCipherTextHex += cipherTextBlockHex;
        previousCiphertextBlock = ciphertextBlock;
    }
    cout << "Final Ciphertext (Hex):" << endl;
    cout << finalCipherTextHex << endl;
    cout << endl << "--- DECRYPTION ---" << endl;
    vector<vector<string>> encryptedMessage(numRows, vector<string>(numC));
    int encryptedBlockCount = finalCipherTextHex.length() / 32;
    vector<vector<string>> decryptedMessage(numRows, vector<string>(numC));
    vector<vector<string>> previousCiphertextBlockDecrypt = IV;
    string finalDecryptedHex = "";

    for (int blockIndex = 0; blockIndex < encryptedBlockCount; ++blockIndex) {
        cout << endl << "--- Decryption Block " << blockIndex + 1 << " ---" << endl;

        // Extract block from ciphertext
        for (int col = 0; col < numC; ++col) {
            for (int row = 0; row < numRows; ++row) {
                int index = row + col * numRows + blockIndex * 16; // Byte index
                encryptedMessage[row][col] = finalCipherTextHex.substr(index * 2, 2);
            }
        }

        vector<vector<string>> decryptedPreviousCiphertext(numRows, vector<string>(numCols));
        vector<vector<string>> state = previousCiphertextBlockDecrypt; // This initialization is now correct.
        printMatrix(state, "Previous Ciphertext Block for Decryption");

        for (int round = 10; round >= 1; --round) {
            cout << endl << "--- Round " << round << " ---" << endl;

            vector<vector<string>> roundKey(numRows, vector<string>(numCols));
            for (int row = 0; row < numRows; ++row) {
                for (int c = 0; c < numCols; ++c) {
                    roundKey[row][c] = keySchedule[row][c + numCols * round];
                }
            }
            printMatrix(roundKey, "Round Key for decryption");

            for (int row = 0; row < numRows; ++row) {
                for (int c = 0; c < numCols; ++c) {
                    state[row][c] = xorHexStrings(state[row][c], roundKey[row][c]);
                }
            }
            printMatrix(state, "After Add Round Key for decryption");

            if (round < 10) {
                invMixColumns(state);
                printMatrix(state, "After Inv Mix Columns for decryption");
            }

            for (int row = 0; row < numRows; ++row) {
                invShiftRowLeft(state[row], row);
            }
            printMatrix(state, "After Inv Shift Rows for decryption");

            for (int row = 0; row < numRows; ++row) {
                for (int c = 0; c < numCols; ++c) {
                    state[row][c] = invByteSubstitution(state[row][c], InvSbox);
                }
            }
            printMatrix(state, "After Inv Byte Substitution for decryption");
        }
        decryptedPreviousCiphertext = state;
        printMatrix(decryptedPreviousCiphertext, "Decrypted Previous Ciphertext for decryption");
        for (int row = 0; row < numRows; ++row) {
            for (int col = 0; col < numC; ++col) {
                decryptedMessage[row][col] = xorHexStrings(encryptedMessage[row][col], decryptedPreviousCiphertext[row][col]);
            }
        }
        printMatrix(decryptedMessage, "Decrypted Message Block after XOR");
        string decryptedTextBlockHex = "";
        for (int col = 0; col < numC; ++col) { // corrected to iterate through columns first
            for (int row = 0; row < numRows; ++row) { // then rows
                decryptedTextBlockHex += decryptedMessage[row][col];
            }
        }
        finalDecryptedHex += decryptedTextBlockHex;
        previousCiphertextBlockDecrypt = encryptedMessage;
    }
    cout << "Final Decrypted Hex (with padding): " << finalDecryptedHex << endl;
    string asciiDecryptedText = "";
    for (size_t i = 0; i < finalDecryptedHex.length(); i += 2) {
        string byteStr = finalDecryptedHex.substr(i, 2);
        char byte = (char)stoi(byteStr, nullptr, 16);
        asciiDecryptedText += byte;
    }
    cout << "Decrypted Text (with padding) : " << asciiDecryptedText << endl;
    // Remove padding
    size_t paddingLength = 0;
    for (int i = asciiDecryptedText.length() - 1; i >= 0; --i) {
        if (asciiDecryptedText[i] == ' ') {
            paddingLength++;
        }
        else {
            break;
        }
    }
    string finalHexWithoutPadding = "";
    if (finalDecryptedHex.length() > 2 * paddingLength)
        finalHexWithoutPadding = finalDecryptedHex.substr(0, finalDecryptedHex.length() - 2 * paddingLength);
    cout << "Final Decrypted Hex (without padding): " << finalHexWithoutPadding << endl;
    string finalDecryptedText = asciiDecryptedText.substr(0, asciiDecryptedText.length() - paddingLength);
    cout << "Final Decrypted Text (without padding): " << finalDecryptedText << endl;
    return 0;
}
