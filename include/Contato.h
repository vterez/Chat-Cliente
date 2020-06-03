#ifndef CONTATO_H_INCLUDED
#define CONTATO_H_INCLUDED

#include "Mensagem.h"
#include <unordered_map>
#include <list>
#include <string>
#include <memory>

class Contato
{
public:
    Contato()=default;
    Contato(int id){nome=L"=ID:"+std::to_wstring(id);chatoffset=705;nomeconhecido=false;novasmensagens=false;offsetnaovisto=0;};
    Contato(std::wstring s){nome=s;chatoffset=705;nomeconhecido=true;novasmensagens=false;offsetnaovisto=0;}
    bool novasmensagens,nomeconhecido;
    std::list<std::unique_ptr<Mensagem>> chat;
    int chatoffset;
    std::wstring nome;
    int offsetnaovisto;
};



#endif // CONTATO_H_INCLUDED
