#include "Envio.h"
#include <iostream>
#include <fstream>
using namespace Envio;

void Envio::FormataTexto()
{
    std::wstring formatado;
    int tam=std::size(stringmensagem);
    ncarac=0;
    nlinhas=1;
    int ncaracfinal=0;
    short int ultimoespaco=-1;
    for(int i=0; i<tam; i++)
    {
        wchar_t c;
        if(ncarac==27)
        {
            if(ultimoespaco!=-1)
            {
                formatado[ultimoespaco]='\n';
                ncarac=26-ultimoespaco;
                nlinhas++;
            }
            else
            {
                formatado+='\n';
                ncaracfinal++;
                ncarac=0;
                nlinhas++;
            }
        }
        c=stringmensagem[i];
        if(c=='\t'&&stringmensagem[i+1]=='\n')
            i++;
        else
        {
            formatado+=c;
            ncarac++;
            ncaracfinal++;
            if(c=='\n')
            {
                ncarac=0;
                nlinhas++;
            }
            if(c==' ')
                ultimoespaco=ncaracfinal-1;
        }
    }
    stringmensagem=formatado;
}


void Envio::Envia ()
{
    enviando=1;
    while(!fila.empty())
    {
        int offset=fila.front()->offset;//deslocamento pra cima
        int dest=filadest.front();//destino
        auto &contatoatual=contatos[dest];
        sf::Packet pacote;
        bool travado=false;//controle para evitar acesso/modificacao na cvatual caso o destino seja ela
        contatoatual.chatoffset-=(10+offset);
        contatoatual.offsetnaovisto=0;
        if(dest==cvatual)
        {
            travachat.lock();//trava o mutex pra nao desenhar coisas erradas
            travado=true;//caso mude no meio da operacao
        }
        for(auto &x:contatoatual.chat)
            x->Enquadra(offset);//aplica o offset em todos os membros do chat atual
        if(dest!=id)
        {
            //cria o pacote e envia, caso nao seja o proprio id
            pacote=fila.front()->Envia(dest,contatoatual.nomeconhecido);
            while(soquete.send(pacote)!=sf::Socket::Status::Done)
            {
                if(!running)
                    return;
            }
        }
        contatoatual.chat.push_back(std::move(fila.front()));//move a mensagem da fila de envio para o chat correspondente
        if(travado)
            travachat.unlock();
        std::lock_guard<std::mutex> lk(travaconversas);
        if(idsconversas[0]!=dest)//se o destino nao for a conversa do topo, reorganiza as conversas
        {
            ordemconversas.remove(dest);
            ordemconversas.push_front(dest);
            int a=0;
            while(pesquisanome)
            {
                sf::sleep(sf::seconds(0.5f));
            }
            std::lock_guard<std::mutex> lk2(travanomes);
            for(auto i:ordemconversas)
            {
                idsconversas[a]=i;
                auto& contatoatual2=contatos[i];
                textosidsconversas[a].setString(contatoatual2.nome);
                existenotificacao[a]=contatoatual2.novasmensagens;
                a++;
                if(a==8)
                    break;
            }
        }
        else
        {
            contatoatual.novasmensagens=false;
            existenotificacao[0]=false;
        }
        fila.pop_front();
        filadest.pop_front();
    }
    enviando=0;
}
sf::Uint8* Envio::Colaimagem(HBITMAP hBitmap,float &tx,float &ty,long unsigned int &tam)
{
    OpenClipboard(NULL);
    hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
    CloseClipboard();
    //Create a DC to get hBitmap information
    HDC hDC = GetDC(nullptr);

    //Create BITMAPINFO variable, set size
    BITMAPINFO MyBMInfo = {};
    MyBMInfo.bmiHeader.biSize = sizeof( MyBMInfo.bmiHeader );

    //Get the BITMAPINFO structure from the bitmap
    GetDIBits(hDC, hBitmap, 0, 0, nullptr, &MyBMInfo, DIB_RGB_COLORS);

    //Create the bitmap pixel array each element is [b,g,r]
    BYTE* lpPixels = new BYTE[MyBMInfo.bmiHeader.biSizeImage];

    //Setting up the structure of the buffer to be received
    MyBMInfo.bmiHeader.biCompression = BI_RGB;  // No-compression

    //Now get the actual data from the picture
    GetDIBits(hDC, hBitmap, 0, MyBMInfo.bmiHeader.biHeight, (LPVOID)lpPixels, &MyBMInfo, DIB_RGB_COLORS);

    tam=MyBMInfo.bmiHeader.biSizeImage;
    //Now create an array of SFML pixels we want to fill
    sf::Uint8 *lpPixelWithAlpha = new sf::Uint8[tam]; //Add room for alpha
    sf::Uint8 *vfinal = new sf::Uint8[tam];
    //Loop through each pixel, with steps of four RGBA!
    int u=MyBMInfo.bmiHeader.biSizeImage-1;
    for(long unsigned int x=0; x<MyBMInfo.bmiHeader.biSizeImage; x+=4)
    {
        lpPixelWithAlpha[u-3] = lpPixels[x+2];    //lpPixels = r
        lpPixelWithAlpha[u-2] = lpPixels[x+1];    //lpPixels = g
        lpPixelWithAlpha[u-1] = lpPixels[x];    //lpPixels = b
        lpPixelWithAlpha[u] = lpPixels[x+3];        //Nada alpha (just to adjust if you like)
        u-=4;
    }
    tx=MyBMInfo.bmiHeader.biWidth;
    ty=MyBMInfo.bmiHeader.biHeight;
    for(long unsigned int l=0; l<ty; l++)
    {
        int k=0,atual=0;
        for(long unsigned int c=0; c<4*tx; c++)
        {
            if(atual==0)
                k++;
            vfinal[(int)(l*4*tx+c)]=lpPixelWithAlpha[(int)((l+1)*4*tx-4*k+atual)];
            atual=(atual+1)%4;
        }

    }

    //Remove old DIBsection
    delete[] lpPixels;
    delete[] lpPixelWithAlpha;

    //Release the DC
    ReleaseDC(::GetDesktopWindow(), hDC);

    return vfinal;
};

bool Envio::GetFromClipboard()
{
    std::wstring s=sf::Clipboard::getString();
    if(!s.empty())
    {
        for(auto &c:s)
        {
            if(ncarac>=86)
            {
                stringmensagem+='\t';
                stringmensagem+='\n';
                nlinhas++;
                ncarac=0;
            }
            if(c=='\n')
            {
                stringmensagem+='\n';
                nlinhas++;
                ncarac=0;
            }
            else if(c=='\t')
            {
                stringmensagem+=' ';
                stringmensagem+=' ';
                stringmensagem+=' ';
                stringmensagem+=' ';
                ncarac+=4;
            }
            else if(c>9)
            {
                stringmensagem+=c;
                ncarac++;
            }
        }
        return 0;
    }
    OpenClipboard(NULL);
    HBITMAP hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
    CloseClipboard();
    if(!hBitmap)
        return 0;
    vprint=Envio::Colaimagem(hBitmap,tx,ty,tam);
    return 1;
}
void Envio::EnviaFinal()
{
    sf::Packet pacote;
    pacote<<6;
    soquete.send(pacote);
}

void Envio::EnviaCheck()
{
    while(running)
    {
        sf::sleep(sf::seconds(5.f));
        sf::Packet pacote;
        pacote<<5;
        soquete.send(pacote);
    }
}

void Envio::AtualizaContador(sf::Text& texto)
{
    int segundos=0,minutos=0;
    while(1)
    {
        sf::sleep(sf::seconds(1.f));
        segundos++;
        if(segundos==60)
        {
            segundos=0;
            minutos++;
        }
        if(!gravando)
            return;
        editando=1;
        if(segundos<10)
            texto.setString(std::to_string(minutos)+":0"+std::to_string(segundos));
        else
            texto.setString(std::to_string(minutos)+":"+std::to_string(segundos));
        editando=0;
    }
}
void Envio::GravaSom()
{
    sf::SoundBufferRecorder recorder;
    sf::Text contagem;
    contagem.setFont(fonte);
    contagem.setCharacterSize(20);
    contagem.setString("0:00");
    contagem.setFillColor(sf::Color::Black);
    contagem.setPosition(20,0);
    sf::RenderWindow window(sf::VideoMode(230,90),"Gravador",sf::Style::None);
    window.setFramerateLimit(10);
    recorder.start();
    gravando=true;
    std::thread(Envio::AtualizaContador,std::ref(contagem)).detach();
    while(window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::KeyPressed && event.key.code==sf::Keyboard::Escape)
            {
                gravando=0;
                recorder.stop();
                window.close();
                return;
            }
            if(event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2f posimouse=(sf::Vector2f)sf::Mouse::getPosition(window);
                if(limenvia.contains(posimouse))
                {
                    gravando=0;
                    recorder.stop();
                    fila.push_back(std::make_unique<Som>(recorder.getBuffer(),1,retpadrao2,contagem));
                    filadest.push_back(cvatual);
                    if(!enviando)
                    {
                        std::thread(Envio::Envia).detach();
                    }
                    window.close();
                    return;
                }
                if(limcancela.contains(posimouse))
                {
                    gravando=0;
                    recorder.stop();
                    window.close();
                    return;
                }
            }
        }
        while(editando)
        {
            sf::sleep(sf::seconds(0.05));
        };
        window.clear(sf::Color(136,150,157));
        window.draw(retenvia);
        window.draw(retcancela);
        window.draw(textoenvia);
        window.draw(textocancela);
        window.draw(contagem);
        window.display();
    }
}
