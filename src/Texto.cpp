#include "Texto.hpp"
#include "Som.hpp"
#include "RSom.hpp"
#include "RTexto.hpp"
#include "ImagemTexto.hpp"
#include "ImagemPura.hpp"
#include "RImagemPura.hpp"
#include "RImagemTexto.hpp"
#include <iostream>
Texto::Texto(std::wstring& s,bool p,int l,int n,int p_enter,int R,Mensagem *m,int nr):Mensagem(1,p,0,n,R),texto(s,fonte,20)
{
    primeiroenter=p_enter;
    texto.setFillColor(sf::Color::Black);
    if(s.find(L"http://")==0||s.find(L"www.")!=std::wstring::npos||s.find(L"https://")==0||s.find(L".com")!=std::wstring::npos)
    {
        texto.setStyle(sf::Text::Underlined);
        hiperlink=true;
    }
    else
        hiperlink=false;
    texto.setString(s);
    sf::Rect recttext=texto.getLocalBounds();
    contorno.setSize(sf::Vector2f(recttext.width+20,recttext.height+15));
    if(!p)
    {
        contorno.setOrigin(0,recttext.height+15);
        contorno.setPosition(305,705);
        texto.setOrigin(0,recttext.top+recttext.height);
        texto.setPosition(313,698);
        limites=contorno.getGlobalBounds();
        limitesresp.top=limites.top;
        limitesresp.height=limites.height;
        limitesresp.left=limites.left+limites.width;
        limitesresp.width=995-limites.width;
    }
    else
    {
        contorno.setOrigin(recttext.width+20,recttext.height+15);
        contorno.setPosition(1295,705);
        texto.setOrigin(recttext.width,recttext.top+recttext.height);
        texto.setPosition(1284,698);
        limites=contorno.getGlobalBounds();
        limitesresp.top=limites.top;
        limitesresp.height=limites.height;
        limitesresp.left=300;
        limitesresp.width=995-limites.width;
    }
    offset=limites.height;
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
}

void Texto::Enquadra(int off)
{
    texto.move(0,-10-off);
    contorno.move(0,-10-off);
    this->posi-=(10+off);
    limites.top-=(10+off);
    limitesresp.top-=(10+off);
    if(tiporesp)
        resp->Enquadra(off);
}

void Texto::Desenha(sf::RenderWindow& window,float off)
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
    if(tiporesp)
        resp->Desenha(window,off);
};

int Texto::Clicou(sf::Vector2f& posimouse,bool botao,float off,int &aux)
{
    bool muda=(off!=0)? 1:0;
    if(muda)
    {
        limitesresp.top+=off;
    }
    if(limitesresp.contains(posimouse))
    {
        aux=nmensagem;
        if(muda)
            limitesresp.top-=off;
        return 3;
    }
    if(muda)
    {
        limites.top+=off;
        limitesresp.top-=off;
    }
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
            ShellExecute(0, 0, s.c_str(), 0, 0, SW_SHOW );
        }
        if(off!=0)
            limites.top-=off;
        return 1;
    }
    if(muda)
        limites.top-=off;
    if(tiporesp)
    {
        if(muda)
            resp->limites.top+=off;
        if(resp->limites.contains(posimouse))
        {
            if(off!=0)
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

sf::Packet Texto::Envia(int dest,bool conhecido,int mandaresp)
{
    sf::Packet pacote;
    pacote<<1<<dest<<conhecido<<offset;
    std::wstring s=texto.getString();
    pacote<<s<<primeiroenter;
    pacote<<mandaresp;
    return pacote;
}
