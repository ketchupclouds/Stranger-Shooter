/*
 * funcoes.c
 * 
 *  2017 Lucca, Julia e Paulo
 * 
 */


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>
#include "globais.h"
#include "defs.h"
#include "funcoes.h"
#include "telas.h"

// Sprites

Sprite createSprite(int x, int y, SDL_Surface* loaded){
	Sprite p;
	
	p.x = x;
	p.y = y;
	
	p.image = loaded;
	p.width = loaded->w;
	p.height = loaded->h;
	
	return p;
	}

void draw(Sprite p){
	
	SDL_Rect dstRect;
	
	dstRect.x = p.x;
	dstRect.y = p.y;
	
	SDL_BlitSurface( p.image, NULL, gScreenSurface, &dstRect );
	}



// Collision Functions

int collided(Sprite rect1, Sprite rect2){
	if (rect1.x < rect2.x + rect2.width &&
		rect1.x + rect1.width > rect2.x &&
		rect1.y < rect2.y + rect2.height &&
		rect1.height + rect1.y > rect2.y) {
			return true;
	}
	
	return false;	
}

int collided_circle(Sprite circle1, Sprite circle2){
	
	float circle1r = circle1.height/2;
	float circle2r = circle2.height/2;
	
	float dx = circle1.x - circle2.x;
	float dy = circle1.y - circle2.y;
	float distance = sqrt(dx * dx + dy * dy);

	if (distance < circle1r + circle2r) {
		return true;
	}
	
	return false;
}

// The Game -> Função Principal ( e sim, vc acaba de perder o Jogo)

int theGame (int tela){
	if(tela == 1) menuPrincipal();
	else {
		puts("Código de Tela Inválido");
		return 1;}
	
	return 0;
	}


// FUNÇÕES DO SDL -> FAVOR NÃO ALTERAR 

int init() {
    //flag de inicializacao
    int success = true;

    //Inicie SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else {
        //Crie janela
        gWindow = SDL_CreateWindow( "Stranger Shooter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL ) {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else {
            //Inicie a carga da imagem
            int imgFlags = IMG_INIT_PNG;
            if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
                printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                success = false;
            }
            else {
                //Pegue a superficie da janela
                gScreenSurface = SDL_GetWindowSurface( gWindow );
            }
        }
    }
    return success;
}

int loadMedia(SDL_Surface* *imagem)
{
    //flag de carga 
    int success = true;

    //Carregue a superficie PNG
    *imagem = loadSurface( "./loaded.png" );
    if( imagem == NULL ) {
        printf( "Failed to load PNG image!\n" );
        success = false;
    }
    
    return success;
}

void closing() {
    //libere a imagem carregada
    //SDL_FreeSurface( gPNGSurface );
    //gPNGSurface = NULL;

    //Destrua janela
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    //Quit SDL
    IMG_Quit();
    SDL_Quit();
}

SDL_Surface* loadPNG( char *path ) {
    SDL_Surface* loadedSurface = IMG_Load( path );
    if( loadedSurface == NULL ) {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    }
    else {
        SDL_SetSurfaceBlendMode(loadedSurface, SDL_BLENDMODE_BLEND);
    }
    return loadedSurface;
}

SDL_Surface* loadSurface( char *path ) {
    //A imagem final apos ser otimizada
    SDL_Surface* optimizedSurface = NULL;

    //Carregue a imagem no caminho especificado em path
    SDL_Surface* loadedSurface = IMG_Load( path );
    if( loadedSurface == NULL ) {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    }
    else {
        //Converte a superficie para o formato da tela.
        optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
        if( optimizedSurface == NULL ) {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path, SDL_GetError() );
        }

        //Libera a superficie velha
        SDL_FreeSurface( loadedSurface );
    }
    return optimizedSurface;
}