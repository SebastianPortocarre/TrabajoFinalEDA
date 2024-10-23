// RegistroCiudadanos.h
#ifndef REGISTROCIUDADANOS_H
#define REGISTROCIUDADANOS_H

#include <vector>
#include <cstdint>
#include <string>
#include <fstream>
#include "Ciudadano.h"
#include "Tablas.h"
#include "CuckooHashTab.h" // Asegúrate de incluir el encabezado

class RegistroCiudadanos {
public:
    RegistroCiudadanos();
    ~RegistroCiudadanos();

    // Funciones principales
    void generarCiudadanosAleatorios(int cantidad);    // Generar ciudadanos aleatorios
    void insertarCiudadanoManual();                     // Insertar ciudadano manualmente
    CiudadanoOptimizado* buscarCiudadano(uint32_t dni); // Buscar ciudadano por DNI
    bool eliminarCiudadano(uint32_t dni);              // Eliminar ciudadano por DNI
    void imprimirTresDniAleatorios();                   // Imprimir tres DNIs aleatorios
    void exportarACSV(const std::string& nombre_archivo); // Exportar datos a CSV
    Tablas tablas;

private:
    std::vector<CiudadanoOptimizado> ciudadanos; // Vector que almacena a todos los ciudadanos

    // Bitmap para rastrear DNIs ya utilizados y evitar duplicados
    std::vector<bool> used_dnis;

    // Rango válido de DNI
    static const uint32_t DNI_MIN = 10000000;
    static const uint32_t DNI_MAX = 99999999;

    // Instancia de CuckooHashTab
    CuckooHashTab cuckooHashTable;

    // Funciones auxiliares para generar datos aleatorios
    uint32_t generarDniAleatorio();
    std::string generarNombreAleatorio();
    std::string generarApellidoAleatorio();
    uint32_t generarTelefonoAleatorio();
    std::string generarEmailAleatorio(const std::string& nombre, const std::string& dominio);
    std::string generarUbicacionAleatoria();

    // Funciones para cargar y guardar datos
    bool cargarDesdeArchivo(const std::string& nombre_archivo_binario);
    bool guardarEnArchivo(const std::string& nombre_archivo_binario);
};

#endif // REGISTROCIUDADANOS_H
