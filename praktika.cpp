#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <locale>
#include <sstream>

using namespace std;
short int spravka;
//хехехех
//Фцнкция для считывания ключа из файла и сихраняет в вектор magic
vector<int> readMagicKey(const string& fileName) {
    ifstream keyFile(fileName);  //чтение из файла
    vector<int> magic;
    if (keyFile.is_open()) {
        string line;
        getline(keyFile, line);
        keyFile.close();

        stringstream ss(line);
        int number;
        while (ss >> number) {
            magic.push_back(number);  //считанное число добавляется в конец  вектора magic
        }
    }
    else {
        cerr << "Не удалось открыть файл с ключом." << endl;
    }
    return magic;
}

string simpleSubstitutionCipher(string text, string key) {
    string result = text; // Инициализация результата шифрования как копия исходного текста

        // Преобразование текста в верхний регистр и удаление всех пробелов
    transform(result.begin(), result.end(), result.begin(), ::toupper); // Преобразование всех символов в верхний регистр
    result.erase(remove_if(result.begin(), result.end(), ::isspace), result.end()); // Удаление всех пробелов

        // Разделение текста на блоки по длине ключа
    int blockSize = key.length(); // Длина ключа шифрования (количество символов)
    int numBlocks = result.length() / blockSize; // Количество полных блоков в тексте
    if (result.length() % blockSize != 0) {
        numBlocks++; // Если длина текста не кратна длине ключа, добавляем еще один блок
    }

        // Дополнение последнего блока символами, если нужно
    if (result.length() % blockSize != 0) {
        int padding = blockSize - (result.length() % blockSize); // Рассчитываем количество символов для дополнения блока
        for (int i = 0; i < padding; i++) {
            result += 'Ъ'; // Добавление символа "Ъ" для дополнения блока
        }
    }

        // Использование ключа для перестановки букв в каждом блоке
    for (int i = 0; i < numBlocks; i++) { // Проходим по каждому блоку текста
        string block = result.substr(i * blockSize, blockSize); // Извлекаем текущий блок текста
        string processedBlock(blockSize, ' '); // Создаем пустой блок для обработанного текста, заполняя его пробелами

        for (int j = 0; j < blockSize; j++) { // Проходим по каждому символу в блоке
            int index = key[j] - '0' - 1; // Вычисляем индекс перестановки для текущего символа (индекс в ключе)
            processedBlock[j] = block[index]; // Заменяем текущий символ в обрабатываемом блоке символом с рассчитанного индекса исходного блока
        }
        result.replace(i * blockSize, blockSize, processedBlock); // Заменяем исходный блок текста в result на обработанный блок
    }
    return result; // Возвращаем результат шифрования/дешифрования
}

string decrypt(const string& encryptedText, const vector<int>& magic) {
    int blockSize = magic.size(); // Размер блока (длина ключа)
    int numBlocks = (encryptedText.length() + blockSize - 1) / blockSize; // Определение количества блоков, с округлением вверх

    string decryptedText = encryptedText; // Сохраняем зашифрованный текст перед дешифрацией

    for (int i = 0; i < numBlocks; i++) { // Цикл по каждому блоку
        string block = encryptedText.substr(i * blockSize, blockSize); // Извлекаем текущий блок из зашифрованного текста
        string processedBlock(blockSize, ' '); // Создаем пустой блок для обработанного текста, заполняя его пробелами

        for (int j = 0; j < blockSize; j++) { // Перебираем символы в блоке
            int index = magic[j] - 1; // Получаем индекс перестановки, используя массив magic
            processedBlock[index] = block[j]; // Переставляем символ с позиции j в блоке в позицию index в обработанном блоке
        }

        decryptedText.replace(i * blockSize, blockSize, processedBlock); // Заменяем блок в исходном тексте на обработанный
    }

    // Удаляем символы 'Ъ' из конца строки
    size_t lastCharPos = decryptedText.find_last_not_of('Ъ'); // Находим позицию последнего символа, не равного 'Ъ'
    if (lastCharPos != string::npos) { // Проверяем, есть ли такие символы
        decryptedText.erase(lastCharPos + 1); // Удаляем все символы после последнего не-'Ъ' символа
    }
    return decryptedText; // Возвращаем дешифрованный текст
}

string simpleSubstitutionDecipher(string text, const vector<int>& magic) {
    string decryptedText = decrypt(text, magic);
    return decryptedText;
}

bool isFileValid(const string& fileName) {
    ifstream fin(fileName);
    if (!fin.is_open()) {
        cerr << "Не удалось открыть файл для проверки." << endl;
        return false;
    }

    string line;
    getline(fin, line);
    fin.close();

    if (line.empty()) {
        return false;
    }

    for (unsigned char c : line) {
        if (!isalnum(c) && !isspace(c) && !(c >= 192 && c <= 255)) {
            return false;
        }
    }
    return true;
}

void info() //Справка
{
    cout << "Справка" << endl;
    cout << "1. В качестве первого аргумента должно быть введено имя файла для чтения. Лучше используйте файлы 1.txt или 2.txt" << endl;
    cout << "Проверьте корректность введённого имени файла или пути. Также обратите внимание, что прежде чем использовать сторонний файл, то нужно будет поместить его в каталог решения и сохранить с кодировкой (кириллица)" << endl;
    cout << "2. В качестве второго аргумента должно быть введено имя файла для записи туда шифрованных/дешифрованных данных. Лучше используйте файлы 1.txt или 2.txt" << endl;
    cout << "Проверьте корректность введённого имени файла или пути. Также обратите внимание, что прежде чем использовать сторонний файл, то нужно будет поместить его в каталог и сохранить с кодировкой (кириллица)" << endl;
    cout << "3. В качестве третьего аргумента должен быть введён числовой ключ. Обратите внимание, что ключ должен состоять из неповторяющихся цифр от 1 до 9 " << endl;
    cout << "4. В качестве четвертого аргумента должно быть введено действе. enc - шифрование, dec - дешифрование." << endl;
    cout << "Обратите внимание, что ключ в командой строке при дешифровке менять НЕЛЬЗЯ!" << endl;
}

string readFile(const string& filename) {
    if (!isFileValid(filename)) {
        cerr << "Файл для чтения некорректный: пустой или содержит неверные символы." << endl;
    }

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл для чтения данных" << endl;
        cerr << "Проверьте правильность введенного имени файла или пути." << endl;
        cerr << "Для помощи обратитесь к справке. Для ее вызова вместо первого аргумента введите info или сейчас напишите 1: ";
        cin >> spravka;
        if (spravka == 1) {
            info();
        }
        else {
            exit(1);
        }
    }
    string data;
    getline(file, data);
    file.close();

    return data;
}

void writeFile(const string& filename, const string& data) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка открытия файла " << filename << " для записи данных" << endl;
        cerr << "Проверьте правильность введенного имени файла или пути." << endl;
        cerr << "Для помощи обратитесь к справке. Для ее вызова вместо первого аргумента введите info или сейчас напишите 1: ";
        cin >> spravka;
        if (spravka == 1) {
            info();
        }
        else {
            exit(1);
        }
        exit(1);
    }
    file << data;
    file.close();
}

void provKey(string kk) {
    if (kk.find_first_not_of("1234567890") != string::npos) {
        cerr << "Ключ должен содержать только цифры (кроме нуля)" << endl;
        exit(1);
    }
    bool keyRepit = true;
    for (int i = 0; i < kk.length(); ++i) {
        for (int j = i + 1; j < kk.length(); ++j) {
            if (kk[i] == kk[j]) { // Проверка на совпадение цифр
                keyRepit = false;
            }
        }
    }
    if (!keyRepit) {
        cerr << "Ключ не должен содержать повторяющиеся цифры." << endl;
        exit(1);
    }
}

int main(int argc, char* argv[]) 
{
    setlocale(LC_ALL, "Russian");
    cerr << "Вид командной строки: <input_file> <output_file> <key> <action>" << endl;
    cerr << "Для помощи обратитесь вы можете обратиться к справке. Для ее вызова вместо первого аргумента введите info или сейчас напишите 1. Если хотите продолжить работу программы, то нажмите любую другую цифру. ";
    cin >> spravka;
    if (spravka == 1) {
        info();
    }
    else {
        exit(1);
    }

    if (strcmp(argv[1], "info") == 0) {
        info();
    }
    if (argc != 5) {
        cerr << "Неверное количество аргументов." << endl;
        cerr << "Вид командной строки: <input_file> <output_file> <key> <action>" << endl;
        cerr << "Для помощи обратитесь вы можете обратиться к справке. Для ее вызова вместо первого аргумента введите info или сейчас напишите 1: ";
        cin >> spravka;
        if (spravka == 1) {
            info();
        }
        else {
            exit(1);
        }
        return 1;
    }

    string inputFileName = argv[1];
    string outputFileName = argv[2];
    string key = argv[3];
    string action = argv[4];

    provKey(key);

    // Запись ключа в файл 3.txt
    ofstream keyFile("3.txt");
    if (keyFile.is_open()) {
        keyFile << key;
        keyFile.close();
    }
    else {
        cerr << "Не удалось открыть файл для записи ключа. Проверьте существует ли файл 3.txt" << endl;
        return 1;
    }

    string inputText = readFile(inputFileName);

    string outputText;
    if (action == "enc") {
        outputText = simpleSubstitutionCipher(inputText, key);
        writeFile(outputFileName, outputText);
        cout << "Шифрование завершено." << endl;

        cout << "Если вы хотите дешифровать файл, то нажмите 1, если хотите выйти из программы, то любое другое значение." << endl;
        int desh;
        cin >> desh;
        if (desh == 1)
        {
            inputFileName = argv[2];
            outputFileName = argv[1];

            inputText = readFile(inputFileName);

            // Генерация "магического" ключа для дешифровки
            vector<int> magic;
            for (int i = 0; i < key.length(); i++) {
                magic.push_back(key[i] - '0');
            }

            outputText = simpleSubstitutionDecipher(inputText, magic);
            cout << "Дешифрование завершено." << endl;

            writeFile(outputFileName, outputText);
        }
    }
    else if (action == "dec") {
        // Генерация "магического" ключа для дешифровки
        vector<int> magic;
        for (int i = 0; i < key.length(); i++) {
            magic.push_back(key[i] - '0');
        }
        outputText = simpleSubstitutionDecipher(inputText, magic);
        writeFile(outputFileName, outputText);
        cout << "Дешифрование завершено." << endl;
    }
    else {
        cerr << "Неверное действие. Используйте 'enc' для шифрования или 'dec' для дешифрования." << endl;
        return 1;
    }
    return 0;
}

