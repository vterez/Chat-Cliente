#ifndef IMAGEMPURA_H
#define IMAGEMPURA_H

#include "Imagem.h"


class ImagemPura : public Imagem
{
    public:
        ImagemPura(sf::Uint8* rgba,bool p,float x,float y,sf::Uint64 t);
        virtual ~ImagemPura(){};

        virtual void Enquadra(int off) override;
        virtual void Desenha(sf::RenderWindow& window,float off) override;
        virtual int Clicou(sf::Vector2f& posimouse,bool botao,float off) override;
        virtual sf::Packet Envia(int dest,bool conhecido) override;

    protected:

    private:
};

#endif // IMAGEMPURA_H
