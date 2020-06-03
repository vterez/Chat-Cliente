#ifndef MENSAGEM_H
#define MENSAGEM_H
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <windows.h>

//using namespace std;
class Mensagem
{
public:
    Mensagem(int t,bool p,int o):tipo(t),propria(p),offset(o) {};
    virtual ~Mensagem() {};

    const short int tipo;
    float posi;
    bool propria;
    int offset;
    sf::FloatRect limites;

    virtual void Enquadra(int off)=0;
    virtual void Desenha(sf::RenderWindow& window,float off)=0;
    virtual int Clicou(sf::Vector2f& posimouse,bool botao,float off)=0;
    virtual sf::Packet Envia(int dest,bool conhecido)=0;

    template <typename T>
    sf::Vector2f poenomeio(T t)
    {
        if constexpr (std::is_base_of<sf::Transformable,T>::value)
        {
            sf::Vector2f posi;
            sf::FloatRect rect;
            rect=t.getGlobalBounds();
            posi.x=rect.left+(rect.width)/2;
            posi.y=rect.top+(rect.height)/2;
            return posi;
        }
        return sf::Vector2f(0,0);
    }
};

#endif // MENSAGEM_H
