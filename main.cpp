#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// Classe base per una transazione
class Transazione {
public:
    Transazione(const std::string& data, float importo, const std::string& descrizione) : data(data), importo(importo), descrizione(descrizione) {}

    virtual ~Transazione() = default;

    virtual std::string getType() const = 0;

    virtual void SalvaNelFile(std::ofstream& outFile) const {
        outFile << getType() << ", " << data << ", " << importo << " €, " << descrizione << "\n";
    }

    static Transazione* CaricaDalFile(const std::string& line);

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

// Classe derivata per rappresentare un entrata
class Bonifico : public Transazione {
public:
    Bonifico(const std::string& data, float importo, const std::string& descrizione) : Transazione(data, importo, descrizione) {}

    std::string getType() const override {
        return "Bonifico";
    }
};

// Classe derivata per rappresentare un'uscita
class Versamento : public Transazione {
public:
    Versamento(const std::string& data, float importo, const std::string& descrizione) : Transazione(data, importo, descrizione) {}

    std::string getType() const override {
        return "Versamento";
    }
};

// Funzione per caricare una transazione da una riga del file
Transazione* Transazione::CaricaDalFile(const std::string &line) {
    std::string tipo, data, descrizione;
    float importo;
    char delimiter = ',';
    size_t pos = 0, prevPos = 0;

    // Estraggo il tipo
    pos = line.find(delimiter, prevPos);
    tipo = line.substr(prevPos, pos-prevPos);
    prevPos = pos + 1;

    // Estraggo la data
    pos = line.find(delimiter, prevPos);
    data = line.substr(prevPos, pos - prevPos);
    prevPos = pos + 1;

    // Estraggo l'importo
    pos = line.find(delimiter, prevPos);
    importo = std::stof(line.substr(prevPos, pos - prevPos));
    prevPos = pos + 1;

    // Estraggo la descrizione
    descrizione = line.substr(prevPos);

    if (tipo == "Bonifico") {
        return new Bonifico (data, importo, descrizione);
    } else if (tipo == "Versamento") {
        return new Versamento (data, importo, descrizione);
    } else{
        return nullptr;
    }
}

// Funzione per salvare tutte le transazioni su file
void SalvaTransazioniFile(const std::vector<Transazione*>& transactions, const std::string& nomefile) {
    std::ofstream outFile(nomefile);
    if (outFile.is_open()) {
        for(const auto& transaction : transactions) {
            transaction->SalvaNelFile(outFile);
        }
        outFile.close();
    } else {
        std::cerr << "Impossibile aprire il file per la scrittura" << std::endl;
    }
}

// Funzione per caricare tutte le transazioni da un file
std::vector<Transazione*> CaricaDalFile(const std::string& nomefile) {
    std::vector<Transazione*> transactions;
    std::ifstream inFile(nomefile);
    std::string line;

    if (inFile.is_open()) {
        while(std::getline(inFile, line)) {
            Transazione* transaction = Transazione::CaricaDalFile(line);
            if (transaction) {
                transactions.push_back(transaction);
            }
        }
        inFile.close();
    } else{
        std::cerr << "Impossibile aprire il file per la lettura" << std::endl;
    }
    return transactions;
}

// Classe per rappresentare un conto corrente di una persona random (Me stesso in questo caso)
class ContoCorrente {
public:
    ContoCorrente(const std::string& nome, const std::string& cognome, const std::string& indirizzo, const std::string& numero_telefonico, float conto_corrente)
    : nome(nome), cognome(cognome), indirizzo(indirizzo), numero_telefonico(numero_telefonico), conto_corrente(conto_corrente) {}

    ~ContoCorrente() {
        for(auto& transaction : transactions) {
            delete transaction;
        }
    }

    void addBonifico(const std::string& data, float importo, const std::string& descrizione) {
        transactions.push_back(new Bonifico(data, importo,descrizione));
        conto_corrente += importo;
    }

    void addVersamento(const std::string& data, float importo, const std::string& descrizione) {
        transactions.push_back(new Versamento(data, importo, descrizione));
        conto_corrente -= importo;
    }

    void MostraAccount() const {
        std::cout << "Nome e Cognome: " << nome << " " << cognome << std::endl;
        std::cout << "Indirizzo: " << indirizzo << std::endl;
        std::cout << "Numero di Telefono : " << numero_telefonico << std::endl;
        std::cout << "Saldo attuale: " << conto_corrente << " €" << std::endl;
    }

    void MostraTransazioni() const {
        std::cout << "\nTransazioni: " << std::endl;
        for (const auto& transaction : transactions) {
            transaction->display();
        }
    }

    void SalvaAccountNelFile(const std::string& nomefile) const {
        std::ofstream outFile(nomefile);
        if(outFile.is_open()) {
            // Salvo le informazioni personali e il saldo
            outFile << "Proprietario: " << nome << " " << cognome << "\nIndirizzo: " << indirizzo << "\nNumero di Telefono: " << numero_telefonico << "\nSaldo Attuale: " << conto_corrente << " €\n\nUltimi movimenti effettuati: \n";
            // Salvo le transazioni
            for(const auto& transaction : transactions) {
                transaction->SalvaNelFile(outFile);
            }
            outFile.close();
        } else {
            std::cerr << "Impossibile aprire il file per la scrittura." << std::endl;
        }
    }

    void CaricaAccountDalFile(const std::string& nomefile) {
        std::ifstream inFile(nomefile);
        std::string line;

        if(inFile.is_open()) {

            // Carico le informazioni personali e il saldo
            // Estraggo il nome
            std::getline(inFile, line);
            nome = line.substr(line.find(':') + 2);

            // Estraggo il cognome
            std::getline(inFile, line);
            cognome = line.substr(line.find(':') + 2);

            // Estraggo l'indirizzo
            std::getline(inFile, line);
            indirizzo = line.substr(line.find(':') + 2);

            // Estraggo il numero di telefono
            std::getline(inFile, line);
            numero_telefonico = line.substr(line.find(':') + 2);

            // Estraggo il saldo
            std::getline(inFile, line);
            conto_corrente = std::stof(line.substr(line.find(':') + 2));

            //Salto la riga vuota e il titolo successivo per le transazioni
            std::getline(inFile, line);
            std::getline(inFile, line);

            // Carico le transazioni
            while (std::getline(inFile, line)) {
                Transazione* transaction = Transazione::CaricaDalFile(line);
                if (transaction) {
                    transactions.push_back(transaction);

                    // Aggiorno il saldo in base al tipo di transazione
                    if (transaction->getType() == "Bonifico") {
                        conto_corrente += transaction->getImporto();
                    } else if (transaction->getType() == "Versamento") {
                        conto_corrente -= transaction->getImporto();
                    }
                }
            }
            inFile.close();
        } else {
            std::cerr << "Impossibile aprire il file per la lettura." << std::endl;
        }
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

int main() {
     // Crea un conto corrente con informazioni iniziali
     ContoCorrente account("Tommaso", "Fici", "Via Ciro Menotti 7; Firenze", "3476601816", 500.0);

     // Aggiungo alcune transazioni
     account.addBonifico("15-03-2024", 500.0, "Lavoro part-time");
     account.addVersamento("07-08-2024", 181.55, "Cena Ristorante Armanda");
     account.addVersamento("22-08-2024", 50.59, "Acquisto Amazon");

     // Mostra le informazioni del conto e le transazioni
     account.MostraAccount();
     account.MostraTransazioni();

    // Salvo il conto e le transazioni su file
     account.SalvaAccountNelFile("conto_corrente.txt");

     // Creo un nuovo conto per caricare i dati dal file
     ContoCorrente loadedAccount("", "", "", "",0.0);
     loadedAccount.CaricaAccountDalFile("conto_corrente.txt");

     // Mostro le informazioni del conto caricato e le transazioni
     std::cout << "\nDati caricati dal file:\n";
     loadedAccount.MostraAccount();
     loadedAccount.MostraTransazioni();

    return 0;
}