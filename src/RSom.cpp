#include "RSom.hpp"
#include <iostream>
RSom::RSom(int f,sf::Text& d,bool p,int n,Mensagem* o):Resposta(n,o,std::wstring(L"\u25B8 "+d.getString()))
{
    if(!p)
    {
        contorno.setPosition(305,675-f);
        texto.setPosition(340,678-f);
    }
    else
    {
        contorno.setPosition(995,675-f);
        texto.setPosition(1025,678-f);
    }
    limites=contorno.getGlobalBounds();
}

void RSom::Enquadra(int off)
{
    texto.move(0,-10-off);
    contorno.move(0,-10-off);
    limites.top-=(10+off);
};

void RSom::Desenha(sf::RenderWindow& window,float off)
{
    bool muda=(off!=0)? 1:0;
    if(muda)
    {
        contorno.move(0,off);
        texto.move(0,off);
    }
    window.draw(contorno);
    window.draw(texto);
    if(muda)
    {
        contorno.move(0,-off);
        texto.move(0,-off);
    }
}
