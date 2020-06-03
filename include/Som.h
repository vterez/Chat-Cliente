#ifndef SOM_H
#define SOM_H

#include "Mensagem.h"
#include <SFML/Audio.hpp>


class Som : public Mensagem
{
    public:
        Som(sf::Int16* v,sf::Uint64 sc,unsigned int cc,unsigned int sr,bool p,sf::RectangleShape& r);
        Som(const sf::SoundBuffer& buffer,bool p,sf::RectangleShape& r,sf::Text& t);
        virtual ~Som(){};

        static int nmusic;
        sf::Music music;
        std::string nome;
        sf::RectangleShape ret;
        sf::CircleShape botaoplay;
        sf::Text duracao;

        void Enquadra(int off) override;
        void Desenha(sf::RenderWindow& window,float off) override;
        int Clicou(sf::Vector2f& posimouse,bool botao,float off) override;
        sf::Packet Envia(int dest,bool conhecido);

    protected:

    private:
};

#endif // SOM_H
