#include <iostream>
#include <cstring>
#include <limits>
#include "ListaSensor.h"
// Profe no tengo arduino una disculpita

/**
 * @file main.cpp
 * @brief Sistema de gestión polimórfica de sensores (IoT).
 * @details Simula sensores de temperatura y presión que almacenan lecturas
 *          en listas genéricas y se gestionan mediante polimorfismo.
 */

/**
 * @class SensorBase
 * @brief Clase base abstracta para todos los sensores
 */
class SensorBase {
public:
    explicit SensorBase(const char* nombre_) {
        std::memset(nombre, 0, 50);
        std::strncpy(nombre, nombre_, 49);
    }

    virtual ~SensorBase() {} ///< Destructor virtual.

    virtual void procesarLectura() = 0; ///< Método puro a implementar.
    virtual void imprimirInfo() const = 0; ///< Método puro a implementar.

    const char* getNombre() const { return nombre; }

protected:
    char nombre[50]; ///< Identificador del sensor.
};

/**
 * @class SensorTemperatura
 * @brief Representa un sensor que almacena lecturas tipo float.
 */
class SensorTemperatura : public SensorBase {
public:
    explicit SensorTemperatura(const char* nombre_) : SensorBase(nombre_) {}
    ~SensorTemperatura() override {
        std::cout << "  [Destructor Sensor " << nombre << "] Liberando Lista Interna..." << std::endl;
        historial.clear();
    }

    void registrarLectura(float valor) {
        historial.push_back(valor);
    }

    void procesarLectura() override {
        float minVal = 0.0f;
        bool removed = historial.removeLowest(minVal);
        if (removed)
            std::cout << "[Sensor Temp] Lectura más baja (" << minVal << ") eliminada.\n";
        double promedio = historial.average();
        std::cout << "[Sensor Temp] Promedio actual: " << promedio << std::endl;
    }

    void imprimirInfo() const override {
        std::cout << "[Temperatura] ID: " << nombre << " | #lecturas: " << historial.size() << std::endl;
        historial.imprimir();
    }

private:
    ListaSensor<float> historial;
};

/**
 * @class SensorPresion
 * @brief Representa un sensor que almacena lecturas tipo int.
 */
class SensorPresion : public SensorBase {
public:
    explicit SensorPresion(const char* nombre_) : SensorBase(nombre_) {}
    ~SensorPresion() override {
        std::cout << "  [Destructor Sensor " << nombre << "] Liberando Lista Interna..." << std::endl;
        historial.clear();
    }

    void registrarLectura(int valor) {
        historial.push_back(valor);
    }

    void procesarLectura() override {
        double promedio = historial.average();
        std::cout << "[Sensor Presion] Promedio de lecturas: " << promedio << std::endl;
    }

    void imprimirInfo() const override {
        std::cout << "[Presion] ID: " << nombre << " | #lecturas: " << historial.size() << std::endl;
        historial.imprimir();
    }

private:
    ListaSensor<int> historial;
};

/**
 * @class ListaGeneral
 * @brief Lista enlazada que almacena punteros a SensorBase (polimorfismo).
 */
class ListaGeneral {
public:
    struct NodoG {
        SensorBase* sensor;
        NodoG* siguiente;
        explicit NodoG(SensorBase* s) : sensor(s), siguiente(nullptr) {}
    };

    ListaGeneral() : cabeza(nullptr) {}
    ~ListaGeneral() { clear(); }

    void insertar(SensorBase* s) {
        NodoG* nuevo = new NodoG(s);
        if (!cabeza) cabeza = nuevo;
        else {
            NodoG* aux = cabeza;
            while (aux->siguiente) aux = aux->siguiente;
            aux->siguiente = nuevo;
        }
    }

    SensorBase* buscarPorID(const char* id) const {
        NodoG* aux = cabeza;
        while (aux) {
            if (std::strcmp(aux->sensor->getNombre(), id) == 0)
                return aux->sensor;
            aux = aux->siguiente;
        }
        return nullptr;
    }

    void procesarTodos() {
        NodoG* aux = cabeza;
        while (aux) {
            aux->sensor->procesarLectura();
            aux = aux->siguiente;
        }
    }

    void imprimirTodos() const {
        NodoG* aux = cabeza;
        while (aux) {
            aux->sensor->imprimirInfo();
            aux = aux->siguiente;
        }
    }

    void clear() {
        NodoG* aux = cabeza;
        while (aux) {
            NodoG* sig = aux->siguiente;
            delete aux->sensor;
            delete aux;
            aux = sig;
        }
        cabeza = nullptr;
    }

private:
    NodoG* cabeza;
};

/**
 * @brief Lee una cadena de texto como nombre del sensor.
 */
void leerNombre(char* buffer, size_t size) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.getline(buffer, static_cast<std::streamsize>(size));
}

/**
 * @brief Función principal. Simula el uso del sistema.
 */
int main() {
    ListaGeneral gestion;
    bool running = true;

    while (running) {
        std::cout << "\n1. Crear Sensor Temperatura\n"
                  << "2. Crear Sensor Presion\n"
                  << "3. Registrar Lectura\n"
                  << "4. Procesar Lecturas\n"
                  << "5. Mostrar Sensores\n"
                  << "6. Salir\n"
                  << "Opcion: ";

        int opcion;
        std::cin >> opcion;

        if (opcion == 1) {
            char id[50];
            std::cout << "ID Sensor Temp: ";
            leerNombre(id, 50);
            gestion.insertar(new SensorTemperatura(id));
        } else if (opcion == 2) {
            char id[50];
            std::cout << "ID Sensor Presion: ";
            leerNombre(id, 50);
            gestion.insertar(new SensorPresion(id));
        } else if (opcion == 3) {
            char id[50];
            std::cout << "ID del sensor: ";
            leerNombre(id, 50);
            SensorBase* base = gestion.buscarPorID(id);
            if (!base) {
                std::cout << "No encontrado.\n";
                continue;
            }
            if (auto* sT = dynamic_cast<SensorTemperatura*>(base)) {
                float val;
                std::cout << "Valor float: ";
                std::cin >> val;
                sT->registrarLectura(val);
            } else if (auto* sP = dynamic_cast<SensorPresion*>(base)) {
                int val;
                std::cout << "Valor int: ";
                std::cin >> val;
                sP->registrarLectura(val);
            }
        } else if (opcion == 4) {
            gestion.procesarTodos();
        } else if (opcion == 5) {
            gestion.imprimirTodos();
        } else if (opcion == 6) {
            gestion.clear();
            running = false;
            std::cout << "Sistema cerrado.\n";
        }
    }
    return 0;
}
