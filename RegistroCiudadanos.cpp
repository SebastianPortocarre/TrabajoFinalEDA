#include "RegistroCiudadanos.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
using namespace std;

// Constructor
RegistroCiudadanos::RegistroCiudadanos() {
    // Intentar cargar los datos desde el archivo binario
    string nombre_archivo_binario = "ciudadanos.bin";
    ifstream infile(nombre_archivo_binario, ios::binary);
    if (infile.good()) {
        infile.close();
        if (cargarDesdeArchivo(nombre_archivo_binario)) {
            cout << "Datos cargados exitosamente desde " << nombre_archivo_binario << ".\n";
            // Ordenar el vector por DNI después de cargar
            sort(ciudadanos.begin(), ciudadanos.end(), [](const CiudadanoOptimizado& a, const CiudadanoOptimizado& b) -> bool {
                return a.dni < b.dni;
            });
            cout << "Vector de ciudadanos ordenado por DNI después de la carga.\n";
            return;
        } else {
            cerr << "Error al cargar los datos desde " << nombre_archivo_binario << ". Generando nuevos datos.\n";
        }
    } else {
        cout << "Archivo de datos no encontrado. Generando 33 millones de ciudadanos.\n";
        generarCiudadanosAleatorios(33000000);
        // Ordenar el vector por DNI después de generar
        sort(ciudadanos.begin(), ciudadanos.end(), [](const CiudadanoOptimizado& a, const CiudadanoOptimizado& b) -> bool {
            return a.dni < b.dni;
        });
        cout << "Vector de ciudadanos ordenado por DNI después de la generación.\n";
        if (guardarEnArchivo(nombre_archivo_binario)) {
            cout << "Datos guardados exitosamente en " << nombre_archivo_binario << ".\n";
        } else {
            cerr << "Error al guardar los datos en " << nombre_archivo_binario << ".\n";
        }
    }
}

RegistroCiudadanos::~RegistroCiudadanos() {
    // Guardar los datos al finalizar
    string nombre_archivo_binario = "ciudadanos.bin";
    if (guardarEnArchivo(nombre_archivo_binario)) {
        cout << "Datos guardados exitosamente en " << nombre_archivo_binario << " al finalizar.\n";
    } else {
        cerr << "Error al guardar los datos en " << nombre_archivo_binario << " al finalizar.\n";
    }
}

// Función para generar 33 millones de ciudadanos aleatorios
void RegistroCiudadanos::generarCiudadanosAleatorios(int cantidad) {
    if (cantidad <= 0) {
        cerr << "Cantidad inválida para generar ciudadanos.\n";
        return;
    }

    cout << "Generando " << cantidad << " ciudadanos aleatorios...\n";

    // Configuración de generadores aleatorios
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<uint32_t> dis_dni(DNI_MIN, DNI_MAX);
    uniform_int_distribution<uint32_t> dis_telefono(900000000, 999999999);
    uniform_int_distribution<int> dis_nacionalidad(0, 1);
    uniform_int_distribution<int> dis_estado_civil(0, 3);

    // Listas ampliadas de nombres y apellidos para generar nombres completos
    vector<string> nombres = {
        "Juan", "María", "Carlos", "Ana", "Luis", "Carmen",
        "Pedro", "Lucía", "Jorge", "Sofía", "Miguel", "Elena",
        "Andrés", "Isabel", "Fernando", "Laura", "Ricardo", "Patricia",
        "Daniel", "Claudia", "Gabriel", "Fernanda", "Santiago", "Valeria"
    };
    vector<string> apellidos = {
        "Pérez", "González", "Rodríguez", "López", "García", "Martínez",
        "Sánchez", "Ramírez", "Torres", "Flores", "Díaz", "Morales",
        "Vásquez", "Jiménez", "Rojas", "Alvarez", "Castillo", "Vega",
        "Ortiz", "Silva", "Mendoza", "Cortez", "Ruiz", "Reyes"
    };
    vector<string> departamentos_lista = { "Lima", "Arequipa", "Cusco", "La Libertad", "Piura", "Junín", "Ancash", "Puno", "Apurímac", "Tacna" };
    vector<string> provincias_lista = { "Lima", "Arequipa", "Cusco", "Trujillo", "Piura", "Huancayo", "Huaraz", "Juliaca", "Abancay", "Tacna" };
    vector<string> ciudades_lista = { "Lima", "Arequipa", "Cusco", "Trujillo", "Piura", "Huancayo", "Huaraz", "Juliaca", "Abancay", "Tacna" };
    vector<string> distritos_lista = { "Miraflores", "San Isidro", "Cusco", "Trujillo", "Piura", "Huancayo", "Huaraz", "Juliaca", "Abancay", "Tacna" };
    vector<string> ubicaciones_lista = { "Av. Pardo 123", "Calle 50 #456", "Jr. Las Flores 789", "Av. Arequipa 321", "Calle Lima 654" };
    vector<string> dominios = { "example.com", "correo.com", "mail.com", "peru.com", "hotmail.com", "gmail.com" };

    // Generador para seleccionar elementos aleatorios de las listas
    uniform_int_distribution<> dis_nombres(0, nombres.size() - 1);
    uniform_int_distribution<> dis_apellidos(0, apellidos.size() - 1);
    uniform_int_distribution<> dis_departamentos(0, departamentos_lista.size() - 1);
    uniform_int_distribution<> dis_provincias(0, provincias_lista.size() - 1);
    uniform_int_distribution<> dis_ciudades(0, ciudades_lista.size() - 1);
    uniform_int_distribution<> dis_distritos(0, distritos_lista.size() - 1);
    uniform_int_distribution<> dis_ubicaciones(0, ubicaciones_lista.size() - 1);
    uniform_int_distribution<> dis_dominios(0, dominios.size() - 1);

    ciudadanos.reserve(cantidad);

    for (int i = 0; i < cantidad; ++i) {
        CiudadanoOptimizado ciudadano;

        // Generar DNI único (Nota: Este método no garantiza unicidad)
        ciudadano.dni = dis_dni(gen);
        // Para evitar duplicados, considera implementar una verificación adicional

        // Generar Nombre y Apellido
        string nombre = nombres[dis_nombres(gen)];
        string apellido = apellidos[dis_apellidos(gen)];
        string nombre_completo = nombre + " " + apellido;
        ciudadano.nombres_apellidos = tablas.obtenerIndiceNombreApellido(nombre_completo);

        // Generar Lugar de Nacimiento
        string lugar_nacimiento = ciudades_lista[dis_ciudades(gen)];
        ciudadano.lugar_nacimiento = tablas.obtenerIndiceLugarNacimiento(lugar_nacimiento);

        // Generar Nacionalidad
        ciudadano.nacionalidad = (dis_nacionalidad(gen) == 0) ? Nacionalidad::Peruano : Nacionalidad::Extranjero;

        // Generar Dirección
        string departamento = departamentos_lista[dis_departamentos(gen)];
        ciudadano.direccion.departamento = tablas.obtenerIndiceDepartamento(departamento);

        string provincia = provincias_lista[dis_provincias(gen)];
        ciudadano.direccion.provincia = tablas.obtenerIndiceProvincia(provincia);

        string ciudad = ciudades_lista[dis_ciudades(gen)];
        ciudadano.direccion.ciudad = tablas.obtenerIndiceCiudad(ciudad);

        string distrito = distritos_lista[dis_distritos(gen)];
        ciudadano.direccion.distrito = tablas.obtenerIndiceDistrito(distrito);

        string ubicacion = ubicaciones_lista[dis_ubicaciones(gen)];
        ciudadano.direccion.ubicacion = tablas.obtenerIndiceUbicacion(ubicacion);

        // Generar Teléfono
        ciudadano.telefono = dis_telefono(gen);

        // Generar Email
        string email = nombre + "." + apellido + "@" + dominios[dis_dominios(gen)];
        ciudadano.email = tablas.obtenerIndiceEmail(email);

        // Generar Estado Civil
        ciudadano.estado_civil = static_cast<EstadoCivil>(dis_estado_civil(gen));

        // Añadir al vector de ciudadanos
        ciudadanos.push_back(ciudadano);

        // Mostrar progreso cada 1 millón de registros
        if ((i + 1) % 1000000 == 0) {
            cout << (i + 1) << " ciudadanos generados.\n";
        }
    }

    cout << "Generación de ciudadanos aleatorios completada.\n";
}

// Función para insertar un ciudadano manualmente
void RegistroCiudadanos::insertarCiudadanoManual() {
    cout << "Entrando a insertarCiudadanoManual.\n";
    CiudadanoOptimizado nuevo_ciudadano;
    string input_str;
    int input_enum;

    // Ingreso de DNI
    cout << "Ingrese el DNI: ";
    cin >> nuevo_ciudadano.dni;
    if (nuevo_ciudadano.dni < DNI_MIN || nuevo_ciudadano.dni > DNI_MAX) {
        cerr << "DNI fuera de rango válido (" << DNI_MIN << " - " << DNI_MAX << ").\n";
        return;
    }

    // Verificar si el DNI ya existe
    if (buscarCiudadano(nuevo_ciudadano.dni) != nullptr) {
        cerr << "Error: El DNI " << nuevo_ciudadano.dni << " ya existe en el registro.\n";
        return;
    }

    // Ingreso de Nombre y Apellido
    string nombre, apellido;
    cout << "Ingrese el Nombre: ";
    cin >> nombre;
    cout << "Ingrese el Apellido: ";
    cin >> apellido;
    string nombre_completo = nombre + " " + apellido;
    nuevo_ciudadano.nombres_apellidos = tablas.obtenerIndiceNombreApellido(nombre_completo);

    // Ingreso de Lugar de Nacimiento
    cout << "Ingrese el Lugar de Nacimiento: ";
    cin >> input_str;
    nuevo_ciudadano.lugar_nacimiento = tablas.obtenerIndiceLugarNacimiento(input_str);

    // Ingreso de Nacionalidad
    cout << "Ingrese la Nacionalidad (0: Peruano, 1: Extranjero): ";
    cin >> input_enum;
    if (input_enum == 0)
        nuevo_ciudadano.nacionalidad = Nacionalidad::Peruano;
    else if (input_enum == 1)
        nuevo_ciudadano.nacionalidad = Nacionalidad::Extranjero;
    else {
        cerr << "Opción de Nacionalidad no válida.\n";
        return;
    }

    // Ingreso de Dirección
    cout << "Ingrese el Departamento: ";
    cin >> input_str;
    nuevo_ciudadano.direccion.departamento = tablas.obtenerIndiceDepartamento(input_str);

    cout << "Ingrese la Provincia: ";
    cin >> input_str;
    nuevo_ciudadano.direccion.provincia = tablas.obtenerIndiceProvincia(input_str);

    cout << "Ingrese la Ciudad: ";
    cin >> input_str;
    nuevo_ciudadano.direccion.ciudad = tablas.obtenerIndiceCiudad(input_str);

    cout << "Ingrese el Distrito: ";
    cin >> input_str;
    nuevo_ciudadano.direccion.distrito = tablas.obtenerIndiceDistrito(input_str);

    cout << "Ingrese la Ubicación: ";
    cin.ignore(); // Limpiar el buffer
    getline(cin, input_str);
    nuevo_ciudadano.direccion.ubicacion = tablas.obtenerIndiceUbicacion(input_str);

    // Ingreso de Teléfono
    cout << "Ingrese el Teléfono: ";
    cin >> nuevo_ciudadano.telefono;

    // Ingreso de Email
    cout << "Ingrese el Email: ";
    cin >> input_str;
    nuevo_ciudadano.email = tablas.obtenerIndiceEmail(input_str);

    // Ingreso de Estado Civil
    cout << "Ingrese el Estado Civil (0: Soltero, 1: Casado, 2: Divorciado, 3: Viudo): ";
    cin >> input_enum;
    switch (input_enum) {
        case 0:
            nuevo_ciudadano.estado_civil = EstadoCivil::Soltero;
            break;
        case 1:
            nuevo_ciudadano.estado_civil = EstadoCivil::Casado;
            break;
        case 2:
            nuevo_ciudadano.estado_civil = EstadoCivil::Divorciado;
            break;
        case 3:
            nuevo_ciudadano.estado_civil = EstadoCivil::Viudo;
            break;
        default:
            cerr << "Opción de Estado Civil no válida.\n";
            return;
    }

    // Insertar el ciudadano en la posición correcta para mantener el orden
    auto it = lower_bound(ciudadanos.begin(), ciudadanos.end(), nuevo_ciudadano.dni,
        [](const CiudadanoOptimizado& c, uint32_t dni_buscar) -> bool {
            return c.dni < dni_buscar;
        });

    ciudadanos.insert(it, nuevo_ciudadano);
    cout << "Ciudadano insertado correctamente.\n";
    cout << "Saliendo de insertarCiudadanoManual.\n";
}

// Función para buscar un ciudadano por DNI utilizando búsqueda binaria
CiudadanoOptimizado* RegistroCiudadanos::buscarCiudadano(uint32_t dni) {
    // Realizar búsqueda binaria
    auto it = lower_bound(ciudadanos.begin(), ciudadanos.end(), dni,
        [](const CiudadanoOptimizado& c, uint32_t dni_buscar) -> bool {
            return c.dni < dni_buscar;
        });

    if (it != ciudadanos.end() && it->dni == dni) {
        return new CiudadanoOptimizado(*it); // Retornar una copia del ciudadano encontrado
    }

    return nullptr; // No encontrado
}

// Función para eliminar un ciudadano por DNI
bool RegistroCiudadanos::eliminarCiudadano(uint32_t dni) {
    // Realizar búsqueda binaria
    auto it = lower_bound(ciudadanos.begin(), ciudadanos.end(), dni,
        [](const CiudadanoOptimizado& c, uint32_t dni_buscar) -> bool {
            return c.dni < dni_buscar;
        });

    if (it != ciudadanos.end() && it->dni == dni) {
        ciudadanos.erase(it);
        cout << "Ciudadano con DNI " << dni << " eliminado correctamente.\n";
        return true;
    }

    cout << "Ciudadano con DNI " << dni << " no encontrado.\n";
    return false;
}

// Función para imprimir tres DNIs aleatorios
void RegistroCiudadanos::imprimirTresDniAleatorios() {
    if (ciudadanos.empty()) {
        cerr << "No hay ciudadanos registrados.\n";
        return;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<size_t> dis(0, ciudadanos.size() - 1);

    cout << "Tres DNIs aleatorios:\n";
    for (int i = 0; i < 3; ++i) {
        size_t idx = dis(gen);
        cout << (i + 1) << ". " << ciudadanos[idx].dni << "\n";
    }
}

// Función para exportar los datos a un archivo CSV
void RegistroCiudadanos::exportarACSV(const string& nombre_archivo) {
    cout << "Iniciando exportación a CSV: " << nombre_archivo << "\n";
    ofstream archivo_csv(nombre_archivo);
    if (!archivo_csv.is_open()) {
        cerr << "No se pudo abrir el archivo " << nombre_archivo << " para escribir.\n";
        return;
    }

    // Escribir la cabecera del CSV
    archivo_csv << "DNI,NombreApellido,LugarNacimiento,Nacionalidad,Departamento,Provincia,Ciudad,Distrito,Ubicacion,Telefono,Email,EstadoCivil\n";

    // Iterar sobre todos los ciudadanos
    for (const auto& ciudadano : ciudadanos) {
        // Verificar que los índices sean válidos antes de acceder
        if (ciudadano.nombres_apellidos >= tablas.nombres_apellidos.size() ||
            ciudadano.lugar_nacimiento >= tablas.lugares_nacimiento.size() ||
            ciudadano.direccion.departamento >= tablas.departamentos.size() ||
            ciudadano.direccion.provincia >= tablas.provincias.size() ||
            ciudadano.direccion.ciudad >= tablas.ciudades.size() ||
            ciudadano.direccion.distrito >= tablas.distritos.size() ||
            ciudadano.direccion.ubicacion >= tablas.ubicaciones.size() ||
            ciudadano.email >= tablas.emails.size()) {
            cerr << "Error: Índices fuera de rango para el ciudadano con DNI " << ciudadano.dni << ".\n";
            continue; // Saltar este registro
        }

        archivo_csv << ciudadano.dni << ","
                   << "\"" << tablas.nombres_apellidos[ciudadano.nombres_apellidos] << "\","
                   << "\"" << tablas.lugares_nacimiento[ciudadano.lugar_nacimiento] << "\","
                   << "\"" << ((ciudadano.nacionalidad == Nacionalidad::Peruano) ? "Peruano" : "Extranjero") << "\","
                   << "\"" << tablas.departamentos[ciudadano.direccion.departamento] << "\","
                   << "\"" << tablas.provincias[ciudadano.direccion.provincia] << "\","
                   << "\"" << tablas.ciudades[ciudadano.direccion.ciudad] << "\","
                   << "\"" << tablas.distritos[ciudadano.direccion.distrito] << "\","
                   << "\"" << tablas.ubicaciones[ciudadano.direccion.ubicacion] << "\","
                   << ciudadano.telefono << ","
                   << "\"" << tablas.emails[ciudadano.email] << "\","
                   << "\"" << ((ciudadano.estado_civil == EstadoCivil::Soltero) ? "Soltero" :
                               (ciudadano.estado_civil == EstadoCivil::Casado) ? "Casado" :
                               (ciudadano.estado_civil == EstadoCivil::Divorciado) ? "Divorciado" :
                               "Viudo") << "\"\n";
    }

    archivo_csv.close();
    cout << "Datos exportados correctamente a " << nombre_archivo << ".\n";
}

// Función para guardar los datos en un archivo binario
bool RegistroCiudadanos::guardarEnArchivo(const string& nombre_archivo_binario) {
    ofstream outfile(nombre_archivo_binario, ios::binary);
    if (!outfile.is_open()) {
        cerr << "No se pudo abrir el archivo " << nombre_archivo_binario << " para escribir.\n";
        return false;
    }

    size_t total = ciudadanos.size();
    outfile.write(reinterpret_cast<const char*>(&total), sizeof(size_t)); // Guardar el tamaño del vector

    outfile.write(reinterpret_cast<const char*>(ciudadanos.data()), ciudadanos.size() * sizeof(CiudadanoOptimizado));
    outfile.close();
    return true;
}

// Función para cargar los datos desde un archivo binario
bool RegistroCiudadanos::cargarDesdeArchivo(const string& nombre_archivo_binario) {
    ifstream infile(nombre_archivo_binario, ios::binary);
    if (!infile.is_open()) {
        cerr << "No se pudo abrir el archivo " << nombre_archivo_binario << " para leer.\n";
        return false;
    }

    size_t total;
    infile.read(reinterpret_cast<char*>(&total), sizeof(size_t)); // Leer el tamaño del vector

    ciudadanos.resize(total);
    infile.read(reinterpret_cast<char*>(ciudadanos.data()), total * sizeof(CiudadanoOptimizado));

    infile.close();
    cout << total << " ciudadanos cargados desde " << nombre_archivo_binario << ".\n";

    return true;
}

// Funciones auxiliares para generar datos aleatorios
uint32_t RegistroCiudadanos::generarDniAleatorio() {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<uint32_t> dis(DNI_MIN, DNI_MAX);
    return dis(gen);
}

string RegistroCiudadanos::generarNombreAleatorio() {
    static vector<string> nombres = { "Juan", "María", "Carlos", "Ana", "Luis", "Carmen",
                                               "Pedro", "Lucía", "Jorge", "Sofía", "Miguel", "Elena",
                                               "Andrés", "Isabel", "Fernando", "Laura", "Ricardo", "Patricia",
                                               "Daniel", "Claudia", "Gabriel", "Fernanda", "Santiago", "Valeria" };
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dis(0, nombres.size() - 1);
    return nombres[dis(gen)];
}

string RegistroCiudadanos::generarApellidoAleatorio() {
    static vector<string> apellidos = { "Pérez", "González", "Rodríguez", "López", "García", "Martínez",
                                                 "Sánchez", "Ramírez", "Torres", "Flores", "Díaz", "Morales",
                                                 "Vásquez", "Jiménez", "Rojas", "Alvarez", "Castillo", "Vega",
                                                 "Ortiz", "Silva", "Mendoza", "Cortez", "Ruiz", "Reyes" };
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dis(0, apellidos.size() - 1);
    return apellidos[dis(gen)];
}

uint32_t RegistroCiudadanos::generarTelefonoAleatorio() {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<uint32_t> dis(900000000, 999999999);
    return dis(gen);
}

string RegistroCiudadanos::generarEmailAleatorio(const string& nombre, const string& dominio) {
    string email = nombre;
    replace(email.begin(), email.end(), ' ', '.');
    transform(email.begin(), email.end(), email.begin(), ::tolower);
    email += "@" + dominio;
    return email;
}

string RegistroCiudadanos::generarUbicacionAleatoria() {
    static vector<string> ubicaciones = { "Av. Pardo 123", "Calle 50 #456", "Jr. Las Flores 789",
                                                   "Av. Arequipa 321", "Calle Lima 654", "Av. Cusco 987",
                                                   "Calle Trujillo 111", "Jr. Piura 222", "Av. Junín 333", "Calle Tacna 444" };
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dis(0, ubicaciones.size() - 1);
    return ubicaciones[dis(gen)];
}