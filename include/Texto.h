#ifndef TEXTO_H
#define TEXTO_H

#include "Mensagem.h"
#include <string>
#include <windows.h>
#include <shellapi.h>

class Texto : virtual public Mensagem
{
    public:
        Texto(std::wstring& s,bool p,sf::RectangleShape& r,int l);
        virtual ~Texto(){};

        inline static sf::Text textopadrao;
        sf::RectangleShape ret;
        sf::Text texto;
        bool hiperlink;

        virtual void Enquadra(int off) override;
        virtual void Desenha(sf::RenderWindow& window,float off) override;
        virtual int Clicou(sf::Vector2f& posimouse,bool botao,float off) override;
        virtual sf::Packet Envia(int dest,bool conhecido) override;
    protected:

    private:
};

#endif // TEXTO_H
