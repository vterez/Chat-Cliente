#ifndef IMAGEMTEXTO_H
#define IMAGEMTEXTO_H

#include "Imagem.h"
#include "Texto.h"


class ImagemTexto : public Imagem, public Texto
{
    public:
        ImagemTexto(sf::Uint8* rgba,std::wstring& st,bool p,float x,float y,sf::Uint64 t,int o,sf::RectangleShape& r,int l);
        virtual ~ImagemTexto(){};

        int linhas;
        sf::FloatRect limitesimagem;

        virtual void Enquadra(int off) override;
        virtual void Desenha(sf::RenderWindow& window,float off) override;
        virtual int Clicou(sf::Vector2f& posimouse,bool botao,float off) override;
        virtual sf::Packet Envia(int dest,bool conhecido) override;

    protected:

    private:
};

#endif // IMAGEMTEXTO_H
