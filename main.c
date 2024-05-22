#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH 715
#define WINDOW_HEIGHT 500
#define PACMAN_SIZE 30
#define PACMAN_SPEED 1
#define ENEMY_SIZE 25
#define ENEMY_SPEED 1.5
#define NUM_ENEMIES 6
#define NUM_PACMANS 1
#define MAX_BARRIERS 50
#define MAX_BAR_WIDTH 20
#define MAX_BAR_HEIGHT 50
#define NUM_NOODLES 4

typedef struct {
    int x, y, dx, dy, lives;
} Pacman;

typedef struct {
    int x, y, dx, dy;
} Enemy;

typedef struct {
    int x, y, width, height;
} Barrier;

typedef struct {
    int x, y;
    bool isVisible;
} Noodle;

// Déclaration des fonctions
void initBarriers();
void handleInput(SDL_Event event);
void updatePacman();
void loadMenuTexture(SDL_Renderer* renderer);
void updateEnemies();
SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* filePath);
bool checkCollision(Enemy* enemy, Pacman* pacman);
bool isSpawningOnBarrier(int x, int y, int size);
void initEnemies(bool* gameOver);
bool canMove(Enemy* enemy, int dx, int dy);
bool detectObstacle(Enemy* enemy, int dx, int dy);
void initNoodles();
void loadNoodleTexture(SDL_Renderer* renderer);
void renderNoodles(SDL_Renderer* renderer);
void checkNoodleCollision();

// Variables globales
Pacman pacmanSingle = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0, 0, 5};
Pacman pacmans[NUM_PACMANS];
Enemy enemies[NUM_ENEMIES];
Noodle noodles[NUM_NOODLES];
SDL_Texture* noodleTexture = NULL;
SDL_Texture* enemyTexture = NULL;
SDL_Texture* pacmanTextureGauche1 = NULL;
SDL_Texture* pacmanTextureGauche2 = NULL;
SDL_Texture* pacmanTextureDerriere = NULL;
SDL_Texture* pacmanTextureDevant1 = NULL;
SDL_Texture* pacmanTextureDevant2 = NULL;
SDL_Texture* pacmanTextureDroit1 = NULL;
SDL_Texture* pacmanTextureDroit2 = NULL;
SDL_Texture* pacmanTextureDebut = NULL;
SDL_Texture* backgroundTexture = NULL;
SDL_Texture* menuTexture = NULL;
SDL_Texture* deathTexture = NULL;
Barrier barriers[MAX_BARRIERS];

// Fonction pour charger les textures de Pacman
void loadPacmanTextures(SDL_Renderer* renderer) {
    const char* filePathGauche1 = "assets/totoro_gauche_1.png";
    const char* filePathGauche2 = "assets/totoro_gauche_2.png";
    const char* filePathDerriere = "assets/totoro_dos.png";
    const char* filePathDevant1 = "assets/totoro_bas_1.png";
    const char* filePathDevant2 = "assets/totoro_bas_2.png";
    const char* filePathDroit1 = "assets/totoro_droit_1.png";
    const char* filePathDroit2 = "assets/totoro_droit_2.png";
    const char* filePathDebut = "assets/totoro.png";

    SDL_Surface* tempSurfaceGauche1 = IMG_Load(filePathGauche1);
    SDL_Surface* tempSurfaceGauche2 = IMG_Load(filePathGauche2);
    SDL_Surface* tempSurfaceDerriere = IMG_Load(filePathDerriere);
    SDL_Surface* tempSurfaceDevant1 = IMG_Load(filePathDevant1);
    SDL_Surface* tempSurfaceDevant2 = IMG_Load(filePathDevant2);
    SDL_Surface* tempSurfaceDroit1 = IMG_Load(filePathDroit1);
    SDL_Surface* tempSurfaceDroit2 = IMG_Load(filePathDroit2);
    SDL_Surface* tempSurfaceDebut = IMG_Load(filePathDebut);

    if (!tempSurfaceGauche1 || !tempSurfaceGauche2 || !tempSurfaceDerriere || !tempSurfaceDevant1 || !tempSurfaceDevant2 || !tempSurfaceDroit1 || !tempSurfaceDroit2 || !tempSurfaceDebut) {
        printf("Impossible de charger l'une des images! Erreur SDL: %s\n", IMG_GetError());
        exit(1);
    }

    pacmanTextureGauche1 = SDL_CreateTextureFromSurface(renderer, tempSurfaceGauche1);
    pacmanTextureGauche2 = SDL_CreateTextureFromSurface(renderer, tempSurfaceGauche2);
    pacmanTextureDerriere = SDL_CreateTextureFromSurface(renderer, tempSurfaceDerriere);
    pacmanTextureDevant1 = SDL_CreateTextureFromSurface(renderer, tempSurfaceDevant1);
    pacmanTextureDevant2 = SDL_CreateTextureFromSurface(renderer, tempSurfaceDevant2);
    pacmanTextureDroit1 = SDL_CreateTextureFromSurface(renderer, tempSurfaceDroit1);
    pacmanTextureDroit2 = SDL_CreateTextureFromSurface(renderer, tempSurfaceDroit2);
    pacmanTextureDebut = SDL_CreateTextureFromSurface(renderer, tempSurfaceDebut);

    SDL_FreeSurface(tempSurfaceGauche1);
    SDL_FreeSurface(tempSurfaceGauche2);
    SDL_FreeSurface(tempSurfaceDerriere);
    SDL_FreeSurface(tempSurfaceDevant1);
    SDL_FreeSurface(tempSurfaceDevant2);
    SDL_FreeSurface(tempSurfaceDroit1);
    SDL_FreeSurface(tempSurfaceDroit2);
    SDL_FreeSurface(tempSurfaceDebut);
}

void initBarriers() {
    // H
    barriers[0].x = 206;
    barriers[0].y = 165;
    barriers[0].width = 57;
    barriers[0].height = 8;

    barriers[1].x = 315;
    barriers[1].y = 164;
    barriers[1].width = 160;
    barriers[1].height = 8;

    barriers[2].x = 260;
    barriers[2].y = 115;
    barriers[2].width = 52;
    barriers[2].height = 8;

    barriers[3].x = 360;
    barriers[3].y = 370;
    barriers[3].width = 90;
    barriers[3].height = 10;

    barriers[4].x = 150;
    barriers[4].y = 416;
    barriers[4].width = 350;
    barriers[4].height = 8;

    barriers[5].x = 130;
    barriers[5].y = 325;
    barriers[5].width = 50;
    barriers[5].height = 10;

    barriers[6].x = 120;
    barriers[6].y = 385;
    barriers[6].width = 35;
    barriers[6].height = 8;

    barriers[7].x = 495;
    barriers[7].y = 350;
    barriers[7].width = 100;
    barriers[7].height = 8;

    barriers[8].x = 550;
    barriers[8].y = 203;
    barriers[8].width = 40;
    barriers[8].height = 8;

    barriers[9].x = 164;
    barriers[9].y = 50;
    barriers[9].width = 325;
    barriers[9].height = 8;

    barriers[10].x = 120;
    barriers[10].y = 150;
    barriers[10].width = 40;
    barriers[10].height = 8;

    barriers[11].x = 490;
    barriers[11].y = 85;
    barriers[11].width = 50;
    barriers[11].height = 8;

    barriers[27].x = 533;
    barriers[27].y = 270;
    barriers[27].width = 50;
    barriers[27].height = 8;

    //V
    barriers[12].x = 115;
    barriers[12].y = 160;
    barriers[12].width = 8;
    barriers[12].height = 235;

    barriers[13].x = 590;
    barriers[13].y = 203;
    barriers[13].width = 8;
    barriers[13].height = 155;

    barriers[14].x = 545;
    barriers[14].y = 90;
    barriers[14].width = 8;
    barriers[14].height = 120;

    barriers[15].x = 490;
    barriers[15].y = 350;
    barriers[15].width = 8;
    barriers[15].height = 70;

    barriers[16].x = 150;
    barriers[16].y = 390;
    barriers[16].width = 8;
    barriers[16].height = 30;

    barriers[17].x = 150;
    barriers[17].y = 60;
    barriers[17].width = 8;
    barriers[17].height = 98;

    barriers[18].x = 260;
    barriers[18].y = 55;
    barriers[18].width = 8;
    barriers[18].height = 60;

    barriers[19].x = 205;
    barriers[19].y = 55;
    barriers[19].width = 8;
    barriers[19].height = 110;

    barriers[20].x = 305;
    barriers[20].y = 122;
    barriers[20].width = 8;
    barriers[20].height = 50;

    barriers[21].x = 475;
    barriers[21].y = 170;
    barriers[21].width = 8;
    barriers[21].height = 80;

    barriers[22].x = 450;
    barriers[22].y = 290;
    barriers[22].width = 8;
    barriers[22].height = 80;

    barriers[23].x = 350;
    barriers[23].y = 220;
    barriers[23].width = 8;
    barriers[23].height = 160;

    barriers[24].x = 180;
    barriers[24].y = 270;
    barriers[24].width = 8;
    barriers[24].height = 60;

    barriers[25].x = 255;
    barriers[25].y = 165;
    barriers[25].width = 8;
    barriers[25].height = 60;

    barriers[26].x = 490;
    barriers[26].y = 50;
    barriers[26].width = 8;
    barriers[26].height = 35;

    // Grosses Barrières 
    barriers[28].x = 553;
    barriers[28].y = 7;
    barriers[28].width = 200;
    barriers[28].height = 200;

    barriers[29].x = 553;
    barriers[29].y = 0;
    barriers[29].width = 200;
    barriers[29].height = 200;

    barriers[30].x = 550;
    barriers[30].y = -20;
    barriers[30].width = 200;
    barriers[30].height = 200;

    barriers[31].x = 595;
    barriers[31].y = 50;
    barriers[31].width = 500;
    barriers[31].height = 500;

    barriers[32].x = 495;
    barriers[32].y = 355;
    barriers[32].width = 200;
    barriers[32].height = 200;

    barriers[33].x = 150;
    barriers[33].y = 420;
    barriers[33].width = 500;
    barriers[33].height = 200;

    barriers[34].x = 0;
    barriers[34].y = 420;
    barriers[34].width = 500;
    barriers[34].height = 200;

    barriers[35].x = 0;
    barriers[35].y = 0;
    barriers[35].width = 115;
    barriers[35].height = 800;

    barriers[36].x = 0;
    barriers[36].y = 0;
    barriers[36].width = 900;
    barriers[36].height = 55;

    barriers[37].x = 495;
    barriers[37].y = 35;
    barriers[37].width = 200;
    barriers[37].height = 55;

    barriers[38].x = 100;
    barriers[38].y = 35;
    barriers[38].width = 55;
    barriers[38].height = 120;

    barriers[39].x = 100;
    barriers[39].y = 390;
    barriers[39].width = 55;
    barriers[39].height = 120;

    // Arbres1
    barriers[40].x = 540;
    barriers[40].y = 183;
    barriers[40].width = 12;
    barriers[40].height = 50;

    barriers[41].x = 525;
    barriers[41].y = 165;
    barriers[41].width = 40;
    barriers[41].height = 50;
    
    // Arbres2
    barriers[42].x = 195;
    barriers[42].y = 270;
    barriers[42].width = 20;
    barriers[42].height = 50;

    barriers[43].x = 200;
    barriers[43].y = 290;
    barriers[43].width = 10;
    barriers[43].height = 50;    
}

void handleInput(SDL_Event event) {
    switch (event.key.keysym.sym) {
        case SDLK_UP:    pacmanSingle.dy = -1; pacmanSingle.dx = 0; break;
        case SDLK_DOWN:  pacmanSingle.dy = 1;  pacmanSingle.dx = 0; break;
        case SDLK_LEFT:  pacmanSingle.dx = -1; pacmanSingle.dy = 0; break;
        case SDLK_RIGHT: pacmanSingle.dx = 1;  pacmanSingle.dy = 0; break;
        default: break;
    }
}

void updatePacman() {
    int newX = pacmanSingle.x + pacmanSingle.dx * PACMAN_SPEED;
    int newY = pacmanSingle.y + pacmanSingle.dy * PACMAN_SPEED;

    for (int i = 0; i < MAX_BARRIERS; i++) {
        int barrierX = barriers[i].x;
        int barrierY = barriers[i].y;
        int barrierWidth = barriers[i].width;
        int barrierHeight = barriers[i].height;

        // Vérifier la collision avec la barrière
        if (newX < barrierX + barrierWidth &&
            newX + PACMAN_SIZE > barrierX &&
            newY < barrierY + barrierHeight &&
            newY + PACMAN_SIZE > barrierY) {
            // Collision détectée, annuler le déplacement
            newX = pacmanSingle.x;
            newY = pacmanSingle.y;
        }
    }

    // Limiter les mouvements à l'intérieur de la fenêtre
    if (newX < 0) newX = 0;
    if (newX > WINDOW_WIDTH - PACMAN_SIZE) newX = WINDOW_WIDTH - PACMAN_SIZE;
    if (newY < 0) newY = 0;
    if (newY > WINDOW_HEIGHT - PACMAN_SIZE) newY = WINDOW_HEIGHT - PACMAN_SIZE;

    pacmanSingle.x = newX;
    pacmanSingle.y = newY;
}

void loadMenuTexture(SDL_Renderer* renderer) {
    const char* menuFilePath = "assets/menu.png";
    SDL_Surface* tempSurface = IMG_Load(menuFilePath);
    if (!tempSurface) {
        printf("Impossible de charger l'image du menu %s! Erreur SDL: %s\n", menuFilePath, IMG_GetError());
        exit(1);
    }
    menuTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
}

void initEnemies(bool* gameOver) {
    for (int i = 0; i < NUM_ENEMIES; i++) {
        do {
            enemies[i].x = rand() % (WINDOW_WIDTH - ENEMY_SIZE);
            enemies[i].y = rand() % (WINDOW_HEIGHT - ENEMY_SIZE);
        } while (isSpawningOnBarrier(enemies[i].x, enemies[i].y, ENEMY_SIZE) || checkCollision(&enemies[i], &pacmanSingle)); // Vérification de la collision avec une barrière et Pacman
        
        enemies[i].dx = (rand() % 2 == 0) ? 1 : -1;
        enemies[i].dy = (rand() % 2 == 0) ? 1 : -1;
    }
}

void loadEnemyTexture(SDL_Renderer* renderer) {
    const char* filePath = "assets/noiraude.png";
    SDL_Surface* tempSurface = IMG_Load(filePath);
    if (!tempSurface) {
        printf("Impossible de charger l'image %s! SDL Error: %s\n", filePath, IMG_GetError());
        exit(1);
    }
    enemyTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
}

SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* filePath) {
    SDL_Surface* tempSurface = IMG_Load(filePath);
    if (!tempSurface) {
        printf("Impossible de charger l'image %s! SDL Error: %s\n", filePath, IMG_GetError());
        exit(1);
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return texture;
}

bool canMove(Enemy* enemy, int dx, int dy) {
    int newX = enemy->x + dx;
    int newY = enemy->y + dy;

    for (int i = 0; i < MAX_BARRIERS; i++) {
        if (newX < barriers[i].x + barriers[i].width &&
            newX + ENEMY_SIZE > barriers[i].x &&
            newY < barriers[i].y + barriers[i].height &&
            newY + ENEMY_SIZE > barriers[i].y) {
            return false;
        }
    }

    if (newX < 0 || newX > WINDOW_WIDTH - ENEMY_SIZE || newY < 0 || newY > WINDOW_HEIGHT - ENEMY_SIZE) {
        return false;
    }

    return true;
}

bool detectObstacle(Enemy* enemy, int dx, int dy) {
    int newX = enemy->x + dx;
    int newY = enemy->y + dy;

    for (int i = 0; i < MAX_BARRIERS; i++) {
        if (newX < barriers[i].x + barriers[i].width &&
            newX + ENEMY_SIZE > barriers[i].x &&
            newY < barriers[i].y + barriers[i].height &&
            newY + ENEMY_SIZE > barriers[i].y) {
            return true;
        }
    }

    return false;
}

void updateEnemies() {
    for (int i = 0; i < NUM_ENEMIES; i++) {
        int dx = enemies[i].dx * ENEMY_SPEED;
        int dy = enemies[i].dy * ENEMY_SPEED;

        if (!detectObstacle(&enemies[i], dx, dy)) {
            enemies[i].x += dx;
            enemies[i].y += dy;
        } else {
            // Essayer une nouvelle direction si obstacle détecté
            int newDx = 0, newDy = 0;
            bool foundDirection = false;
            for (int attempts = 0; attempts < 4; attempts++) { // Try up to 4 different directions
                newDx = (rand() % 3 - 1); // -1, 0, or 1
                newDy = (rand() % 3 - 1); // -1, 0, or 1

                if ((newDx != 0 || newDy != 0) && !detectObstacle(&enemies[i], newDx * ENEMY_SPEED, newDy * ENEMY_SPEED)) {
                    foundDirection = true;
                    break;
                }
            }

            if (foundDirection) {
                enemies[i].dx = newDx;
                enemies[i].dy = newDy;
                enemies[i].x += enemies[i].dx * ENEMY_SPEED;
                enemies[i].y += enemies[i].dy * ENEMY_SPEED;
            } else {
                // If no valid direction found, reverse direction
                enemies[i].dx *= -1;
                enemies[i].dy *= -1;
            }
        }
    }
}

void loadNoodleTexture(SDL_Renderer* renderer) {
    const char* filePath = "assets/nouilles.png";
    SDL_Surface* tempSurface = IMG_Load(filePath);
    if (!tempSurface) {
        printf("Impossible de charger l'image %s! SDL Error: %s\n", filePath, IMG_GetError());
        exit(1);
    }
    noodleTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
}

void initNoodles() {
    for (int i = 0; i < NUM_NOODLES; i++) {
        noodles[i].x = rand() % (WINDOW_WIDTH - PACMAN_SIZE);
        noodles[i].y = rand() % (WINDOW_HEIGHT - PACMAN_SIZE);
        noodles[i].isVisible = true;
    }
}

void renderNoodles(SDL_Renderer* renderer) {
    SDL_Rect noodleRect = {0, 0, PACMAN_SIZE, PACMAN_SIZE};
    for (int i = 0; i < NUM_NOODLES; i++) {
        if (noodles[i].isVisible) {
            noodleRect.x = noodles[i].x;
            noodleRect.y = noodles[i].y;
            SDL_RenderCopy(renderer, noodleTexture, NULL, &noodleRect);
        }
    }
}

bool checkCollision(Enemy* enemy, Pacman* pacman) {
    return !(enemy->x + ENEMY_SIZE < pacman->x || enemy->x > pacman->x + PACMAN_SIZE ||
             enemy->y + ENEMY_SIZE < pacman->y || enemy->y > pacman->y + PACMAN_SIZE);
}

bool isSpawningOnBarrier(int x, int y, int size) {
    for (int i = 0; i < MAX_BARRIERS; i++) {
        if (x < barriers[i].x + barriers[i].width &&
            x + size > barriers[i].x &&
            y < barriers[i].y + barriers[i].height &&
            y + size > barriers[i].y) {
            return true;
        }
    }
    return false;
}

void checkNoodleCollision() {
    SDL_Rect pacmanRect = {pacmanSingle.x, pacmanSingle.y, PACMAN_SIZE, PACMAN_SIZE};
    static int score = 0; // initialisation du score a 0 pour apres l'incrémenter
    for (int i = 0; i < NUM_NOODLES; i++) // on parcours toutes les nouilles
    {
        if (noodles[i].isVisible) // et si la nouille est visible et que y'a collision on ajoute 1
        {
            SDL_Rect noodleRect = {noodles[i].x, noodles[i].y, PACMAN_SIZE, PACMAN_SIZE};
            if (SDL_HasIntersection(&pacmanRect, &noodleRect)) {
                noodles[i].isVisible = false;
                score++; // On incrémente quand on touche une nouille
                // on affiche le score au fur et a mesure dans la console
                printf("Nouilles recuperees : %d/%d\n", score, NUM_NOODLES);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "Could not initialize SDL_image: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    loadPacmanTextures(renderer);
    loadEnemyTexture(renderer);
    loadNoodleTexture(renderer);

    backgroundTexture = loadTexture(renderer, "assets/map_1.png");
    loadMenuTexture(renderer);
    deathTexture = loadTexture(renderer, "assets/mort.png");

    initBarriers(); // Initialisation des barrières
    bool gameOver = false;
    initEnemies(&gameOver);
    initNoodles();

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                handleInput(event);
            } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                // Code pour relancer le jeu après la mort
                if (gameOver) {
                    // Réinitialisation du jeu
                    pacmanSingle.lives = 5;
                    initBarriers();
                    initEnemies(&gameOver);
                    initNoodles();
                    gameOver = false;
                }
            }
        }

        if (!gameOver) {
            if (pacmanSingle.dx == 0 && pacmanSingle.dy == 0) {
                SDL_RenderCopy(renderer, menuTexture, NULL, NULL);
                SDL_RenderPresent(renderer);
                continue;
            }

            updatePacman();
            updateEnemies();
            checkNoodleCollision();

            for (int i = 0; i < NUM_ENEMIES; i++) {
                if (checkCollision(&enemies[i], &pacmanSingle)) {
                    pacmanSingle.lives--;
                    printf("Pacman touché ! Vies restantes : %d\n", pacmanSingle.lives);
                    if (pacmanSingle.lives <= 0) {
                        printf("Pacman a perdu toutes ses vies ! Fin du jeu.\n");
                        // Code pour la fin du jeu
                        gameOver = true;
                    }

                    do {
                        enemies[i].x = rand() % (WINDOW_WIDTH - ENEMY_SIZE);
                        enemies[i].y = rand() % (WINDOW_HEIGHT - ENEMY_SIZE);
                    } while (isSpawningOnBarrier(enemies[i].x, enemies[i].y, ENEMY_SIZE) || checkCollision(&enemies[i], &pacmanSingle)); // Vérification de la collision avec une barrière et Pacman
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 125, 175, 66, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        // Affichage des barrières
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0); // Rendre les barrières transparentes
        for (int i = 0; i < MAX_BARRIERS; i++) {
            SDL_Rect barrierRect = { barriers[i].x, barriers[i].y, barriers[i].width, barriers[i].height };
            SDL_RenderFillRect(renderer, &barrierRect);
        }

        renderNoodles(renderer);

        // Affichage des ennemis
        SDL_Rect enemyRect = { 0, 0, ENEMY_SIZE, ENEMY_SIZE };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        
        for (int i = 0; i < NUM_ENEMIES; i++) {
            enemyRect.x = enemies[i].x;
            enemyRect.y = enemies[i].y;
            SDL_RenderCopy(renderer, enemyTexture, NULL, &enemyRect);
        }

        // Affichage de Pacman
        SDL_Rect pacmanRect = { pacmanSingle.x, pacmanSingle.y, PACMAN_SIZE, PACMAN_SIZE };
        SDL_Texture* currentTexture = NULL;

        if (pacmanSingle.dy == -1) {
            currentTexture = pacmanTextureDerriere;
        } else if (pacmanSingle.dx == 1) {
            currentTexture = (SDL_GetTicks() / 250) % 2 == 0 ? pacmanTextureDroit1 : pacmanTextureDroit2;
        } else if (pacmanSingle.dx == -1) {
            currentTexture = (SDL_GetTicks() / 250) % 2 == 0 ? pacmanTextureGauche1 : pacmanTextureGauche2;
        } else if (pacmanSingle.dy == 1) {
            currentTexture = (SDL_GetTicks() / 250) % 2 == 0 ? pacmanTextureDevant1 : pacmanTextureDevant2;
        }

        if (currentTexture != NULL) {
            SDL_RenderCopy(renderer, currentTexture, NULL, &pacmanRect);
        }

        // Affichage de l'écran de fin si nécessaire
        if (gameOver) {
            SDL_RenderCopy(renderer, deathTexture, NULL, NULL);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    SDL_DestroyTexture(pacmanTextureGauche1);
    SDL_DestroyTexture(pacmanTextureGauche2);
    SDL_DestroyTexture(pacmanTextureDerriere);
    SDL_DestroyTexture(pacmanTextureDevant1);
    SDL_DestroyTexture(pacmanTextureDevant2);
    SDL_DestroyTexture(pacmanTextureDroit1);
    SDL_DestroyTexture(pacmanTextureDroit2);
    SDL_DestroyTexture(pacmanTextureDebut);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(enemyTexture);
    SDL_DestroyTexture(menuTexture);
    SDL_DestroyTexture(deathTexture);
    SDL_DestroyTexture(noodleTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
