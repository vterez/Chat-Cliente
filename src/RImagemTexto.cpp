#include "RImagemTexto.hpp"

RImagemTexto::RImagemTexto(float f,int nr,bool p,Mensagem* o):Resposta(nr,o,L"\u25A0 Imagem+Texto")
{
    texto.setFillColor(sf::Color::White);
    if(!p)
    {
        contorno.setPosition(305,675-f);
        texto.setPosition(340,678-f);
    }
    else
    {
        contorno.setPosition(985,675-f);
        texto.setPosition(1025,678-f);
    }
    limites=contorno.getGlobalBounds();
}

void RImagemTexto::Enquadra(int off)
{
    texto.move(0,-10-off);
    contorno.move(0,-10-off);
    limites.top-=(10+off);
};

void RImagemTexto::Desenha(sf::RenderWindow& window,float off)
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
