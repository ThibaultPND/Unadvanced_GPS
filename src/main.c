/***************************************************************
**  Auteur : Thibault Chassagne
**  MAJ : 15/03/2024
*
**  Nom : Unadvanced GPS
*
*?  Description : Ce logiciel à pour but de crée un simulateur de GPS
*
gcc -o build\prog -Iinclude -Iinclude/SDL2 src/*.c -Llib -lSDL2main -lSDL2 -static -DSDL_MAIN_HANDLED && build\monProgramme.exe
***************************************************************/
#include <Project.h>
#include <SDL2/SDL.h>
#include "TxUtils.h"
#include "node.h"
#include "button.h"

cursor_t cursor_mode;

void cursorToNode(void *args)
{
    cursor_mode = NODE;
}
void cursorToMove(void *args)
{
    cursor_mode = MOVE;
}
void cursorToLink(void *args)
{
    cursor_mode = LINK;
}
void buttonStart(void *args)
{
    cursor_mode = START;
}
void buttonEnd(void *args)
{
    cursor_mode = END;
}

int main(int argc, char *argv[])
{
    printf("Debut du programme...\n\n");

    // Initialisation
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        // Erreur init SDL.
    }

    window = SDL_CreateWindow("GPS 6D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LARGEUR_FENETRE, HAUTEUR_FENETRE, 0);
    if (window == NULL)
    {
        SDL_Quit();
        // Erreur lors de la création de la fenêtre.
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        // Erreur création du rendu
    }
    srand(SDL_GetTicks());
    SDL_Point mouse;

    // Creation des  boutons
    int button_count = 5;
    Button boutons[20];
    initButton(&boutons[0], 794, 185, 160, 57, "assets/node_button.bmp", &cursorToNode);
    initButton(&boutons[1], 794, 265, 160, 57, "assets/move_button.bmp", &cursorToMove);
    initButton(&boutons[2], 794, 345, 160, 57, "assets/link_button.bmp", &cursorToLink);
    initButton(&boutons[3], 794, 415, 78, 78, "assets/start_button.bmp", &buttonStart);
    initButton(&boutons[4], 876, 415, 78, 78, "assets/end_button.bmp", &buttonEnd);

    // Initialisation des noeuds
    NodeList *nodes = CreateNodeList();

    // Mode de la souris
    cursor_mode = NODE;

    // Programme
    SDL_bool programLaunched = SDL_TRUE;
    while (programLaunched)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                programLaunched = SDL_FALSE;
                break;
            case SDL_MOUSEBUTTONDOWN:
                SDL_GetMouseState(&mouse.x, &mouse.y);
                if (mouse.x >= LARGEUR_FENETRE - LARGEUR_GUI - 10) // 10 = Epsilon
                {
                    handleButtons(mouse, boutons, button_count);
                }
                else
                {
                    handleNodeClickAction(mouse, &nodes, cursor_mode);
                }
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_RETURN:
                    // Verifie les conditions de recherche.
                    if (SDL_FALSE)
                    {
                        ErrorBox("Les points de départ et d'arrivé ne sont pas tous défini. \nVeuillez utiliser les boutons de la souris pour les mettre en place");
                        break;
                    }
                    // Lancement de la recherche.
                    break;
                case SDLK_BACKSPACE:
                    system("cls");
                    ClearNodeList(&nodes);
                    printf("cleared\n");
                }
                break;
            default:
                break;
            }
        }

        // Boucle de rendu.
        SDL_SetRenderDrawColor(renderer, 0xF0, 0xF0, 0xF0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        renderMap(nodes);
        renderCursorMode(cursor_mode);
        drawButton(boutons, button_count);
        SDL_RenderPresent(renderer);

        // Limitation du tps du programme à 60.
        SDL_Delay(16);
    }

    // Libération des ressources
    NodeList *current = nodes;
    NodeList *next = NULL;
    while (current != NULL)
    {
        next = current->next;
        ClearNodeList(&current->node->neighbours);
        free(current->node);
        free(current);
        current = next;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    printf("...fin du programme\n");

    return EXIT_SUCCESS;
}