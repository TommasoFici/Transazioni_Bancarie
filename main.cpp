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

//Classe per rappresentare un conto corrente di una persona random (Me stesso in questo caso)
class ContoCorrente {
public:
    ContoCorrente(const std::string& nome, const std::string& cognome, const std::string& indirizzo, const std::string& numero_telefonico, float conto_corrente)
    : nome(nome), cognome(cognome), indirizzo(indirizzo), numero_telefonico(numero_telefonico), conto_corrente(conto_corrente) {}

    ~ContoCorrente() {
        for(auto& transaction : transactions) {
            delete transaction;
        }
    }

    void MostraAccount() const {
        std::cout << "Nome e Cognome: " << nome << " " << cognome << std::endl;
        std::cout << "Indirizzo: " << indirizzo << std::endl;
        std::cout << "Numero di Telefono : " << numero_telefonico << std::endl;
        std::cout << "Saldo attuale: " << conto_corrente << " €" << std::endl;
    }

    float getConto_corrente() const {
        return conto_corrente;
    }

private:
    std::string nome;
    std::string cognome;
    std::string indirizzo;
    std::string numero_telefonico;
    float conto_corrente;
    std::vector<Transazione*> transactions;

};