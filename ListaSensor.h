#ifndef LISTA_SENSOR_H
#define LISTA_SENSOR_H

#include <iostream>
#include <cstring>

/**
 * @file ListaSensor.h
 * @brief Definición de la clase genérica ListaSensor para manejar listas enlazadas simples.
 * @date Octubre 2025
 *
 * Esta clase plantilla implementa una lista enlazada simple genérica.
 * Se utiliza para almacenar lecturas de sensores (float o int) en las clases derivadas.
 * No utiliza STL y maneja memoria dinámicamente con punteros.
 */

/**
 * @class ListaSensor
 * @brief Implementa una lista enlazada simple genérica.
 * @tparam T Tipo de dato almacenado (ejemplo: int, float).
 */
template <typename T>
class ListaSensor {
public:
    /**
     * @struct Nodo
     * @brief Representa un nodo de la lista enlazada.
     */
    struct Nodo {
        T dato;               ///< Valor almacenado.
        Nodo* siguiente;      ///< Puntero al siguiente nodo.
        Nodo(const T& d) : dato(d), siguiente(nullptr) {}
    };

    /** @brief Constructor por defecto. Inicializa lista vacía. */
    ListaSensor() : cabeza(nullptr), tamano(0) {}

    /** @brief Destructor. Libera todos los nodos. */
    ~ListaSensor() {
        clear();
    }

    /** @brief Constructor de copia. Crea una copia profunda de otra lista. */
    ListaSensor(const ListaSensor& other) : cabeza(nullptr), tamano(0) {
        Nodo* actual = other.cabeza;
        while (actual) {
            push_back(actual->dato);
            actual = actual->siguiente;
        }
    }

    /** @brief Operador de asignación. Crea una copia profunda. */
    ListaSensor& operator=(const ListaSensor& other) {
        if (this == &other) return *this;
        clear();
        Nodo* actual = other.cabeza;
        while (actual) {
            push_back(actual->dato);
            actual = actual->siguiente;
        }
        return *this;
    }

    /**
     * @brief Inserta un nuevo valor al final de la lista.
     * @param valor Valor a insertar.
     */
    void push_back(const T& valor) {
        Nodo* nuevo = new Nodo(valor);
        if (!cabeza) {
            cabeza = nuevo;
        } else {
            Nodo* aux = cabeza;
            while (aux->siguiente) aux = aux->siguiente;
            aux->siguiente = nuevo;
        }
        ++tamano;
        std::cout << "[Log] Insertando Nodo valor: " << valor << std::endl;
    }

    /**
     * @brief Busca un valor en la lista.
     * @param valor Valor a buscar.
     * @return true si el valor existe.
     */
    bool buscar(const T& valor) const {
        Nodo* aux = cabeza;
        while (aux) {
            if (aux->dato == valor) return true;
            aux = aux->siguiente;
        }
        return false;
    }

    /**
     * @brief Elimina la primera ocurrencia de un valor.
     * @param valor Valor a eliminar.
     * @return true si se eliminó exitosamente.
     */
    bool removeValue(const T& valor) {
        Nodo* actual = cabeza;
        Nodo* previo = nullptr;
        while (actual) {
            if (actual->dato == valor) {
                if (previo) previo->siguiente = actual->siguiente;
                else cabeza = actual->siguiente;
                std::cout << "[Log] Nodo eliminado con valor: " << actual->dato << std::endl;
                delete actual;
                --tamano;
                return true;
            }
            previo = actual;
            actual = actual->siguiente;
        }
        return false;
    }

    /**
     * @brief Calcula el promedio de los valores almacenados.
     * @return Promedio (como double).
     */
    double average() const {
        if (!cabeza) return 0.0;
        double suma = 0.0;
        Nodo* aux = cabeza;
        while (aux) {
            suma += static_cast<double>(aux->dato);
            aux = aux->siguiente;
        }
        return suma / static_cast<double>(tamano);
    }

    /**
     * @brief Elimina el nodo con el valor más bajo.
     * @param outMin Referencia donde se guarda el valor eliminado.
     * @return true si se eliminó un valor.
     */
    bool removeLowest(T& outMin) {
        if (!cabeza) return false;
        Nodo* aux = cabeza;
        Nodo* minNode = cabeza;
        Nodo* prev = nullptr;
        Nodo* prevMin = nullptr;

        while (aux) {
            if (aux->dato < minNode->dato) {
                minNode = aux;
                prevMin = prev;
            }
            prev = aux;
            aux = aux->siguiente;
        }

        if (prevMin) prevMin->siguiente = minNode->siguiente;
        else cabeza = minNode->siguiente;

        outMin = minNode->dato;
        delete minNode;
        --tamano;
        return true;
    }

    /** @brief Libera todos los nodos de la lista. */
    void clear() {
        Nodo* aux = cabeza;
        while (aux) {
            Nodo* siguiente = aux->siguiente;
            std::cout << "[Log] Nodo liberado (valor): " << aux->dato << std::endl;
            delete aux;
            aux = siguiente;
        }
        cabeza = nullptr;
        tamano = 0;
    }

    /** @brief Devuelve el número de elementos. */
    size_t size() const { return tamano; }

    /** @brief Imprime la lista (solo para depuración). */
    void imprimir() const {
        Nodo* aux = cabeza;
        std::cout << "[Lista] ";
        while (aux) {
            std::cout << aux->dato;
            if (aux->siguiente) std::cout << " -> ";
            aux = aux->siguiente;
        }
        std::cout << std::endl;
    }

private:
    Nodo* cabeza; ///< Puntero al primer nodo.
    size_t tamano; ///< Número de elementos en la lista.
};

#endif // LISTA_SENSOR_H
