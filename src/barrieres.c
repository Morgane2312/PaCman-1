#include "barrieres.h"

// Définition du tableau des barrières.
Barrier barriers[MAX_BARRIERS];

void initBarriers() {

    barriers[0] = (Barrier){206, 165, 57, 8};
    barriers[1] = (Barrier){315, 164, 160, 8};
    barriers[2] = (Barrier){260, 115, 52, 8};
    barriers[3] = (Barrier){360, 370, 90, 10};
    barriers[4] = (Barrier){150, 416, 350, 8};
    barriers[5] = (Barrier){130, 325, 50, 10};
    barriers[6] = (Barrier){120, 385, 35, 8};
    barriers[7] = (Barrier){495, 350, 100, 8};
    barriers[8] = (Barrier){550, 203, 40, 8};
    barriers[9] = (Barrier){164, 50, 325, 8};
    barriers[10] = (Barrier){120, 150, 40, 8};
    barriers[11] = (Barrier){490, 85, 50, 8};
    barriers[27] = (Barrier){533, 270, 50, 8};
    barriers[12] = (Barrier){115, 160, 8, 235};
    barriers[13] = (Barrier){590, 203, 8, 155};
    barriers[14] = (Barrier){545, 90, 8, 120};
    barriers[15] = (Barrier){490, 350, 8, 70};
    barriers[16] = (Barrier){150, 390, 8, 30};
    barriers[17] = (Barrier){150, 60, 8, 98};
    barriers[18] = (Barrier){260, 55, 8, 60};
    barriers[19] = (Barrier){205, 55, 8, 110};
    barriers[20] = (Barrier){305, 122, 8, 50};
    barriers[21] = (Barrier){475, 170, 8, 80};
    barriers[22] = (Barrier){450, 290, 8, 80};
    barriers[23] = (Barrier){350, 220, 8, 160};
    barriers[24] = (Barrier){180, 270, 8, 60};
    barriers[25] = (Barrier){255, 165, 8, 60};
    barriers[26] = (Barrier){490, 50, 8, 35};
    barriers[27] = (Barrier){533, 270, 50, 8};
    barriers[28] = (Barrier){553, 7, 200, 200};
    barriers[29] = (Barrier){553, 0, 200, 200};
    barriers[30] = (Barrier){550, -20, 200, 200};
    barriers[31] = (Barrier){595, 50, 500, 500};
    barriers[32] = (Barrier){495, 355, 200, 200};
    barriers[33] = (Barrier){150, 420, 500, 200};
    barriers[34] = (Barrier){0, 420, 500, 200};
    barriers[35] = (Barrier){0, 0, 115, 800};
    barriers[36] = (Barrier){0, 0, 900, 55};
}

void drawBarriers(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    for (int i = 0; i < MAX_BARRIERS; i++) {
        SDL_Rect rect = {barriers[i].x, barriers[i].y, barriers[i].width, barriers[i].height};
        SDL_RenderFillRect(renderer, &rect);
    }
}
