#include "RTexto.hpp"

RTexto::RTexto(float f,int nr,std::wstring& s,bool p,Mensagem *o):Resposta(nr,o,s)
{
    auto ret=texto.getGlobalBounds();
    texto.setFillColor(sf::Color::Black);
    if(!p)
    {
        contorno.setPosition(305,675-f);
        texto.setPosition(305,678-f);
    }
    else
    {
        texto.setOrigin(ret.left+ret.width,0);
        contorno.setPosition(985,675-f);
        texto.setPosition(1285,678-f);
    }
    limites=contorno.getGlobalBounds();
};

void RTexto::Desenha(sf::RenderWindow& window,float off)
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

void RTexto::Enquadra(int off)
{
    texto.move(0,-10-off);
    contorno.move(0,-10-off);
    limites.top-=(10+off);
}