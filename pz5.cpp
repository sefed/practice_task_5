#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <windows.h>


class DynamicArray {
private:
    int* data;
    int size;

public:
    DynamicArray(int arrSize) : data(nullptr), size(0) {
        if (arrSize < 0) {
            std::cout << "Ошибка: размер не может быть отрицательным" << std::endl;
            return;
        }
        size = arrSize;
        if (size > 0) {
            data = new int[size];
            for (int i = 0; i < size; i++) data[i] = 0;
        }
    }

    DynamicArray(const DynamicArray& other) : data(nullptr), size(other.size) {
        if (size > 0) {
            data = new int[size];
            for (int i = 0; i < size; i++) data[i] = other.data[i];
        }
    }

    // виртуальный деструктор
    virtual ~DynamicArray() {
        delete[] data;
    }

    // сеттер
    void setValue(int index, int value) {
        if (0 <= index && index < size) {
            if (-100 <= value && value <= 100) {
                data[index] = value;
            } else {
                std::cout << "Ошибка: значение " << value << " выходит за пределы [-100, 100]" << std::endl;
            }
        } else {
            std::cout << "Ошибка: индекс " << index << " выходит за границы массива" << std::endl;
        }
    }

    // геттер
    int getValue(int index) const {
        if (0 <= index && index < size) {
            return data[index];
        } else {
            std::cout << "Ошибка: индекс " << index << " выходит за границы массива" << std::endl;
            return 0;
        }
    }

    // вывод в консоль
    void print() const {
        for (int i = 0; i < size; i++) std::cout << getValue(i) << " ";
        std::cout << std::endl;
    }

    // добавление значения в конец
    void addValue(int value) {
        if (-100 <= value && value <= 100) {
            int* newData = new int[size + 1];
            for (int i = 0; i < size; i++) newData[i] = data[i];
            newData[size] = value;
            delete[] data;
            data = newData;
            size++;
        } else {
            std::cout << "Ошибка: значение " << value << " выходит за пределы [-100, 100]" << std::endl;
        }
    }

    // операция сложения массивов
    DynamicArray add(const DynamicArray& other) const {
        int maxSize = (size > other.size) ? size : other.size;
        DynamicArray result(maxSize);
        for (int i = 0; i < maxSize; i++) {
            int val1 = (i < size) ? getValue(i) : 0;
            int val2 = (i < other.size) ? other.getValue(i) : 0;
            result.setValue(i, val1 + val2);
        }
        return result;
    }

    // операция вычитания массивов
    DynamicArray subtract(const DynamicArray& other) const {
        int maxSize = (size > other.size) ? size : other.size;
        DynamicArray result(maxSize);
        for (int i = 0; i < maxSize; i++) {
            int val1 = (i < size) ? getValue(i) : 0;
            int val2 = (i < other.size) ? other.getValue(i) : 0;
            result.setValue(i, val1 - val2);
        }
        return result;
    }

    // геттер для размера
    int getSize() const {
        return size;
    }

    // Виртуальный метод сохранения
    virtual void saveToFile() const {
        
    }
};

// функция для получения текущего времени
std::string getCurrentDateTime() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[256];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", ltm);
    return std::string(buffer);
}


class ArrTxt : public DynamicArray {
public:
    // используем конструктор базового класса
    ArrTxt(int size) : DynamicArray(size) {}
    ArrTxt(const DynamicArray& other) : DynamicArray(other) {}

    void saveToFile() const override {
        std::string fname = getCurrentDateTime() + ".txt";
        std::ofstream fout(fname);
        if (!fout) {
            std::cerr << "Не удалось создать файл: " << fname << std::endl;
            return;
        }
        int n = getSize();
        for (int i = 0; i < n; ++i) {
            fout << getValue(i);
            if (i + 1 < n) fout << " ";
        }
        fout << "\n";
        fout.close();
        std::cout << "Сохранено в " << fname << std::endl;
    }
};


class ArrCSV : public DynamicArray {
public:
    ArrCSV(int size) : DynamicArray(size) {}
    ArrCSV(const DynamicArray& other) : DynamicArray(other) {}

    void saveToFile() const override {
        std::string fname = getCurrentDateTime() + ".csv";
        std::ofstream fout(fname);
        if (!fout) {
            std::cerr << "Не удалось создать файл: " << fname << std::endl;
            return;
        }
        int n = getSize();
        for (int i = 0; i < n; ++i) {
            fout << getValue(i);
            if (i + 1 < n) fout << ",";
        }
        fout << "\n";
        fout.close();
        std::cout << "Сохранено в " << fname << std::endl;
    }
};


int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    ArrTxt arr(5);
    arr.setValue(0, 10);
    arr.setValue(1, -5);
    arr.setValue(2, 20);
    arr.setValue(3, 0);
    arr.setValue(4, 100);

    arr.print();


    arr.saveToFile();


    DynamicArray* p = new ArrCSV(arr);
    p->saveToFile();
    delete p;

    return 0;
}
