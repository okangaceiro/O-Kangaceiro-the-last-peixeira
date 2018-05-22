#include <allegro.h>
//objetos
struct obj {int wx,wy,x,y,w,h,dir;};
struct obj
    p = {42,0,14,23,42,44,4}, f[4];

//funções
void sair();
void inimigos();
void restart();
void mapa(int);
void control();
void fecha_programa();
//enumeracao das telas
enum{ MAINMENU, GAMESCREEN, TITLEHELP, TITLECREDITS, LOADING, ENDGAME, EXITGAME};
void mainmenu();
void titlecredits();
void titlehelp();
void gamescreen();
void titleload();
void titlend();

//Variáveis Globais

int continua = 0;
int sai    = 0;
int som = 0;
int vida = 0;
int vidas = 3;
int morre = 0;
int inicio = 0;
int fase1 = 0;
int antX = 0;
int antY = 0;
int width  = 1048;
int height = 670;
int map [31][28];
int comendo = 0;
volatile int exit_program;
int screen_state;

SAMPLE *ponto, *morto, *comecou, *fase, *morrendo, *musica;
MIDI *song;
BITMAP *buffer,*press, *load1, *load2, *milho, *fundoload, *fundoendgame, *itens, *fundomapa, *menu, *cursor, *newgame, *help, *credits, *menuhelp, *menucredits, *gameover, *gameover1;
FONT *f48;

int main() {

    //Iniciação
	allegro_init();
    	install_keyboard();
    	install_mouse();
	set_color_depth(32);
	set_window_title("O Kangaceiro");
	set_close_button_callback(sair);
	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, width, height, 0, 0);

	exit_program = FALSE;
    LOCK_FUNCTION(fecha_programa);
    LOCK_VARIABLE(exit_program);
    set_close_button_callback(fecha_programa);

      screen_state = MAINMENU;
	//musica tema
    	song = load_midi("som/Take_Me_Home_Country_Roads.mid");
	//Variáveis Locais
    	play_midi(song,TRUE);
	while (!exit_program)
	{
	    if(screen_state == MAINMENU)
            mainmenu();
        else if(screen_state == GAMESCREEN){
            gamescreen();
        }
        else if(screen_state == TITLEHELP)
            titlehelp();
        else if(screen_state == TITLECREDITS)
            titlecredits();
        else if(screen_state == LOADING)
            titleload();
        else if(screen_state == ENDGAME)
            titlend();
        else if(screen_state == EXITGAME)
            exit_program = TRUE;
    }
    destroy_midi(song);
    //Finalizacao
	return 0;
}
END_OF_MAIN();

void fecha_programa() { exit_program = TRUE; }
END_OF_FUNCTION(fecha_programa)

void gamescreen(){

    int exit_screen = FALSE;
    int press_space = 0;
    //Variáveis Locais
	buffer = create_bitmap(width, height);
	fundomapa = load_bitmap("img/mapa.bmp", NULL);
	itens = load_bitmap("img/personagens.bmp", NULL);
	f48 = load_font("font/dpcomic.pcx", NULL, NULL);
	comecou = load_sample("som/inicio.wav");
	ponto = load_sample("som/comendo.wav");
	fase = load_sample("som/som.wav");
	song = load_midi("som/Take_Me_Home_Country_Roads.mid");
	morrendo = load_sample("som/morre.wav");
	press = load_bitmap("img/press.bmp", NULL);

	int i;

	for (i = 0; i< 4; i++)
    {
        f[i].wx = 0;
        f[i].wy = 53 + i*41;
        f[i].x = 12 + i;
        f[i].y = 14;
        f[i].w = 40;
        f[i].h = 40;
        f[i].dir = 1;

    }

    mapa(1);
	while (!exit_program && !exit_screen)
	{
	    antX = p.x;
	    antY = p.y;
	    draw_sprite(buffer, fundomapa, 5, 5);
		textprintf_ex(buffer, f48, 925, 250, 0xffffff, -1,"%i", comendo);
		if(vida) control();
		mapa(0);
		if(vidas>0 && comendo>=285){
            masked_blit(itens, buffer, 0, 0, 365, 290, 38,40);
		}
		if (vida || inicio) masked_blit(itens, buffer, p.wx, p.wy, p.x*28-8, p.y*21-7, p.w, p.h);
		// Quando inicia, coloca os monstros no local de inicio.
		if(inicio)
        {
            for (i = 0; i< 4; i++)
            {
                f[i].x = 12 + i;
                f[i].y = 14;
                f[i].dir = 1;
            }
        }
		for (i = 0; i < vidas; i++) masked_blit(itens, buffer, 160, 0, 820 + i*45, 600, 42, 44);
		inimigos();
		if(!vida && press_space>=7)
        {
            draw_sprite(buffer, press, 0, 0);
            if(press_space == 14) press_space=0;
        }
        press_space ++;
		draw_sprite(screen, buffer, 0, 0);
		rest(100);
		clear(buffer);
		restart();
		//sons
		if (som == 2) {stop_sample(ponto); play_sample(ponto, 255, 128, 1000, 0);}
		som = 0;
        if(vidas<=0){
            play_midi(song,TRUE);
            continua = 0;
            exit_screen = TRUE;
            screen_state = ENDGAME;
		}
		 if(((antX==13 &&  antY==13)|| (antX==14 && antY==14) || (antX==15 && antY==15)|| (antX==16 && antY==16)) && comendo>=285){
                comendo++;
                p.x = 14;
                p.y = 23;
                vida = 0;
                inicio = 0;
                continua = 0;
                play_midi(song,TRUE);
                exit_screen = TRUE;
                screen_state = ENDGAME;
            }
	}

	//Finalização
	destroy_font(f48);
	destroy_bitmap(itens);
	destroy_bitmap(fundomapa);
	destroy_bitmap(buffer);
	destroy_sample(ponto);
	destroy_sample(comecou);
	destroy_sample(fase);
	destroy_sample(morrendo);
}

void mainmenu(){

    //Variáveis Locais
	int exit_screen = FALSE;

	buffer = create_bitmap(width, height);
	menu = load_bitmap("img/telas/MenuPac.bmp", NULL);
	cursor = load_bitmap("img/cursor.bmp", NULL);
	newgame = load_bitmap("img/telas/newgame.bmp", NULL);
    	help = load_bitmap("img/telas/help.bmp", NULL);
    	credits = load_bitmap("img/telas/credits.bmp", NULL);



	//Game Loop
	while(!exit_program && !exit_screen)
    {
          draw_sprite(buffer, menu, 0, 0);

              if(mouse_x > 320 && mouse_x < 703 && mouse_y > 300 && mouse_y < 370)
                {

                    draw_sprite(buffer, newgame, 0, 0);

                    if(mouse_b == 1)
                        {
                            exit_screen = TRUE;
                            screen_state = LOADING;
                        }
                }
                else if(mouse_x > 320 && mouse_x < 703 && mouse_y > 400 && mouse_y < 472)
                    {
                        draw_sprite(buffer, help, 0 , 0);

                        if(mouse_b == 1)
                        {
                            exit_screen = TRUE;
                            screen_state = TITLEHELP;
                        }
                    }
                 else if(mouse_x > 320 && mouse_x < 703 && mouse_y > 505 && mouse_y < 570)
                    {
                        draw_sprite(buffer, credits, 0, 0);
                        if(mouse_b == 1)
                            {
                                exit_screen = TRUE;
                                screen_state = TITLECREDITS;
                            }
                    }
                if(mouse_x > 20 && mouse_x < 75 && mouse_y > 20 && mouse_y < 70){
                    if(mouse_b == 1){
                        exit_screen = TRUE;
                        screen_state = EXITGAME;
                    }
                }
              draw_sprite(buffer, cursor, mouse_x-6, mouse_y);
              draw_sprite(screen, buffer, 0, 0);

              clear(buffer);

    }

    //Finalização
    destroy_bitmap(buffer);
    destroy_bitmap(menu);
    destroy_bitmap(cursor);
    destroy_bitmap(newgame);
    destroy_bitmap(help);
    destroy_bitmap(credits);


}

void titlehelp(){

    int exit_screen = FALSE;

    //BITMAPS
    buffer = create_bitmap(width, height);
    menuhelp = load_bitmap("img/telas/MenuHelp.bmp", NULL);
    cursor = load_bitmap("img/cursor.bmp", NULL);

    //LOOP

    while(!exit_program && !exit_screen)
    {
          draw_sprite(buffer, menuhelp, 0, 0);
          if(mouse_x > 20 && mouse_x < 75 && mouse_y > 20 && mouse_y < 70)
            {
                if(mouse_b == 1)
                    {
                        exit_screen = TRUE;
                        screen_state = MAINMENU;
                    }
            }
          draw_sprite(buffer, cursor, mouse_x-6, mouse_y);
          draw_sprite(screen, buffer, 0, 0);
          clear(buffer);
    }

  //finalizacao
  destroy_bitmap(buffer);
  destroy_bitmap(menuhelp);
  destroy_bitmap(cursor);
}

void titlecredits(){

    int exit_screen = FALSE;

    //BITMAPS
    buffer = create_bitmap(width, height);
    menucredits = load_bitmap("img/telas/MenuCredits.bmp", NULL);
    cursor = load_bitmap("img/cursor.bmp", NULL);

    //LOOP
    while(!exit_program && !exit_screen)
    {
              draw_sprite(buffer, menucredits, 0, 0);
              if(mouse_x > 20 && mouse_x < 75 && mouse_y > 20 && mouse_y < 70)
                {
                    if(mouse_b == 1)
                        {
                            exit_screen = TRUE;
                            screen_state = MAINMENU;
                        }
                }
              draw_sprite(buffer, cursor, mouse_x-6, mouse_y);
              draw_sprite(screen, buffer, 0, 0);
              clear(buffer);

    }
    //finalizacao
    destroy_bitmap(buffer);
    destroy_bitmap(menucredits);
    destroy_bitmap(cursor);

}

void titlend(){

    int exit_screen = FALSE;
    int i, contador=0;

    //BITMAPS
    buffer = create_bitmap(width, height);
    itens = load_bitmap("img/personagens.bmp", NULL);
    fundoendgame = load_bitmap("img/fundofinal.bmp", NULL);
    gameover = load_bitmap("img/gameover0.bmp", NULL);
    gameover1 = load_bitmap("img/gameover1.bmp", NULL);
    cursor = load_bitmap("img/cursor.bmp", NULL);
     while(!exit_program && !exit_screen){

            if(comendo>=285 && vidas>0){

                    for (i = 0; i < 5; i++){
                        draw_sprite(buffer, fundoendgame, 0, 0);
                        masked_blit(itens, buffer, i*60, 344, 425, 508, p.w, p.h);
                        masked_blit(itens, buffer, i*60, 285, 450, 508, p.w, p.h);
                        draw_sprite(screen, buffer, 0, 0);
                        rest(200);
                    }
                    if(contador == 12){
                            vidas = 3;
                            comendo = 0;
                            exit_screen = TRUE;
                            screen_state = MAINMENU;
                        }
            }
            else if(vidas<=0){

                 draw_sprite(buffer, gameover, 0, 0);
                 if(mouse_x > 335 && mouse_x < 717 && mouse_y > 286 && mouse_y < 357)
                 {
                    draw_sprite(buffer, gameover1, 0, 0);
                    if(mouse_b == 1)
                        {
                            vidas = 3;
                            comendo = 0;
                            exit_screen = TRUE;
                            screen_state = MAINMENU;
                        }
                 }
                 draw_sprite(buffer, cursor, mouse_x-6, mouse_y);
              }
              contador++;
              draw_sprite(screen, buffer, 0, 0);
              clear(buffer);

   }

    destroy_bitmap(fundoendgame);
    destroy_bitmap(itens);
    destroy_bitmap(gameover);
    destroy_bitmap(gameover1);
    destroy_bitmap(cursor);
    destroy_bitmap(buffer);

}
void titleload(){

    int exit_screen = FALSE;
    int contador=0;

    //BITMAP
    buffer = create_bitmap(SCREEN_W, SCREEN_H);
    fundoload = load_bitmap("img/loadframes/fundo.bmp",NULL);
    load1 = load_bitmap("img/loadframes/load1.bmp", NULL);
    load2 = load_bitmap("img/loadframes/load2.bmp", NULL);
    milho = load_bitmap("img/loadframes/milho.bmp", NULL);

    while(!exit_program && !exit_screen){

            int i, j;
            if(contador == 4)
            {
                    exit_screen = TRUE;
                    screen_state = GAMESCREEN;
            }
            for (i = 0; i < 4; i++) {
                            draw_sprite(buffer, fundoload, 5 ,5);
                            masked_blit(load1, buffer, i*50, 0, SCREEN_W/2, SCREEN_H/2, 44, 93);
                            masked_blit(milho,buffer, 0,0,(SCREEN_W/2)-58, (SCREEN_H/2)+90, 36 + (i*36),60);
                            draw_sprite(screen, buffer,0,0);
                            rest(200);
                            clear(buffer);
            }
            for (j = 0; j < 5; j++){
                            draw_sprite(buffer, fundoload, 5 ,5);
                            masked_blit(load2, buffer, j*100, 0, (SCREEN_W/2)-54, SCREEN_H/2, 100, 93);
                            draw_sprite(screen, buffer,0,0);
                            rest(200);
            }
            contador++;
      draw_sprite(screen, buffer, 0, 0);
      clear(buffer);


  }

  //finalizacao
  destroy_bitmap(buffer);
  destroy_bitmap(fundoload);
  destroy_bitmap(load1);
  destroy_bitmap(load2);
  destroy_bitmap(milho);
}

void restart(){

    if (morre || !vida) stop_sample(fase);

    if (inicio)
    {
        vida = vidas;
        play_sample(comecou, 255, 128, 1000, 0);
        rest(4000);
        stop_sample(comecou);
        play_sample(fase, 255, 128, 1000, 1);
        inicio = 0;
        fase1 = 0;
    }

    if(key[KEY_SPACE] && !vida)
    {
        stop_midi();
        while(key[KEY_SPACE]); inicio = 1;
    }
    if (morre && vida)
    {
        int i;
        vida--;
        vidas--;
        rest(500);
        stop_sample(fase);
        play_sample(morrendo, 255, 128, 1000, 0);
        for (i = 0; i < 5; i++)
        {
            draw_sprite(buffer, fundomapa, 5, 5);
            masked_blit(itens, buffer, 60 + i*60, 236, p.x*28-8, p.y*21-7, p.w, p.h);
            mapa(0);
            draw_sprite(screen, buffer, 0, 0);
            rest(200);
            clear(buffer);
        }
        stop_sample(morrendo);
        play_sample(fase, 255, 128, 1000, 1);
    }
    if (inicio || morre)
    {
        int i;
        if (!morre) mapa(1);
        p.x = 14;
        p.y = 23;
        p.dir = 4;
        morre = 0;
        for(i = 0; i < 4; i++)
        {
            f[i].x = 12 + i;
            f[i].y = 14;
            continua = 1;
        }
    }
}
void inimigos(){
    int i;
    for(i = 0; i<4 ; i++){

            if(f[i].x > 10 && f[i].x < 17 && f[i].y == 14) f[i].dir = rand() % 4;
            else if((f[i].y == 12 || f[i].y == 13) && (f[i].x == 13 || f[i].x == 14)) f[i].dir = 1;

             //FANTASMAS ATRAVESSANDO A TELA (NO PONTO)
            else if(f[i].x < 1) f[i].x = 27;
            else if(f[i].x > 26 && f[i].dir == 2) f[i].x = 1;

            //Sr. de Engenho = perseguidor

            else if(i==2 && f[2].x < p.x-1 && f[2].dir != 0 && map[f[2].y][f[2].x+1] != 1) f[2].dir = 2;
            else if(i==2 && f[2].x > p.x-1 && f[2].dir != 2 && map[f[2].y][f[2].x-1] != 1) f[2].dir = 0;
            else if(i==2 && f[2].y < p.y-1 && f[2].dir != 1 && map[f[2].y+1][f[2].x] != 1) f[2].dir = 3;
            else if(i==2 && f[2].y > p.y-1 && f[2].dir != 3 && map[f[2].y-1][f[2].x] != 1) f[2].dir = 1;

            //Lobisomem = fica um pouco longe do Kangaceiro
            else if(i==3 && f[3].x < p.x+16 && f[3].dir != 0 && map[f[3].y][f[3].x+1] != 1) f[3].dir = 2;
            else if(i==3 && f[3].x > p.x+16 && f[3].dir != 2 && map[f[3].y][f[3].x-1] != 1) f[3].dir = 0;
            else if(i==3 && f[3].y < p.y+16 && f[3].dir != 1 && map[f[3].y+1][f[3].x] != 1) f[3].dir = 3;
            else if(i==3 && f[3].y < p.y+16 && f[3].dir != 3 && map[f[3].y-1][f[3].x] != 1) f[3].dir = 1;

            //Curupira = tenta imitar a posição do Kangaceiro
            else if(i==0 && f[0].x < p.x-8 && f[0].dir != 0 && map[f[0].y][f[0].x+1] != 1) f[0].dir = 2;
            else if(i==0 && f[0].x > p.x-8 && f[0].dir != 2 && map[f[0].y][f[0].x-1] != 1) f[0].dir = 0;
            else if(i==0 && f[0].y < p.y-8 && f[0].dir != 1 && map[f[0].y+1][f[0].x] != 1) f[0].dir = 3;
            else if(i==0 && f[0].y > p.y-8 && f[0].dir != 3 && map[f[0].y-1][f[0].x] != 1) f[0].dir = 1;
            else
            {


                //Boitatá = anda aleatoriamente pelo mapa, funcao já implementada

                //direita
                if (f[i].dir == 2 && map[f[i].y][f[i].x+1] == 1)
                {
                    if(map[f[i].y-1][f[i].x] != 1 && map[f[i].y+1][f[i].x] != 1)
                    {
                        (rand()%2) ? (f[i].dir = 1) : (f[i].dir = 3);
                    }
                    else (map[f[i].y-1][f[i].x] != 1) ? (f[i].dir = 1) : (f[i].dir = 3);
                }
                //esquerda
                if (f[i].dir == 0 && map[f[i].y][f[i].x-1] == 1)
                {
                    if(map[f[i].y-1][f[i].x] != 1 && map[f[i].y+1][f[i].x] != 1)
                    {
                        (rand()%2) ? (f[i].dir = 1) : (f[i].dir = 3);
                    }
                    else (map[f[i].y-1][f[i].x] != 1) ? (f[i].dir = 1) : (f[i].dir = 3);
                }
                //baixo
                if (f[i].dir == 3 && map[f[i].y+1][f[i].x] == 1)
                {
                    if(map[f[i].y][f[i].x-1] != 1 && map[f[i].y][f[i].x+1] != 1)
                    {
                        (rand()%2) ? (f[i].dir = 0) : (f[i].dir = 2);
                    }
                    else (map[f[i].y][f[i].x-1] != 1) ? (f[i].dir = 0) : (f[i].dir = 2);
                }
                //cima
                if (f[i].dir == 1 && map[f[i].y-1][f[i].x] == 1)
                {
                    if(map[f[i].y][f[i].x-1] != 1 && map[f[i].y][f[i].x+1] != 1)
                    {
                        (rand()%2) ? (f[i].dir = 0) : (f[i].dir = 2);
                    }
                    else (map[f[i].y][f[i].x-1] != 1) ? (f[i].dir = 0) : (f[i].dir = 2);
                }
            }

            if(f[i].dir == 0 && map[f[i].y][f[i].x-1] != 1) f[i].x--; // esq
            else if(f[i].dir == 1 && map[f[i].y-1][f[i].x] != 1) f[i].y--; //cima
            else if(f[i].dir == 2 && map[f[i].y][f[i].x+1] != 1) f[i].x++; // dir
            else if(f[i].dir == 3 && map[f[i].y+1][f[i].x] != 1) f[i].y++; // baixo

    if(f[i].dir==0 || f[i].dir==1) masked_blit(itens, buffer, f[i].wx, f[i].wy, f[i].x*28 - 8, f[i].y * 21 - 7, f[i].w, f[i].h);
    else if(f[i].dir==2 || f[i].dir==3) masked_blit(itens, buffer, f[i].wx+60, f[i].wy, f[i].x*28 - 8, f[i].y * 21 - 7, f[i].w, f[i].h);

    //colisão
    if(vidas && (p.x == f[i].x && p.y == f[i].y || antX == f[i].x && antY == f[i].y) && continua == 1)
    {

        morre = 1;

    }

    }

}
void control(){
    if (key[KEY_LEFT] && map [p.y][p.x-1] != 1) p.dir=0;
    if (key[KEY_UP] && map [p.y-1][p.x] != 1) p.dir=1;
    if (key[KEY_RIGHT] && map [p.y][p.x+1] != 1) p.dir=2;
    if (key[KEY_DOWN] && map [p.y+1][p.x] != 1) p.dir=3;

    if(p.dir == 0 && map [p.y][p.x-1] != 1) p.x--;
    if(p.dir == 1 && map [p.y-1][p.x] != 1) p.y--;
    if(p.dir == 2 && map [p.y][p.x+1] != 1) p.x++;
    if(p.dir == 3 && map [p.y+1][p.x] != 1) p.y++;

    //atravessando a tela
    if (p.x > 26 && p.dir) p.x =0;
    if (p.x < 1 && !p.dir) p.x = 27;


    //comendo as bolinhas
    if (map[p.y][p.x] == 2)
    {
        comendo++;
        som = 2;
        map[p.y][p.x] = 0;
    }

}
void mapa(int mod){
    int i, j;
    // numero 31 é vertical, e 28 horizontal. isso é para por os trocinhos dele comer
    // 1 == barreira, 2 == sem barreira e pontos , 4 == sem barreira e sem pontos
    char mp[31][28] =  {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                        {1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1},
                        {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
                        {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
                        {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
                        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
                        {1,2,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,2,1},
                        {1,2,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,2,1},
                        {1,2,2,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,2,2,1},
                        {1,1,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,2,1,1,2,2,2,2,2,2,2,2,2,2,1,1,2,1,1,1,1,1,1},
                        {1,1,1,1,1,1,2,1,1,2,1,1,1,4,4,1,1,1,2,1,1,2,1,1,1,1,1,1},
                        {1,1,1,1,1,1,2,1,1,2,1,1,0,0,0,0,1,1,2,1,1,2,1,1,1,1,1,1},
                        {0,0,0,0,0,0,2,2,2,2,1,0,0,0,0,0,0,1,2,2,2,2,0,0,0,0,0,0},
                        {1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1},
                        {1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1},
                        {1,1,1,1,1,1,2,1,1,2,2,2,2,2,2,2,2,2,2,1,1,2,1,1,1,1,1,1},
                        {1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1},
                        {1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1},
                        {1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1},
                        {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
                        {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
                        {1,2,2,2,1,1,2,2,2,2,2,2,2,2,0,2,2,2,2,2,2,2,1,1,2,2,2,1},
                        {1,1,1,2,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,2,1,1,1},
                        {1,1,1,2,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,2,1,1,1},
                        {1,2,2,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,2,2,1},
                        {1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1},
                        {1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1},
                        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
                        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};


                        for (i=0; i<31;i++)
                        {
                            for(j=0; j<28; j++)
                            {
                                if (mod) map[i][j] = mp [i][j];
                                if (map [i][j] == 2) masked_blit(itens, buffer, 145, 3, j*28 + 10, i*21 + 16, 6, 6); //pontinhos
                            }

                        }





}
void sair(){sai= 1;}
END_OF_FUNCTION(sair);
