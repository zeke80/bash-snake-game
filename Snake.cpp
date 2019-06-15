#include<windows.h>
#include<iostream>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#define ARR 72
#define IZQ 75
#define DER 77
#define ABA 80

#define W 119
#define A 97
#define D 100
#define S 115

#define ESC 27
#define ENT 13
#define POS 43
#define NEG 45

struct player{
	char nom[18];
	int score;
	player *sig;
};

int winer;

//SNAKE 1
int cuerpo[2664][2];
int n;
int tam;
int x,y;
int dir;

//SNAKE 2
int cuerpo1[2664][2];
int n1;
int tam1;
int x1,y1;
int dir1;

bool bw;
int pa;
bool fs,ft,fa;
int xc,yc;
int xs,ys;
int xt,yt;
int xa,ya;
int v;
int comando;
int sco;
player *p = NULL;
FILE *f;

char tecla;

void mostrar_cursor(){
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 100;							//ESTILO DEL CURSOR
	cci.bVisible = TRUE;						//OCULTAR O MOSTRAR CURSOR
	
	SetConsoleCursorInfo(hCon,&cci);
}

void ocultar_cursor(){
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 50;							//ESTILO DEL CURSOR
	cci.bVisible = FALSE;						//OCULTAR O MOSTRAR CURSOR
	
	SetConsoleCursorInfo(hCon,&cci);
}

void gotoxy(int x, int y){
	HANDLE hCon;
	COORD dwPos;
	
	dwPos.X = x;
	dwPos.Y = y;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hCon,dwPos);
}

int color(int b, int f){
	HANDLE  hConsole;
	WORD    ForeColor = 0;
	WORD    BackColor;
	WORD    wAttributesOld;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

  // **  Open the current console input buffer.
  // **  Abrir la consola actual memoria intermedia de entrada.
  if( ( hConsole = CreateFile("CONOUT$", GENERIC_WRITE | GENERIC_READ,FILE_SHARE_READ | FILE_SHARE_WRITE,
                     0L, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0L) )
                 == (HANDLE) -1 ) {
      printf("\nError: Unable to open console.\n");
      return( -1 );
  }

  // **  Get and Save information on the console screen buffer.
  // **  Obtener y guardar informaci�n sobre la pantalla de la consola de amortiguaci�n.
	GetConsoleScreenBufferInfo( hConsole, &csbi );
	wAttributesOld = csbi.wAttributes;

	BackColor = b;
	ForeColor= f;
	SetConsoleTextAttribute( hConsole, (WORD) ( (BackColor << 4) | ForeColor) );
	return 0;
}

void espacio(char *c,char *u){
	
	while(c != u){
		if (*c == ' ') *c = '-';	
		c++;			
	}
}

void leer_score(){
	int i = 1;
	char c[200];
	bool d1 = false;
	
	p = new player;
	p->sig = NULL;
	
	player *a = p;
	
	f = fopen("score.txt","r");
	
	if(!f){
		f = fopen("score.txt","w");
		if(!f){
			color(0,4);
			gotoxy(36,18);
			printf("*ERROR NO SE PUDO CREAR EL ARCHIVO*");
			gotoxy(6,27);
			system("pause");
			exit(1);	
		}		
	}
	else{
		while(fscanf(f,"%199s",c)==1){
			strupr(c);
			switch(i){
				case 1:
					if(d1 == true){
						a->sig = new player;
						a = a->sig;
						a->sig = NULL;
					}
					d1 = true;			
					strcpy(a->nom,c);
					break;
				case 2:
					a->score = atoi(c);
					i = 0;
					break;
			}
			i++;
		}		
	}
	
	if (d1 == false){
		free(p);
		p = NULL;
	}	
	
	fclose(f);
}

void escribir_score(){
	player *a = p;
	
	f = fopen("score.txt","w");
	
	if (!f){
		color(0,4);
		gotoxy(36,18);
		printf("*ERROR NO SE PUDO CREAR EL ARCHIVO*");
		gotoxy(6,27);
		system("pause");
		exit(1);	
	}
	
	while(a){
		
		fprintf(f,"%s\n",a->nom);
		fprintf(f,"%i\n",a->score);
		
		if(a->sig);
			fprintf(f,"\n");
		
		a = a->sig;	
	}
		
	fclose(f);
}

void mapa(){

	//ESQUINAS
	if(bw == false)
		color(0,13);
	else
		color(0,10);
	gotoxy(4,4);printf("%c",201);
	gotoxy(4,28);printf("%c",200);
	gotoxy(116,4);printf("%c",187);
	gotoxy(116,28);printf("%c",188);
	
	//BORDES HORIZONTALES
	for(int i = 5; i < 116; i++){
		gotoxy(i,4);printf("%c",205);
		gotoxy(i,28);printf("%c",205);
	}
	
	//BORDES VERTICALES
	for(int i = 5; i < 28; i++){
		gotoxy(4,i);printf("%c",186);
		gotoxy(116,i);printf("%c",186);	
	}

	//MARCO DEL MARCADOR	
	color(0,13);
	gotoxy(4,1);printf("%c",201);
	gotoxy(4,3);printf("%c",200);
	gotoxy(116,1);printf("%c",187);
	gotoxy(116,3);printf("%c",188);
	
	gotoxy(4,2);printf("%c",186);	
	gotoxy(116,2);printf("%c",186);
	
	for(int i = 5; i < 116; i++){
		gotoxy(i,1);printf("%c",205);
		gotoxy(i,3);printf("%c",205);
	}
	
	gotoxy(101,1);printf("%c",203);
	gotoxy(101,2);printf("%c",186);
	gotoxy(101,3);printf("%c",202);
	
	gotoxy(84,1);printf("%c",203);
	gotoxy(84,2);printf("%c",186);
	gotoxy(84,3);printf("%c",202);	
	
	color(0,10);
	gotoxy(103,2);
	printf("TAMA%cO: 0",164);
	
	color(0,6);
	gotoxy(86,2);
	printf("VELOCIDAD: 0");	
	
	color(0,4);
	gotoxy(6,2);
	printf("SCORE: 000000");
	
	color(0,5);
	gotoxy(72,2);
	printf("TIEMPO: ");		
	
	color(0,15);
}

int divbit(int m,int s){
	int con = -1;
	
	do{
		s /= 10;
		con++;	
	}while(s != 0);
	
	m -= con;
	
	return m;	
}

void marcador(){		
	
	gotoxy(111,2);printf("    ");
	color(0,4);
	gotoxy(13,2);printf("000000");
/*	color(0,15);
	gotoxy(99,2);printf("   ");	*/
	gotoxy(80,2);
	printf("   ");
	
	color(0,10);
	gotoxy(111,2);
	printf("%i",tam-1);
	
	color(0,6);
	gotoxy(97,2);
	printf("%i",320 - v);		
	
	color(0,4);
	gotoxy(divbit(18,sco),2);
	printf("%i",sco);
	
	if(bw == true){
		color(0,5);
		gotoxy(80,2);
		printf("%i",pa);
	}	
	
	color(0,15);
}

void begin(){
	winer = 0;
	
	x = 105;
	y = 15;
	x1 = 15;
	y1 = 15;

	dir = 3;
	dir1 = 4;

	n = 1;
	n1 = 1;

	/*tam = 4;
	tam1 = 4;*/

	tam = 10;
	tam1 = 10;

	for(int i = 0;i < tam;i++){
		cuerpo[i][0] = 0;
		cuerpo[i][1] = 0;

		cuerpo1[i][0] = 0;
		cuerpo1[i][1] = 0;
	}

	do{
		/*x = (rand() % 91) + 15;
		y = (rand() % 3) + 15;
		x1 = (rand() % 91) + 15;
		y1 = (rand() % 3) + 15;*/

		xc = (rand() % 111) + 5;
		yc = (rand() % 23) + 5;
	}while((x == xc && y == yc)||(x1 == xc && y1 == yc)||(xc % 2 == 0));

	bw = false;
	v = 100;
	sco = 0;
	comando = 0;
	fs = false;
	ft = false;
	fa = false;
	pa = 0;
	xs = 0;
	ys = 0;
	xt = 0;
	yt = 0;
	xa = 0;
	ya = 0;
	
	color(0,4);
	gotoxy(xc,yc);printf("%c",254);
	color(0,15);
}

void guardar_posicion(){
	
	cuerpo[n][0] = x;
	cuerpo[n][1] = y;
	n++;
	if(n == tam)
		n = 1;

	cuerpo1[n1][0] = x1;
	cuerpo1[n1][1] = y1;
	n1++;
	if(n1 == tam1)
		n1 = 1;
}

void pintar_cuerpo(){
	color(0,10);
	gotoxy(x,y);
	printf("O");
	color(0,15);

	color(0,11);
	gotoxy(x1,y1);
	printf("O");
	color(0,15);
}

void borrar_cuerpo(){	
	gotoxy(cuerpo[n][0],cuerpo[n][1]);
	printf(" ");	

	gotoxy(cuerpo1[n1][0],cuerpo1[n1][1]);
	printf(" ");
}

void movi(){
	if(kbhit()){
		tecla = getch();
		switch(tecla){
			case ARR:
				if(dir != 2)
					dir = 1;
				break;
			case ABA:
				if(dir != 1)
					dir = 2;
				break;
			case IZQ:
				if(dir != 4)
					dir = 3;
				break;
			case DER:
				if(dir != 3)
					dir = 4;
				break;
			case W:
				if(dir1 != 2)
					dir1 = 1;
				break;
			case S:
				if(dir1 != 1)
					dir1 = 2;
				break;
			case A:
				if(dir1 != 4)
					dir1 = 3;
				break;
			case D:
				if(dir1 != 3)
					dir1 = 4;
				break;
			case POS:
					comando = 1;
				break;
			case NEG:
					comando = 2;
				break;																
		}
	}	
}

void ejecutar(){
	if(dir == 1)y--;
	if(dir == 2)y++;
	if(dir == 3)x -= 2;
	if(dir == 4)x += 2;

	if(dir1 == 1)y1--;
	if(dir1 == 2)y1++;
	if(dir1 == 3)x1 -= 2;
	if(dir1 == 4)x1 += 2;
	
	if(comando == 1)v -= 10;
	if(comando == 2)v += 10;
	if(v<20)v = 20;
	if(v>200)v = 200;
	comando = 0;	
}

void proba_comida(){
	bool a;
	
	if((rand() % 100) + 1 < 11 && fs == false){
		color(0,9);
		fs = true;
		do{
			a = false;
			xs = (rand() % 111) + 5;
			ys = (rand() % 23) + 5;
			for(int i = 1; i < tam; i++){
				if(xs == cuerpo[i][0] && ys == cuerpo[i][1])
					a = true;
			}
		}while(a == true || xs % 2 == 0 || (xs == xc && ys == yc) || (xs == xt && ys == yt) || (xs == xa && ys == ya));
		tam++;
		gotoxy(xs,ys);printf("%c",254);		
	}
	
	if((rand() % 100) + 1 < 11 && ft == false){
		color(0,10);
		ft = true;
		do{
			a = false;
			xt = (rand() % 111) + 5;
			yt = (rand() % 23) + 5;
			for(int i = 1; i < tam; i++){
				if(xt == cuerpo[i][0] && yt == cuerpo[i][1])
					a = true;
			}
		}while(a == true || xt % 2 == 0 || (xt == xc && yt == yc) || (xt == xs && yt == ys) || (xt == xa && yt == ya));
		tam++;
		gotoxy(xt,yt);printf("%c",254);		
	}
	
	if((rand() % 100) + 1 < 11 && fa == false){
		color(0,13);
		fa = true;
		do{
			a = false;
			xa = (rand() % 111) + 5;
			ya = (rand() % 23) + 5;
			for(int i = 1; i < tam; i++){
				if(xa == cuerpo[i][0] && ya == cuerpo[i][1])
					a = true;
			}
		}while(a == true || xa % 2 == 0 || (xa == xc && ya == yc) || (xa == xs && ya == ys) || (xa == xt && ya == yt));
		tam++;
		gotoxy(xa,ya);printf("%c",254);		
	}	
	
	color(0,15);
}

void proba_comida1(){
	bool a;
	
	if((rand() % 100) + 1 < 11 && fs == false){
		color(0,9);
		fs = true;
		do{
			a = false;
			xs = (rand() % 111) + 5;
			ys = (rand() % 23) + 5;
			for(int i = 1; i < tam1; i++){
				if(xs == cuerpo1[i][0] && ys == cuerpo1[i][1])
					a = true;
			}
		}while(a == true || xs % 2 == 0 || (xs == xc && ys == yc) || (xs == xt && ys == yt) || (xs == xa && ys == ya));
		tam1++;
		gotoxy(xs,ys);printf("%c",254);		
	}
	
	if((rand() % 100) + 1 < 11 && ft == false){
		color(0,10);
		ft = true;
		do{
			a = false;
			xt = (rand() % 111) + 5;
			yt = (rand() % 23) + 5;
			for(int i = 1; i < tam1; i++){
				if(xt == cuerpo1[i][0] && yt == cuerpo1[i][1])
					a = true;
			}
		}while(a == true || xt % 2 == 0 || (xt == xc && yt == yc) || (xt == xs && yt == ys) || (xt == xa && yt == ya));
		tam1++;
		gotoxy(xt,yt);printf("%c",254);		
	}
	
	if((rand() % 100) + 1 < 11 && fa == false){
		color(0,13);
		fa = true;
		do{
			a = false;
			xa = (rand() % 111) + 5;
			ya = (rand() % 23) + 5;
			for(int i = 1; i < tam1; i++){
				if(xa == cuerpo1[i][0] && ya == cuerpo1[i][1])
					a = true;
			}
		}while(a == true || xa % 2 == 0 || (xa == xc && ya == yc) || (xa == xs && ya == ys) || (xa == xt && ya == yt));
		tam1++;
		gotoxy(xa,ya);printf("%c",254);		
	}	
	
	color(0,15);
}

void comer(){
	bool a;

	//SNAKE 1
	
	color(0,4);

	if(x == xc && y == yc){
		do{
			a = false;
			xc = (rand() % 111) + 5;
			yc = (rand() % 23) + 5;
			for(int i = 1; i < tam; i++){
				if(xc == cuerpo[i][0] && yc == cuerpo[i][1])
					a = true;
			}
		}while(a == true || xc % 2 == 0 || (xc == xs && yc == ys) || (xc == xt && yc == yt) || (xc == xa && yc == ya));
	//	v--;
		tam++;
		sco += 10;
		gotoxy(xc,yc);printf("%c",254);
		proba_comida();
	}		
	
	if(x == xs && y == ys){
		fs = false;
		v -= 5;
		sco += 300;
		xs = 0;
		ys = 0;	
	}
	
	if(x == xt && y == yt){
		ft = false;
		tam *= 2;
		sco += 300;
		xt = 0;
		yt = 0;	
	}
	
	if(x == xa && y == ya){
		fa = false;			
		bw = true;
		pa = 200;
		mapa();		
		xa = 0;
		ya = 0;	
	}			
	
	//SNAKE 2

	color(0,4);

	if(x1 == xc && y1 == yc){
		do{
			a = false;
			xc = (rand() % 111) + 5;
			yc = (rand() % 23) + 5;
			for(int i = 1; i < tam1; i++){
				if(xc == cuerpo1[i][0] && yc == cuerpo1[i][1])
					a = true;
			}
		}while(a == true || xc % 2 == 0 || (xc == xs && yc == ys) || (xc == xt && yc == yt) || (xc == xa && yc == ya));
	//	v--;
		tam1++;
		sco += 10;
		gotoxy(xc,yc);printf("%c",254);
		proba_comida1();
	}		
	
	if(x1 == xs && y1 == ys){
		fs = false;
		v -= 5;
		sco += 300;
		xs = 0;
		ys = 0;	
	}
	
	if(x1 == xt && y1 == yt){
		ft = false;
		tam1 *= 2;
		sco += 300;
		xt = 0;
		yt = 0;	
	}
	
	if(x1 == xa && y1 == ya){
		fa = false;			
		bw = true;
		pa = 200;
		mapa();		
		xa = 0;
		ya = 0;	
	}		
}

bool game_over(){
	if(bw == false){
		if((y == 4 || y == 28 || x == 3 || x == 117) && (y1 == 4 || y1 == 28 || x1 == 3 || x1 == 117)){
			winer = 3;
			return true;
		}
		if(y == 4 || y == 28 || x == 3 || x == 117){
			winer = 2;
			return true;
		}
		if(y1 == 4 || y1 == 28 || x1 == 3 || x1 == 117){
			winer = 1;
			return true;
		}
	}
	else{
		if(y == 4)
			y = 27;
		if(y == 28)
			y = 5;
		if(x == 3)
			x = 115;
		if(x == 117)
			x = 5;		

		if(y1 == 4)
			y1 = 27;
		if(y1 == 28)
			y1 = 5;
		if(x1 == 3)
			x1 = 115;
		if(x1 == 117)
			x1 = 5;			
	}
				
	int i;
	for(i = 0; i < tam; i++){
		if((x == cuerpo[i][0] && y == cuerpo[i][1]) || (x == cuerpo1[i][0] && y == cuerpo1[i][1])){
			bw = false;
			winer = 2;
			break;
		}
	}

	for(i = 0; i < tam1; i++){
		if((x1 == cuerpo1[i][0] && y1 == cuerpo1[i][1]) || (x1 == cuerpo[i][0] && y1 == cuerpo[i][1])){
			bw = false;
			if(winer == 2)
				winer = 3;
			else
				winer = 1;
			return true;
		}
	}

	if(winer == 2)
		return true;
	
/*	
	for(int i; i < tam; i++){
		if(3 == cuerpo[i][0] || 4 == cuerpo[i][1]){
			bw = false;
			return true;
		}
	}
	
	for(int i; i < tam; i++){
		if(117 == cuerpo[i][0] || 28 == cuerpo[i][1]){
			bw = false;
			return true;
		}
	}			*/	
	
	return false;		
}

void limpiar(){
	color(0,15);
	for(int i = 5; i < 28; i++)
		for(int j = 5; j < 116; j++){
			gotoxy(j,i);
			printf(" ");
		}		
}

void contas(){
	if(bw == true){
		pa--;	
		if(pa == 0){
			bw = false;
			mapa();
		}
	}
}

void jugar(){
	begin();

	while(tecla != ESC && !game_over()){
		borrar_cuerpo();	
		guardar_posicion();
		pintar_cuerpo();		
		comer();
		
		movi();
		movi();
		
		ejecutar();
		contas();
		marcador();
		Sleep(v);
	}
	
	char name[63];
	
	gotoxy(42,9);
	if(winer == 3){
		color(0,5);
		printf("TIE");
	}
	else{
		if(winer == 1)
			color(0,10);
		else
			color(0,11);
		printf("WINER PLAYER %i\n",winer);
	}

	color(0,4);
	gotoxy(42,10);
	printf("%c%c%c %c%c%c %c%c%c%c%c %c%c%c %c%c%c%c %c   %c %c%c%c %c%c%c",220,223,223,220,223,220,219,219,220,219,219,219,223,223,220,223,223,220,219,219,219,223,223,219,223,220);
	gotoxy(42,11);
	printf("%c %c %c%c%c %c %c %c %c%c%c %c  %c  %c %c  %c%c%c %c%c%c",219,219,219,220,219,219,223,219,219,223,223,219,219,219,219,219,223,223,219,223,220);
	gotoxy(42,12);
	printf(" %c%c %c %c %c   %c %c%c%c  %c%c    %c   %c%c%c %c %c",223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223);
	
	color(0,5);
	
	gotoxy(50,14);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187);		
	gotoxy(50,15);
	printf("%c                  %c",186,186);
	gotoxy(50,16);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);	
	
	mostrar_cursor();
	
	do{
		gotoxy(51,15);
		color(0,15);
		fflush(stdin);
		gets(name);
		if(strcmp(name,"")==0 || strlen(name)>18){
			color(0,4);
			gotoxy(50,14);
			printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187);		
			gotoxy(50,15);
			printf("%c                  %c",186,186);
			gotoxy(50,16);
			printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);
			gotoxy(36,18);
			printf("*ERROR ESPACIO DEL NOMBRE VACIO O SOBRE EXTENDIDO");
			gotoxy(70,15);
			printf("                                             ");		
		}		
	}while(strcmp(name,"")==0 || strlen(name)>18);
	
	espacio(&name[0],&name[strlen(name)]);
	strcpy(name,strupr(name));
	
	ocultar_cursor();
	limpiar();
	
	if(!p){
		p = new player;
		p->sig = NULL;
		strcpy(p->nom,name);
		p->score = sco;
	}
	else{
		player *a = p;
		if(sco >= a->score){
			a = new player;
			a->sig = p;
			p = a;
			strcpy(a->nom,name);
			a->score = sco;
		}
		else{
			while(a->sig && sco < a->sig->score){
				a = a->sig;
			}
			if(a->sig){
				player *n = new player;
				n->sig = a->sig;
				strcpy(n->nom,name);
				n->score = sco;
				a->sig = n;
			}
			else{
				player *n = new player;
				n->sig = NULL;
				a->sig = n;
				strcpy(n->nom,name);
				n->score = sco;					
			}	
		}	
	}
	
	escribir_score();
	
	color(0,15);				
}

void creditos(){
	color(0,2);
	
	gotoxy(44,6);
	printf("%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c%c %c%c%c",220,223,223,219,223,220,219,223,223,219,223,220,223,219,223,223,219,223,220,223,223,220,220,223,223);
	gotoxy(44,7);
	printf("%c   %c%c%c %c%c%c %c %c  %c   %c  %c  %c  %c%c",219,219,223,220,219,223,223,219,219,219,219,219,219,223,220);
	gotoxy(44,8);
	printf(" %c%c %c %c %c%c%c %c%c  %c%c%c  %c   %c%c  %c%c ",223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223);
	
	gotoxy(39,11);
	printf("LEAD DEVELOPERS          EZEQUIEL MONTERO");
	
	gotoxy(39,13);
	printf("MUSIC BY                 EZEQUIEL MONTERO");
	
	gotoxy(39,15);
	printf("CONCEPT WORK             EZEQUIEL MONTERO");
	
	gotoxy(39,17);
	printf("SNAKE THEORY SUPPORT     EZEQUIEL MONTERO");
	
	gotoxy(39,19);
	printf("PROFESSIONAL CONSULTANT  EZEQUIEL MONTERO");
	
	gotoxy(39,21);
	printf("SPECIAL THANKS           EZEQUIEL MONTERO");				
	
	gotoxy(6,27);
	system("pause");
	color(0,15);
}

void score(){
	player *a = p;
	
	color(0,1);
	gotoxy(42,6);
	printf("%c %c %c%c%c %c%c%c %c %c   %c%c%c %c%c%c %c%c%c%c %c%c%c %c%c%c",219,219,223,219,223,220,223,223,219,219,220,223,223,220,223,223,220,223,223,220,219,223,220,219,223,223);
	color(0,9);
	gotoxy(42,7);
	printf("%c%c%c  %c  %c %c %c%c%c    %c%c %c   %c  %c %c%c%c %c%c%c",219,223,219,219,219,219,219,223,219,223,220,219,219,219,219,223,220,219,223,223);
	color(0,3);
	gotoxy(42,8);
	printf("%c %c %c%c%c  %c%c %c %c   %c%c   %c%c  %c%c  %c %c %c%c%c",223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223);
	
/*	gotoxy(42,10);
	printf("%c%c%c %c%c%c %c%c %c %c%c%c   %c%c %c%c %c%c%c %c%c%c %c%c   %c%c %c %c%c%c %c%c%c%c%c %c%c",219,223,220,220,223,220,219,220,219,219,220,223,219,223,220,223,220,223,220,219,223,220,219,223,219,220,219,220,223,220,219,219,220,219,219,219,223);
	gotoxy(42,11);
	printf("%c%c%c %c%c%c %c %c%c %c%c%c   %c%c %c%c %c%c%c %c%c%c %c%c   %c %c%c %c%c%c %c %c %c %c%c",219,223,220,219,223,219,219,223,219,219,223,220,220,219,223,220,223,220,223,219,223,220,219,220,219,223,219,219,223,219,219,223,219,219,220);	
*/
	color(0,4);
	gotoxy(42,11);
	printf("RANK            SCORE             NAME");
	
	if(a){
		color(0,6);
		gotoxy(42,14);
		printf("1ST");	
		gotoxy(58,14);
		printf("000000");
		gotoxy(divbit(63,a->score),14);
		printf("%i",a->score);
		gotoxy(76,14);
		printf("%s",a->nom);
	
		
		if(a->sig){
			a = a->sig;
			color(0,11);
			gotoxy(42,16);
			printf("2ND");
			gotoxy(58,16);
			printf("000000");
			gotoxy(divbit(63,a->score),16);
			printf("%i",a->score);
			gotoxy(76,16);
			printf("%s",a->nom);
		}
		
		if(a->sig){
			a = a->sig;	
			gotoxy(42,18);
			printf("3RD");
			gotoxy(58,18);
			printf("000000");
			gotoxy(divbit(63,a->score),18);
			printf("%i",a->score);
			gotoxy(76,18);
			printf("%s",a->nom);	
		}
		
		if(a->sig){
			a = a->sig;	
			color(0,15);
			gotoxy(42,20);
			printf("4TH");
			gotoxy(58,20);
			printf("000000");
			gotoxy(divbit(63,a->score),20);
			printf("%i",a->score);
			gotoxy(76,20);
			printf("%s",a->nom);
		}
		
		if(a->sig){
			a = a->sig;	
			gotoxy(42,22);
			printf("5TH");
			gotoxy(58,22);
			printf("000000");
			gotoxy(divbit(63,a->score),22);
			printf("%i",a->score);
			gotoxy(76,22);
			printf("%s",a->nom);
		}
	
	}
	gotoxy(6,27);
	system("pause");
	color(0,15);		
}

void menu(){
	int opc = 1;
	bool b;
	char op;
	mapa();
	leer_score();

	while(opc != 4 || b != true){
		b = false;
		
		gotoxy(47,15);printf("  ");
		gotoxy(47,18);printf("  ");
		gotoxy(47,21);printf("  ");
		gotoxy(47,24);printf("  ");
		
	
		color(0,10);
		gotoxy(50,10);
		printf("%c%c%c %c%c %c %c%c%c %c %c %c%c%c",220,223,223,219,220,219,220,223,220,219,219,219,223,223);
		gotoxy(50,11);
		printf(" %c%c %c%c%c%c %c%c%c %c%c%c %c%c%c",223,220,219,223,219,219,219,220,219,219,223,220,219,223,223);
		gotoxy(50,12);
		printf("%c%c  %c  %c %c %c %c %c %c%c%c",223,223,223,223,223,223,223,223,223,223,223);
		
		color(0,13);		
		
		if(opc == 1){
			color(0,14);
			gotoxy(47,15);printf(">>");
		}
		gotoxy(50,14);
		printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187);		
		gotoxy(50,15);
		printf("%c      JUGAR       %c",186,186);
		gotoxy(50,16);
		printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);
		
		color(0,13);
		
		if(opc == 2){
			color(0,14);
			gotoxy(47,18);printf(">>");
		}		
		gotoxy(50,17);
		printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187);		
		gotoxy(50,18);
		printf("%c      SCORE       %c",186,186);
		gotoxy(50,19);
		printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);

		color(0,13);

		if(opc == 3){
			color(0,14);
			gotoxy(47,21);printf(">>");
		}
		gotoxy(50,20);
		printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187);		
		gotoxy(50,21);
		printf("%c     CREDITOS     %c",186,186);
		gotoxy(50,22);
		printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);
		
		color(0,13);

		if(opc == 4){
			color(0,14);
			gotoxy(47,24);printf(">>");
		}
		gotoxy(50,23);
		printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",201,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,187);		
		gotoxy(50,24);
		printf("%c      SALIR       %c",186,186);
		gotoxy(50,25);
		printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",200,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,205,188);						
		
		color(0,15);

		if(kbhit){
			op = getch();
				
			switch(op){
				case ARR:
					opc--;
					break;
				case ABA:
					opc++;
					break;
				case ENT:
					b = true;
					break;	
			}
			if(opc == 5)
				opc = 1;
			if(opc == 0)
				opc = 4;
		}
		
		
		if(b == true)
			switch(opc){
				case 1:
					limpiar();
					jugar();
					mapa();
					limpiar();
					break;
				case 2:
					limpiar();
					score();
					limpiar();
					break;
				case 3:
					limpiar();
					creditos();
					limpiar();
					break;	
				case 4:
					break;	
			}
		
	//	Sleep(50);	
	}
}

int main(){
	color(0,15);
	ocultar_cursor();
	srand(time(NULL));
	
	menu();
	
	gotoxy(4,29);
//	system("pause");
	return 0;
}
