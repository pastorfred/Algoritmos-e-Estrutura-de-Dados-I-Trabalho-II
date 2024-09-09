#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

#define DATA_EXTRACAO 0
#define DESCRICAO 1
#define ESTADO 2
#define COMPLEMENTO 3
#define IMPLANTACAO 4
#define LOGRADOURO_NOME 5
#define NUM_INICIAL 6
#define NUM_FINAL 7
#define DEFRONTE 8
#define CRUZAMENTO_NOME 9
#define LADO 10
#define LOCAL_DE_INSTAL 11
#define LATITUDE 12
#define LONGITUDE 13

// classe listaPlaca para fazer a lista das placas da rua
class listaPlaca
{
public:
    // classe Nodo para navegar a lista
    class Nodo
    {
    public:
        double latitude;    // armazena a latitude da placa
        double longitude;   // armazena a longitude da placa
        string localizacao; // armazena a localização da placa (calçada, leito da via, etc.)
        string desc;        // armazena a descricao da placa
        string data;        // armazena a data de expedicao da placa
        Nodo *prox;         // ponteiro para o próximo da lista
        // construtor do nodo
        Nodo(double la = 0, double lo = 0, string l = "", string d = "", string de = "")
        {
            latitude = la;
            longitude = lo;
            localizacao = l;
            data = d;
            desc = de;
            prox = nullptr;
        }
    };
    Nodo *primeiraplaca;
    double totalPlacas; // numero total de placas em uma rua

    // construtor da lista de placas, com valores default em 0
    listaPlaca()
    {
        primeiraplaca = new Nodo();
        totalPlacas = 0;
    }
    int comparaPlaca(string p1, string p2)
    {
        if (p1.compare(p2) < 0)
            return 0;
        else if (p1.compare(p2) > 0)
            return 1;
        else
            return 2;
    }
    // adiciona uma placa para a lista
    void addPlaca(double la, double lo, string l, string d, string da)
    {
        // novo nodo
        Nodo *novo = new Nodo(la, lo, l, d, da);
        // se a lista estiver vazia, a primeira placa vira a nova placa
        if (totalPlacas == 0)
        {
            primeiraplaca = novo;
            totalPlacas++;
            return;
        }
        else if (totalPlacas >= 1)
        { // se a lista não estiver vazia, coloca a placa no final da lista
            Nodo *aux = primeiraplaca;
            while (aux->prox != nullptr)
            {
                aux = aux->prox;
            }
            aux->prox = novo;
            novo->prox = nullptr;
            totalPlacas++;
            return;
        }
    }
    // apresentar o numero total de sinalizações registradas em uma rua especifica
    void sinalregRua()
    {
        double porcentagem;
        porcentagem = 100 * (totalPlacas / 91228);
        cout << "A quantidade de placas nesta rua e " << totalPlacas << ", que e equivalente a " << porcentagem << "% do total de placas." << endl;
    }
    void imprimePlacas()
    {
        Nodo *aux = primeiraplaca;
        if (totalPlacas == 0)
        {
            return;
        }
        while (aux != nullptr)
        {
            cout << "Descricao da placa: " << aux->desc << endl;
            cout << "Data da placa: " << aux->data << endl;
            cout << "Localizacao da placa: " << aux->localizacao << endl;
            cout << "Latitude da placa: " << aux->latitude << endl;
            cout << "Longitude da placa: " << aux->longitude << endl;
            cout << endl;
            aux = aux->prox;
        }
        cout << "fim da lista de placas" << endl;
    }
};

class listaRua
{
public:
    // classe nodo para navegar a lista
    class Nodo
    {
    public:
        listaPlaca *placas = new listaPlaca();
        string id;   // ponteiro para a lista de placas
        string nome; // nome da rua
        Nodo *prox;  // ponteiro para a proxima rua
        Nodo *ant;   // ponteiro para a rua anterior
        // construtor do nodo
        Nodo(string n = "", string i = "")
        {
            nome = n;
            id = i;
            prox = nullptr;
            ant = nullptr;
        }
    };
    Nodo *primeirarua; // ponteiro para o inicio da lista de ruas
    Nodo *ultimarua;   // ponteiro para o final da lista de ruas
    int totalRuas;     // numero total de ruas na lista
    listaRua()
    {
        primeirarua = new Nodo("");
        ultimarua = new Nodo("");
        primeirarua->prox = ultimarua;
        ultimarua->ant = primeirarua;
        totalRuas = 0;
    }
    // compara pra ver se uma rua é "maior" que a outra (ordem alfabetica)
    int comparaRua(string n1, string n2)
    {
        if (n1.compare(n2) < 0)
            return 0;
        else if (n1.compare(n2) > 0)
            return 1;
        else
            return 2;
    }
    // adiciona uma rua para a lista
    void addRua(string i, string n, double la, double lo, string l, string d, string da)
    {
        Nodo *novo = new Nodo(n, i);
        // se não há ruas na lista, insere a nova no inicio
        if (totalRuas == 0)
        {
            primeirarua = novo;
            novo->placas->addPlaca(la, lo, l, d, da);
            totalRuas++;
            return;
        }
        else // se há ruas na lista, faz a função abaixo
        {
            Nodo *aux = primeirarua;
            while (aux->prox != nullptr)
            {
                aux = aux->prox;
            }
            ultimarua = aux;

            // percorre a lista até o final para verificar ruas duplicadas
            aux = primeirarua;
            while (aux != nullptr)
            {
                if (comparaRua(aux->nome, novo->nome) == 2)
                {
                    aux->placas->addPlaca(la, lo, l, d, da);
                    return;
                }
                aux = aux->prox;
            }
            if (addRuaAntes(novo->id, novo->nome, la, lo, l, d, da) == 1) // se a rua nova não for maior que as da lista, ela automaticamente vai pro final
            {
                return;
            }
            else
            {
                ultimarua->prox = novo;
                novo->ant = ultimarua;
                ultimarua = novo;
                ultimarua->placas->addPlaca(la, lo, l, d, da);
                totalRuas++;
            }
        }
    }
    int addRuaAntes(string i, string n, double la, double lo, string l, string d, string da)
    {
        Nodo *novo = new Nodo(n, i);
        Nodo *aux = primeirarua;
        // percorre a lista para ver se a rua nova é maior que as ruas na lista
        while (aux != nullptr)
        {
            if (comparaRua(aux->nome, novo->nome) == 1)
            {
                // se a rua nova for "maior" que a rua anterior, insere ela antes
                if (aux->ant == nullptr)
                {
                    primeirarua = novo;
                    novo->ant = aux->ant;
                    novo->prox = aux;
                    aux->ant = novo;
                    novo->placas->addPlaca(la, lo, l, d, da);
                    totalRuas++;
                    return 1;
                }
                else
                {
                    aux->ant->prox = novo;
                    novo->prox = aux;
                    novo->ant = aux->ant;
                    aux->ant = novo;
                    novo->placas->addPlaca(la, lo, l, d, da);
                    totalRuas++;
                    return 1;
                }
            }
            aux = aux->prox;
        }
        return 0;
    }
    void imprimeLista()
    {
        Nodo *aux = primeirarua;
        if (totalRuas == 0)
        {
            return;
        }
        while (aux != nullptr)
        {
            cout << aux->id << " " << aux->nome << endl;
            aux = aux->prox;
        }
        cout << "fim da lista" << endl;
    }

    // apresentar as sinalizações que não possuem latitude e longitude
    int noLocation()
    {
        int noLalo = 0;
        Nodo *aux = primeirarua;
        while (aux != nullptr)
        {
            listaPlaca::Nodo *temp = aux->placas->primeiraplaca;
            while (temp != nullptr)
            {
                if ((temp->latitude) == 0 && (temp->longitude) == 0)
                {
                    noLalo++;
                }
                temp = temp->prox;
            }
            aux = aux->prox;
        }
        return noLalo;
    }

    // apresentar os 10 dias que mais registram obras

    void printObras()
    {
#ifdef DEBUG
        int numplacas = 0, numplacas2 = 0;
#endif
        int counter = 0;
        string ordem[11] = {""};
        int quant[11] = {0};
        Nodo *confirm = primeirarua;
        listaPlaca::Nodo *datas = confirm->placas->primeiraplaca;
        string data = datas->data;
        while (confirm->prox != nullptr)
        {
            Nodo *aux = primeirarua;
            int count = 0;
            while (aux != nullptr)
            {
                listaPlaca::Nodo *temp = aux->placas->primeiraplaca;
                while (temp != nullptr)
                {
                    if (temp->data == data)
                    {
                        count++;
                    }
                    temp = temp->prox;
                }
                aux = aux->prox;
                counter++;

                if (counter % 2800000 == 0)
                {
                    cout << (static_cast<double>(counter) / 280555810) * 100 << " % das datas lidas e organizadas..." << endl;
                }

#ifdef DEBUG

                numplacas++;
#endif
            }
            for (int i = 0; i < 10; i++)
            {
                if (ordem[i] == data)
                {
                    break;
                }
                if (count > quant[i])
                {
                    int aux2 = quant[i];
                    string tempp = ordem[i];
                    ordem[i] = data;
                    quant[i] = count;
                    quant[i + 1] = aux2;
                    ordem[i + 1] = tempp;
                    break;
                }
            }
            if (datas->prox == nullptr)
            {
                confirm = confirm->prox;
                datas = confirm->placas->primeiraplaca;
                data = datas->data;
            }
            else
            {
                datas = datas->prox;
                data = datas->data;
                count++;

#ifdef DEBUG

                numplacas++;
                numplacas2++;

#endif
            }
        }
        cout << endl;
        for (int a = 0; a < 10; a++)
        {
            cout << a + 1 << ") "
                 << "Data: " << ordem[a] << "; Quantidade de obras na data: " << quant[a] << endl;
        }
        cout << endl;
    }

    // modo navegação
    void navega()
    {
        Nodo *aux = primeirarua->prox;
        char input = 'Y';
        char placas = ' ';
        while (input == 'Y' || input == 'y')
        {
            cout << "Voce esta atualmente na rua: " << aux->id << " " << aux->nome << "." << endl;
            cout << "Voce gostaria de saber mais sobre as placas desta rua? Y/N " << endl;
            cin >> placas;
            if (placas == 'y' || placas == 'Y')
            {
                cout << "Numero de placas na rua: " << aux->placas->totalPlacas << endl;
                cout << endl;
                cout << "Primeira placa registrada: " << endl;
                cout << "Desrcicao: " << aux->placas->primeiraplaca->desc << endl;
                cout << "Data: " << aux->placas->primeiraplaca->data << endl;
                cout << "Localizacao: " << aux->placas->primeiraplaca->localizacao << endl;
                cout << endl;
                cout << "Ultima placa registrada: " << endl;
                listaPlaca::Nodo *ultimaplaca = aux->placas->primeiraplaca;
                while (ultimaplaca->prox != nullptr)
                {
                    ultimaplaca = ultimaplaca->prox;
                }
                cout << "Desrcicao: " << ultimaplaca->desc << endl;
                cout << "Data: " << ultimaplaca->data << endl;
                cout << "Localizacao: " << ultimaplaca->localizacao << endl;
                cout << endl;
                aux->placas->sinalregRua();
                cout << endl;
            }
            char move = ' ';
            cout << "Trocar de rua? (F para ir para frente e V para voltar, N para sair) " << endl;
            cin >> move;
            if (move == 'F' || move == 'f')
            {
                if (aux->prox == nullptr)
                {
                    cout << "Nao tem mais ruas." << endl;
                }
                else
                {
                    aux = aux->prox;
                }
            }
            else if (move == 'V' || move == 'v')
            {
                if (aux == primeirarua)
                {
                    cout << "Nao tem rua anterior." << endl;
                }
                else
                {
                    aux = aux->ant;
                }
            }
            else
            {
                cout << "Continuar no modo navegacao? Y/N ";
                cin >> input;
                if (input == 'Y' || input == 'y')
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
        }
        cout << "Saindo do modo navegacao..." << endl;
        return;
    }

    void lerArquivo()
    {
        ifstream fin;
        fin.open("data.csv", ios::in);

        string line = "";
        string id = "";
        string word = "";
        string descricao = "";
        string rua = "";
        string tiporua = "";
        string datarua = "";
        string localplaca = "";
        string datas[10000] = {"defaul"};
        double latitude = 0;
        double longitude = 0;

        int count = 0;
        getline(fin, line); // remove cabecalho
        while (fin.good())
        {
            int i = 0;
            getline(fin, word);
            stringstream ss(word);
            while (getline(ss, word, ';'))
            {
                switch (i)
                {
                case DESCRICAO:
                    descricao = word;
                    break;
                case IMPLANTACAO:
                    datarua = word;
                    break;
                case LOGRADOURO_NOME:
                    id = word.substr(0, word.find(" "));
                    rua = word.substr(word.find(" ") + 1, word.length());
                    break;
                case LATITUDE:
                    localplaca = word;
                    ss << word;
                    ss >> latitude;
                    break;
                case LONGITUDE:
                    ss << word;
                    ss >> longitude;
                    break;
                default:
                    break;
                }
                i++;
            }
            if (latitude == 0 || longitude == 0)
            {
                latitude = 0;
                longitude = 0;
            }
            addRua(id, rua, latitude, longitude, localplaca, datarua, descricao);
            count++;
            if (count % 10000 == 0)
                cout << (static_cast<double>(count) / 92288) * 100 << " % do arquivo lido..." << endl;
        }
        fin.close();
    }
};

int main()
{
    listaRua *rua = new listaRua();
    cout << "Comecei a ler o arquivo! Por favor aguarde..." << endl;
    rua->lerArquivo();
    cout << "Concluido!" << endl
         << "Removendo os prefixos..." << endl
         << endl;
    int select = 0;
    while (select >= 0 && select < 4)
    {
        cout << "Pressione o numero correspondente (1, 2, 3 ou 4):" << endl;
        cout << "1) Olhar a quantidade de placas sem latitude e longitude." << endl;
        cout << "2) Olhar as 10 datas com mais obras (demora aprox. 10 minutos para terminar)." << endl;
        cout << "3) Entrar no modo navegacao (inclui a quantidade de placas de uma rua com porcentagem relativa ao total e a primeira e ultima placa da rua)." << endl;
        cout << "4) Sair do programa." << endl;
        cin >> select;
        if (select == 1)
        {
            cout << endl;
            cout << "Numero de placas sem latitude e longitude = " << rua->noLocation() << endl
                 << endl;
        }
        else if (select == 2)
        {
            cout << "Imprimindo top 10 datas que mais tiveram obras..." << endl;
            rua->printObras();
        }
        else if (select == 3)
        {
            char n = ' ';
            cout << "Voce gostaria de entrar no modo navegacao? Y/N " << endl;
            cin >> n;
            if (n == 'Y' || n == 'y')
            {
                rua->navega();
            }
        }
        else
        {
            select = 4;
        }
    }
    return 0;
}