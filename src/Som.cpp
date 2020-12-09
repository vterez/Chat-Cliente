#include "Som.hpp"
#include "Texto.hpp"
#include "RTexto.hpp"
#include "RSom.hpp"
#include "RImagemTexto.hpp"
#include "RImagemPura.hpp"
#include "ImagemPura.hpp"
#include "ImagemTexto.hpp"
#include <iostream>
int Som::nmusic=0;
Som::Som(sf::Int16* v,sf::Uint64 sc,unsigned int cc,unsigned int sr,bool p,sf::RectangleShape& r):Mensagem(2,p,90,-1)
{
    //construtor de load
    sf::SoundBuffer buffer;
    buffer.loadFromSamples(v,sc,cc,sr);
    nome="sides/audios/"+std::to_string(nmusic)+".wav";
    buffer.saveToFile(nome);
    music.openFromFile(nome);
    nmusic++;
    botaoplay=sf::CircleShape(8,3);
    botaoplay.setFillColor(sf::Color::Red);
    contorno=r;
    r.setSize(sf::Vector2f(300,30));
    if(!p)
    {
        contorno.setPosition(305,680);
    }
    else
    {
        contorno.setPosition(795,680);
    }
}
Som::Som(const sf::SoundBuffer& buffer,bool p,sf::Text& t,int n,int R,Mensagem *m,int nr):Mensagem(2,p,30,n,R),duracao(t)
{
    //construtor padrao
    nome="sides/audios/"+std::to_string(nmusic)+".wav";
    buffer.saveToFile(nome);
    music.openFromFile(nome);
    nmusic++;
    contorno.setSize(sf::Vector2f(300,30));
    botaoplay=sf::CircleShape(8,3);
    botaoplay.setFillColor(sf::Color::Red);
    botaoplay.setRotation(210.f);
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
    if(!p)
    {
        contorno.setPosition(305,675);
        botaoplay.setPosition(320,700);
        duracao.setPosition(340,675);
        limites=contorno.getGlobalBounds();
        limitesresp=limites;
        limitesresp.left+=limites.width;
        limitesresp.width=695;
    }
    else
    {
        contorno.setPosition(995,675);
        botaoplay.setPosition(1010,700);
        duracao.setPosition(1030,675);
        limites=contorno.getGlobalBounds();
        limitesresp=limites;
        limitesresp.left=300;
        limitesresp.width=695;
    }
}
void Som::Enquadra(int off)
{
    posi-=(10+off);
    duracao.move(0,-10-off);
    contorno.move(0,-10-off);
    botaoplay.move(0,-10-off);
    limites.top-=(10+off);
    limitesresp.top-=(10+off);
    if(tiporesp>0)
        resp->Enquadra(off);
};

void Som::Desenha(sf::RenderWindow &window,float off)
{
    if(off!=0)
    {
        contorno.move(0,off);
        botaoplay.move(0,off);
        duracao.move(0,off);
    }
    window.draw(contorno);
    window.draw(botaoplay);
    window.draw(duracao);
    if(tiporesp)
    {
        resp->Desenha(window,off);
    }
    if(off!=0)
    {
        contorno.move(0,-off);
        botaoplay.move(0,-off);
        duracao.move(0,-off);
    }
};

int Som::Clicou(sf::Vector2f& posimouse,bool botao,float off, int &nmsg)
{
    if(off!=0)
    {
        limites.top+=off;
    }
    if(limites.contains(posimouse))
    {
        auto status=music.getStatus();
        if(status == sf::Music::Playing)
        {
            if(!botao)
                music.pause();
            else
                music.setPlayingOffset(sf::seconds(0));
        }
        else if(status == sf::Music::Paused)
            music.play();
        else
        {
            music.setPlayingOffset(sf::seconds(0));
            music.play();
        }
        if(off!=0)
            limites.top-=off;
        return 1;
    }
    if(off!=0)
        limitesresp.top+=off;
    if(limitesresp.contains(posimouse))
    {
        nmsg=nmensagem;
        if(off!=0)
            limitesresp.top-=off;
        return 3;
    }
    else if(tiporesp)
    {
        if(off!=0)
            resp->limites.top+=off;
        if(resp->limites.contains(posimouse))
        {
            if(off!=0)
                resp->limites.top-=off;
            nmsg=resp->nmensagem;
            return 4;
        }
        if(off!=0)
            resp->limites.top-=off;
        return 0;
    }
    else
        return 0;
};

sf::Packet Som::Envia(int dest,bool conhecido,int mandaresp)
{
    sf::Packet pacote;
    pacote<<2<<dest<<conhecido<<offset;
    sf::SoundBuffer buffer;
    buffer.loadFromFile(nome);
    auto samples=buffer.getSamples();
    auto samplecount=buffer.getSampleCount();
    auto channelcount=buffer.getChannelCount();
    auto samplerate=buffer.getSampleRate();
    pacote<<samplecount<<channelcount<<samplerate;
    for(unsigned int i=0; i<samplecount; i++)
        pacote<<samples[i];
    pacote<<duracao.getString();
    std::wstring a=duracao.getString();
    pacote<<mandaresp;
    return pacote;
}

