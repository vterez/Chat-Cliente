#include "BaseInclude.h"

sf::TcpSocket soquete;

std::atomic<bool> running=true;
std::atomic<bool> enviando=false;
std::atomic<bool> gravando=false;
std::atomic<bool> editando=false;
std::atomic<bool> visualizaprint=false;
std::atomic<bool> idnaoexiste=false;
std::atomic<int> cvatual=0;
std::atomic<int> scrollcontatos=0;
std::mutex travachat,travaconversas,travanomes;
std::atomic<bool> pesquisanome=false;
std::atomic<bool> existenotificacao[8];

//std::unordered_map<int,std::list<std::unique_ptr<Mensagem>>> chat;
//std::unordered_map<int,int> chatoffset;
std::list<std::unique_ptr<Mensagem>> fila;
std::list<int> filadest;
std::list<int> ordemconversas;
int idsconversas[8];
int id;
std::wstring nome;
//std::unordered_map<int,std::wstring> nomes;
std::list<int> ordempesquisa;
std::unordered_map<int,Contato> contatos;

int ncarac=0;
int nlinhas=1;
std::wstring stringmensagem;
std::wstring stringpesquisa;
sf::Font fonte;
sf::Text textoenvia,textocancela;
sf::Text textomensagem;
sf::Text textoantespesquisa;
sf::Text textopesquisa;
sf::Text textosidsconversas[8];
sf::Text antesdigitarnome;
sf::Text digitarnome;
sf::Text nomefixo;
sf::Text idfixo;
sf::Text nomecvatual;

sf::RectangleShape retpadrao1(sf::Vector2f(300,30));
sf::RectangleShape retpadrao2(sf::Vector2f(300,30));
sf::RectangleShape retenvia(sf::Vector2f(96,30));
sf::RectangleShape retcancela=retenvia;
sf::RectangleShape caixa(sf::Vector2f(1000,90));
sf::RectangleShape cobre(sf::Vector2f(1000,680));
sf::RectangleShape retmic(sf::Vector2f(30,30));
sf::CircleShape circulomic(5);
sf::RectangleShape caixapesquisa;
sf::Sprite spritefecha;
sf::Texture texturefecha;
sf::RectangleShape retcontato[8];
sf::RectangleShape info;
sf::RectangleShape retnome(sf::Vector2f(300,30));
sf::RectangleShape retcvatual(sf::Vector2f(1000,30));
sf::CircleShape notificacao[8];
sf::RectangleShape linha(sf::Vector2f(1000,10));

sf::FloatRect limgravador;
sf::FloatRect limenvia,limcancela,limfecha;
sf::FloatRect limpesquisa,limconversas[8];

sf::Sprite spriteprint;
sf::Uint8* vprint;//pixels print
sf::Texture tprint;//texture print
float tx=0,ty=0;//dimensoes print
long unsigned int tam=0;//tamanho de vprint

bool Inicializa()
{
    std::fstream pegaip;
    pegaip.open("config.txt",std::fstream::in);
    std::string getip;
    std::getline(pegaip,getip);
    pegaip.close();
    //testa conexao
    sf::Socket::Status estado=soquete.connect(getip,1000,sf::seconds(15.f));
    if(estado!=sf::Socket::Done)
        return 0;

    fonte.loadFromFile("sides/consola.ttf");
    antesdigitarnome.setFont(fonte);
    antesdigitarnome.setCharacterSize(20);
    antesdigitarnome.setFillColor(sf::Color(0,0,0));
    digitarnome=antesdigitarnome;
    antesdigitarnome.setString("Digite seu nome");
    retnome.setFillColor(sf::Color::White);
    retnome.setPosition(0,30);
    digitarnome.setPosition(0,30);
    sf::RenderWindow window(sf::VideoMode(300,60),"Nome",sf::Style::None);
    window.setFramerateLimit(20);
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type==sf::Event::KeyPressed)
            {
                if(event.key.code==sf::Keyboard::Enter)
                {
                    window.close();
                    break;
                }
            }
            if(event.type==sf::Event::TextEntered)
            {
                auto c=event.text.unicode;
                if(c==8&&nome.size()!=0)
                {
                    nome.pop_back();
                    digitarnome.setString(nome);
                    break;
                }
                if(c!=13&&c!=8&&nome.size()<20)
                {
                    nome+=c;
                    digitarnome.setString(nome);
                    break;
                }
            }
        }
        window.clear(sf::Color(232,111,130));
        window.draw(antesdigitarnome);
        window.draw(retnome);
        window.draw(digitarnome);
        window.display();
    }
//troca info com soquete
    sf::Packet pacote,pacote2;
    soquete.receive(pacote);
    pacote>>id;
    pacote2<<nome;
    soquete.send(pacote2);
    //nomes[id]=nome;
    contatos.emplace(id,nome);
    ordemconversas.push_front(id);
    cvatual=id;
//formata textos
    Texto::textopadrao.setFillColor(sf::Color(0,0,0));
    Texto::textopadrao.setCharacterSize(20);
    Texto::textopadrao.setFont(fonte);
    textoenvia=Texto::textopadrao;
    textoenvia.setString("Enviar");
    textoenvia.setOrigin(poenomeio(textoenvia));
    textoenvia.setPosition(170,70);
    textocancela=Texto::textopadrao;
    textocancela.setString("Cancelar");
    textocancela.setOrigin(poenomeio(textocancela));
    textocancela.setPosition(52,70);
    textomensagem=Texto::textopadrao;
    textomensagem.setPosition(300,710);
    textoantespesquisa=Texto::textopadrao;
    textoantespesquisa.setPosition(0,70);
    textoantespesquisa.setString("Digite #id ou @nome do user\ndesejado (- para apagar)");
    textopesquisa=Texto::textopadrao;
    textopesquisa.setPosition(5,125);
    antesdigitarnome.setString("Digite o novo nome ou esc");
    nomefixo=Texto::textopadrao;
    nomefixo.setString(nome);
    nomefixo.setPosition(5,5);
    idfixo=Texto::textopadrao;
    idfixo.setCharacterSize(18);
    idfixo.setFillColor(sf::Color(255,0,0));
    idfixo.setString("#"+std::to_string(id));
    idfixo.setPosition(5,30);
    nomecvatual=Texto::textopadrao;
    nomecvatual.setString(nome);
    nomecvatual.setPosition(305,5);
    //idfixo.setStyle(sf::Text::Bold);
//formata sprites
    retpadrao1.setFillColor(sf::Color(81,91,250));
    retpadrao2=retpadrao1;
    retpadrao2.setFillColor(sf::Color(120,151,200));
    retenvia.setFillColor(sf::Color(34,177,76));
    retenvia.setPosition(120,55);
    retcancela.setFillColor(sf::Color(183,28,55));
    retcancela.setPosition(5,55);
    caixa.setPosition(sf::Vector2f(300,710));
    cobre.setFillColor(sf::Color(232,111,130));
    cobre.setPosition(300,30);
    retmic.setOrigin(poenomeio(retmic));
    retmic.setPosition(1275,760);
    retmic.setFillColor(sf::Color(136,150,157));
    circulomic.setOrigin(poenomeio(circulomic));
    circulomic.setFillColor(sf::Color::Red);
    circulomic.setPosition(1275,760);
    caixapesquisa.setFillColor(sf::Color::White);
    caixapesquisa.setPosition(0,120);
    caixapesquisa.setSize(sf::Vector2f(298,40));
    for(int i=0; i<8; i++)
    {
        retcontato[i]=sf::RectangleShape(sf::Vector2f(298,80));
        retcontato[i].setFillColor(sf::Color(84,186,232));
        retcontato[i].setOutlineColor(sf::Color(190,196,187));
        retcontato[i].setOutlineThickness(2);
        retcontato[i].setPosition(0,160+80*i);
        limconversas[i]=retcontato[i].getGlobalBounds();
        idsconversas[i]=-1;
        textosidsconversas[i]=Texto::textopadrao;
        textosidsconversas[i].setPosition(5,165+80*i);
        notificacao[i]=sf::CircleShape(5);
        notificacao[i].setFillColor(sf::Color(255,223,176));
        notificacao[i].setPosition(280,200+80*i);
        existenotificacao[i]=false;
    }
    idsconversas[0]=id;
    textosidsconversas[0].setString(nome);
    info=sf::RectangleShape(sf::Vector2f(300,160));
    info.setFillColor(sf::Color(232,207,130));
    info.setPosition(0,0);
    texturefecha.loadFromFile("sides/X.jpeg");
    spritefecha=sf::Sprite(texturefecha);
    spritefecha.setScale(0.1,0.1);
    spritefecha.setPosition(10,10);
    retcvatual.setFillColor(sf::Color(188,235,196));
    retcvatual.setPosition(300,0);
    linha.setFillColor(sf::Color(243,182,191,100));
    linha.setPosition(300,506);

//inicializa limites
    limgravador=retmic.getGlobalBounds();
    limenvia=retenvia.getGlobalBounds();
    limcancela=retcancela.getGlobalBounds();
    limfecha=spritefecha.getGlobalBounds();
    limpesquisa=caixapesquisa.getGlobalBounds();

//marca nao blocking pra manipula�ao em threads
    soquete.setBlocking(0);
    return 1;

}

int DesfazEnter()
{
    auto it=stringmensagem.rbegin();
    int ret=0;
    while((*it)!='\n')
    {
        it++;
        ret++;
        if(it==stringmensagem.rend())
            return std::size(stringmensagem);
    }
    return ret;
}

void ChecaClique(sf::Vector2f posimouse,bool botao,float off)
{
    for(auto it=contatos[cvatual].chat.rbegin(); it!=contatos[cvatual].chat.rend(); ++it)
    {
        int retorno=(*it)->Clicou(posimouse,botao,off);
        if(retorno==2)
        {
            Imagem *p=dynamic_cast<Imagem*>((*it).get());
            tprint=p->texture;
            spriteprint=sf::Sprite(tprint);
            spriteprint.setOrigin(poenomeio(spriteprint));
            if(p->ty>700||p->tx>1200)
            {
                if(1200/p->tx<700/p->ty)
                    spriteprint.setScale(1200/p->tx,1200/p->tx);
                else
                    spriteprint.setScale(700/p->ty,700/p->ty);
            }
            spriteprint.setPosition(650,400);
            visualizaprint=true;
        }
        if(retorno!=0)
            return;
    }
}

void MudaNome(int user)
{
    std::wstring novonome=contatos[user].nome;
    digitarnome.setString(novonome);
    bool mudou=false;
    sf::RenderWindow window(sf::VideoMode(300,60),"Nome",sf::Style::None);
    window.setFramerateLimit(20);
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type==sf::Event::KeyPressed)
            {
                if(event.key.code==sf::Keyboard::Enter)
                {
                    mudou=true;
                    window.close();
                    break;
                }
                if(event.key.code==sf::Keyboard::Escape)
                {
                    window.close();
                    break;
                }
            }
            if(event.type==sf::Event::LostFocus)
            {
                window.close();
            }
            if(event.type==sf::Event::TextEntered)
            {
                auto c=event.text.unicode;
                if(c==8&&novonome.size()!=0)
                {
                    novonome.pop_back();
                    digitarnome.setString(novonome);
                }
                else if(c!=13&&c!=8&&novonome.size()<20)
                {
                    novonome+=c;
                    digitarnome.setString(novonome);
                }
            }
        }
        window.clear(sf::Color(200,191,231));
        window.draw(antesdigitarnome);
        window.draw(retnome);
        window.draw(digitarnome);
        window.display();
    }
    if(mudou)
    {
        std::lock_guard<std::mutex> lk(travaconversas),lk2(travanomes);
        int a=0;
        contatos[user].nome=novonome;
        for(auto i:ordemconversas)
        {
            idsconversas[a]=i;
            if(contatos.count(i)!=0)
                textosidsconversas[a].setString(contatos[i].nome);
            a++;
            if(a==8)
                break;
        }
        if(user==id)
        {
            nomefixo.setString(novonome);
        }
        if(user==cvatual)
            nomecvatual.setString(novonome);
    }
    return;
}

std::wstring LowerCase(std::wstring ent)
{
    std::wstring retorno;
    for(auto &c:ent)
    {
        if(c>=65&&c<=90)
            retorno+=(c+32);
        else if(c>=192&&c<=223)
            retorno+=(c+32);
        else
            retorno+=c;
    }
    return retorno;
}

void Pesquisa()
{
    std::wstring nomepesquisa=LowerCase(stringpesquisa.substr(1));
    ordempesquisa.clear();
    std::lock_guard<std::mutex> lk(travanomes);
    for(auto& [k,v]:contatos)
    {
        if(LowerCase(v.nome).find(nomepesquisa)!=std::wstring::npos)
            ordempesquisa.push_back(k);
    }
    int a=0;
    for(auto i:ordempesquisa)
    {
        idsconversas[a]=i;
        if(contatos.count(i)!=0)
            textosidsconversas[a].setString(contatos[i].nome);
        a++;
        if(a==8)
            break;
    }
    travanomes.unlock();
    while(a<8)
    {
        textosidsconversas[a].setString("");
        idsconversas[a]=-1;
        a++;
    }
}
