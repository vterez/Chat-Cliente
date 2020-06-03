#ifndef IMAGEM_H
#define IMAGEM_H

#include "Mensagem.h"
#include <windows.h>
#include <objbase.h>
#include <shlobj.h>

class Imagem : virtual public Mensagem
{
    public:
        Imagem(sf::Uint8 *rgba,bool p,float x,float y,sf::Uint64 t,int l);
        virtual ~Imagem();

        sf::Texture texture;
        sf::Sprite sprite;
        sf::Uint8 *vetorpixels;
        float tx,ty;
        sf::Uint64 tam;

        virtual void Enquadra(int off)=0;
        virtual void Desenha(sf::RenderWindow& window,float off)=0;
        virtual int Clicou(sf::Vector2f& posimouse,bool botao,float off);
        virtual sf::Packet Envia(int dest,bool conhecido)=0;
};

#endif // IMAGEM_H
