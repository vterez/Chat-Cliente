#ifndef BASEINCLUDE_H_INCLUDED
#define BASEINCLUDE_H_INCLUDED

#include <memory>
#include <thread>
#include <string>
#include <list>
#include <unordered_map>
#include <atomic>
#include <mutex>    
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <windows.h>
#include "ImagemTexto.h"
#include "ImagemPura.h"
#include "Som.h"
#include <fstream>
#include <iostream>
#include "Contato.h"

//soquete
extern sf::TcpSocket soquete;

//controle de fluxo
extern std::atomic<bool> running;//bool para parar o recebe e o check
extern std::atomic<bool> enviando;//bool pra lan�ar thread de envio
extern std::atomic<bool> gravando;//bool para editar o temporizador de gravacao
extern std::atomic<bool> editando;//bool complementar do gravando
extern std::atomic<bool> visualizaprint;//estado de visualizar (global pois thread de clique precisas)
extern std::atomic<bool> idnaoexiste;//estado de tentou conectar com id inexistente
extern std::atomic<int> cvatual;//int para compartilhar a conversa atual entre threads para bloqueio
extern std::atomic<int> scrollcontatos;
extern std::mutex travachat;//mutex pra travar mudancas no chat atual
extern std::mutex travaconversas;//mutex pra travar mudancas na ordem das conversas recentes
extern std::mutex travanomes;//mutex para travar acesso aos nomes;
extern std::atomic<bool> pesquisanome;
extern std::atomic<bool> existenotificacao[8];

//armazenamento de mensagens
//extern std::unordered_map<int,std::list<std::unique_ptr<Mensagem>>> chat; //mapa com as conversas
//extern std::unordered_map<int,int> chatoffset; //mapa com os offsets pra controle de scrolling
extern std::list<std::unique_ptr<Mensagem>> fila;//fila de mensagens para envio
extern std::list<int> filadest;//fila de destino das mensagens
extern std::list<int> ordemconversas;//ordem das conversas da barra lateral
extern int idsconversas[8];
extern int id;
//extern std::unordered_map<int,std::wstring> nomes;
extern std::list<int> ordempesquisa;
extern std::unordered_map<int,Contato> contatos;
//extern std::unordered_map<int,Contato>::mapped_type contatoatual;
//manipulacao de texto
extern int ncarac,nlinhas; //numero de caracteres e de linhas da mensagem a enviar, para fins de formatacao
extern std::wstring stringmensagem; //wstring da mensagem a enviar
extern std::wstring stringpesquisa;
extern sf::Font fonte;//fonte padrao
extern sf::Text textoenvia,textocancela;//texto dos botoes de envia e cancela de audio e print
extern sf::Text textomensagem;//texto da mensagem a enviar
extern sf::Text textoantespesquisa;//texto antes da barra de pesquisa
extern sf::Text textopesquisa;//texto barra de pesquisa
extern sf::Text textosidsconversas[8];
extern sf::Text antesdigitarnome;//info para digitar nome
extern sf::Text digitarnome;//texto enquanto digita o nome
extern sf::Text nomefixo;
extern sf::Text idfixo;
extern sf::Text nomecvatual;

//botoes e caixas de texto
extern sf::RectangleShape retpadrao1,retpadrao2;//retangulos das mensagens esquerda e direita
extern sf::RectangleShape retenvia,retcancela;//retangulos dos botoes de envia e cancela de audio e print
extern sf::RectangleShape caixa;//caixa de texto da mensagem
extern sf::RectangleShape cobre;//retangulo para cobrir mensagens grandes
extern sf::RectangleShape retmic;//retangulo do simbolo de microfone
extern sf::CircleShape circulomic;//cirulo vermelho de microfone
extern sf::RectangleShape caixapesquisa;//caixa de pesquisa
extern sf::Sprite spritefecha;//X de fechar o print
extern sf::Texture texturefecha;//X de fechar o print
extern sf::RectangleShape retcontato[8];//conversas
extern sf::RectangleShape info;//parte de cima
extern sf::RectangleShape retnome;//barra para digitar nome
extern sf::RectangleShape retcvatual;//barra verde com o nome da cvatual
extern sf::CircleShape notificacao[8];//bolinha de notificacao
extern sf::RectangleShape linha;

//limites de sprites
extern sf::FloatRect limgravador,limenvia,limcancela,limfecha;//limites dos botoes para checagem de mouse
extern sf::FloatRect limpesquisa,limconversas[8];//limites da barra da esquerda

//manipulacao de imagens
extern sf::Sprite spriteprint;//sprite pra print
extern sf::Texture tprint;//textura pra print
extern sf::Uint8* vprint;//vetor pra print
extern float ty,tx;//dimensoes do print
extern long unsigned int tam;//tamanho do vetor do print

bool Inicializa();

template <typename T>
sf::Vector2f poenomeio(T t)
{
    if constexpr (std::is_base_of<sf::Transformable,T>::value)
    {
        sf::Vector2f posi;
        sf::FloatRect rect;
        rect=t.getGlobalBounds();
        posi.x=rect.left+(rect.width)/2;
        posi.y=rect.top+(rect.height)/2;
        return posi;
    }
    return sf::Vector2f(0,0);
};
int DesfazEnter();
void ChecaClique(sf::Vector2f posimouse,bool botao, float off);
void MudaNome(int user);
std::wstring LowerCase(std::wstring ent);
void Pesquisa();
#endif // BASEINCLUDE_H_INCLUDED

