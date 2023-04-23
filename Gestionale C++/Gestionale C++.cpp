
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

static void AggiuntaMenu(string dolceOrdinato, int& dim, fstream& writer, fstream& reader, string path, string pathTemp, string pathDispensa)
{
    string line, line2, split;
    int N, scelta, controllo = 0, min = 2000, max = 5000;
    prodotto p;
    fstream dispensa;
    if (dim < 100)
    {
        //SALVATAGGIO DI DOLCE ED INGREDIENTI SU FILE TEMPORANEO
        writer.open(pathTemp, ios::out | ios::app);
        p.dolce = dolceOrdinato;
        cout << "Inserire il numero di ingredienti necessari: ";
        cin >> N;
        writer << p.dolce << ";";
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
                writer << p.ingrediente[i - 1] << " " << p.quantità[i - 1] << ";";
                break;
            case 1:
                writer << p.ingrediente[i - 1] << " " << p.quantità[i - 1] << " g;";
                break;
            case 2:
                writer << p.ingrediente[i - 1] << " " << p.quantità[i - 1] << " ml;";
                break;
            }

            //SALVATAGGIO DEGLI INGREDIENTI SU DISPENSA
            dispensa.open(pathDispensa, ios::out | ios::app);
            reader.open(pathDispensa, ios::in);
            controllo = 0;
            while (getline(reader, line)) {
                split = line.substr(0, line.find(" "));
                if (split != p.ingrediente[i - 1])
                    controllo += 0;
                else
                    controllo += 1;
            }
            if (controllo == 0 && scelta == 0)
                dispensa << p.ingrediente[i - 1] << " " << (rand() % (max - min + 1)) + min << endl;
            else if(controllo == 0 && scelta == 1)
                dispensa << p.ingrediente[i - 1] << " " << (rand() % (max - min + 1)) + min << " g" << endl;
            else if(controllo == 0 && scelta == 2)
                dispensa << p.ingrediente[i - 1] << " " << (rand() % (max - min + 1)) + min << " ml" << endl;
            reader.close();
            dispensa.close();
        }
        writer.close();

        //SALVATAGGIO DEI PROCEDIMENTI SU FILE TEMPORANEO
        writer.open(pathTemp, ios::out | ios::app);
        cout << "Inserire il numero di procedimenti necessari: ";
        cin >> N;
        for (int i = 1; i <= N; i++)
        {
            cout << "Inserire il " << i << " passaggio: ";
            cin >> p.procedimento[i - 1];
            writer << i << ". " << p.procedimento[i - 1] << ";";
        }
        writer << endl;
        writer.close();

        //ESTRAZIONE DEL DOLCE SU FILE HTML
        writer.open(path, ios::out | ios::app);
        reader.open(pathTemp, ios::in);
        while (getline(reader, line))
        {
            split = line.substr(0, line.find(";"));
            writer << "<li> " << split << " </li>" << endl;
        }
        reader.close();
        writer.close();
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

static void EliminaDolce(string dolceSelezionato, fstream& writer, fstream& reader, string pathTemp, string pathApp)
{
    string line;
    writer.open(pathApp, ios::out | ios::app);
    reader.open(pathTemp, ios::in);
    while (getline(reader, line))
    {
        string split = line.substr(0, line.find(";"));
        if (split != dolceSelezionato)
        {
            writer << line << endl;
        }
    }
    reader.close();
    writer.close();
}
static void ModificaDolce(string dolceSelezionato, string nuovoDolce, fstream& writer, fstream& reader, string pathTemp, string pathApp)
{
    string line;
    writer.open(pathApp, ios::out | ios::app);
    reader.open(pathTemp, ios::in);
    while (getline(reader, line))
    {
        string split = line.substr(0, line.find(";"));
        if (split != dolceSelezionato)
        {
            writer << line << endl;
        }
        else {
            writer << nuovoDolce;
            string split2 = line.substr(split.length());
            writer << split2 << endl;
        }
    }
    reader.close();
    writer.close();
}

static void Sostituzione(string appoggio, string vecchio) {
    remove(vecchio.c_str());
    if (rename(appoggio.c_str(), vecchio.c_str()) == 0)
        cout << "Elemento cancellato/modificato con successo!" << endl;
    else
        perror("Error renaming file");
}


/*
string Spaziatura(string input) {
    input.replace(input.find("-"), 1, " ");
    return input;
}
*/

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



#pragma endregion

int main()
{
    int scelta, dim = 0, r;
    bool c = false;
    char uscita;
    string dolce, nuovoDolce;
    string path = "ListaDolci.html", pathTemp = "ListaDolciTemp.csv", pathApp = "ListaDolciApp.csv", pathOrdine = "RicetteOrdine.csv", pathDispensa ="Dispensa.csv";
    fstream ListaDolci, ListaDolciTemp, reader, ricetteOrdini, writer;
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
            //dolce = Spaziatura(dolce);
            htmlI(ListaDolci, path);
            AggiuntaMenu(dolce, dim, writer, reader, path, pathTemp, pathDispensa);
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
        case 3:
            system("CLS");
            RicavaMenu(reader, pathTemp);
            cout << "Inserire il dolce che si desidera eliminare: ";
            cin >> dolce;
            dolce[0] = toupper(dolce[0]);
            r = Ricerca(dolce, pathTemp);
            if (r == -1) {
                cout << "Dolce non trovato!" << endl;
            }
            else {
                
                EliminaDolce(dolce, writer, reader, pathTemp, pathApp);
                Sostituzione(pathApp, pathTemp);
            }
            break;
        case 4:
            system("CLS");
            RicavaMenu(reader, pathTemp);
            cout << "Inserire il dolce che si desidera modificare: ";
            cin >> dolce;
            dolce[0] = toupper(dolce[0]);
            r = Ricerca(dolce, pathTemp);
            if (r == -1) {
                cout << "Dolce non trovato!" << endl;
            }
            else {
                cout << "Inserire il nuovo dolce: ";
                cin >> nuovoDolce;
                nuovoDolce[0] = toupper(nuovoDolce[0]);
                ModificaDolce(dolce, nuovoDolce, writer, reader, pathTemp, pathApp);
                Sostituzione(pathApp, pathTemp);
            }
            break;
        }
        cout << "Premere un tasto per continuare...";
        _getch();
    } while (!c);
}
