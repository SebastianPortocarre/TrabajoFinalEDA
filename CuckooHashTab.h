#ifndef CUCKOOHASHTAB_H
#define CUCKOOHASHTAB_H

#include <string>
#include <vector>
#include <functional>

class CuckooHashTab {
public:
    CuckooHashTab(int initial_size, int num_tables = 2);

    void insertarComprimido(const std::string& key, const std::string& ciudadanoData);
    std::string buscarComprimido(const std::string& key);
    void eliminar(const std::string& key);

private:
    struct Entry {
        std::string key;                  
        std::vector<char> compressedData; 
    };

    int size;   
    int num_tables; 
    std::vector<std::vector<Entry>> tables; 

    
    size_t hash1(const std::string& key) const;
    size_t hash2(const std::string& key) const;
    size_t default_hash(const std::string& key, int table_idx) const;

    // Cálculo del índice hash basado en el número de tabla
    int hash(int table_idx, const std::string& key) const;

    // Rehashing cuando se alcanzan demasiadas colisiones
    void rehash();
    bool insertRehash(const Entry& entry, std::vector<std::vector<Entry>>& new_tables, int rehash_count);

    // Límite para realizar rehash en caso de colisiones continuas
    const int rehash_limit = 10;
};

#endif // CUCKOOHASHTAB_H
