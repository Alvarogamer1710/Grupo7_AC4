#include <iostream>
#include <string>

using namespace std;

// --- ESTRUCTURAS DE DATOS (NODOS) ---

// Nodo para un Contacto individual
struct Contacto {
    string nombre;
    string telefono; // Usado como ID único para duplicados
    int edad;
    Contacto* siguiente; // Puntero al siguiente contacto en la lista

    Contacto(string n, string t, int e) : nombre(n), telefono(t), edad(e), siguiente(nullptr) {}
};

// Nodo para un Usuario (Perfil)
struct Usuario {
    string username;
    Contacto* listaContactos; // Cabecera de la lista de contactos de este usuario
    Usuario* siguiente;       // Puntero al siguiente usuario en el sistema

    Usuario(string u) : username(u), listaContactos(nullptr), siguiente(nullptr) {}
};

// --- CLASE GESTORA DEL SISTEMA ---

class SistemaTinder {
private:
    Usuario* listaUsuariosHead; // Inicio de la lista enlazada de usuarios

    // Función auxiliar para verificar si un contacto ya existe en una lista específica
    bool existeContacto(Contacto* cabecera, string telefono) {
        Contacto* actual = cabecera;
        while (actual != nullptr) {
            if (actual->telefono == telefono) {
                return true;
            }
            actual = actual->siguiente;
        }
        return false;
    }

public:
    SistemaTinder() : listaUsuariosHead(nullptr) {
        inicializarDatos();
    }

    // Destructor para limpiar toda la memoria (Free Store)
    ~SistemaTinder() {
        Usuario* usuarioActual = listaUsuariosHead;
        while (usuarioActual != nullptr) {
            Contacto* contactoActual = usuarioActual->listaContactos;
            while (contactoActual != nullptr) {
                Contacto* tempC = contactoActual;
                contactoActual = contactoActual->siguiente;
                delete tempC; // Liberar contacto
            }
            Usuario* tempU = usuarioActual;
            usuarioActual = usuarioActual->siguiente;
            delete tempU; // Liberar usuario
        }
    }

    // --- GESTIÓN DE USUARIOS ---

    void agregarUsuario(string nombre) {
        Usuario* nuevo = new Usuario(nombre); // Free Store
        if (!listaUsuariosHead) {
            listaUsuariosHead = nuevo;
        } else {
            Usuario* temp = listaUsuariosHead;
            while (temp->siguiente) temp = temp->siguiente;
            temp->siguiente = nuevo;
        }
    }

    Usuario* buscarUsuario(string nombre) {
        Usuario* temp = listaUsuariosHead;
        while (temp) {
            if (temp->username == nombre) return temp;
            temp = temp->siguiente;
        }
        return nullptr;
    }

    void listarUsuarios() {
        cout << "\n--- Perfiles Disponibles ---\n";
        Usuario* temp = listaUsuariosHead;
        while (temp) {
            cout << "- " << temp->username << endl;
            temp = temp->siguiente;
        }
    }

    // --- GESTIÓN DE CONTACTOS ---

    void agregarContacto(Usuario* usuario, string nombre, string telefono, int edad) {
        if (!usuario) return;

        // Detección de duplicados antes de insertar
        if (existeContacto(usuario->listaContactos, telefono)) {
            cout << "Error: El contacto con telefono " << telefono << " ya existe.\n";
            return;
        }

        Contacto* nuevo = new Contacto(nombre, telefono, edad); // Free Store

        // Inserción al final de la lista
        if (!usuario->listaContactos) {
            usuario->listaContactos = nuevo;
        } else {
            Contacto* temp = usuario->listaContactos;
            while (temp->siguiente) temp = temp->siguiente;
            temp->siguiente = nuevo;
        }
        cout << "Contacto agregado exitosamente.\n";
    }

    void eliminarContacto(Usuario* usuario, string telefono) {
        if (!usuario || !usuario->listaContactos) return;

        Contacto* actual = usuario->listaContactos;
        Contacto* anterior = nullptr;

        while (actual) {
            if (actual->telefono == telefono) {
                if (anterior == nullptr) {
                    // Borrar el primero (cabeza)
                    usuario->listaContactos = actual->siguiente;
                } else {
                    // Borrar uno intermedio o final
                    anterior->siguiente = actual->siguiente;
                }
                delete actual; // Liberar memoria
                cout << "Contacto eliminado.\n";
                return;
            }
            anterior = actual;
            actual = actual->siguiente;
        }
        cout << "Contacto no encontrado.\n";
    }

    void buscarContacto(Usuario* usuario, string busqueda) {
        if (!usuario) return;
        Contacto* temp = usuario->listaContactos;
        bool encontrado = false;
        while (temp) {
            if (temp->nombre == busqueda || temp->telefono == busqueda) {
                cout << "Encontrado: " << temp->nombre << " (" << temp->telefono << ") - " << temp->edad << " años.\n";
                encontrado = true;
            }
            temp = temp->siguiente;
        }
        if (!encontrado) cout << "No se encontraron coincidencias.\n";
    }

    void verMisContactos(Usuario* usuario) {
        if (!usuario) return;
        cout << "\n--- Contactos de " << usuario->username << " ---\n";
        Contacto* temp = usuario->listaContactos;
        int count = 0;
        if (!temp) cout << "(Lista vacia)\n";
        while (temp) {
            cout << ++count << ". " << temp->nombre << " | Tel: " << temp->telefono << " | Edad: " << temp->edad << endl;
            temp = temp->siguiente;
        }
    }

    // --- FUNCIONES AVANZADAS: IMPORTAR ---

    void importarContactos(Usuario* destino, string nombreOrigen) {
        if (destino->username == nombreOrigen) {
            cout << "No puedes importarte a ti mismo.\n";
            return;
        }

        Usuario* origen = buscarUsuario(nombreOrigen);
        if (!origen) {
            cout << "Usuario origen no encontrado.\n";
            return;
        }

        cout << "Importando contactos de " << origen->username << " a " << destino->username << "...\n";

        Contacto* tempOrigen = origen->listaContactos;
        int importados = 0;
        int duplicados = 0;

        while (tempOrigen) {
            // Verificar si ya existe en el destino para evitar duplicados
            if (!existeContacto(destino->listaContactos, tempOrigen->telefono)) {
                // Crear una COPIA profunda (nuevo nodo en free store)
                agregarContacto(destino, tempOrigen->nombre, tempOrigen->telefono, tempOrigen->edad);
                importados++;
            } else {
                duplicados++;
            }
            tempOrigen = tempOrigen->siguiente;
        }
        cout << "Proceso finalizado. Importados: " << importados << ". Omitidos (duplicados): " << duplicados << ".\n";
    }

    // --- CARGA INICIAL (HARDCODED) ---
    void inicializarDatos() {
        // Crear 3 perfiles
        agregarUsuario("Juan_Dev");
        agregarUsuario("Maria_CEO");
        agregarUsuario("Carlos_Designer");

        Usuario* u1 = buscarUsuario("Juan_Dev");
        Usuario* u2 = buscarUsuario("Maria_CEO");
        Usuario* u3 = buscarUsuario("Carlos_Designer");

        // Regla: Al menos 5 contactos, cantidades diferentes para cada uno.

        // Usuario 1: 5 contactos
        agregarContacto(u1, "Ana", "600111001", 24);
        agregarContacto(u1, "Bea", "600111002", 25);
        agregarContacto(u1, "Clara", "600111003", 22);
        agregarContacto(u1, "Diana", "600111004", 27);
        agregarContacto(u1, "Elena", "600111005", 21);

        // Usuario 2: 6 contactos
        agregarContacto(u2, "Pedro", "600222001", 30);
        agregarContacto(u2, "Pablo", "600222002", 31);
        agregarContacto(u2, "Luis", "600222003", 29);
        agregarContacto(u2, "Jorge", "600222004", 35);
        agregarContacto(u2, "Marta", "600222005", 28);
        agregarContacto(u2, "Sofia", "600222006", 26);

        // Usuario 3: 7 contactos (Algunos repetidos a propósito para probar lógica de duplicados futura)
        agregarContacto(u3, "Raul", "600333001", 40);
        agregarContacto(u3, "Santi", "600333002", 41);
        agregarContacto(u3, "Toni", "600333003", 38);
        agregarContacto(u3, "Ana", "600111001", 24); // Repetida de Juan (para probar importación)
        agregarContacto(u3, "Victor", "600333005", 33);
        agregarContacto(u3, "Hugo", "600333006", 32);
        agregarContacto(u3, "Ivan", "600333007", 36);

        cout << "--- Sistema Inicializado con Datos de Prueba ---\n";
    }
};

// --- MENÚ Y MAIN ---

int main() {
    SistemaTinder sistema; // Se ejecuta el constructor y la carga inicial
    string inputUsuario;

    while (true) {
        cout << "\n==============================\n";
        cout << "      TINDER CONTACT MANAGER      \n";
        cout << "==============================\n";
        sistema.listarUsuarios();
        cout << "Escribe el nombre de usuario para iniciar sesion (o 'salir'): ";
        cin >> inputUsuario;

        if (inputUsuario == "salir") break;

        Usuario* usuarioActual = sistema.buscarUsuario(inputUsuario);

        if (usuarioActual) {
            int opcion = -1;
            while (opcion != 0) {
                cout << "\n--- Sesion de: " << usuarioActual->username << " ---\n";
                cout << "1. Ver mis contactos\n";
                cout << "2. Agregar nuevo contacto\n";
                cout << "3. Buscar contacto\n";
                cout << "4. Eliminar contacto\n";
                cout << "5. Importar contactos de otro usuario\n";
                cout << "0. Cerrar sesion\n";
                cout << "Opcion: ";
                cin >> opcion;

                if (opcion == 1) {
                    sistema.verMisContactos(usuarioActual);
                }
                else if (opcion == 2) {
                    string n, t; int e;
                    cout << "Nombre: "; cin >> n;
                    cout << "Telefono (ID): "; cin >> t;
                    cout << "Edad: "; cin >> e;
                    sistema.agregarContacto(usuarioActual, n, t, e);
                }
                else if (opcion == 3) {
                    string q;
                    cout << "Buscar (Nombre o Tel): "; cin >> q;
                    sistema.buscarContacto(usuarioActual, q);
                }
                else if (opcion == 4) {
                    string t;
                    cout << "Telefono a eliminar: "; cin >> t;
                    sistema.eliminarContacto(usuarioActual, t);
                }
                else if (opcion == 5) {
                    string origen;
                    cout << "Nombre del usuario del cual importar: "; cin >> origen;
                    sistema.importarContactos(usuarioActual, origen);
                }
            }
        } else {
            cout << "Usuario no encontrado. Intenta de nuevo.\n";
        }
    }

    return 0;
}