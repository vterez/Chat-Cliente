#include "Envio.h"
#include "Recebimento.h"
int main(){
	SetConsoleCP(1252);
    SetConsoleOutputCP(1252);

    if(!Inicializa())
        return -1;

    std::thread t1(Recebimento::Recebe);
    std::thread t2(Envio::EnviaCheck);

    float alturabase=0;
    bool mandandoprint=false,scrolling=false,pesquisando=false;
    bool controledesenho=0,mostrabarrinha=0;
    sf::Vector2f posimouse;
    float delta;

    sf::RenderWindow window(sf::VideoMode(1300,800),"Chat");
    window.setFramerateLimit(30);
    float dt = 1.f/100.f; // Modify this to change physics rate.
    float accumulator = 0.f;
    bool drawn = false;
    sf::Clock clock;
    while(window.isOpen())
    {
        accumulator += clock.restart().asSeconds();
        while(accumulator>=dt)
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                switch(event.type)
                {
                case sf::Event::Closed:
                {
                    window.close();
                    running=false;
                    break;
                }
                case sf::Event::MouseButtonPressed:
                {
                    posimouse=(sf::Vector2f)sf::Mouse::getPosition(window);
                    bool botaodireito;
                    if(event.mouseButton.button == sf::Mouse::Left)
                        botaodireito=0;
                    else
                        botaodireito=1;
                    if(!mandandoprint&&!visualizaprint)
                    {
                        if(limgravador.contains(posimouse))
                        {
                            std::thread(Envio::GravaSom).detach();
                        }
                        else if(posimouse.x<300)
                        {
                            if(limpesquisa.contains(posimouse))
                                pesquisando=true;
                            else
                            {
                                std::lock_guard<std::mutex> lk(travaconversas);
                                for(int a=0; a<8; a++)
                                    if(limconversas[a].contains(posimouse))
                                    {
                                        if(idsconversas[a]!=-1)
                                        {
                                            if(botaodireito)
                                                std::thread(MudaNome,idsconversas[a]).detach();
                                            else
                                            {
                                                cvatual=idsconversas[a];
                                                std::lock_guard<std::mutex> lk2(travanomes);
                                                auto& contatoatual=contatos[cvatual];
                                                contatoatual.novasmensagens=false;
                                                existenotificacao[a]=false;
                                                nomecvatual.setString(contatoatual.nome);
                                                alturabase=-contatoatual.offsetnaovisto;
                                                if(alturabase>675)
                                                {
                                                    alturabase-=200;
                                                    int poslinha=506;
                                                    int offset=contatoatual.chatoffset-30;
                                                    if(offset<0&&alturabase>-offset)
                                                    {
                                                        while(alturabase>-offset)
                                                        {alturabase-=5;
                                                        poslinha-=5;}
                                                    }
                                                    contatoatual.offsetnaovisto=0;
                                                    if(alturabase!=0)
                                                    {
                                                        scrolling=true;
                                                        mostrabarrinha=true;
                                                        linha.setPosition(300,poslinha);
                                                    }
                                                }
                                                else
                                                {
                                                    linha.setPosition(300,705-alturabase);
                                                    alturabase=0;
                                                    contatoatual.offsetnaovisto=0;
                                                    mostrabarrinha=true;
                                                }
                                            }
                                        }
                                        break;
                                    }
                                pesquisanome=false;
                            }
                        }
                        else
                        {
                            std::thread(ChecaClique,posimouse,botaodireito,alturabase).detach();
                        }
                    }
                    else if(mandandoprint&&!visualizaprint)
                    {
                        if(limenvia.contains(posimouse))
                        {
                            mandandoprint=false;
                            if(ncarac==0&&nlinhas==1)
                            {
                                fila.push_back(std::make_unique<ImagemPura>(vprint,1,tx,ty,tam));
                                filadest.push_back(cvatual);
                            }
                            else
                            {
                                Envio::FormataTexto();
                                fila.push_back(std::make_unique<ImagemTexto>(vprint,stringmensagem,1,tx,ty,tam,215+23*nlinhas,retpadrao2,nlinhas));
                                filadest.push_back(cvatual);
                                ncarac=0;
                                nlinhas=1;
                                stringmensagem.clear();
                                textomensagem.setString(stringmensagem);
                            }
                            vprint=nullptr;
                            if(!enviando)
                            {
                                std::thread(Envio::Envia).detach();
                            }
                        }
                        if(limcancela.contains(posimouse))
                        {
                            delete[] vprint;
                            tprint.~Texture();
                            spriteprint.~Sprite();
                            mandandoprint=false;
                        }
                    }
                    else if(visualizaprint)
                    {
                        if(limfecha.contains(posimouse))
                            visualizaprint=false;
                    }
                    break;
                }
                case sf::Event::KeyPressed:
                {
                    switch(event.key.code)
                    {
                    case sf::Keyboard::Enter:
                    {
                        if(!event.key.shift)
                        {
                            if(mandandoprint)
                            {
                                if(ncarac==0&&nlinhas==1)
                                {
                                    fila.push_back(std::make_unique<ImagemPura>(vprint,1,tx,ty,tam));
                                    filadest.push_back(cvatual);
                                }
                                else
                                {
                                    Envio::FormataTexto();
                                    fila.push_back(std::make_unique<ImagemTexto>(vprint,stringmensagem,1,tx,ty,tam,215+23*nlinhas,retpadrao2,nlinhas));
                                    filadest.push_back(cvatual);
                                    ncarac=0;
                                    nlinhas=1;
                                    stringmensagem.clear();
                                    textomensagem.setString(stringmensagem);
                                    textomensagem.setPosition(300,710);
                                }
                                mandandoprint=false;
                                vprint=nullptr;
                                if(!enviando)
                                {
                                    std::thread(Envio::Envia).detach();
                                }
                                break;
                            }
                            if(!pesquisando)
                            {
                                if(nlinhas==1&&ncarac==0)
                                    break;
                                Envio::FormataTexto();
                                fila.push_back(std::make_unique<Texto>(stringmensagem,1,retpadrao2,nlinhas));
                                filadest.push_back(cvatual);
                                nlinhas=1;
                                ncarac=0;
                                textomensagem.setPosition(300,710);
                                if(!enviando)
                                {
                                    std::thread t3(Envio::Envia);
                                    t3.detach();
                                }
                                stringmensagem.clear();
                            }
                            else
                            {
                                std::lock_guard<std::mutex> lockguard(travaconversas);
                                if(stringpesquisa.front()=='#')
                                {
                                    int cod=std::stoi(stringpesquisa.substr(1));
                                    if(contatos.count(cod)==0)
                                    {
                                        ordemconversas.push_front(cod);
                                        contatos.emplace(cod,cod);
                                        cvatual=cod;
                                        sf::Packet pacote;
                                        pacote<<7<<cod;
                                        while(soquete.send(pacote)!=sf::Socket::Status::Done) {};
                                    }
                                    else if(cod!=idsconversas[0])
                                    {
                                        std::list<int>::iterator itremover;
                                        for(auto it=ordemconversas.begin(); it!=ordemconversas.end(); ++it)
                                        {
                                            if((*it)==cod)
                                            {
                                                itremover=it;
                                                break;
                                            }
                                        }
                                        cvatual=cod;
                                        ordemconversas.erase(itremover);
                                        ordemconversas.push_front(cod);
                                        int a=0;
                                        std::lock_guard<std::mutex> lk(travanomes);
                                        for(auto i:ordemconversas)
                                        {
                                            idsconversas[a]=i;
                                            auto& contatoatual=contatos[i];
                                            textosidsconversas[a].setString(contatoatual.nome);
                                            existenotificacao[a]=contatoatual.novasmensagens;
                                            a++;
                                            if(a==8)
                                                break;
                                        }
                                    }
                                }
                                else if(stringpesquisa.front()=='@')
                                {
                                    Pesquisa();
                                    pesquisanome=true;
                                }
                                pesquisando=false;
                                stringpesquisa.clear();
                                textopesquisa.setString("");
                            }
                        }
                        else if(nlinhas!=1 || (nlinhas == 1 && ncarac>0))
                        {
                            stringmensagem+='\n';
                            ncarac=0;
                            nlinhas++;
                            if(nlinhas>4)
                                textomensagem.move(0,-23);
                        }
                        textomensagem.setString(stringmensagem);
                        break;
                    }
                    case sf::Keyboard::Escape:
                    {
                        if(pesquisando)
                        {
                            stringpesquisa.clear();
                            textopesquisa.setString("");
                            pesquisando=false;
                        }
                        else if(mandandoprint)
                        {
                            delete[] vprint;
                            tprint.~Texture();
                            spriteprint.~Sprite();
                            mandandoprint=false;
                        }
                        else if(pesquisanome)
                        {
                            std::lock_guard<std::mutex> lk(travaconversas),lk2(travanomes);
                            int a=0;
                            for(auto i:ordemconversas)
                            {
                                idsconversas[a]=i;
                                auto& contatoatual=contatos[i];
                                textosidsconversas[a].setString(contatoatual.nome);
                                existenotificacao[a]=contatoatual.novasmensagens;
                                a++;
                                if(a==8)
                                    break;
                            }
                            pesquisanome=false;
                        }
                        else if(!visualizaprint)
                        {
                            Envio::EnviaFinal();
                            window.close();
                            running=false;
                        }
                        else
                            visualizaprint=false;
                        break;
                    }
                    case sf::Keyboard::Q:
                    {
                        if(event.key.alt)
                        {
                            {
                                if(ncarac==86)
                                {
                                    stringmensagem+='\t';
                                    stringmensagem+='\n';
                                    nlinhas++;
                                    if(nlinhas>4)
                                        textomensagem.move(0,-23);
                                    ncarac=0;
                                }
                                stringmensagem+='/';
                                ncarac++;
                            }
                            textomensagem.setString(stringmensagem);
                            break;
                        }
                    }
                    case sf::Keyboard::W:
                    {
                        if(event.key.alt)
                        {
                            {
                                if(ncarac==86)
                                {
                                    stringmensagem+='\t';
                                    stringmensagem+='\n';
                                    ncarac=0;
                                    nlinhas++;
                                    if(nlinhas>4)
                                        textomensagem.move(0,-23);
                                }
                                stringmensagem+='?';
                                ncarac++;
                            }
                            textomensagem.setString(stringmensagem);
                        }
                        break;
                    }
                    case sf::Keyboard::Up:
                    {
                        if(nlinhas>4&&textomensagem.getPosition().y<710)
                            textomensagem.move(0,23);
                        break;
                    }
                    case sf::Keyboard::Down:
                    {
                        sf::FloatRect retmensagem=textomensagem.getGlobalBounds();
                        retmensagem.top-=92;
                        if(nlinhas>4&&caixa.getGlobalBounds().intersects(retmensagem))
                            textomensagem.move(0,-23);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                    }
                    break;
                }
                case sf::Event::TextEntered:
                {
                    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)&&!sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
                    {
                        auto c=event.text.unicode;
                        if(c==27)
                            break;
                        alturabase=0;
                        scrolling=false;
                        mostrabarrinha=false;
                        if(!pesquisando)
                        {
                            if(c==8)
                            {
                                if(std::size(stringmensagem)>0)
                                {
                                    if(stringmensagem.back()=='\n')
                                    {
                                        stringmensagem.pop_back();
                                        nlinhas--;
                                        if(nlinhas>3)
                                            textomensagem.move(0,+23);
                                        if(stringmensagem.back()=='\t')
                                        {
                                            stringmensagem.pop_back();
                                            ncarac=86;
                                        }
                                        else
                                        {
                                            ncarac=DesfazEnter();
                                        }
                                    }
                                    else
                                    {
                                        stringmensagem.pop_back();
                                        ncarac--;
                                        if(stringmensagem.back()=='\n')
                                        {
                                            stringmensagem.pop_back();
                                            nlinhas--;
                                            if(nlinhas>3)
                                                textomensagem.move(0,+23);
                                            if(stringmensagem.back()=='\t')
                                            {
                                                stringmensagem.pop_back();
                                                ncarac=86;
                                            }
                                            else
                                            {
                                                ncarac=DesfazEnter();
                                            }
                                        }
                                    }
                                }
                            }
                            else if(c!=13)
                            {
                                if(ncarac==86)
                                {
                                    stringmensagem+='\t';
                                    stringmensagem+='\n';
                                    ncarac=0;
                                    nlinhas++;
                                    if(nlinhas>4)
                                        textomensagem.move(0,-23);
                                }
                                stringmensagem+=c;
                                ncarac++;
                            }
                            textomensagem.setString(stringmensagem);
                        }
                        else
                        {
                            if(c==8&&stringpesquisa.size()!=0)
                                stringpesquisa.pop_back();
                            else if(c!=13&&c!=8)
                                stringpesquisa+=c;
                            textopesquisa.setString(stringpesquisa);
                        }
                    }
                    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::V))
                    {
                        if(Envio::GetFromClipboard())
                        {
                            mandandoprint=true;
                            scrolling=false;
                            tprint=sf::Texture();
                            tprint.create(tx,ty);
                            tprint.update(vprint);
                            tprint.setSmooth(1);
                            spriteprint=sf::Sprite(tprint);
                            spriteprint.setOrigin(poenomeio(spriteprint));
                            if(ty>600||tx>1000)
                            {
                                if(1000/tx<600/ty)
                                    spriteprint.setScale(1000/tx,1000/tx);
                                else
                                    spriteprint.setScale(600/ty,600/ty);
                            }
                            spriteprint.setPosition(795,305);
                        }
                        textomensagem.setString(stringmensagem);
                    }
                    break;
                }
                case sf::Event::MouseWheelScrolled:
                {
                    delta=event.mouseWheelScroll.delta;
                    posimouse.x=event.mouseWheelScroll.x;
                    posimouse.y=event.mouseWheelScroll.y;
                    if(posimouse.x>300)
                    {
                        if(posimouse.y>710)
                        {
                            if(delta<0)
                                textomensagem.move(0,3*delta);
                            else if(textomensagem.getPosition().y<710)
                                textomensagem.move(0,3*delta);
                        }
                        else if(!mandandoprint&&!visualizaprint)
                        {
                            mostrabarrinha=false;
                            int offset=contatos[cvatual].chatoffset-30;
                            if(delta>0&&offset<0&&alturabase<-offset)
                            {
                                alturabase+=15*delta;
                                while(alturabase>-offset)
                                    alturabase-=15;
                            }
                            else if(delta<0)
                            {
                                alturabase+=15*delta;
                            }
                            textomensagem.setPosition(300,710);
                            if(alturabase<0)
                                alturabase=0;
                            if(alturabase!=0)
                                scrolling=true;
                            else
                                scrolling=false;
                        }
                    }
                    else
                    {
                        std::lock_guard<std::mutex> lk(travaconversas);
                        if(delta>0)
                        {
                            scrollcontatos-=delta;
                            if(scrollcontatos<0)
                                scrollcontatos=0;
                        }
                        else
                        {
                            int tlist=ordemconversas.size();
                            if(tlist>8)
                            {
                                scrollcontatos-=delta;
                                while(tlist-scrollcontatos<8)
                                    scrollcontatos--;
                            }
                        }
                        auto ite=ordemconversas.begin();
                        std::advance(ite,scrollcontatos.load());
                        int a=0;
                        std::lock_guard<std::mutex> lk2(travanomes);
                        for(auto it=ite; it!=ordemconversas.end(); ++it)
                        {
                            idsconversas[a]=(*it);
                            auto& contatoatual=contatos[(*it)];
                            textosidsconversas[a].setString(contatoatual.nome);
                            existenotificacao[a]=contatoatual.novasmensagens;
                            a++;
                            if(a==8)
                                break;
                        }
                    }
                }

                default:
                {
                    break;
                }
                }
            }
            accumulator-=dt;
            drawn=false;
        }
        if(drawn)
            sf::sleep(sf::seconds(0.01f));
        else
        {
            drawn=1;
            window.clear(sf::Color(232,111,130));
            if(visualizaprint)
            {
                window.draw(spriteprint);
                window.draw(spritefecha);
            }
            else if(scrolling)
            {
                if(cvatual!=-1)
                {
                    travachat.lock();
                    auto& chat=contatos[cvatual].chat;
                    for(auto it=chat.rbegin(); it!=chat.rend(); it++)
                    {
                        if((*it)->posi>-alturabase)
                        {
                            controledesenho=true;
                            (*it)->Desenha(window,alturabase);
                        }
                        else if(controledesenho)
                        {
                            controledesenho=false;
                            break;
                        }
                    }
                    travachat.unlock();
                }
                window.draw(caixa);
                window.draw(textomensagem);
                window.draw(retmic);
                window.draw(circulomic);
                travaconversas.lock();
                for(int i=0; i<8; i++)
                {
                    window.draw(retcontato[i]);
                    window.draw(textosidsconversas[i]);
                    if(existenotificacao[i])
                        window.draw(notificacao[i]);
                }
                travaconversas.unlock();
                window.draw(info);
                window.draw(textoantespesquisa);
                window.draw(caixapesquisa);
                window.draw(textopesquisa);
                window.draw(idfixo);
                window.draw(nomefixo);
                window.draw(retcvatual);
                window.draw(nomecvatual);
                if(mostrabarrinha)
                    window.draw(linha);
            }
            else if(!mandandoprint)
            {
                window.draw(caixa);
                window.draw(textomensagem);
                window.draw(cobre);
                window.draw(retmic);
                window.draw(circulomic);
                if(cvatual!=-1)
                {
                    travachat.lock();
                    auto& chat=contatos[cvatual].chat;
                    for(auto it=chat.rbegin(); it!=chat.rend(); it++)
                    {
                        if((*it)->posi>-alturabase)
                        {
                            controledesenho=true;
                            (*it)->Desenha(window,alturabase);
                        }
                        else if(controledesenho)
                        {
                            controledesenho=false;
                            break;
                        }
                    }
                    travachat.unlock();
                }
                travaconversas.lock();
                for(int i=0; i<8; i++)
                {
                    window.draw(retcontato[i]);
                    window.draw(textosidsconversas[i]);
                    if(existenotificacao[i])
                        window.draw(notificacao[i]);
                }
                travaconversas.unlock();
                window.draw(info);
                window.draw(textoantespesquisa);
                window.draw(caixapesquisa);
                window.draw(textopesquisa);
                window.draw(idfixo);
                window.draw(nomefixo);
                window.draw(retcvatual);
                window.draw(nomecvatual);
                if(mostrabarrinha)
                    window.draw(linha);
            }
            else
            {
                window.draw(caixa);
                window.draw(textomensagem);
                window.draw(cobre);
                window.draw(retenvia);
                window.draw(retcancela);
                window.draw(textoenvia);
                window.draw(textocancela);
                window.draw(spriteprint);
            }
            window.display();
        }
    }
    t1.join();
    t2.join();
}