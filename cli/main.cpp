#include "core/record.h"
#include <fstream>
#include <iostream>
#include <filesystem>

int main() {
    std::filesystem::create_directories("data");

    Record rec{"user:1", "João", 123456, false};
    std::ofstream out("data/test.rec", std::ios::binary);
    if (!out) {
        std::cerr << "Erro ao abrir o arquivo para escrita.\n";
        return 1;
    }
    rec.serialize(out);
    out.close();

    std::ifstream in("data/test.rec", std::ios::binary);
    if (!in) {
        std::cerr << "Erro ao abrir o arquivo para leitura.\n";
        return 1;
    }
    Record loaded = Record::deserialize(in);

    std::cout << loaded.key << " | " << loaded.value << "\n";
}
