
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <conio.h>
#include <array>
#include <libxml/parser.h>
#include <libxml/tree.h>

using namespace std;
struct prodotto
{
    string dolce;
    string ingrediente[100];
    string procedimento[100];
    int indice = 0;
    int quantità[100];
};

prodotto p;

#pragma region Funzioni

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
    int N, scelta, controllo = 0, min = 2000, max = 5000, uscita = 0;
    srand(time(NULL));

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
            p.ingrediente[i - 1][0] = toupper(p.ingrediente[i - 1][0]);
            cout << "Inserire la quantita di quell'ingrediente: ";
            cin >> p.quantità[i - 1];
            do {
                cout << "Seleziona l'unita di misura del " << i << " ingrediente (0 - no unita misura / 1 - g / 2 - ml): ";
                cin >> scelta;
                switch (scelta)
                {
                default:
                    cout << "Input non valido!" << endl;
                    uscita = 0;
                    break;
                case 0:
                    writer << p.ingrediente[i - 1] << " " << p.quantità[i - 1] << ";";
                    uscita = 1;
                    break;
                case 1:
                    writer << p.ingrediente[i - 1] << " " << p.quantità[i - 1] << " g;";
                    uscita = 1;
                    break;
                case 2:
                    writer << p.ingrediente[i - 1] << " " << p.quantità[i - 1] << " ml;";
                    uscita = 1;
                    break;
                }
            } while (uscita == 0);
            

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
                dispensa << endl << p.ingrediente[i - 1] << " " << (rand() % (100 - 10 + 1)) + 10;
            else if (controllo == 0 && scelta == 1)
                dispensa << endl << p.ingrediente[i - 1] << " " << (rand() % (max - min + 1)) + min << " g;";
            else if (controllo == 0 && scelta == 2)
                dispensa << endl << p.ingrediente[i - 1] << " " << (rand() % (max - min + 1)) + min << " ml;";
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
    ListaDolci << "<!DOCTYPE html> <html> <head> <title>Menu</title> <link rel=\"shortcut icon" "> href = \"immagini/favicon.ico" "/> <meta name=\"viewport" "content=\"width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.15.4/css/all.css" 
    integrity="sha384-DyZ88mC6Up2uqS4h/KRgHuoeGwBcD4Ng9SiP4dIRy0EXTlnuz47vAwmeGwVChigm" 
    crossorigin="anonymous"/>
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">
<link rel="stylesheet" href="style.css" type="text/css">
<link rel="stylesheet" href="menuStyle.css" type="text/css">
  </head>
  <body>
    <nav class="sticky">
      <input type="checkbox" id="check">
      <label for="check" class="check-button">
        <i class="fas fa-bars"></i>
      </label>
      <div class="div-logo">
        <a href="homepage.html"><img src="immagini/logo.png" alt="logo"></a>
      </div>
      <ul>
        <li><a href="storia.html" target="_blank" class="a-elementi"> Storia</a></li>
        <li><a href="territorio.html" target="_blank" class="a-elementi">Territorio</a></li>
        <li><a href="monumenti.html" target="_blank" class="a-elementi">Monumenti</a></li>
        <li><a href="cultura.html" target="_blank" class="a-elementi">Cultura</a></li>
        <li><a href="trasporti.html" target="_blank" class="a-elementi">Trasporti</a></li>
        <li><a href="sport.html" target="_blank" class="a-elementi">Sport</a></li>
        <li><a href="mappa.html" target="_blank" class="a-elementi">Mappa</a></li>
      </ul>
    </nav> 

      <div class="div-corpo">
        <h1 style="color:#fff; margin-left:35px;">Menù</h1>
          <div class="div-colonna-palazzo1">
          </div>
          <div class="div-colonna-palazzo2">
            <ul>" << endl;
    ListaDolci.close();
}
static void htmlF(fstream& ListaDolci, string path) {
    ListaDolci.open(path, ios::out | ios::app);
    ListaDolci << "</ul>" << endl << "</body>" << endl << "</html>" << endl;
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
    if (rename(appoggio.c_str(), vecchio.c_str()) != 0)
        perror("Errore!");
}

static int Contatore() {
    fstream reader;
    string line;
    int contatore = 0;
    reader.open("Dispensa.csv", ios::in);
    while (getline(reader, line))
    {
        contatore++;
    }
    return contatore;
}
static void SottrazioneDispensa(int array[], string array2[]) {
    string line;
    fstream reader, writer, writer2;
    string pathDispensa = "Dispensa.csv", sottostringa3;
    int controllo = 0, indice = 0, newNum = 0, num, nRighe = Contatore();
    writer.open("DispensaApp.csv", ios::out);
    reader.open(pathDispensa, ios::in);
    while (getline(reader, line))
    {
        string ingrediente = line.substr(0, line.find(" "));
        indice = 0;
        for (int i = 0; i < nRighe; i++)
        {
            if (ingrediente == array2[indice]) {
                int inizio2 = line.find(" ");
                int fine2 = line.find(" ", inizio2 + 1);                                
                string newQuantita = line.substr(inizio2 + 1, fine2 - inizio2 - 1);     
                num = stoi(newQuantita);                                            
                newNum = num - array[indice];
                if (newNum > 0) {
                    if (fine2 != -1) {
                        sottostringa3 = line.substr(fine2 + 1);
                        writer << ingrediente << " " << newNum << " " << sottostringa3 << endl;
                    }
                    else
                        writer << ingrediente << " " << newNum << ";" << endl;
                }
                else {
                    if (fine2 != -1) {
                        sottostringa3 = line.substr(fine2 + 1);
                        writer << ingrediente << " " << 0 << " " << sottostringa3 << endl;
                    }
                    else
                        writer << ingrediente << " " << 0 << ";" << endl;
                    
                    writer2.open("ListaSpesa.csv", ios::out | ios::app);
                    writer2 << ingrediente << " " << newNum * -1 << ";" << endl;
                    writer2.close();
                }
                controllo = 0;
                break;
            }
            else
                controllo++;
            indice++;
        }
        if(controllo != 0)
            writer << line << endl;
    }
    reader.close();
    writer.close();
    Sostituzione("DispensaApp.csv", "Dispensa.csv");
}
static void Ordinazione(string dolce, int quantita, string pathTemp) {
    string line;
    int prodotto = 0, indice = 0;
    int array[100];
    string array2[100];
    fstream reader, writer;
    string sottostringa3;
    reader.open(pathTemp, ios::in);
    while (getline(reader, line))
    {
        if (line.find(dolce) != string::npos)
        {
            int inizio = line.find(";"); // Trova il primo carattere ";" nella riga
            while (inizio != string::npos)
            {
                int fine = line.find(";", inizio + 1); // Trova il prossimo carattere ";" nella riga
                string sottostringa = line.substr(inizio + 1, fine - inizio - 1); // Estrae la sottostringa tra i due caratteri ";"
                if (sottostringa.find("1.") != string::npos) {
                    break;
                }
                else {
                    string ingrediente = line.substr(inizio + 1, sottostringa.find(" "));
                    int inizio2 = sottostringa.find(" "); // Trova la quantità
                    int fine2 = sottostringa.find(" ", inizio2 + 1);
                    string sottostringa2 = sottostringa.substr(inizio2 + 1, fine2 - inizio2 - 1);
                    if (fine2 != -1) {
                        sottostringa3 = sottostringa.substr(fine2 + 1);
                        int num = stoi(sottostringa2);          //moltiplicare la quantità per il n di dolci ordinati
                        prodotto = num * quantita;
                    }
                    else {
                        int num = stoi(sottostringa2);          //moltiplicare la quantità per il n di dolci ordinati
                        prodotto = num * quantita;
                    }
                    array[indice] = prodotto;
                    array2[indice] = ingrediente;
                    indice++;
                    inizio2 = fine2;
                }
                inizio = fine;
            }
        }
    }
    SottrazioneDispensa(array, array2);
    reader.close();
}

static void Spesa() {
    fstream reader, writer, reader2, writer2;
    srand(time(NULL));
    int min = 2000, max = 5000, indice = 0, indice2 = 0, contatore = 0, nRighe = Contatore();
    string pathDispensa = "Dispensa.csv", line, ingredienteSpesa, ingredienteDispensa, unità, array[100], array2[100];
    reader.open("ListaSpesa.csv", ios::in);
    while (getline(reader, line)) {
        ingredienteSpesa = line.substr(0, line.find(" "));
        array2[indice2] = ingredienteSpesa;
        indice2++;
        reader2.open("Dispensa.csv", ios::in);
        while (getline(reader2, line)) {
            ingredienteDispensa = line.substr(0, line.find(" "));
            int inizio = line.find(" ");
            int fine = line.find(" ", inizio + 1);
            int fine2 = line.find(";");
            if (fine != -1) {
                unità = line.substr(fine, fine2);
            }
            else
                unità = "";

            if (ingredienteSpesa == ingredienteDispensa) {

                if (unità == " g;")
                    array[indice] = ingredienteDispensa + " " + to_string((rand() % (max - min + 1)) + min) + " g;";
                else if (unità == " ml;")
                    array[indice] = ingredienteDispensa + " " + to_string((rand() % (max - min + 1)) + min) + " ml;";
                else
                    array[indice] = ingredienteDispensa + " " + to_string((rand() % (100 - 20 + 1)) + 20) + ";";
                indice++;
            }
        }
        reader2.close();
    }
    reader.close();

    writer.open("DispensaAppApp.csv", ios::out);
    reader.open("Dispensa.csv", ios::in);
        while (getline(reader, line))
        {
            string ingrediente = line.substr(0, line.find(" "));
            indice = 0;
            for (int i = 0; i < nRighe; i++)
            {
            
            if (ingrediente == array2[indice]) {
                writer << array[i] << endl;
                contatore = 0;
                break;
            }
            else
                contatore++;
            indice++;
        }
        if (contatore != 0)
            writer << line << endl;
    }
    reader.close();
    writer.close();
    Sostituzione("DispensaAppApp.csv", "Dispensa.csv");
}


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
                if (sottostringa.find("1.") != string::npos) {
                    cout << endl << "Procedimenti: " << endl << endl;
                }
                cout << sottostringa << endl;
                inizio = fine;

            }
        }
    }
    readering.close();
    _getch();
    reader.close();
    ricetteOrdini.close();
}

#pragma endregion

int main()
{
    int scelta, dim = 0, r, q;
    bool c = false;
    char uscita;
    int array[100];
    int contatore = 0;
    string dolce, nuovoDolce, line;
    string path = "ListaDolci.html", pathTemp = "ListaDolciTemp.csv", pathApp = "ListaDolciApp.csv", pathOrdine = "RicetteOrdine.csv", pathDispensa = "Dispensa.csv";
    fstream ListaDolci, ListaDolciTemp, reader, ricetteOrdini, writer;
    do {
        system("CLS");
        cout << "1 - Aggiunta dolce\n2 - Ordinazione\n3 - Elimina dolce\n4 - Modifica dolce\n5 - Visualizza Dispensa\n6 - Fai la spesa\n0 - Uscita\n" << endl;
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
                    system("CLS");
                    cout << "Dolce non trovato!" << endl;
                    _getch();
                }
                else {
                    cout << "Inserire la quantità di dolci: ";
                    cin >> q;
                    system("CLS");
                    Ordinazione(dolce, q, pathTemp);
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
            do {
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
            } while (r == -1);
            
            break;
        case 4:
            system("CLS");
            RicavaMenu(reader, pathTemp);
            do {
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
            } while (r == -1);
            
            break;
        case 5:
            system("CLS");
            reader.open("Dispensa.csv", ios::in);
            cout << "Dispensa: " << endl << endl;
            while (getline(reader, line)) {
                cout << line << endl;
            }
            reader.close();
            break;
        case 6:
            Spesa();
            remove("ListaSpesa.csv");
            break;
        }
        cout << "Premere un tasto per continuare...";
        _getch();
    } while (!c);
}
