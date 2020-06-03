#ifndef RECEBIMENTO_H_INCLUDED
#define RECEBIMENTO_H_INCLUDED

#include "BaseInclude.h"
namespace Recebimento{
void RecebeMensagemTexto(sf::Packet &pacote,int linhas,std::list<std::unique_ptr<Mensagem>>& chat);
void RecebeMensagemSom(sf::Packet &pacote,std::list<std::unique_ptr<Mensagem>>& chat);
void RecebeMensagemImagem(sf::Packet &pacote,std::list<std::unique_ptr<Mensagem>>& chat);
void RecebeMensagemImagemTexto(sf::Packet &pacote,int linhas,std::list<std::unique_ptr<Mensagem>>& chat);
void Recebe();
}
#endif // RECEBIMENTO_H_INCLUDED
