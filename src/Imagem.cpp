#include "Imagem.hpp"

Imagem::Imagem(sf::Uint8* rgba,bool p,float x,float y,sf::Uint64 t,int l,int n):Mensagem(3,p,l,n)
{
    vetorpixels=rgba;
    texture.create(x,y);
    texture.update(vetorpixels);
    texture.setSmooth(1);
    sprite=sf::Sprite(texture);
    if(y>210||x>300)
    {
        if(300/x<210/y)
            sprite.setScale(300/x,300/x);
        else
            sprite.setScale(210/y,210/y);
    }
    sf::FloatRect rets=sprite.getLocalBounds();
    propria=p;
    tx=x;
    ty=y;
    tam=t;
    if(!p)
    {
        sprite.setOrigin(0,rets.top+rets.height);
        sprite.setPosition(305,705);
    }
    else
    {
        sprite.setOrigin(rets.left+rets.width,rets.top+rets.height);
        sprite.setPosition(1295,705);
    }
}

int Imagem::Clicou(sf::Vector2f& posimouse,bool botao,float off,int &aux)
{
    char filename[ MAX_PATH ];

    OPENFILENAME ofn;
    ZeroMemory( &filename, sizeof( filename ) );
    ZeroMemory( &ofn,      sizeof( ofn ) );
    ofn.lStructSize  = sizeof( ofn );
    ofn.hwndOwner    = NULL;
    ofn.lpstrFilter  = ".png\0*.*\0";
    ofn.lpstrFile    = filename;
    ofn.nMaxFile     = MAX_PATH;
    ofn.lpstrTitle   = "Selecione o destino e o nome do arquivo";
    ofn.Flags        = OFN_DONTADDTORECENT;

    if (GetSaveFileNameA( &ofn ))
    {
        std::string n=filename;
        n+=".png";
        texture.copyToImage().saveToFile(n);
    }
    return 1;
}
Imagem::~Imagem()
{
    delete[] vetorpixels;
}
