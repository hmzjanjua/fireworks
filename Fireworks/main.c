#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <stdbool.h>
#include <inttypes.h>
#include <time.h>
#include <SDL_image.h>

const int ScreenW=900,ScreenH=700;
SDL_Window *window = NULL;
SDL_Texture *background = NULL;
SDL_Texture *renderTarget =NULL;
SDL_Texture *fireworkImg = NULL;
SDL_Texture *explosion = NULL;

const int pop = 500;
SDL_Rect firework[500];
int fireworkCount=0;
float velX[500],velY[500];
int fireworkHeight[500];
SDL_Rect test;
SDL_Rect explode[500];
int jugarh=0;

Uint32 startTime;
Uint32 Time();
Uint32 timedifference=0;
float AframeTime = 0;
float AdeltaTime = 0;
int AprevTime = 0;
int AcurrentTime =0;

SDL_Texture *LoadTexture(char filePath[],SDL_Renderer *renderTarget){

    SDL_Surface *texture = NULL;
    SDL_Surface *surface = IMG_Load(filePath);
    if(surface == NULL){

        //printf("Error : Loading image");
    }
    else{
        texture = SDL_CreateTextureFromSurface(renderTarget, surface);
        if(texture==NULL){
            printf("Error : texture rendering");
        }
    }

    SDL_FreeSurface(surface);
    return texture;
}

Uint32 Time(){
    return SDL_GetTicks()-startTime;
}

unsigned int rand_interval(unsigned int min, unsigned int max){
    int r;
    const unsigned int range = 1 + max - min;
    const unsigned int buckets = RAND_MAX / range;
    const unsigned int limit = buckets * range;

    do
    {
        r = rand();
    } while (r >= limit);

    return min + (r / buckets);
}

void fireworkSpawn(){
    //printf("\nfirework spawned : %d",fireworkCount);
    firework[fireworkCount].h=120;
    firework[fireworkCount].w=50;
    firework[fireworkCount].x= rand()%900;
    firework[fireworkCount].y= 695;
    velX[fireworkCount]=-20;
    velY[fireworkCount]=-20;
    fireworkHeight[fireworkCount]=rand_interval(50,350);
    fireworkCount++;
    return;
}

void spawnExplosion(int height,int count,int time,int x){
    jugarh++;
    explode[count].x=x-50;
    explode[count].h=150;
    explode[count].w=150;
    explode[count].y=height-75;

    int i=0;
    if(jugarh%1==0){
        for(i;i<fireworkCount;i++){
            explode[i].y=1500;
        }
    }
}

int myRandom(){
    int randomnumber;
    randomnumber = rand() % 10;
    printf("%d\n", randomnumber);
    return randomnumber;
}



void Quit(){
    SDL_Quit();
}

void Load(){
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("FireWorks",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,ScreenW,ScreenH,SDL_WINDOW_SHOWN);
    renderTarget = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    background = LoadTexture("res/nightSky.jpg",renderTarget);
    fireworkImg = LoadTexture("res/firework.png",renderTarget);
    explosion = LoadTexture("res/explosion.png",renderTarget);
    startTime=SDL_GetTicks();

    test.h=20;
    test.w=20;
    test.y=400;
    test.x=0;
    int i=0;


}

void Draw(){
        int i=rand_interval(0,255);

        SDL_SetRenderDrawColor(renderTarget,255,50,i,255);

        SDL_RenderClear(renderTarget);
        SDL_RenderCopy(renderTarget,background,NULL,NULL);
        SDL_RenderFillRect(renderTarget,&test);
        for(i=0;i<pop;i++){
            SDL_RenderCopy(renderTarget,explosion,NULL,&explode[i]);
            SDL_RenderCopy(renderTarget,fireworkImg,NULL,&firework[i]);
        }


        SDL_RenderPresent(renderTarget);

        return;

}

void Logic(){
    Uint8 *keystates = SDL_GetKeyboardState(NULL);
    int i =0;

    if(keystates[SDL_SCANCODE_A]){
        fireworkSpawn();
    }


    //1 sec tics
    AprevTime = AcurrentTime;
    AcurrentTime= SDL_GetTicks();
    AdeltaTime = (AcurrentTime - AprevTime)/1000.0f;
    AframeTime+=AdeltaTime;

    if(AframeTime>=0.5){
        AframeTime=0;
        fireworkSpawn();

    }



    //movement

    for(i=0;i<=fireworkCount;i++){
        if( firework[i].y<fireworkHeight[i]){
                //printf("fireworkcount %d destroyed! at %d\n",fireworkCount,firework[i].x);
                spawnExplosion(fireworkHeight[i],fireworkCount,Time()/1000,firework[i].x);
                firework[i].h=0;
                firework[i].w=0;
                firework[i].y=1000;

                //spawnExplosion()
        }
        if(firework[i].y<900)
        firework[i].y+=velY[i];
        velY[i]+=0.32;
    }
    test.x=Time();
}

int main(int argc, char** argv)
{



    srand(time(NULL));
    const int FPS = 60;
    float frameTime = 0;
    float deltaTime = 0;
    int prevTime = 0;
    int currentTime =0;
    srand(time(NULL));

    Load();

    bool isRunning = true;
    SDL_Event ev;

    while(isRunning){

        prevTime = currentTime;
        currentTime= SDL_GetTicks();
        deltaTime = (currentTime - prevTime)/1000.0f;

        while(SDL_PollEvent(&ev) !=0){
            if(ev.type == SDL_QUIT)
                isRunning = false;
            }

        frameTime+=deltaTime;
        if(frameTime >= 0.01666666f){  //60 frames per sec
            frameTime = 0;
            Draw();
            Logic();

        }
    }

    Quit();
    return 0;
}
