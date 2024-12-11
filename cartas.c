#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <windows.h>
#include <conio.h>

#define BARALHO 52


typedef struct carta {
	int Valor;
	char Naipe[10];
} Carta;

typedef struct elementopilha {  //Struct para as pilhas
    Carta carta;
	struct elementopilha *anterior;
} ElPilha;

typedef struct pilha {    //Struct Pilha
	struct elementopilha *topo;
} Pilha;

typedef struct elementoLista{

	Carta carta;
	struct elementoLista *proximo;

} ElLista;

typedef struct lista{

	ElLista *inicio;

} Lista;


typedef struct jogador {

	char NomeJogador[50];
	Pilha *Monte;
	int nCartasM;
	Lista *Mao;
	int Vitorias;

} Jogador;

typedef struct elementoListaVencedores {

	Jogador jogador;
	int NuVitorias;

} ListaVencedores;


void esconderCursor();
void mostrarCursor();
void gotoxy(int x, int y);
void SetCor(int ForgC);



void inicializaJogo (int *nJogador, int *nCartas);
Jogador* inicializaJogadores(int nJogador);
void telaCarregamento();

//Funções especificas do baralho

struct carta* criaBaralho();                                       //Função que inicializa um baralho
void embaralhaBaralho(struct carta *p);
void printaBaralho(struct carta *p);
struct carta* iniciaBaralhoCompra(int nCartas);
struct pilha* iniciaPilhaCompra(struct carta *BaralhoC, int nCartas);

int PrintaJogoPadrao(struct jogador *Jogadores, struct lista *Descarte, struct pilha *PilhaDeCompra);
void JogadasRM (struct jogador *Jogadores, int nJogadores, int x, int y, struct pilha *PilhaDeCompra, struct lista *Descarte, int *i);
Pilha* roubamonteDesempilha(struct pilha *p);                         //Funções equivalentes a roubar um monte (desempilha e  empilha)
void roubamonteEmpilha(struct pilha *p, struct pilha *nMonte);

void printaGanhaJogadores (struct jogador *Jog, int NumeroJogadores);
void printaCartasGanha(int QuantPilha, struct carta *C);

struct pilha* criaPilha();
int vaziaPilha(struct pilha *p);
void empilha(struct pilha *p, struct carta v);
int removePilha(struct pilha *p, struct carta *v);
int quantPilha (struct pilha *p);
void liberaPilha(struct pilha *p);

struct lista* crialista();
void liberalista (struct lista *p);
int vazialista (struct lista *p);
void adicionarpi (struct lista *p, struct carta v);
int removerF (Lista *p, struct carta *v);
int removerP (Lista *p, struct carta *v, int c);
int recupC (Lista *p, struct carta v);
int quantLista (struct lista *p);

void quantCartas (struct jogador *Jogadores, int nJogadores);          //Funcoes do fim jogo
void Insercao (int nJogadores, struct jogador *p);
void rbGanhadores (struct jogador *Jog, int nJogadores);
Carta* criaVetorGanha (struct jogador *Jog);
void particionar (int esq, int dir, int *i, int *j, struct carta *c);
void quickSort (int esq, int dir, struct carta *c);




int main(){

	int x = 45, y = 0, i = 0, j = 0;

	int NumeroJogadores, NumeroCartas, QuantPilha, cartav;
	Pilha *PilhaDeCompra;
	Jogador *Jogadores;
	Carta *baralho;
	Lista *Descarte;
	Carta v;
	Carta *v2;

	do {

		inicializaJogo(&NumeroJogadores,&NumeroCartas);
		baralho = iniciaBaralhoCompra(NumeroCartas);
		PilhaDeCompra = iniciaPilhaCompra(baralho,NumeroCartas);
		Jogadores = inicializaJogadores(NumeroJogadores);
		telaCarregamento();

		Descarte = crialista();

		do {

			JogadasRM(Jogadores, NumeroJogadores, x, y, PilhaDeCompra, Descarte, &i);

		} while(PilhaDeCompra->topo != NULL);

		quantCartas (Jogadores, NumeroJogadores);
		Insercao(NumeroJogadores, Jogadores);
		rbGanhadores(Jogadores, NumeroJogadores);
		v2 = criaVetorGanha(Jogadores);
		quickSort (0, Jogadores[0].nCartasM-1, v2);
		QuantPilha = Jogadores[0].nCartasM;
		printaGanhaJogadores (Jogadores, NumeroJogadores);
		printaCartasGanha(QuantPilha, v2);

		setbuf(stdin, NULL);
		getchar();

	} while (j == 0);






	return 0;

}

void esconderCursor(){										//Função para esconder o cursor do terminal (copiei do google edwaldo me perdoa :(

   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);

}

void gotoxy (int x, int y){

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position;
    position.X = x;
    position.Y = y;
    SetConsoleCursorPosition(hConsole, position);

}

void SetCor(int ForgC) {									//Função para determinar a cor do texto no terminal (vou nem falar mais da onde veio)
	WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if(GetConsoleScreenBufferInfo(hStdOut, &csbi)){

		wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);

     }

     return;

 }

 void inicializaJogo (int *nJogador, int *nCartas){

	int a, b;

	do {

		printf (
		"\n"
		"\n"
		);

		SetCor(12);
		printf (
		"                                               Bem-vindo ao Rouba-Monte                                                  \n"
		);

		printf (
		"\n"
		);

		SetCor(12);
		printf(
		"                                                  1. Iniciar jogo                                                       \n"
		"                                                  2. Creditos                                                           \n"
		"                                                  3. Pontuacao                                                          \n"
		"                                                  4. Sair do jogo                                                       \n"
		);

		printf(
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		);

        setbuf  (stdin, NULL);
		scanf ("%d", &a);
		system ("cls");

		if (a == 2){

            printf (
            "\n"
            "\n"
            );

			SetCor(9);
			printf(
			"                                               Bem-vindo ao Rouba-Monte                                                  \n"
			"\n"
			);

			SetCor(12);
			printf(
			"                                             Feito por Jonathas Figueiredo                                             \n\n"
			"                                                Engenharia da Computacao                                               \n\n"
			"                                                   UEMG DIVINOPOLIS                                                      \n"
			);

			printf(
			"\n"
			"\n"
			"\n"
			"\n"
			"\n"
			"\n"
			);

            setbuf (stdin, NULL);
            getchar();

		}

		if (a == 3){
			break;
		}

		system ("cls");

	} while (a != 1 && a != 4);

	if (a == 4){

		printf ("Encerrando o jogo!\n");
		sleep(2);
		exit(1);

	}

	SetCor(9);
	printf(
	"\n\n"
	"                                               Bem-vindo ao Rouba-Monte                                                  \n"
	"                                                                                                                           \n"
	);

	SetCor(12);
	printf(
	"                                       Digite a quantidade de jogadores desejada:                                        \n"
	);

	printf(
	"                                                                                                                           \n"
	);

	SetCor(12);
	printf(
	"                                                   Digite aqui:                                                            \n"
	);

	SetCor(9);
	printf(
	"\n"
	"\n"
	"\n"
	"\n"
	"\n"
	"\n"
	"\n"
	"\n"
	);

    gotoxy (64,10);
	scanf ("%d", &b);
	*nJogador = b;
	system ("cls");

	SetCor(9);
	printf (
	"\n\n"
	"                                               Bem-vindo ao Rouba-Monte                                                  \n"
	"\n\n"
	);

	SetCor(12);
	printf(
	"                                      Digite o numero de baralhos a serem utilizados:                                       \n"
	);

	SetCor(9);
	printf(
	"                                                                                                                          \n"
	);

	SetCor(12);
	printf(
	"                                                   Digite aqui:                                                           \n"
	);

	SetCor(9);
	printf(
	"                                                                                                                          \n"
	"\n"
	"\n"
	"\n"
	"\n"
	"\n"
	"\n"
	"\n"
	);

	gotoxy(64, 10);
	scanf ("%d", &b);
	*nCartas = b;

	system ("cls");

}


Jogador* inicializaJogadores(int nJogador){                       //Funcao que inicializa os dados necessarios para cada jogador

	Jogador *p = (Jogador*) malloc(sizeof(Jogador) * nJogador);

	int x = 47, y = 5;

	for (int i = 0; i < nJogador; i++){


		gotoxy(x, y);
		SetCor(12);

		printf ("Digite o nome do jogador %d: ", i+1);
		setbuf (stdin, NULL);
		SetCor(9);
		fgets (p[i].NomeJogador, 50, stdin);
		p[i].NomeJogador[strcspn(p[i].NomeJogador, "\n")] = 0;

		p[i].Monte = criaPilha();
		p[i].nCartasM = 0;
		p[i].Mao = crialista();
		p[i].Vitorias = 0;

		y += 3;



	}

	system ("cls");
	return p;

}

void telaCarregamento(){                           //Frescuragem de tela de carregamento

    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 5010000;

	esconderCursor();
	SetCor(9);
	for(int i = 0; i < 5; i++){

		printf (
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                   Carregando:                                                                \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		);

		nanosleep(&ts, NULL);
		system ("cls");

		printf (
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                   Carregando: .                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		);

		nanosleep(&ts, NULL);
		system ("cls");

		printf (
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                   Carregando: ..                                                       \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		);

		nanosleep(&ts, NULL);
		system ("cls");

		printf (
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                   Carregando: ...                                                      \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		);

		nanosleep(&ts, NULL);
		system ("cls");

		printf (
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                   Carregando: ....                                                     \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		);

        nanosleep(&ts, NULL);
		system ("cls");

		printf (
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                   Carregando: .....                                                    \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		);

		nanosleep(&ts, NULL);
		system ("cls");

		printf (
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                   Carregando: ......                                                   \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		"                                                                                                                        \n"
		);

		nanosleep(&ts, NULL);
		system ("cls");

	}

}


Carta* criaBaralho(){                                              //Inicia um vetor de 52 posição com cada carta (1 a 13 de cada naipe)

	Carta *baralho = (Carta*) malloc(sizeof(Carta) * 52);

	int i = 0;

	for (i = 0; i < 13; i++){

		baralho[i].Valor = i+1;
		strcpy(baralho[i].Naipe, "Copas");

	}

	for (i = 13; i < 26; i++){

		baralho[i].Valor = i+1 - 13;
		strcpy(baralho[i].Naipe, "Ouros");

	}

	for (i = 26; i < 39; i++){

		baralho[i].Valor = i+1 - 26;
		strcpy(baralho[i].Naipe, "Paus");

	}

	for (i = 39; i < 52; i++){

		baralho[i].Valor = i+1 - 39;
		strcpy(baralho[i].Naipe, "Espadas");

	}

	return baralho;

}

void embaralhaBaralho(struct carta *p){    								//Função que embaralha as cartas usando um pseudo-algoritmo de randomização com rand

	srand(time(NULL));

	int a, b;
	Carta c;

	for (int i = 0; i < 100; i++){

		a = rand() % 52;
		b = rand() % 52;

		c = p[a];
		p[a] = p[b];
		p[b] = c;

	}

}

void printaBaralho(struct carta *p){     								//Função de printar para controle do código (não se aplica no jogo)


	for (int i = 0; i < BARALHO; i++){

		printf ("%d Naipe: %s\n", p[i].Valor, p[i].Naipe);

	}

}

Carta* iniciaBaralhoCompra(int nCartas){        //Cria um vetor de cartas de tamanho N e recebe as cartas embaralhadas

	int vefBaralho = 0;
	Carta *p = (Carta*) malloc(sizeof(Carta) * (nCartas * 52));
    Carta *baralho;
    int j = 0;

	for (int i = 0; i < nCartas * 52; i++){

		if (vefBaralho == 0){

			baralho = criaBaralho();
			embaralhaBaralho(baralho);
			vefBaralho = 1;

		}

		p[i] = baralho[j];
		j++;

		if (i > 0){
			if (i % 51 == 0){
				j = 0;
				vefBaralho = 0;

			}

		}

	}

	return p;

}

Pilha* iniciaPilhaCompra(struct carta *BaralhoC, int nCartas){    //Passa as cartas embaralhadas para a a pilha de compra

	Pilha *p = criaPilha();

	for (int i = 0; i < nCartas * 52; i++){

		empilha(p, BaralhoC[i]);

	}

	return p;

}


int PrintaJogoPadrao(struct jogador *Jogadores, struct lista *Descarte, struct pilha *PilhaDeCompra){

    int QuantPilha;

	SetCor(8);
	gotoxy(53, 0);
	printf ("Pilha de compras");
	SetCor(7);
	gotoxy(56, 1);
	QuantPilha = quantPilha(PilhaDeCompra);
	printf ("%d cartas\n", QuantPilha);

    int x = 42, y = 3;
	int i;

	int nLista = quantLista(Descarte);

	if (nLista > 0){

		ElLista *aux = Descarte->inicio;


		for(i = 1; i <= nLista; i++){

			SetCor(10);

			gotoxy(x, y);
			printf (
			"------\n"
			);

			gotoxy(x,y+1);
			printf (
			"|*   |\n"
			);

			SetCor(15);
			gotoxy(x,y+2);
			printf (
			"  %d  \n"
			, aux->carta.Valor);

			SetCor(10);
			gotoxy(x,y+3);
			printf (
			"|   *|\n"
			);

			gotoxy(x,y+4);
			printf (
			"------\n"
			);

			aux = aux->proximo;
			x += 8;

			if (i % 5 == 0){

				x = 42;
				y += 5;

			}

			printf ("\n\n\n\n\n");

		}

	}

	return y;

}

void JogadasRM (struct jogador *Jogadores, int nJogadores, int x, int y, struct pilha *PilhaDeCompra, struct lista *Descarte, int *i){

	int Cor;
	Carta J1, J2, J3;
	int j;
	int VefJogada, vefFuncao = -1;
	Pilha *nMonte;
	int quantidadePilha;



	VefJogada = 1;
	Carta cartav, cartav1;


	do {

		if (*i % nJogadores == 0)
			*i = 0;

		int nLista = quantLista(Descarte);
		y = PrintaJogoPadrao(Jogadores, Descarte, PilhaDeCompra);

		if (Descarte->inicio == NULL)
			y -= 5;



		for (j = 0; j < nJogadores; j++){

			if (j % 2 == 0)
				Cor = 11;

			else
				Cor = 14;

			if (nLista % 5 == 0 && nLista != 0 && j == 0)
				y -= 5;

			if (Jogadores[j].Monte->topo == NULL){

				SetCor(Cor);
				gotoxy(49,y+5);
				printf ("Topo do monte de %s: 0\n", Jogadores[j].NomeJogador);
				y++;


			}
			else{

				SetCor(Cor);
				gotoxy(49,y+5);
				quantidadePilha = quantPilha(Jogadores[j].Monte);
				printf ("Topo do monte de %s: %d (Tamanho: %d)\n", Jogadores[j].NomeJogador, Jogadores[j].Monte->topo->carta.Valor, quantidadePilha);
				y++;

			}
	}

		if (*i % 2 == 0)
			Cor = 11;

		else
			Cor = 14;

		SetCor(Cor);

		gotoxy(50, y + 7);
		printf ("Vez de ");

		SetCor(4);
		printf ("%s!\n", Jogadores[*i].NomeJogador);
		SetCor(Cor);

		if (Jogadores[*i].Mao->inicio == NULL){

			SetCor(4);
			gotoxy (37, y + 8);
			printf ("Sua mao esta vazia! ");
			SetCor(2);
			printf("Comprando da pilha de compras...\n");

			SetCor(Cor);
			removePilha(PilhaDeCompra, &cartav);
			adicionarpi(Jogadores[*i].Mao, cartav);

			gotoxy(47, y + 9);
			printf ("Carta adicionada a sua mao!\n");

			SetCor(10);
			gotoxy(55, y + 10);
			printf (
			"------\n"
			);

			gotoxy(55, y + 11);
			printf (
			"|*   |\n"
			);

			SetCor(15);
			gotoxy(55, y + 12);
			printf (
			"  %d  \n"
			, cartav.Valor);

			SetCor(10);
			gotoxy(55, y + 13);
			printf (
			"|   *|\n"
			);

			gotoxy(55, y + 14);
			printf (
			"------\n"
			);

		}

		for (j = 0; j < nJogadores; j++){

			if (Jogadores[*i].Monte->topo != NULL)
					J1 = Jogadores[*i].Monte->topo->carta;

			if (Jogadores[j].Monte->topo != NULL)
					J2 = Jogadores[j].Monte->topo->carta;

			if (Jogadores[*i].Mao->inicio != NULL){

				if (Jogadores[*i].Mao->inicio != NULL)
						J3 = Jogadores[*i].Mao->inicio->carta;

			}


			if (*i != j){

				if (Jogadores[j].Monte->topo != NULL && Jogadores[*i].Monte->topo != NULL){

					if (J3.Valor == J2.Valor){

						gotoxy(26, y + 15);
						printf ("Sua carta e igual ao topo do monte de %s! Roubando seu monte e mais uma jogada!\n", Jogadores[j].NomeJogador);
						nMonte = roubamonteDesempilha(Jogadores[j].Monte);
						roubamonteEmpilha(Jogadores[*i].Monte, nMonte);
						removerF(Jogadores[*i].Mao, &cartav1);
						empilha(Jogadores[*i].Monte, cartav1);

						VefJogada++;
						break;
					}

				}

			}

			if (VefJogada < 2){  //Para garantir que nao se faça duas jogadas em um round, esse verificação torna-se necessaria

				if (J1.Valor == J3.Valor && Jogadores[*i].Monte->topo != NULL){

						gotoxy(26, y + 15);
						printf ("Sua carta em mao e igual a o topo de seu monte! Monte maior e mais uma jogada!\n");
						removerF(Jogadores[*i].Mao, &cartav1);
						empilha(Jogadores[*i].Monte, cartav1);

						VefJogada++;

					}

				else {


					if(Descarte->inicio != NULL){

						vefFuncao = recupC(Descarte, cartav);

					}

					if (vefFuncao >= 0){

						gotoxy(26, y + 15);
						printf ("Sua carta e igual a uma da area de descarte! Monte maior e mais uma jogada!\n");

						removerP(Descarte, &cartav1, vefFuncao);
						empilha(Jogadores[*i].Monte, cartav1);
						removerF(Jogadores[*i].Mao, &cartav1);
						empilha(Jogadores[*i].Monte, cartav1);

						VefJogada++;
						vefFuncao = -1;

					}

				}

			}

		}

		VefJogada--;

		if (VefJogada == 0){

			gotoxy(26, y + 15);
			printf ("Sem outras opcoes disponiveis! Descartando sua carta e fim da jogada\n");
			removerF(Jogadores[*i].Mao, &cartav1);
			adicionarpi(Descarte, cartav1);
			*i += 1;

		}


		setbuf(stdin, NULL);
		getchar();
		system ("cls");



	} while (VefJogada != 0 && PilhaDeCompra->topo != NULL);

}

Pilha* roubamonteDesempilha(struct pilha *p){

	Carta v;

	Pilha *nMonte = criaPilha();
	ElPilha *aux = p->topo;

	while (p->topo != NULL){

		aux = p->topo;
		removePilha(p, &v);
		p->topo = aux->anterior;
		free(aux);
		empilha(nMonte, v);

	}

	p->topo = NULL;

	return nMonte;

}
void roubamonteEmpilha(struct pilha *p, struct pilha *nMonte){

	Carta v;

	ElPilha *aux;

	while (nMonte->topo != NULL){

        aux = nMonte->topo;
		removePilha(nMonte, &v);
		nMonte->topo = aux->anterior;
		free(aux);
		empilha(p, v);

	}

}




Pilha* criaPilha(){                                     //Funcoes especificas das pilhas

	Pilha *p;
	p = (Pilha*) malloc(sizeof(Pilha) * 1);
	p->topo = NULL;
	return p;

}

int vaziaPilha(Pilha *p){

	if (p->topo == NULL)
		return 1;

	else
		return 0;

}

void empilha(struct pilha *p, struct carta v){

	ElPilha *novo = (ElPilha*) malloc(sizeof(ElPilha) * 1);

	novo->carta = v;

	novo->anterior = p->topo;

	p->topo = novo;

}

int removePilha(struct pilha *p, struct carta *v){    //Função de compra de carta com remoção de elemento de pilha

	if (vaziaPilha(p) == 1)
		return 0;

	else {

		*v = p->topo->carta;
		ElPilha *novo;
		novo = p->topo;
		p->topo = novo->anterior;

	}
	return 1;

}

int quantPilha (Pilha *p){     //Contar as cartas de uma pilha

	ElPilha *aux;
	aux = p->topo;
	int cont = 0;

	 if (p->topo == NULL){

        return 0;

    }

	while (aux != NULL){

		aux = aux->anterior;
		cont++;

	}

	return cont;

}

void liberaPilha(Pilha *p){

	ElPilha *aux;
    aux = p->topo;

	while (aux != NULL){

		p->topo = aux->anterior;
		free (aux);

	}

	free(p);

}



Lista* crialista (){ 										//Funcoes das listas

	Lista *p = (Lista*) malloc(sizeof(Lista) * 1);
	p->inicio = NULL;
	return p;

}

void liberalista (Lista *p){

  if (p->inicio != NULL){

    ElLista *novo;
    while (p->inicio != NULL){

      novo = p->inicio;
      p->inicio = p->inicio->proximo;
      free (novo);

    }

    free(p);

  }

}

int vazialista (Lista *p){

	if (p->inicio == NULL){
		return 1;
	}
	else{
		return 0;
	}

}


void adicionarpi (struct lista *p, struct carta v){

	ElLista *novo = (ElLista*) malloc(sizeof(ElLista) * 1);
	ElLista *aux;

	aux = p->inicio;
	novo->carta = v;
	novo->proximo = aux;
	p->inicio = novo;
	return;

}



int removerF (Lista *p, struct carta *v){

	ElLista *aux = p->inicio;
	ElLista *novo = p->inicio;

	while (novo->proximo != NULL){

		aux  = novo;
		novo = novo->proximo;

	}

	*v = novo->carta;

	if (novo == p->inicio)

		p->inicio = novo->proximo;



	else
		aux->proximo = novo->proximo;

	free(novo);
	return 1;

}

int removerP (Lista *p, struct carta *v, int c){

	int quantidade = quantLista(p);

    ElLista *novo;
    ElLista *aux = p->inicio;

	if(vazialista(p) == 1)
		return 0;

	if (c <= 0 || c > quantidade){

		return 0;

	}

	else {

		for (int i = 0; i < c - 1; i++){

			novo = aux;
			aux = aux->proximo;

		}

		if (aux == p->inicio){

			p->inicio = aux->proximo;

		}

		else {

		novo->proximo = aux->proximo;

		}

	}

	*v = aux->carta;
	free (aux);
	return 1;

}

int recupC (Lista *p, struct carta v){

	ElLista *aux = p->inicio;
	int i = 1;

	while (aux->proximo != NULL){

		if (aux->carta.Valor == v.Valor){
			return i;
		}

		i++;

		aux = aux->proximo;
	}

	if (aux->carta.Valor == v.Valor){
		return i;
	}

  return -1;

}

int quantLista (Lista *p){

	int cont;
    ElLista *aux = p->inicio;

	if (p->inicio == NULL)
		return 0;

	else {

		cont = 1;

		while (aux->proximo != NULL){

			aux = aux->proximo;
			cont++;

		}

	}

	return cont;

}

void quantCartas (struct jogador *Jogadores, int nJogadores){

	for (int i = 0; i < nJogadores; i++){

		Jogadores[i].nCartasM = quantPilha(Jogadores[i].Monte);

	}

}

void Insercao (int nJogadores, struct jogador *p){

	int i, j, x, min;

	Jogador temp;

	for (i = 0; i < nJogadores - 1; i++){

		min = i;

		for (j = i + 1; j < nJogadores; j++){

			if  (p[j].nCartasM > p[min].nCartasM){

				min = j;

			}

		}

		temp = p[min];
		p[min] = p[i];
		p[i] = temp;

	}

}

void rbGanhadores (struct jogador *Jog, int nJogadores){

	int nCartasG = Jog[0].nCartasM;
	int nGanha = 1;

	for (int i = 0; i < nJogadores; i++){

		if (Jog[i].nCartasM == nCartasG){

			printf ("Ganhador %d: %s\nCartas no monte: %d\n", nGanha, Jog[i].NomeJogador, Jog[i].nCartasM);

		}

	}

}

Carta* criaVetorGanha (struct jogador *Jog){

	Carta v;
	int i = 0;

	int nCartasG = quantPilha(Jog[0].Monte);
	Carta *p = (Carta*) malloc(sizeof(Carta) * nCartasG);
    ElPilha *aux;

	aux = Jog[0].Monte->topo;


	while (Jog[0].Monte->topo != NULL){

		aux = Jog[0].Monte->topo;
		removePilha(Jog[0].Monte, &v);
		Jog[0].Monte->topo = aux->anterior;
		free(aux);
		p[i] = v;
		i++;

	}

	return p;

}

void particionar (int esq, int dir, int *i, int *j, struct carta *c){

	*i = esq;
	*j = dir;

	int x;
	x = c[(*i + *j) / 2].Valor;
	Carta temp;

	do {

		while (x > c[*i].Valor){

			(*i)++;

		}

		while (x < c[*j].Valor){

			(*j)--;

		}

		if (*i <= *j){

			temp = c[*i];
			c[*i] = c[*j];
			c[*j] = temp;
			(*i)++;
			(*j)--;
		}

	} while (*i <= *j);

}

void quickSort (int esq, int dir, struct carta *c){

	int i, j;

	particionar(esq, dir, &i, &j, c);
	if (esq < j)
		quickSort(esq, j, c);
	if (i < dir)
		quickSort(i, dir, c);

}

void printaGanhaJogadores (struct jogador *Jog, int NumeroJogadores){

	for (int i = 0; i < NumeroJogadores; i++){

		printf ("Nome ganhador %d: %s - Cartas em mao: %d\n", i+1, Jog[i].NomeJogador, Jog[i].nCartasM);

	}

}

void printaCartasGanha(int QuantPilha, struct carta *C){

	for (int i = 0; i < QuantPilha; i++){

		printf ("%d %s\n", C[i].Valor, C[i].Naipe);

	}

}
