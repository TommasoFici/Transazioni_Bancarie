#include <iostream>
#include <string>
#include <vector>


using namespace std;

//Classe base per una transazione
class Transazione {
public:
    Transazione(const std::string& data, float importo, const std::string& descrizione)
    : data(data), importo(importo), descrizione(descrizione) {}

    virtual ~Transazione() = default;

    virtual std::string getType() const = 0;

    virtual void display() const {
        cout << "Data: " << data << " | Importo: " << importo << " € | Descrizione: " << descrizione << std::endl;
    }

    float getImporto() const {
        return importo;
    }

protected:
    std::string data;
    float importo;
    std::string descrizione;

};

//Classe derivata per rappresentare un entrata
class Bonifico : public Transazione {
public:
    Bonifico(const std::string& data, float importo, const std::string& descrizione)
    : Transazione(data, importo, descrizione) {}

    std::string getType() const override {
        return "Bonifico";
    }
};

//Classe derivata per rappresentare un'uscita
class Versamento : public Transazione {
public:
    Versamento(const std::string& data, float importo, const std::string& descrizione)
    : Transazione(data, importo, descrizione) {}

    std::string getType() const override {
        return "Versamento";
    }
};