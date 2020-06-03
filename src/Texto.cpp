#include "Texto.h"
#include <iostream>
Texto::Texto(std::wstring& s,bool p,sf::RectangleShape& r,int l):Mensagem(1,p,0)
{
    texto=textopadrao;
    if(s.find(L"http://")==0||s.find(L"www.")!=std::wstring::npos||s.find(L"https://")==0||s.find(L".com")!=std::wstring::npos)
    {
        texto.setStyle(sf::Text::Underlined);
        hiperlink=true;
    }
    else
        hiperlink=false;
    texto.setString(s);
    ret=r;
    sf::Rect recttext=texto.getLocalBounds();
    ret.setSize(sf::Vector2f(recttext.width+20,recttext.height+15));
    this->posi=705;
    if(!p)
    {

        ret.setOrigin(0,recttext.height+15);
        ret.setPosition(305,705);
        texto.setOrigin(0,recttext.top+recttext.height);
        texto.setPosition(313,698);
    }
    else
    {
        ret.setOrigin(recttext.width+20,recttext.height+15);
        ret.setPosition(1295,705);
        texto.setOrigin(recttext.width,recttext.top+recttext.height);
        texto.setPosition(1284,698);
    }
    limites=ret.getGlobalBounds();
    offset=limites.height;
}

void Texto::Enquadra(int off)
{
    texto.move(0,-10-off);
    ret.move(0,-10-off);
    this->posi-=(10+off);
    limites.top-=(10+off);
}

void Texto::Desenha(sf::RenderWindow& window,float off)
{
    if(off!=0)
    {
        ret.move(0,off);
        texto.move(0,off);
    }
    window.draw(ret);
    window.draw(texto);
    if(off!=0)
    {
        ret.move(0,-off);
        texto.move(0,-off);
    }
};

int Texto::Clicou(sf::Vector2f& posimouse,bool botao,float off)
{
    if(off!=0)
        limites.top+=off;
    if(!limites.contains(posimouse))
    {
        if(off!=0)
            limites.top-=off;
        return 0;
    }
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
        ShellExecute(0, 0, s2.c_str(), 0, 0, SW_SHOW );
    }
    if(off!=0)
        limites.top-=off;
    return 1;
};

sf::Packet Texto::Envia(int dest,bool conhecido)
{
    sf::Packet pacote;
    pacote<<1<<dest<<conhecido<<offset;
    std::wstring s=texto.getString();
    pacote<<s;
    return pacote;
}
