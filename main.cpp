#include <iostream>
#include <string>

using namespace std;

// ==========================================
// TU CLASE (ADAPTADA Y COMPLETADA)
// ==========================================

class ListaContactos {
protected:
    class Nodo {
    private:
        std::string nombre;
        std::string telefono; // AGREGADO: Necesario como ID único
        int edad;
        Nodo* nexo;
        Nodo* prev;

    public:
        // Constructor por defecto
        Nodo(){
            this->nombre = "";
            this->telefono = "";
            this->edad = 0;
            this->nexo = nullptr;
            this->prev = nullptr;
        }

        // Constructor corregido: Ahora asigna los valores que recibe
        Nodo(int edad, std::string nombre, std::string telefono){
            this->nombre = nombre;
            this->telefono = telefono;
            this->edad = edad;
            this->nexo = nullptr;
            this->prev = nullptr;
        }

        // Getters y Setters
        void cambiarLast(Nodo* prev) { this->prev = prev; }
        void cambiarNexo(Nodo* nexo) { this->nexo = nexo; }
        Nodo* getNext() { return this->nexo; }
        Nodo* getPrev() { return this->prev; }

        std::string getName() { return this->nombre; }
        std::string getPhone() { return this->telefono; } // Getter nuevo
        int getEdad() { return this->edad; }

        void setEdad(int edad) { this->edad = edad; }
        void setName(std::string nombre) { this->nombre = nombre; }
    };

private:
    Nodo* first;
    Nodo* last;

public:
    ListaContactos() {
        first = nullptr;
        last = nullptr;
    }

    // Destructor para limpiar memoria (Free Store)
    ~ListaContactos() {
        Nodo* actual = first;
        while (actual != nullptr) {
            Nodo* temp = actual;
            actual = actual->getNext();
            delete temp;
        }
    }

    // --- MÉTODOS DE GESTIÓN (AÑADIDOS) ---

    // Verificar si existe un teléfono (para duplicados)
    bool existe(string telefono) {
        Nodo* temp = first;
        while (temp != nullptr) {
            if (temp->getPhone() == telefono) return true;
            temp = temp->getNext();
        }
        return false;
    }

    void agregar(string nombre, string telefono, int edad) {
        if (existe(telefono)) {
            cout << "   -> Error: El contacto " << telefono << " ya existe.\n";
            return;
        }

        Nodo* nuevo = new Nodo(edad, nombre, telefono); // Free Store

        if (first == nullptr) {
            first = nuevo;
            last = nuevo;
        } else {
            // Inserción al final (Lista Doblemente Enlazada)
            last->cambiarNexo(nuevo);
            nuevo->cambiarLast(last);
            last = nuevo;
        }
    }

    void eliminar(string telefono) {
        if (!first) return;

        Nodo* actual = first;
        while (actual != nullptr) {
            if (actual->getPhone() == telefono) {
                // Caso 1: Es el único nodo
                if (actual == first && actual == last) {
                    first = nullptr;
                    last = nullptr;
                }
                // Caso 2: Es el primero
                else if (actual == first) {
                    first = actual->getNext();
                    first->cambiarLast(nullptr);
                }
                // Caso 3: Es el último
                else if (actual == last) {
                    last = actual->getPrev();
                    last->cambiarNexo(nullptr);
                }
                // Caso 4: Está en medio
                else {
                    actual->getPrev()->cambiarNexo(actual->getNext());
                    actual->getNext()->cambiarLast(actual->getPrev());
                }

                delete actual;
                cout << "   -> Contacto eliminado.\n";
                return;
            }
            actual = actual->getNext();
        }
        cout << "   -> Contacto no encontrado.\n";
    }

    void buscar(string termino) {
        Nodo* temp = first;
        bool found = false;
        while (temp != nullptr) {
            if (temp->getName() == termino || temp->getPhone() == termino) {
                cout << "   [ENCONTRADO] " << temp->getName() << " | Tel: " << temp->getPhone() << " | Edad: " << temp->getEdad() << endl;
                found = true;
            }
            temp = temp->getNext();
        }
        if (!found) cout << "   -> No hay coincidencias.\n";
    }

    void imprimirTodo() {
        Nodo* temp = first;
        if (!temp) cout << "   (Lista vacia)\n";
        int i = 1;
        while (temp != nullptr) {
            cout << "   " << i++ << ". " << temp->getName() << " (" << temp->getPhone() << ") - " << temp->getEdad() << " anios\n";
            temp = temp->getNext();
        }
    }

    // Método para importar DE otra lista A esta lista
    // Recibe un puntero a otra lista para leer sus datos
    void importarDesde(ListaContactos* otraLista) {
        if (!otraLista || !otraLista->first) return;

        // Nota: Como 'Nodo' es protected, necesitamos un método público en la otra lista
        // para extraer datos, o declaramos amistad. Para simplificar sin romper encapsulamiento,
        // haré una iteración usando lógica de "copia manual" simulada accediendo a través de getters.

        // HACK: Como estamos dentro de la clase ListaContactos, podemos acceder a los miembros privados/protegidos
        // de *otra instancia* de la misma clase en C++.

        Nodo* nodoFuente = otraLista->first;
        int cont = 0;

        while (nodoFuente != nullptr) {
            // Solo insertamos si NO existe en ESTA lista (la función agregar ya chequea duplicados,
            // pero lo haremos explícito para contar cuántos se importan).
            if (!existe(nodoFuente->getPhone())) {
                agregar(nodoFuente->getName(), nodoFuente->getPhone(), nodoFuente->getEdad());
                cont++;
            }
            nodoFuente = nodoFuente->getNext();
        }
        cout << "   -> Importacion completada. " << cont << " contactos nuevos agregados.\n";
    }
};

// ==========================================
// ESTRUCTURA DE USUARIOS (Linked List de Usuarios)
// ==========================================

// Cada nodo de Usuario tiene un nombre y SU PROPIA 'ListaContactos'
struct NodoUsuario {
    string username;
    ListaContactos* agenda; // Usamos tu clase aquí
    NodoUsuario* siguiente;

    NodoUsuario(string u) {
        username = u;
        agenda = new ListaContactos(); // Instancia de tu clase en el Heap
        siguiente = nullptr;
    }

    ~NodoUsuario() {
        delete agenda; // Se llama al destructor de tu clase ListaContactos
    }
};

// ==========================================
// GESTOR DEL SISTEMA
// ==========================================

class SistemaTinder {
private:
    NodoUsuario* headUsuarios;

public:
    SistemaTinder() : headUsuarios(nullptr) {
        cargarDatosIniciales();
    }

    ~SistemaTinder() {
        NodoUsuario* actual = headUsuarios;
        while (actual != nullptr) {
            NodoUsuario* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }

    void crearUsuario(string nombre) {
        NodoUsuario* nuevo = new NodoUsuario(nombre);
        if (!headUsuarios) {
            headUsuarios = nuevo;
        } else {
            NodoUsuario* temp = headUsuarios;
            while (temp->siguiente) temp = temp->siguiente;
            temp->siguiente = nuevo;
        }
    }

    NodoUsuario* buscarUsuario(string nombre) {
        NodoUsuario* temp = headUsuarios;
        while (temp) {
            if (temp->username == nombre) return temp;
            temp = temp->siguiente;
        }
        return nullptr;
    }

    void listarUsuarios() {
        cout << "\n--- Perfiles Disponibles ---\n";
        NodoUsuario* temp = headUsuarios;
        while (temp) {
            cout << "- " << temp->username << endl;
            temp = temp->siguiente;
        }
    }

    // Datos hardcodeados (requerimiento del enunciado)
    void cargarDatosIniciales() {
        crearUsuario("Juan");
        crearUsuario("Ana");
        crearUsuario("Luis");

        NodoUsuario* u1 = buscarUsuario("Juan");
        NodoUsuario* u2 = buscarUsuario("Ana");
        NodoUsuario* u3 = buscarUsuario("Luis");

        // Usamos el método agregar de TU clase ListaContactos
        // Juan (5 contactos)
        u1->agenda->agregar("C1", "1001", 20);
        u1->agenda->agregar("C2", "1002", 21);
        u1->agenda->agregar("C3", "1003", 22);
        u1->agenda->agregar("C4", "1004", 23);
        u1->agenda->agregar("C5", "1005", 24);

        // Ana (6 contactos)
        u2->agenda->agregar("D1", "2001", 30);
        u2->agenda->agregar("D2", "2002", 31);
        u2->agenda->agregar("D3", "2003", 32);
        u2->agenda->agregar("D4", "2004", 33);
        u2->agenda->agregar("D5", "2005", 34);
        u2->agenda->agregar("D6", "2006", 35);

        // Luis (7 contactos, incluye repetidos para probar lógica)
        u3->agenda->agregar("E1", "3001", 40);
        u3->agenda->agregar("E2", "3002", 41);
        u3->agenda->agregar("E3", "3003", 42);
        u3->agenda->agregar("E4", "3004", 43);
        u3->agenda->agregar("E5", "3005", 44);
        u3->agenda->agregar("C1", "1001", 20); // Repetido de Juan
        u3->agenda->agregar("E7", "3007", 46);
    }
};

// ==========================================
// MAIN
// ==========================================

int main() {
    SistemaTinder app;
    string input;

    while (true) {
        cout << "\n==============================\n";
        cout << "      TINDER MANAGEMENT       \n";
        cout << "==============================\n";
        app.listarUsuarios();
        cout << "Usuario (o 'salir'): ";
        cin >> input;

        if (input == "salir") break;

        NodoUsuario* usuario = app.buscarUsuario(input);

        if (usuario) {
            int op = -1;
            while (op != 0) {
                cout << "\n--- Perfil: " << usuario->username << " ---\n";
                cout << "1. Ver contactos\n";
                cout << "2. Agregar contacto\n";
                cout << "3. Eliminar contacto\n";
                cout << "4. Buscar contacto\n";
                cout << "5. Importar contactos\n";
                cout << "0. Cerrar sesion\n";
                cout << "Opcion: ";
                cin >> op;

                if (op == 1) {
                    usuario->agenda->imprimirTodo();
                }
                else if (op == 2) {
                    string n, t; int e;
                    cout << "Nombre: "; cin >> n;
                    cout << "Telefono: "; cin >> t;
                    cout << "Edad: "; cin >> e;
                    usuario->agenda->agregar(n, t, e);
                }
                else if (op == 3) {
                    string t;
                    cout << "Telefono a borrar: "; cin >> t;
                    usuario->agenda->eliminar(t);
                }
                else if (op == 4) {
                    string q;
                    cout << "Buscar (nom/tel): "; cin >> q;
                    usuario->agenda->buscar(q);
                }
                else if (op == 5) {
                    string origenName;
                    cout << "Nombre del usuario origen: "; cin >> origenName;
                    if (origenName == usuario->username) {
                        cout << "No puedes importarte a ti mismo.\n";
                    } else {
                        NodoUsuario* origen = app.buscarUsuario(origenName);
                        if (origen) {
                            cout << "Importando de " << origenName << "...\n";
                            // Pasamos la lista del otro usuario a la funcion de importacion
                            usuario->agenda->importarDesde(origen->agenda);
                        } else {
                            cout << "Usuario origen no encontrado.\n";
                        }
                    }
                }
            }
        } else {
            cout << "Usuario no encontrado.\n";
        }
    }
    return 0;
}