#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <locale>
#include <stdexcept> 
#include <random> 
#include <fstream>
using namespace std;
vector<string> circularLeftShift(vector<string> word) { //Сдвиг элементов влево (для ключей)
    string temp = word[0];
    for (size_t i = 0; i < word.size() - 1; ++i) {
        word[i] = word[i + 1];
    }
    word[word.size() - 1] = temp;
    return word;
}
vector<string> byteSubstitution(vector<string> word, const vector<unsigned char>& Sbox) { //Выполнение подстановки 
    vector<string> result(word.size());
    for (size_t i = 0; i < word.size(); ++i) {
        int row = stoi(word[i].substr(0, 1), nullptr, 16); //Из строки в число
        int col = stoi(word[i].substr(1, 1), nullptr, 16);
        unsigned char sboxValue = Sbox[row * 16 + col];
        stringstream ss;
        ss << hex << setw(2) << setfill('0') << static_cast<int>(sboxValue); //Число в hex
        result[i] = ss.str();
    }
    return result;
}
vector<string> addRoundConstant(vector<string> word, int round) {//Операция XOR между первым байтом слова и Rcon.
    vector<unsigned char> Rcon = {
        0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
    };
    stringstream ss;
    ss << hex << setw(2) << setfill('0') << static_cast<int>(Rcon[round - 1]);
    string rcStr = ss.str();
    int rc = stoi(rcStr, nullptr, 16);
    int w0 = stoi(word[0], nullptr, 16);
    int result = rc ^ w0;
    stringstream ssResult;
    ssResult << hex << setw(2) << setfill('0') << result;
    word[0] = ssResult.str();
    return word;
}
vector<string> xorWords(vector<string> word1, vector<string> word2) { //XOR для двух слов байт за байтом.
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
string xorHexStrings(const string& hex1, const string& hex2) {  //XOR между двумя hex строками.
    int num1 = stoi(hex1, nullptr, 16);
    int num2 = stoi(hex2, nullptr, 16); //Строку в число
    int xorResult = num1 ^ num2;
    stringstream ss;
    ss << hex << setw(2) << setfill('0') << xorResult;
    return ss.str();
}
string byteSubstitution(const string& byte, const vector<unsigned char>& Sbox) { //Подстановка байта из S-Box.
    int row = stoi(byte.substr(0, 1), nullptr, 16);
    int col = stoi(byte.substr(1, 1), nullptr, 16);
    unsigned char sboxValue = Sbox[row * 16 + col];
    stringstream ss;
    ss << hex << setw(2) << setfill('0') << static_cast<int>(sboxValue);
    return ss.str();
}
void shiftRowLeft(vector<string>& row, int n) {//Сдвиг элементов
    rotate(row.begin(), row.begin() + n, row.end());
}
unsigned char GFMultiply(unsigned char a, unsigned char b) { //Выполняет умножение в конечном поле Галуа.
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
void mixColumns(vector<vector<string>>& state) { //Махинации со строками матриц
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
void printMatrix(const vector<vector<string>>& matrix, const string& label) { //Вывод матрицы
    cout << label << ":" << endl;
    for (int row = 0; row < matrix.size(); ++row) {
        for (int col = 0; col < matrix[0].size(); ++col) {
            cout << matrix[row][col] << " ";
        }
        cout << endl;
    }
}
//Сдесь все то же самое, только для расшифровки
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
//Шифровка и расшифровка
vector<vector<string>> encryptBlockCFB(const vector<vector<string>>& previousCiphertextBlock, const vector<vector<string>>& keySchedule, int numRounds, int numRows, int numCols, const vector<unsigned char>& Sbox) {
    vector<vector<string>> keyStream = previousCiphertextBlock; // SНачинаем с предыдущего шифротекста
    for (int round = 1; round <= numRounds; ++round) {//Раунды
        for (int row = 0; row < numRows; ++row) {
            for (int col = 0; col < numCols; ++col) {
                keyStream[row][col] = byteSubstitution(keyStream[row][col], Sbox);
            }
        }
        // Сдвиг
        for (int row = 0; row < numRows; ++row) { //Сдвиг согласно условию - в первой 0, во 2 - 1 и т.д.
            shiftRowLeft(keyStream[row], row);
        }
        if (round < numRounds) {
            mixColumns(keyStream);
        }
        // Ключи
        vector<vector<string>> roundKey(numRows, vector<string>(numCols));
        for (int row = 0; row < numRows; ++row) {
            for (int col = 0; col < numCols; ++col) {
                roundKey[row][col] = keySchedule[row][col + numCols * (round - 1)]; 
                keyStream[row][col] = xorHexStrings(keyStream[row][col], roundKey[row][col]);
            }
        }
        // AddRoundKey для последнего раунда (раунд 10)
        if (round == numRounds) {
            vector<vector<string>> roundKey(numRows, vector<string>(numCols));
            for (int row = 0; row < numRows; ++row) {
                for (int col = 0; col < numCols; ++col) {
                    roundKey[row][col] = keySchedule[row][col + numCols * round];
                    keyStream[row][col] = xorHexStrings(keyStream[row][col], roundKey[row][col]);
                }
            }
        }
    }
    return keyStream; // Возвращаем итоговый вектор 
}
//Перевод в ASCII
string hexToASCII(const string& hexString) {
    string asciiString = "";
    for (size_t i = 0; i < hexString.length(); i += 2) {
        string byte = hexString.substr(i, 2);
        int decimal = stoi(byte, nullptr, 16);
        asciiString += char(decimal);
    }
    return asciiString;
}
int main() {
    setlocale(LC_ALL, "rus");
    vector<unsigned char> Sbox = {//Это вектор hex символов. Один и тот же 
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
    const int numRows = 4; //Кол-во раундов и параметры для матрицы
    const int numCols = 4;
    const int numRounds = 10;
    ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        cerr << "Unable to open input.txt" << endl;
        return 1;
    }
    string myMessage((istreambuf_iterator<char>(inputFile)),(istreambuf_iterator<char>())); //Анализ текста
    inputFile.close();
    // Доводим до кратности 16
    int messageLength = myMessage.length();
    int Needed = 16 - (messageLength % 16);
    if (Needed != 16) {
        myMessage.append(Needed, ' ');
    }
    string hexMessage = "";
    for (size_t i = 0; i < myMessage.length(); ++i) {
        stringstream ss;
        ss << hex << setw(2) << setfill('0') << (int)(unsigned char)myMessage[i];
        hexMessage += ss.str();
    }
    // Мастер ключ и его матрица (16 байт)
    string zeroKey = "";
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 255);
    for (int i = 0; i < 16; ++i) { 
        int randomByte = distrib(gen);
        stringstream ss;
        ss << hex << setw(2) << setfill('0') << randomByte;
        zeroKey += ss.str();
    }
    cout << "Сгенерированный мастер-ключ (Hex): " << zeroKey << endl;
    vector<vector<string>> matrix(numRows, vector<string>(numCols));
    for (int col = 0; col < numCols; ++col) {
        for (int row = 0; row < numRows; ++row) {
            int index = row + col * numRows;
            matrix[row][col] = zeroKey.substr(index * 2, 2);
        }
    }
    printMatrix(matrix, "Матрица масте-ключа");
    // Rounds key
    vector<vector<string>> keySchedule(numRows, vector<string>(numCols * (numRounds + 1)));
    for (int col = 0; col < numCols; ++col) {
        for (int row = 0; row < numRows; ++row) {
            keySchedule[row][col] = matrix[row][col];
        }
    }
    for (int round = 1; round <= numRounds; ++round) {
        for (int col = numCols * round; col < numCols * (round + 1); ++col) {
            vector<string> w1;
            vector<string> wNk;
            for (int row = 0; row < numRows; ++row) {
                w1.push_back(keySchedule[row][col - 1]);
                wNk.push_back(keySchedule[row][col - numCols]);
            }
            vector<string> g_w1;
            if (col % numCols == 0) {
                g_w1 = circularLeftShift(w1);
                g_w1 = byteSubstitution(g_w1, Sbox);
                g_w1 = addRoundConstant(g_w1, round);
            } else {
                g_w1 = w1;
            }
            vector<string> w_i = xorWords(wNk, g_w1);
            for (int row = 0; row < numRows; ++row) {
                keySchedule[row][col] = w_i[row];
            }
        }
    }
    // IV создание и его матрицы
    string ivString = "";
    for (int i = 0; i < 16; ++i) { 
        int randomByte = distrib(gen);
        stringstream ss;
        ss << hex << setw(2) << setfill('0') << randomByte;
        ivString += ss.str();
    }
    cout << "IV (в hex): " << ivString << endl;
    vector<vector<string>> IV(numRows, vector<string>(numCols));
    for (int col = 0; col < numCols; ++col) {
        for (int row = 0; row < numRows; ++row) {
            int index = (row + col * numRows) * 2;
            IV[row][col] = ivString.substr(index, 2);
        }
    }
    printMatrix(IV, "IV матрица");
    // Шифровка
    string finalCipherTextHex = "";
    vector<vector<string>> previousCiphertextBlock = IV;
    int blockCount = hexMessage.length() / 32;
    for (int i = 0; i< blockCount; ++i) {
        vector<vector<string>> messageBlock(numRows, vector<string>(numCols));
        for (int col = 0; col < numCols; ++col) {
            for (int row = 0; row < numRows; ++row) {
                int index = (row * 2) + (col * numRows * 2) + (i * numCols * numRows * 2);
                messageBlock[row][col] = hexMessage.substr(index, 2);
            }
        }
        //printMatrix(messageBlock, "Message Block"); //Предыдущий текст
        vector<vector<string>> keyStream = encryptBlockCFB(previousCiphertextBlock, keySchedule, numRounds, numRows, numCols, Sbox);
        //printMatrix(keyStream, "Key Stream"); //Матрица ключей
        vector<vector<string>> ciphertextBlock(numRows, vector<string>(numCols));
        for (int row = 0; row < numRows; ++row) {
            for (int col = 0; col < numCols; ++col) {
                ciphertextBlock[row][col] = xorHexStrings(messageBlock[row][col], keyStream[row][col]);
            }
        }
        //printMatrix(ciphertextBlock, "Ciphertext Block");//Рыводим матрицу зашифрованных блоков
        previousCiphertextBlock = ciphertextBlock;
        for (int col = 0; col < numCols; ++col) {
            for (int row = 0; row < numRows; ++row) {
                finalCipherTextHex += ciphertextBlock[row][col];
            }
        }
    }
    
    // Дешифровка (CFB Mode)
    string decryptedHexMessage = "";
    vector<vector<string>> previousCiphertextBlockDecrypt = IV;
    for (int i = 0; i < blockCount; ++i) {
        vector<vector<string>> ciphertextBlock(numRows, vector<string>(numCols));
        for (int col = 0; col < numCols; ++col) {
            for (int row = 0; row < numRows; ++row) {
                int index = (row * 2) + (col * numRows * 2) + (i * numCols * numRows * 2);
                ciphertextBlock[row][col] = finalCipherTextHex.substr(index, 2);
            }
        }
        vector<vector<string>> keyStreamDecrypt = encryptBlockCFB(previousCiphertextBlockDecrypt, keySchedule, numRounds, numRows, numCols, Sbox);
        vector<vector<string>> decryptedBlock(numRows, vector<string>(numCols));
        for (int row = 0; row < numRows; ++row) {
            for (int col = 0; col < numCols; ++col) {
                decryptedBlock[row][col] = xorHexStrings(ciphertextBlock[row][col], keyStreamDecrypt[row][col]);
            }
        }
        previousCiphertextBlockDecrypt = ciphertextBlock;
        for (int col = 0; col < numCols; ++col) {
            for (int row = 0; row < numRows; ++row) {
                decryptedHexMessage += decryptedBlock[row][col];
            }
        }
    }
    string decryptedAsciiMessage = hexToASCII(decryptedHexMessage);//ASCII и вывод в файл
    decryptedAsciiMessage.resize(messageLength);
    ofstream outputFile("output.txt");
    if (outputFile.is_open()) {
        outputFile << "Зашифрованное (Hex): " << finalCipherTextHex << endl;  
        outputFile << "Расшифрованное(ASCII): " << decryptedAsciiMessage << endl;
        outputFile.close();
        cout << "Результат в output.txt" << endl;
    } else {
        cerr << "Невозможно открыть output.txt для записи." << endl;
        return 1;
    }
    return 0;
}
