#include "Recebimento.hpp"
#include <iostream>

void Recebimento::RecebeMensagemTexto(sf::Packet &pacote,int offset,std::list<std::unique_ptr<Mensagem>>& chat,int nrec)
{
    std::wstring s;
    int resp,p_enter;
    pacote>>s>>p_enter>>resp;
    resp*=(-1);
    if(resp!=0)
    {
        for(auto &a:chat)
            if(a->nmensagem==resp)
                chat.push_back(std::make_unique<Texto>(s,0,(offset+10)/23,nrec,p_enter,a->tipo,a.get(),resp));
    }
    else            
        chat.push_back(std::make_unique<Texto>(s,0,(offset+10)/23,nrec,p_enter));
}
void Recebimento::RecebeMensagemSom(sf::Packet &pacote,std::list<std::unique_ptr<Mensagem>>& chat,int nrec)
{
    sf::SoundBuffer buff;
    sf::Sound som;
    sf::Uint64 samplecount;
    unsigned int channelcount;
    unsigned int samplerate;
    pacote>>samplecount>>channelcount>>samplerate;
    auto samples=std::make_unique<sf::Int16[]>(samplecount);
    for(sf::Uint64 i=0; i<samplecount; i++)
        pacote>>samples[i];
    buff.loadFromSamples(samples.get(),samplecount,channelcount,samplerate);
    std::wstring tempo;
    pacote>>tempo;
    int resp;
    pacote>>resp;
    resp*=(-1);
    sf::Text duracao=textopadrao;
    duracao.setString(tempo);
    if(resp!=0)
    {
        for(auto &a:chat)
            if(a->nmensagem==resp)
                chat.push_back(std::make_unique<Som>(buff,0,duracao,nrec,a->tipo,a.get(),resp));
    }
    else
        chat.push_back(std::make_unique<Som>(buff,0,duracao,nrec));
}
void Recebimento::RecebeMensagemImagem(sf::Packet &pacote,std::list<std::unique_ptr<Mensagem>>& chat,int nrec)
{
    sf::Uint64 tam;
    sf::Uint8* rgba;
    float x,y;
    int resp;
    pacote>>tam;
    rgba=new sf::Uint8[tam];
    pacote>>x>>y;
    for(sf::Uint64 i=0; i<tam; i++)
    {
        pacote>>rgba[i];
    }
    pacote>>resp;
    resp*=(-1);
    if(resp!=0)
    {
        for(auto &a:chat)
            if(a->nmensagem==resp)
                chat.push_back(std::make_unique<ImagemPura>(rgba,false,x,y,tam,nrec,a->tipo,a.get(),resp)); 
    }
    else       
        chat.push_back(std::make_unique<ImagemPura>(rgba,false,x,y,tam,nrec));
}

void Recebimento::RecebeMensagemImagemTexto(sf::Packet &pacote,int offset,std::list<std::unique_ptr<Mensagem>>& chat,int nrec)
{
    sf::Uint64 tam;
    sf::Uint8* rgba;
    float x,y;
    pacote>>tam;
    rgba=new sf::Uint8[tam];
    pacote>>x>>y;
    for(sf::Uint64 i=0; i<tam; i++)
    {
        pacote>>rgba[i];
    }
    int linhas,resp,p_enter;
    std::wstring s;
    pacote>>linhas>>s>>p_enter>>resp;
    resp*=(-1);
    if(resp!=0)
    {
        for(auto &a:chat)
            if(a->nmensagem==resp)
                chat.push_back(std::make_unique<ImagemTexto>(rgba,s,0,x,y,tam,offset,linhas,nrec,p_enter,a->tipo,a.get(),resp));
    }
    else
        chat.push_back(std::make_unique<ImagemTexto>(rgba,s,0,x,y,tam,offset,linhas,nrec,p_enter));
}
void Recebimento::Recebe ()
{
    sf::Packet pacote;
    sf::Socket::Status status;
    while(running)
    {
        sf::sleep(sf::seconds(0.2f));
        status=soquete.receive(pacote);
        if(status==sf::Socket::Done)
        {
			std::wcout<<L"entrou no recebimento\n";
            int tipo;
            int remetente;
            int offset;
            bool travado=false;
            pacote>>tipo>>remetente;
            std::lock_guard<std::mutex> lk(travaconversas),lk2(travanomes);
            if(tipo==-1)
            {
                contatos.erase(remetente);
                ordemconversas.remove(remetente);
                int a=0;
                for(auto i:ordemconversas)
                {
                    idsconversas[a]=i;
                    textosidsconversas[a].setString(contatos[i].nome);
                    a++;
                    if(a==8)
                        break;
                }
                while(a<8)
                {
                    idsconversas[a]=-1;
                    textosidsconversas[a].setString("");
                    a++;
                }
                cvatual=idsconversas[0];
            }
            else if(tipo==7)
            {
                std::wstring nomenovo;
                pacote>>nomenovo;
                auto &contatoatual=contatos[remetente];
                contatoatual.nome=nomenovo;
                contatoatual.nomeconhecido=true;
                int a=0;
                for(auto i:ordemconversas)
                {
                    idsconversas[a]=i;
                    textosidsconversas[a].setString(contatos[i].nome);
                    a++;
                    if(a==8)
                        break;
                }
            }
            else
            {
                pacote>>offset;
                if(contatos.count(remetente)==0)
                {
                    ordemconversas.push_front(remetente);
                    contatos.emplace(remetente,remetente);
                    sf::Packet pct;
                    pct<<7<<remetente;
                    while(soquete.send(pct)!=sf::Socket::Status::Done) {};
                }
                else
                {
                    for(auto &x:contatos[remetente].chat)
                        x->Enquadra(offset);
                    ordemconversas.remove(remetente);
                    ordemconversas.push_front(remetente);
                }
                auto &contatoatual=contatos[remetente];
                contatoatual.novasmensagens=true;
                contatoatual.chatoffset-=(10+offset);
                contatoatual.offsetnaovisto-=(10+offset);
                if(remetente==cvatual)
                {
                    travachat.lock();
                    travado=true;
                }
                int a=0;
                switch(tipo)
                {
                case 1:
                {
                    RecebeMensagemTexto(pacote,offset,contatoatual.chat,-contatoatual.nreceb++);
                    break;
                }
                case 2:
                {
                    RecebeMensagemSom(pacote,contatoatual.chat,-contatoatual.nreceb++);
                    break;
                }
                case 3:
                {
                    RecebeMensagemImagem(pacote,contatoatual.chat,-contatoatual.nreceb++);
                    break;
                }
                case 4:
                {
                    RecebeMensagemImagemTexto(pacote,offset,contatoatual.chat,-contatoatual.nreceb++);
                    break;
                }
                default:
                    break;

                }
                if(idsconversas[0]!=remetente)
                {
                    for(auto i:ordemconversas)
                    {
                        idsconversas[a]=i;
                        auto& contatoatual2=contatos[i];
                        textosidsconversas[a].setString(contatoatual2.nome);
                        existenotificacao[a]=contatoatual2.novasmensagens;
                        a++;
                        if(a==8)
                            break;
                    }
                }
                else
                    existenotificacao[0]=true;
                if(travado)
                    travachat.unlock();
            }
            status=sf::Socket::NotReady;
        }
    }

}
