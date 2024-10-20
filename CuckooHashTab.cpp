#include "CuckooHashTab.h"
#include "Compressor.h"
#include <iostream>
#include <stdexcept>
using namespace std;

// Constructor que inicializa las tablas de hash
CuckooHashTab::CuckooHashTab(int initial_size, int num_tables)
    : size(initial_size), num_tables(num_tables) {
    tables.resize(num_tables);
    for (int i = 0; i < num_tables; ++i) {
        tables[i].resize(size);
    }
}

// Función para calcular el índice de hash basado en el número de tabla
int CuckooHashTab::hash(int table_idx, const string& key) const {
    size_t h;
    switch (table_idx) {
        case 0:
            h = hash1(key);
            break;
        case 1:
            h = hash2(key);
            break;
        default:
            h = default_hash(key, table_idx);
            break;
    }
    return h % size;
}

size_t CuckooHashTab::hash1(const string& key) const {
    return hash<string>{}(key);
}

size_t CuckooHashTab::hash2(const string& key) const {
    hash<string> hasher;
    return hasher("salt1" + key);
}

size_t CuckooHashTab::default_hash(const string& key, int table_idx) const {
    hash<string> hasher;
    return hasher("salt" + to_string(table_idx) + key);
}

// Rehash cuando se alcanzan las colisiones
void CuckooHashTab::rehash() {
    cout << "Rehashing tables..." << endl;
    size *= 2;  // Duplicar el tamaño de las tablas

    // Crear nuevas tablas
    vector<vector<Entry>> new_tables(num_tables);
    for (int i = 0; i < num_tables; ++i) {
        new_tables[i].resize(size);
    }

    // Reinsertar los datos en las nuevas tablas
    for (int i = 0; i < num_tables; ++i) {
        for (const auto& entry : tables[i]) {
            if (!entry.key.empty()) {
                if (!insertRehash(entry, new_tables, 0)) {
                    // Si no se puede insertar, incrementar el tamaño y reintentar
                    size *= 2;
                    rehash();
                    return;
                }
            }
        }
    }

    tables = move(new_tables);  // Reemplazar las tablas antiguas por las nuevas
}

// Función para insertar datos durante el rehash
bool CuckooHashTab::insertRehash(const Entry& entry, vector<vector<Entry>>& new_tables, int rehash_count) {
    if (rehash_count >= rehash_limit) {
        return false;
    }

    Entry temp_entry = entry;
    for (int count = 0; count < rehash_limit; ++count) {
        for (int i = 0; i < num_tables; ++i) {
            int h = hash(i, temp_entry.key);
            if (new_tables[i][h].key.empty()) {
                new_tables[i][h] = temp_entry;
                return true;
            }
            swap(temp_entry, new_tables[i][h]);
        }
    }
    return false;
}

// Función para insertar un ciudadano comprimido
void CuckooHashTab::insertarComprimido(const string& key, const string& ciudadanoData) {
    // Comprimir los datos antes de insertarlos
    vector<char> compressedData = Compressor::compress(ciudadanoData);
    Entry entry = { key, compressedData };

    for (int count = 0; count < rehash_limit; ++count) {
        Entry temp_entry = entry;
        for (int i = 0; i < num_tables; ++i) {
            int h = hash(i, temp_entry.key);
            if (tables[i][h].key.empty()) {
                tables[i][h] = temp_entry;
                return;
            }
            swap(temp_entry, tables[i][h]);
        }
    }

    // Si no se pudo insertar, realizar rehash y reintentar
    rehash();
    insertarComprimido(key, ciudadanoData);
}

// Función para buscar un ciudadano por su clave
string CuckooHashTab::buscarComprimido(const string& key) {
    for (int i = 0; i < num_tables; ++i) {
        int h = hash(i, key);
        if (tables[i][h].key == key) {
            // Descomprimir los datos encontrados sin necesidad del tamaño original
            return Compressor::decompress(tables[i][h].compressedData);
        }
    }
    return "";  // No encontrado
}

// Función para eliminar un ciudadano por su clave
void CuckooHashTab::eliminar(const string& key) {
    for (int i = 0; i < num_tables; ++i) {
        int h = hash(i, key);
        if (tables[i][h].key == key) {
            tables[i][h] = Entry();  // Vaciar la entrada
            return;
        }
    }
    throw runtime_error("Datos no encontrados");
}