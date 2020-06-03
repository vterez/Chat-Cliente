#include "Som.h"
#include <iostream>
int Som::nmusic=0;
Som::Som(sf::Int16* v,sf::Uint64 sc,unsigned int cc,unsigned int sr,bool p,sf::RectangleShape& r):Mensagem(2,p,90)
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
    ret=r;
    r.setSize(sf::Vector2f(300,30));
    if(!p)
    {
        ret.setPosition(305,680);
    }
    else
    {
        ret.setPosition(795,680);
    }
    posi=705;
}
Som::Som(const sf::SoundBuffer& buffer,bool p,sf::RectangleShape& r,sf::Text& t):Mensagem(2,p,30)
{
    //construtor padrao
    nome="sides/audios/"+std::to_string(nmusic)+".wav";
    buffer.saveToFile(nome);
    music.openFromFile(nome);
    nmusic++;
    botaoplay=sf::CircleShape(8,3);
    botaoplay.setFillColor(sf::Color::Red);
    botaoplay.setRotation(210.f);
    ret=r;
    duracao=t;
    if(!p)
    {
        ret.setPosition(305,675);
        botaoplay.setPosition(320,700);
        duracao.setPosition(340,675);
    }
    else
    {
        ret.setPosition(995,675);
        botaoplay.setPosition(1010,700);
        duracao.setPosition(1030,675);
    }
    posi=705;
    limites=ret.getGlobalBounds();

}
void Som::Enquadra(int off)
{
    posi-=(10+off);
    duracao.move(0,-10-off);
    ret.move(0,-10-off);
    botaoplay.move(0,-10-off);
    limites.top-=(10+off);
};

void Som::Desenha(sf::RenderWindow &window,float off)
{
    if(off!=0)
    {
        ret.move(0,off);
        botaoplay.move(0,off);
        duracao.move(0,off);
    }
    window.draw(ret);
    window.draw(botaoplay);
    window.draw(duracao);
    if(off!=0)
    {
        ret.move(0,-off);
        botaoplay.move(0,-off);
        duracao.move(0,-off);
    }
};

int Som::Clicou(sf::Vector2f& posimouse,bool botao,float off)
{
    if(off!=0)
        limites.top+=off;
    if(!limites.contains(posimouse))
    {
        limites.top-=off;
        return 0;
    }
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
};

sf::Packet Som::Envia(int dest,bool conhecido)
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
    return pacote;
}

