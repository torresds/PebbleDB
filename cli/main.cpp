#include "db/db.h"
#include <iostream>
#include <vector>
#include <string>

const char* const COLOR_RESET = "\033[0m";
const char* const COLOR_RED = "\033[0;31m";
const char* const COLOR_GREEN = "\033[0;32m";
const char* const COLOR_YELLOW = "\033[0;33m";
const char* const COLOR_BLUE = "\033[0;34m";
const char* const COLOR_CYAN = "\033[0;36m";

void show_usage() {
    std::cout << COLOR_CYAN << "PebbleDB" << COLOR_RESET << std::endl;
    std::cout << std::endl;
    std::cout << COLOR_YELLOW << "Uso:" << COLOR_RESET << std::endl;
    std::cout << "  pebble <cmd> [args...]" << std::endl;
    std::cout << std::endl;
    std::cout << COLOR_YELLOW << "COMANDOS:" << COLOR_RESET << std::endl;
    std::cout << "  " << COLOR_GREEN << "put <chave> <valor>" << COLOR_RESET << "    - Insere/atualiza uma chave com o valor." << std::endl;
    std::cout << "  " << COLOR_GREEN << "get <chave>" << COLOR_RESET << "            - Pega o valor da chave." << std::endl;
    std::cout << "  " << COLOR_GREEN << "del <chave>" << COLOR_RESET << "            - Apaga o par chave-valor." << std::endl;
    std::cout << std::endl;
}

void handle_put(PebbleDB& db, int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << COLOR_RED << "❌ Erro: o comando 'put' utiliza apenas um argumento." << COLOR_RESET << std::endl;
        std::cerr << "   Uso: pebble put <chave> <valor>" << std::endl;
        return;
    }
    std::string key = argv[2];
    std::string value = argv[3];
    db.put(key, value);
    std::cout << COLOR_GREEN << "✅ Sucesso: chave '" << key << "' definida." << COLOR_RESET << std::endl;
}


void handle_get(PebbleDB& db, int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << COLOR_RED << "❌ Erro: o comando 'get' suporta apenas uma chave." << COLOR_RESET << std::endl;
        std::cerr << "   Uso: pebble get <chave>" << std::endl;
        return;
    }
    std::string key = argv[2];
    auto value = db.get(key);
    if (value) {
        std::cout << *value << std::endl;
    } else {
        std::cout << COLOR_YELLOW << "ℹ️ Info: A chave '" << key << "' não foi encontrada." << COLOR_RESET << std::endl;
    }
}


void handle_del(PebbleDB& db, int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << COLOR_RED << "❌ Erro: o comando 'del' suporta apenas uma chave." << COLOR_RESET << std::endl;
        std::cerr << "   Uso: pebble del <chave>" << std::endl;
        return;
    }
    std::string key = argv[2];
    db.del(key);
    std::cout << COLOR_GREEN << "✅ Sucesso: A chave '" << key << "' foi deletada." << COLOR_RESET << std::endl;
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        show_usage();
        return 1;
    }

    PebbleDB db("data");
    std::string command = argv[1];

    if (command == "put") {
        handle_put(db, argc, argv);
    } else if (command == "get") {
        handle_get(db, argc, argv);
    } else if (command == "del") {
        handle_del(db, argc, argv);
    } else {
        std::cerr << COLOR_RED << "❌ Erro: commando não existe '" << command << "'" << COLOR_RESET << std::endl;
        std::cout << std::endl;
        show_usage();
        return 1;
    }

    return 0;
}