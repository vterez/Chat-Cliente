#include "ImagemTexto.h"
#include <iostream>
ImagemTexto::ImagemTexto(sf::Uint8* rgba,std::wstring& st,bool p,float x,float y,sf::Uint64 t,int o,sf::RectangleShape& r,int l):Mensagem(4,p,o),Imagem(rgba,p,x,y,t,210),Texto(st,p,r,l)
{
    linhas=l;
    sprite.move(0,-limites.height);
    limitesimagem=sprite.getGlobalBounds();
    this->offset=limites.height+limitesimagem.height;
};

void ImagemTexto::Enquadra(int off)
{
    sprite.move(0,-10-off);
    limitesimagem.top-=(10+off);
    Texto::Enquadra(off);
};

void ImagemTexto::Desenha(sf::RenderWindow& window,float off)
{
    if(off!=0)
    {
        sprite.move(0,off);
        ret.move(0,off);
        texto.move(0,off);
    }
    window.draw(sprite);
    window.draw(ret);
    window.draw(texto);
    if(off!=0)
    {
        sprite.move(0,-off);
        ret.move(0,-off);
        texto.move(0,-off);
    }
};

int ImagemTexto::Clicou(sf::Vector2f& posimouse,bool botao,float off)
{
    if(off!=0)
        limites.top+=off;
    if(limites.contains(posimouse))
    {
        if(botao)
        {
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
        if(off!=0)
            limites.top-=off;
        return 1;
    }
    else if(limitesimagem.contains(posimouse))
    {
        if(botao)
        {
            return Imagem::Clicou(posimouse,botao,off);
        }
        else
            return 2;
    }
    return 0;
};

sf::Packet ImagemTexto::Envia(int dest,bool conhecido)
{
    sf::Packet pacote;
    pacote<<4<<dest<<conhecido<<offset;
    sf::Uint64 t=tam;
    pacote<<t;
    pacote<<tx<<ty;
    for(long unsigned int i=0; i<tam; i++)
        pacote<<vetorpixels[i];
    std::wstring s=texto.getString();
    pacote<<linhas<<s;
    return pacote;
};
