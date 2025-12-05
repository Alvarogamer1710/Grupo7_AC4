
#include <string>
class ListaContactos {
    protected:
    class Nodo {
        std::string nombre;
        int edad;
        Nodo* nexo;
        Nodo* prev;
    public:
        Nodo(){
            this->nombre = "";
            this->edad = 0;
            this->nexo = nullptr;
            this->prev = nullptr;
        };        Nodo(int edad, std::string nombre){
            this->nombre = "";
            this->edad = 0;
            this->nexo = nullptr;
            this->prev = nullptr;
        };        Nodo(int dato, Nodo* nexo, Nodo* prev){
            this->nombre = "";
            this->edad = 0;
            this->nexo = nexo;
            this->prev = prev;

        };    public:        void cambiarLast(Nodo* prev) {
            this->prev = prev;
        }

        void cambiarNexo(Nodo* nexo) {
            this->nexo = nexo;
        }
        Nodo* getNext() {
            return this->nexo;
        }
        Nodo* getPrev() {
            return this->prev;
        }

        std::string getName() {
            return this->nombre;
        }
        int getEdad() {
            return this->edad;
        }

        void setEdad(int edad) {
            this->edad = edad;
        }

        void setName(std::string nombre) {
            this->nombre = nombre;
        }
    };
private:
    Nodo* first;
    Nodo* last;

public:
    ListaContactos() {
        first = nullptr;
        last = nullptr;

    }


};
