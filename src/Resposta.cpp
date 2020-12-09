#include "Resposta.hpp"
//#include "Mensagem.hpp"

Resposta::Resposta(int n,Mensagem* o,std::wstring&& t):nmensagem(n),contorno(sf::Vector2f(310,30)),original(o),texto(t,fonte,20)
{
    contorno.setFillColor(sf::Color(150,255,189));
}

Resposta::Resposta(int n,Mensagem* o,std::wstring& t):nmensagem(n),contorno(sf::Vector2f(310,30)),original(o),texto(t,fonte,20)
{
    contorno.setFillColor(sf::Color(150,255,189));
}
sf::Font Resposta::fonte=sf::Font();