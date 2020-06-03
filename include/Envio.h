#ifndef ENVIO_H_INCLUDED
#define ENVIO_H_INCLUDED

#include "BaseInclude.h"
#include <windows.h>

namespace Envio
{
void Envia();
sf::Uint8* Colaimagem(HBITMAP hBitmap,float &tx,float &ty,long unsigned int &tam);
bool GetFromClipboard();
void EnviaCheck();
void EnviaFinal();
void GravaSom();
void FormataTexto();
void AtualizaContador(sf::Text& contagem);
}

#endif // ENVIO_H_INCLUDED
