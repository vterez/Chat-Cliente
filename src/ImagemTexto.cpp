#include "ImagemTexto.hpp"
#include "Som.hpp"
#include "RSom.hpp"
#include "RTexto.hpp"
#include "Texto.hpp"
#include "RImagemPura.hpp"
#include "RImagemTexto.hpp"
#include "ImagemPura.hpp"
#include <iostream>
ImagemTexto::ImagemTexto(sf::Uint8* rgba,std::wstring& st,bool p,float x,float y,sf::Uint64 t,int o,int l,int n,int p_enter,int R,Mensagem *m,int nr):Mensagem(4,p,o,n,R),Imagem(rgba,p,x,y,t,210,n),Texto(st,p,l,n,p_enter)
{
    linhas=l;
    sprite.move(0,-limites.height);
    limitestexto=limites;
    limites=sprite.getGlobalBounds();
    this->offset=limites.height+limitestexto.height;
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
    limitesresp.height=limites.height+limitestexto.height;
    bool maior=(limites.width>limitestexto.width) ? 1:0;
    contorno.setOrigin(0,0);
    if(!p)
    {
        if(maior)
        {
            limitesresp.left=limites.left+limites.width;
            limitesresp.width=995-limites.width;
            contorno.setPosition(limites.left-4,limites.top-4);
            contorno.setSize(sf::Vector2f(limites.width+8,limitesresp.height+8));
        }
        else
        {
            limitesresp.left=limitestexto.left+limitestexto.width;
            limitesresp.width=995-limitestexto.width;
            contorno.setPosition(limitestexto.left-4,limites.top-4);
            contorno.setSize(sf::Vector2f(limitestexto.width+8,limitesresp.height+8));
        }
    }
    else
    {
        limitesresp.left=300;
        if(maior)
        {
            limitesresp.width=995-limites.width;
            contorno.setPosition(limites.left-4,limites.top-4);
            contorno.setSize(sf::Vector2f(limites.width+8,limitesresp.height+8));
        }
        else
        {
            limitesresp.width=995-limitestexto.width;
            contorno.setPosition(limitestexto.left-4,limites.top-4);
            contorno.setSize(sf::Vector2f(limitestexto.width+8,limitesresp.height+8));
        }
    }
};

void ImagemTexto::Enquadra(int off)
{
    sprite.move(0,-10-off);
    limitestexto.top-=(10+off);
    Texto::Enquadra(off);
};

void ImagemTexto::Desenha(sf::RenderWindow& window,float off)
{
    bool muda=(off!=0)? 1:0;
    if(muda)
    {
        contorno.move(0,off);
        sprite.move(0,off);
        texto.move(0,off);
    }
    window.draw(contorno);
    window.draw(sprite);
    window.draw(texto);
    if(muda)
    {
        sprite.move(0,-off);
        texto.move(0,-off);
        contorno.move(0,-off);
    }
    if(tiporesp)
        resp->Desenha(window,off);
};

int ImagemTexto::Clicou(sf::Vector2f& posimouse,bool botao,float off,int &aux)
{
    bool muda=(off!=0)? 1:0;
    if(muda)
        limitestexto.top+=off;
    if(limitestexto.contains(posimouse))
    {
        if(botao)
        {
            //std::wstring reserva(texto.getString());
            sf::Clipboard::setString(texto.getString());
        }
        else if(hiperlink)
        {
            std::string s="start ";
            std::string s2=texto.getString();
            for(auto &a:s2)
                if(a!='\n')
                    s+=a;
            ShellExecute(0, 0, s2.c_str(), 0, 0 , SW_SHOW );
        }
        if(muda)
            limitestexto.top-=off;
        return 1;
    }
    else
    {
        if(muda)
        {
            limites.top+=off;
            limitestexto.top-=off;
        }
    } 
    if(limites.contains(posimouse))
    {
        if(botao)
        {
            if(muda)
                limites.top-=off;
            return Imagem::Clicou(posimouse,botao,off,aux);
        }
        limites.top-=off;
        return 2;
    }
    else
    {
        if(muda)
        {
            limites.top-=off;
            limitesresp.top+=off;
        }
    }
    if(limitesresp.contains(posimouse))
    {
        aux=nmensagem;
        if(muda)
            limitesresp.top-=off;
        return 3;
    }
    if(muda)
        limitesresp.top-=off;
    if(tiporesp)
    {
        if(muda)
            resp->limites.top+=off;
        if(resp->limites.contains(posimouse))
        {
            if(muda)
                resp->limites.top-=off;
            aux=resp->nmensagem;
            return 4;
        }
        if(muda)
            resp->limites.top-=off;
        return 0;
    }
    return 0;
};

sf::Packet ImagemTexto::Envia(int dest,bool conhecido,int mandaresp)
{
    sf::Packet pacote;
    pacote<<4<<dest<<conhecido<<offset;
    sf::Uint64 t=tam;
    pacote<<t;
    pacote<<tx<<ty;
    for(long unsigned int i=0; i<tam; i++)
        pacote<<vetorpixels[i];
    std::wstring s=texto.getString();
    pacote<<linhas<<s<<primeiroenter<<mandaresp;
    return pacote;
};
