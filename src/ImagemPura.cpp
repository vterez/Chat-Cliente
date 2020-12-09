#include "ImagemPura.hpp"
#include "Som.hpp"
#include "RSom.hpp"
#include "RTexto.hpp"
#include "Texto.hpp"
#include "RImagemPura.hpp"
#include "RImagemTexto.hpp"
#include "ImagemTexto.hpp"
#include <iostream>
ImagemPura::ImagemPura(sf::Uint8* rgba,bool p,float x,float y,sf::Uint64 t,int n,int R,Mensagem *m,int nr):Mensagem(3,p,210,n,R),Imagem(rgba,p,x,y,t,210,n)
{
    limites=sprite.getGlobalBounds();
    this->offset=this->limites.height;
    if(R)
    {
        switch(R)
        {
        case 1:
        {
            Texto* msg=dynamic_cast<Texto*>(m);
            std::wstring temp=msg->texto.getString().substring(0,msg->primeiroenter);
            resp=std::make_unique<RTexto>(offset,nr,temp,p,msg);
            offset+=resp->limites.height;
            break;
        }
        
        case 2:
        {
            Som* msg=dynamic_cast<Som*>(m);
            resp=std::make_unique<RSom>(offset,msg->duracao,p,nr,msg);
            offset+=resp->limites.height;
            break;
        }
        case 3:
        {
            ImagemPura* msg=dynamic_cast<ImagemPura*>(m);
            resp=std::make_unique<RImagemPura>(offset,nr,p,msg);
            offset+=resp->limites.height;
            break;
        }
        case 4:
        {
            ImagemTexto* msg=dynamic_cast<ImagemTexto*>(m);
            resp=std::make_unique<RImagemTexto>(offset,nr,p,msg);
            offset+=resp->limites.height;
            break;
        }
        default:
            break;
        }
    };
    limitesresp.top=limites.top;
    limitesresp.height=limites.height;
    if(!p)
    {
        limitesresp.left=limites.left+limites.width;
        limitesresp.width=995-limites.width;
    }
    else
    {
        limitesresp.left=300;
        limitesresp.width=995-limites.width;
    }
    contorno.setSize(sf::Vector2f(limites.width+8,limites.height+8));
    contorno.setPosition(limites.left-4,limites.top-4);
};

void ImagemPura::Enquadra(int off)
{
    limites.top-=(10+off);
    sprite.move(0,-10-off);
    limitesresp.top-=(10+off);
    contorno.move(0,-10-off);
    this->posi-=(10+off);
    if(tiporesp)
        resp->Enquadra(off);
};

void ImagemPura::Desenha(sf::RenderWindow& window,float off)
{
    if(off!=0)
    {
        sprite.move(0,off);
        contorno.move(0,off);
    }
    window.draw(contorno);
    window.draw(sprite);
    if(off!=0)
    {
        sprite.move(0,-off);
        contorno.move(0,-off);
    }
    if(tiporesp)
        resp->Desenha(window,off);
};

int ImagemPura::Clicou(sf::Vector2f& posimouse,bool botao,float off,int& aux)
{
    if(off!=0)
    {
        limitesresp.top+=off;
    }
    if(limitesresp.contains(posimouse))
    {
        if(off!=0)
            limitesresp.top-=10;
        aux=nmensagem;
        return 3;
    }
    if(off!=0)
    {
        limites.top+=off;
    }
    if(limites.contains(posimouse))
    {
        if(botao)
        {
            limites.top-=off;
            return Imagem::Clicou(posimouse,botao,off,aux);
        }
        limites.top-=off;
        return 2;
    }
    else if(tiporesp)
    {
        if(off!=0)
            resp->limites.top+=off;
        if(resp->limites.contains(posimouse))
        {
            if(off!=0)
                resp->limites.top-=off;
            aux=resp->nmensagem;
            return 4;
        }
        return 0;
    }
    return 0;
};

sf::Packet ImagemPura::Envia(int dest,bool conhecido,int mandaresp)
{
    sf::Packet pacote;
    pacote<<3<<dest<<conhecido<<offset;
    sf::Uint64 t=tam;
    pacote<<t;
    pacote<<tx<<ty;
    for(long unsigned int i=0; i<tam; i++)
        pacote<<vetorpixels[i];
    pacote<<mandaresp;
    return pacote;
}
