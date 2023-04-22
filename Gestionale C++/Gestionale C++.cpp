
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <conio.h>

using namespace std;
struct prodotto
{
    string dolce;
    string ingrediente[100];
    string procedimento[100];
    int indice = 0;
    int quantità[100];
};
#pragma region Funzioni

//
static int Ricerca(string nome, string filePath)
{
    int posizione = -1;
    ifstream file(filePath);
    string line;
    int riga = 0;
    while (getline(file, line))
    {
        riga++;
        int pos = line.find(';');
        string name = line.substr(0, pos);
        if (name == nome)
        {
            posizione = riga;
            break;
        }
    }
    file.close();
    return posizione;
}

static void AggiuntaMenu(string dolceOrdinato, int& dim, fstream& ListaDolci, fstream& ListaDolciTemp, fstream& reader, string path, string pathTemp)
{
    string line;
    int N, scelta;
    prodotto p;
    if (dim < 100)
    {
        //SALVATAGGIO DI DOLCE ED INGREDIENTI SU FILE TEMPORANEO
        ListaDolciTemp.open(pathTemp, ios::out | ios::app);
        p.dolce = dolceOrdinato;
        cout << "Inserire il numero di ingredienti necessari: ";
        cin >> N;
        ListaDolciTemp << p.dolce << ";";
        for (int i = 1; i <= N; i++) {
            cout << "Inserire l'ingrediente " << i << ": ";
            cin >> p.ingrediente[i - 1];
            cout << "Inserire la quantita di quell'ingrediente: ";
            cin >> p.quantità[i - 1];
            cout << "Seleziona l'unita di misura del " << i << " ingrediente (0 - no unita misura / 1 - g / 2 - ml): ";
            cin >> scelta;
            switch (scelta)
            {
            default:
                cout << "Input non valido!";
                break;
            case 0:
                ListaDolciTemp << p.ingrediente[i - 1] << " " << p.quantità[i - 1] << ";";
                break;
            case 1:
                ListaDolciTemp << p.ingrediente[i - 1] << " " << p.quantità[i - 1] << " g;";
                break;
            case 2:
                ListaDolciTemp << p.ingrediente[i - 1] << " " << p.quantità[i - 1] << " ml;";
                break;
            }
        }
        ListaDolciTemp << endl;
        ListaDolciTemp.close();

        //ESTRAZIONE DEL DOLCE E INGREDIENTI SU FILE HTML
        ListaDolci.open(path, ios::out | ios::app);
        reader.open(pathTemp, ios::in);
        while (getline(reader, line))
        {
            string split = line.substr(0, line.find(";"));
            ListaDolci << "<li> " << split << " </li>" << endl;
        }
        reader.close();
        ListaDolci.close();

        //SALVATAGGIO DEI PROCEDIMENTI SU FILE TEMPORANEO
        ListaDolciTemp.open(pathTemp, ios::out | ios::app);
        cout << "Inserire il numero di procedimenti necessari: ";
        cin >> N;
        for (int i = 1; i <= N; i++)
        {
            cout << "Inserire il " << i << " passaggio: ";
            cin >> p.procedimento[i - 1];
            ListaDolciTemp << i << ". " << p.procedimento[i - 1] << ";";
        }
        ListaDolciTemp << endl;
        ListaDolciTemp.close();
        dim++;
    }
    else {
        cout << "Errore! Limite massimo raggiunto" << endl;
    }
}
static void htmlI(fstream& ListaDolci, string path) {
    remove(path.c_str());
    ListaDolci.open(path, ios::out | ios::app);
    ListaDolci << "<html>" << endl << "<head>" << endl << "<title> Gestionale Pasticceria </title>" << endl << "</head>" << endl << "<body>" << endl << "<h1> Menu </h1>" << endl << "<ul>" << endl;
    ListaDolci.close();
}
static void htmlF(fstream& ListaDolci, string path) {
    ListaDolci.open(path, ios::out | ios::app);
    ListaDolci <<"</ul>" << endl << "</body>" << endl << "</html>" << endl;
    ListaDolci.close();
}

static void RicavaMenu(fstream& reader, string pathTemp) {
    string line, sep = ";";
    reader.open(pathTemp, ios::in);
    while (getline(reader, line))
    {
        string split = line.substr(0, line.find(sep));
        cout << "- " << split << endl;
    }
    reader.close();
}
//

static void StampaProcedimento(string dolceOrdinato, fstream& ricetteOrdini, string pathTemp, string pathOrdine)
{
    string line, sep = ";", ingpath = "Ingredienti.csv", ricpath = "RicettarioGenerale.csv";
    fstream reader, readering;
    cout << "Ingredienti:" << endl << endl;
    readering.open(pathTemp, ios::in);
    while (getline(readering, line))
    {
        if (line.find(dolceOrdinato) != string::npos)
        {
            int inizio = line.find(";"); // Trova il primo carattere ";" nella riga
            while (inizio != string::npos)
            {
                int fine = line.find(";", inizio + 1); // Trova il prossimo carattere ";" nella riga
                string sottostringa = line.substr(inizio + 1, fine - inizio - 1); // Estrae la sottostringa tra i due caratteri ";"
                cout << sottostringa << endl;
                inizio = fine;
            }
        }
    }
    readering.close();
    _getch();
    system("CLS");
    readering.close();
    cout << "Procedimento:" << endl << endl;

    ricetteOrdini.open(pathOrdine, ios::out | ios::app);
    reader.open(pathTemp, ios::in);
    while (getline(reader, line))
    {
        if (line.find(dolceOrdinato) != string::npos)
        {
            while (getline(reader, line))
            {
                int inizio = line.find("1."); // Trova il primo carattere ";" nella riga
                while (inizio != string::npos)
                {
                    int fine = line.find(";", inizio + 1); // Trova il prossimo carattere ";" nella riga
                    string sottostringa = line.substr(inizio + 1, fine - inizio - 1); // Estrae la sottostringa tra i due caratteri ";"
                    ricetteOrdini << sottostringa << endl;
                    inizio = fine;
                }
            }
        }
    }
    reader.close();
    ricetteOrdini.close();
    _getch();
    reader.close();
}
static void EliminaDolce(string dolceSelezionato, fstream& output, string nome_file_mod)
{
    string nome_file = "ListaDolci.csv", line, nome_file2 = "RicettarioGenerale.csv", ricettarioMom = "RicettarioMomentaneo.csv";
    fstream input, input2, output2;
    input.open(nome_file, ios::in);
    while (getline(input, line))
    {
        string split = line.substr(0, line.find(";"));
        if (split != dolceSelezionato)
        {
            output << line << endl;
        }
    }
    input.close();
    output2.open(ricettarioMom, ios::out);
    input2.open(nome_file2, ios::in);
    while (getline(input2, line))
    {
        string split = line.substr(0, line.find(";"));
        if (split != dolceSelezionato)
        {
            output2 << line << endl;
        }
    }
    input2.close();
    output2.close();
}
static void ModificaDolce(string dolceSelezionato, string nuovoDolce, fstream& output, string nome_file_mod)
{
    string nome_file = "ListaDolci.csv", line, rcg = "RicettarioGenerale.csv", nome_file_mod2 = "RicettarioMomentaneo.csv";
    fstream input, input2, output2;
    int indice = 1;
    prodotto p;
    input.open(nome_file, ios::in);
    while (getline(input, line))
    {
        string split = line.substr(0, line.find(";"));
        if (split != dolceSelezionato)
        {
            output << line << endl;
        }
        else {
            output << nuovoDolce;
            string split2 = line.substr(split.length());
            output << split2 << endl;
        }
    }
    input.close();

    output2.open(nome_file_mod2, ios::out);
    input2.open(rcg, ios::in);
    while (getline(input2, line))
    {
        string split = line.substr(0, line.find(";"));
        if (split != dolceSelezionato)
        {
            output2 << line << endl;
        }
        else {
            output2 << nuovoDolce;
            string split2 = line.substr(split.length());
            output2 << split2 << endl;
        }
    }
    input2.close();
    output2.close();
}
static void Sostituzione(string appoggio, string vecchio) {
    remove(vecchio.c_str());
    if (rename(appoggio.c_str(), vecchio.c_str()) == 0)
        cout << "File renamed successfully" << endl;
    else
        perror("Error renaming file");
}
#pragma endregion

int main()
{
    int scelta, dim = 0, r;
    bool c = false;
    char uscita;
    string dolce;
    string path = "ListaDolci.html", pathTemp = "ListaDolciTemp.csv", pathOrdine = "RicetteOrdine.csv";
    fstream ListaDolci, ListaDolciTemp, reader, ricetteOrdini;
    do {
        system("CLS");
        cout << "1 - Aggiunta dolce\n2 - Ordinazione\n3 - Elimina dolce\n4 - Modifica dolce\n5 - Visualizza Dispensa\n0 - Uscita\n" << endl;
        cout << "Inserire la scelta: ";
        cin >> scelta;
        switch (scelta) {
        default:
            cout << "Opzione non valida!" << endl;
            break;
        case 0:
            c = true;
            break;
        case 1:
            system("CLS");
            cout << "Inserire il dolce: ";
            cin >> dolce;
            dolce[0] = toupper(dolce[0]);
            htmlI(ListaDolci, path);
            AggiuntaMenu(dolce, dim, ListaDolci, ListaDolciTemp, reader, path, pathTemp);
            htmlF(ListaDolci, path);
            break;
        case 2:
            remove("RicetteOrdine.csv");
            do
            {
                system("CLS");
                RicavaMenu(reader, pathTemp);
                cout << "Inserire il dolce che si vuole ordinare: ";
                cin >> dolce;
                dolce[0] = toupper(dolce[0]);
                r = Ricerca(dolce, pathTemp);
                if (r == -1) {
                    cout << "Dolce non trovato!" << endl;
                    _getch();
                }
                else {
                    system("CLS");
                    StampaProcedimento(dolce, ricetteOrdini, pathTemp, pathOrdine);
                }
                system("CLS");
                cout << "Inserire un altro dolce? (Y/N) ";
                cin >> uscita;
                uscita = toupper(uscita);
                while (uscita != 'Y' && uscita != 'N') {
                    system("CLS");
                    cout << "Inserire un altro dolce? (Y/N)" << endl << "Inserire come input 'Y' o 'N'" << endl;
                    cin >> uscita;
                    uscita = toupper(uscita);
                }
            } while (uscita != 'N');
            break;
        }
        cout << "Premere un tasto per continuare...";
        _getch();
    } while (!c);
}
