#include "ImagemPura.h"
#include <iostream>
ImagemPura::ImagemPura(sf::Uint8* rgba,bool p,float x,float y,sf::Uint64 t):Mensagem(3,p,210),Imagem(rgba,p,x,y,t,210)
{
    limites=sprite.getGlobalBounds();
    this->offset=this->limites.height;
};

void ImagemPura::Enquadra(int off)
{
    limites.top-=(10+off);
    sprite.move(0,-10-off);
    this->posi-=(10+off);
};

void ImagemPura::Desenha(sf::RenderWindow& window,float off)
{
    if(off!=0)
    {
        sprite.move(0,off);
    }
    window.draw(sprite);
    if(off!=0)
    {
        sprite.move(0,-off);
    }
};

int ImagemPura::Clicou(sf::Vector2f& posimouse,bool botao,float off)
{
    if(off!=0)
        limites.top+=off;
    if(!limites.contains(posimouse))
    {
        limites.top-=off;
        return 0;
    }
    if(botao)
    {
        limites.top-=off;
        return Imagem::Clicou(posimouse,botao,off);
    }
    if(off!=0)
        limites.top-=off;
    return 2;
};

sf::Packet ImagemPura::Envia(int dest,bool conhecido)
{
    sf::Packet pacote;
    pacote<<3<<dest<<conhecido<<offset;
    sf::Uint64 t=tam;
    pacote<<t;
    pacote<<tx<<ty;
    for(long unsigned int i=0; i<tam; i++)
        pacote<<vetorpixels[i];
    return pacote;
}
