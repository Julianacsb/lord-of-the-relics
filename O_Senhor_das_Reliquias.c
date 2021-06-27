/*
Sistema operacional: Windows
Programa em C que implementa um jogo de labirinto
*/

////////////////////////INCLUSAO DE BIBLIOTECAS////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

////////////////////////DEFINICAO DE CONSTANTES////////////////////////
#define NOME_ARQUIVO_ENTRADA "entrada.txt"
#define NOME_ARQUIVO_SAIDA "saida.txt"
#define MAX_OPCOES 10

//////////////////////////DEFINICAO DE ENUMS//////////////////////////
typedef enum TIPO_SALA {
 pedra, chave, monstro, corredor, inicio
} tipo_sala;

/////////////////////////DEFINICAO DE STRUCTS/////////////////////////
typedef struct _opcao {
	char opcao_selecionada;
	int indice_proximo_no;
} opcao;

typedef struct INFO_SALA {
    int indice;
    tipo_sala tipo;
    char texto[500];
    int direita;
    int esquerda;
    int baixo;
    int cima;
    int n_op;
    opcao opcoes[MAX_OPCOES];
    struct INFO_SALA *ant;
	struct INFO_SALA *prox;

} info_sala;

/////////////////////////VARIAVEIS GLOBAIS//////////////////////////
//Referencia para o inicio e para a posicao atual na lista encadeada
info_sala *ptr_inicio, *ptr_atual;
//Referencia para o arquivo de saida
FILE *arquivo_saida;
//Criterios globais
int chave_1=0, chave_2=0, chave_3=0;
int pedra_1=0, pedra_2=0, pedra_3=0;

//LAYOUT
char layout[26][54]={
        {3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,5},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'c',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'c',0,'C',0,'c',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'c',0,'C',0,'C',0,'C',0,'c',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'C',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'C',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,'e',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'d',0,0,0,1},
        {1,0,'e','e','e',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'d','d','d',0,1},
        {1,'e','e','e','e','e','e','e','e',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'d','d','d','d','d','d','d','d',1},
        {1,0,'e','e','e',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'d','d','d',0,1},
        {1,0,0,0,'e',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'d',0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'B',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'B',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'b',0,'b',0,'B',0,'b',0,'b',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'B',0,'B',0,'B',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'B',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,6},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,'y',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'t',0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,9,0,10,0,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,12,0,13,0,14,0,0,0,0,0,0,0,0,0,0,1},
        {7,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,8}};


////////////////////////////PROTOTIPOS//////////////////////////////
char *ler_nome_jogador(char *nome_arquivo);
void cadastrar_sala(int indice, char *texto, tipo_sala tipo, int n_opcoes, opcao *opcoes, int dir, int esq, int bai, int cim);
void cadastrar_salas();
info_sala *buscar_no(int indice);
int ler_indice_proximo_no(int opcao);
int verificar_criterio_acesso(int indice);
void atualizar_criterios_globais(int indice);
void apagar_lista();
void sala(int saida, int pedra, int chave);
void fmonstro(int indice);
int dado();
void fala_monstro(int pedra, int n);
void layout_sala(int e, int d, int c, int b);
void desenho_sala(int indice);

int main() {

	//Abertura do arquivo de saida
	if (!(arquivo_saida = fopen(NOME_ARQUIVO_SAIDA, "w"))) {
		printf("ERRO AO ABRIR O ARQUIVO DE SAIDA!");
		exit(1);
	}

    //Mensagem de boas-vindas
	char nome_jogador[100];
	strcpy(nome_jogador, ler_nome_jogador(NOME_ARQUIVO_ENTRADA));
	printf("\tBem-vindo ao Senhor das Reliquias, %s!\n\n\
        Ha muito tempo atras, Galandriel era o mago mais poderoso\n\
do reino magico de Turim. Seus poderes eram tao lendarios que, aos \n\
poucos, comecaram a despertar a cobica dos homens de todo o reino que\n\
vivia. Com o passar do tempo, os coracoes dos homens se corromperam\n\
completamente com a ganancia pelo poder e, assim, guerras epicas foram\n\
tracadas em busca da conquista dos poderes e dos itens magicos, fazendo com\n\
que o caos imperasse no reino por muito tempo. Certo dia, cansado de tantas\n\
guerras e maldades, Galandriel resolveu esconder seus poderes em um lugar\n\
magico, para que o mal da ganancia nao se espalhasse pelos homens e para\n\
manter seguros seus poderes de mas intencoes.\n\n\
        Assim o mago dividiu seu poder em tres e foram criadas as reliquias\n\
da vida: o cajado dos cajados, a rocha filosofal e a manta da invisibilidade.\n\
Depois, ele os escondeu nas profundezas do reino, em um labirinto magico, no\n\
qual so aqueles realmente dignos de seus poderes seriam capazes de entrar e\n\
conseguir sair com vida. O labirinto permaneceu escondido por milenios,\n\
ficando no esquecimento, ate que a paz prevaleceu no reino.\n\n\
        Voce e um dos moradores do reino e, em um dia atipico, resolve fazer um\n\
passeio pela floresta para se aventurar. No entanto, se perde pelo caminho e\n\
se abriga em uma caverna para passar a noite. Nela, voce acha um portal\n\
misterioso e decide entrar nele por curiosidade. Ao atravessar, percebe que\n\
esta em um labirinto e o  portal se fecha atras de voce...\n\n\
        Prove que voce tem um bom coracao e que e digno dos poderes escondidos.\n\
Entre no labirinto e recupere as reliquias da vida para conseguir sair e se\n\
tornar um(a) poderoso(a) mago(a)...\n\n ", nome_jogador);

	fprintf(arquivo_saida,"Bem-vindo ao Senhor das Reliquias, %s!\n\n\
        Ha muito tempo atras, Galandriel era o mago mais poderoso\n\
do reino magico de Turim. Seus poderes eram tao lendarios que, aos \n\
poucos, comecaram a despertar a cobica dos homens de todo o reino que\n\
vivia. Com o passar do tempo, os coracoes dos homens se corromperam\n\
completamente com a ganancia pelo poder e, assim, guerras epicas foram\n\
tracadas em busca da conquista dos poderes e dos itens magicos, fazendo com\n\
que o caos imperasse no reino por muito tempo. Certo dia, cansado de tantas\n\
guerras e maldades, Galandriel resolveu esconder seus poderes em um lugar\n\
magico, para que o mal da ganancia nao se espalhasse pelos homens e para\n\
manter seguros seus poderes de mas intencoes.\n\n\
        Assim o mago dividiu seu poder em tres e foram criadas as reliquias\n\
da vida: o cajado dos cajados, a rocha filosofal e a manta da invisibilidade.\n\
Depois, ele os escondeu nas profundezas do reino, em um labirinto magico, no\n\
qual so aqueles realmente dignos de seus poderes seriam capazes de entrar e\n\
conseguir sair com vida. O labirinto permaneceu escondido por milenios,\n\
ficando no esquecimento, ate que a paz prevaleceu no reino.\n\n\
        Voce e um dos moradores do reino e, em um dia atipico, resolve fazer um\n\
passeio pela floresta para se aventurar. No entanto, se perde pelo caminho e\n\
se abriga em uma caverna para passar a noite. Nela, voce acha um portal\n\
misterioso e decide entrar nele por curiosidade. Ao atravessar, percebe que\n\
esta em um labirinto e o  portal se fecha atras de voce...\n\n\
        Prove que voce tem um bom coracao e que e digno dos poderes escondidos.\n\
Entre no labirinto e recupere as reliquias da vida para conseguir sair e se\n\
tornar um(a) poderoso(a) mago(a)...\n\n ", nome_jogador);

    char aux;
    aux=getchar();
    fprintf(arquivo_saida, "%c", aux);
    fflush(stdin);

    //Montagem dos nos do jogo em uma lista encadeada
	cadastrar_salas();

	//Carregar no raiz
	ptr_atual = buscar_no(0);

	//laco de controle do jogo
	while(1) {
		//Se no nao eh terminal, apresentar texto e ler a opcao selecionada
        if(ptr_atual->tipo == inicio && (pedra_1==0||pedra_2==0||pedra_3==0)) {
			int indice_proximo_no = -1;
			while(indice_proximo_no == -1) {
				char opcao;
                printf("ENTRADA DO LABIRINTO\n A - Ir para esquerda \n D - Ir para direita \n\
 W - Ir para cima \n S - Ir para baixo\n");
                fprintf(arquivo_saida, "ENTRADA DO LABIRINTO\n A - Ir para esquerda \n\
                         D - Ir para direita \nW - Ir para cima \n S - Ir para baixo\n");

                printf(".._|      | `-.  | `.  -_-_ _-_  _-  _- -_ -  . |   |. |\n\
   `-.._  |    |`!  |`.  -_ -__ -_ _- _-_-  .   |.;    |   _\n\
      | `-!._  |  `;!  ;. _______________ , | .- |   _!.!\n\
..__  |     |`-!._ | `.| |_______________||.\" |  _!.;    |\n\
   |``\"..__ |    |`\";.| ;|_|***********|_|| _!-|   |   _|..-\n\
   |      |``--..|_ | `;!|_|***********|_|| _!-|. ;   |_..!-\n\
   |      |    |   |`-,!_|_|***********|_|||.!-;   |    |\n\
___|______|____!..!,.!,! |_|***********|_||,!,.!.,.!..__|___\n\
       |     |    |  | | |_|***********|_||| | , |   |    |\n\
      |     |    |..!-; ;| |%%**********|_| |`-..|   |    |\n\
      |    ._!.-;  | _!,\"|_|%%**********|_||!._|  `!-!.._ |\n\
     _!.-|    | _.\"|  !; | |***********| |`.| `-._|    |``-.\n\
..-;      |  _.  |  !-| !|_|***********|_|.|`-. | ``._ |\n\
   |  _.-   |  .   |. |/ |_|***********|_|! |`!  `.|    |-._\n\
  _!\" |     !. |  .| .|  [ ]MMMMMMMMMMM[ ] \\|  `. | `._  |\n\
-    |   .    |.|  |/| /                 \\|`.  |`!    |.|\n\
     .   . .|. .   |/|  /                     \\ |`!   |`.|\n\
  _.      ! |   . | /                       \\|  `  |  `.\n");
                fprintf(arquivo_saida, " .._|      | `-.  | `.  -_-_ _-_  _-  _- -_ -  . |   |. |\n\
   `-.._  |    |`!  |`.  -_ -__ -_ _- _-_-  .   |.;    |   _\n\
      | `-!._  |  `;!  ;. _______________ , | .- |   _!.!\n\
..__  |     |`-!._ | `.| |_______________||.\" |  _!.;    |\n\
   |``\"..__ |    |`\";.| ;|_|***********|_|| _!-|   |   _|..-\n\
   |      |``--..|_ | `;!|_|***********|_|| _!-|. ;   |_..!-\n\
   |      |    |   |`-,!_|_|***********|_|||.!-;   |    |\n\
___|______|____!..!,.!,! |_|***********|_||,!,.!.,.!..__|___\n\
       |     |    |  | | |_|***********|_||| | , |   |    |\n\
      |     |    |..!-; ;| |%%**********|_| |`-..|   |    |\n\
      |    ._!.-;  | _!,\"|_|%%**********|_||!._|  `!-!.._ |\n\
     _!.-|    | _.\"|  !; | |***********| |`.| `-._|    |``-.\n\
..-;      |  _.  |  !-| !|_|***********|_|.|`-. | ``._ |\n\
   |  _.-   |  .   |. |/ |_|***********|_|! |`!  `.|    |-._\n\
  _!\" |     !. |  .| .|  [ ]MMMMMMMMMMM[ ] \\|  `. | `._  |\n\
-    |   .    |.|  |/| /                 \\|`.  |`!    |.|\n\
     .   . .|. .   |/|  /                     \\ |`!   |`.|\n\
  _.      ! |   . | /                       \\|  `  |  `.\n");
				printf("%s", ptr_atual->texto);
				fprintf(arquivo_saida, "%s", ptr_atual->texto);
				scanf(" %c", &opcao);
                fprintf(arquivo_saida, "%c\n", opcao);
				if(opcao=='d'){opcao='D';}
				
				//Ler proximo no a partir da opcao
				indice_proximo_no = ler_indice_proximo_no(opcao);
				if(indice_proximo_no == -1) {
					printf("OPCAO INVALIDA!\n");
					fprintf(arquivo_saida, "OPCAO INVALIDA!\n");
				}
			}
			//Se o acesso ao proximo no selecionado esta liberado, ler no, atualizar posicao atual e criterios globais
			if(verificar_criterio_acesso(indice_proximo_no)) {
				info_sala *ptr_proximo_no = buscar_no(indice_proximo_no);
				ptr_atual = ptr_proximo_no;
				atualizar_criterios_globais(indice_proximo_no);
			}
            layout_sala(ptr_atual->esquerda, ptr_atual->direita, ptr_atual->cima, ptr_atual->baixo);
        }
		else if(ptr_atual->tipo != inicio) {
			int indice_proximo_no = -1;
			while(indice_proximo_no == -1) {
				char opcao;
				printf("%s", ptr_atual->texto);
				fprintf(arquivo_saida, "%s", ptr_atual->texto);
				scanf(" %c", &opcao);
                fprintf(arquivo_saida, "%c\n", opcao);
				if(opcao=='d'){opcao='D';}
				if(opcao=='a'){opcao='A';}
				if(opcao=='s'){opcao='S';}
				if(opcao=='w'){opcao='W';}

				//Ler proximo no a partir da opcao
				indice_proximo_no = ler_indice_proximo_no(opcao);
				if(indice_proximo_no == -1) {
					printf("OPCAO INVALIDA!\n");
					fprintf(arquivo_saida, "OPCAO INVALIDA!\n");
				}
			}
			//Se o acesso ao proximo no selecionado esta liberado, ler no, atualizar posicao atual e criterios globais
			if(verificar_criterio_acesso(indice_proximo_no)) {
				info_sala *ptr_proximo_no = buscar_no(indice_proximo_no);
				ptr_atual = ptr_proximo_no;
				atualizar_criterios_globais(indice_proximo_no);
			}
            fmonstro(indice_proximo_no);
            desenho_sala(indice_proximo_no);
            if(indice_proximo_no!=0){
            layout_sala(ptr_atual->esquerda, ptr_atual->direita, ptr_atual->cima, ptr_atual->baixo);
            }
        }
		//Se no eh terminal, apresentar texto e finalizar programa
		else if (ptr_atual->tipo == inicio && pedra_1==1 && pedra_2==1 && pedra_3==1){
            printf("FIM DO JOGO!");
			fprintf(arquivo_saida, "FIM DO JOGO!");
			break;
		}
	}

	//Apagar toda a lista e liberar espaco de memoria alocado
	apagar_lista();

	//Fechar o arquivo de saida
	fclose(arquivo_saida);

	return 0;
}

////////////////////////OUTRAS FUNCOES////////////////////////
//Funcao que le o nome do jogador a partir de um arquivo texto
char *ler_nome_jogador(char *nome_arquivo) {
	FILE *arquivo_entrada;
	char *nome = NULL, buffer[100];
	//Abrir o arquivo texto para leitura
	arquivo_entrada = fopen(nome_arquivo, "r");
	if (arquivo_entrada == NULL) {
		printf("\nERRO AO ABRIR O ARQUIVO DE ENTRADA!");
		fprintf(arquivo_saida, "\nERRO AO ABRIR O ARQUIVO DE ENTRADA!");
		exit(1);
	}
	//Ler nome do jogador no arquivo
	if(!feof(arquivo_entrada)) {
		nome = fgets(buffer, 100, arquivo_entrada);
	}
	//Fechar o arquivo de entrada
	fclose(arquivo_entrada);
	//Retornar o nome lido
	return nome;
}

//Funcao que cadastra um novo no na lista encadeada
void cadastrar_sala(int indice, char *texto, tipo_sala tipo, int n_opcoes, opcao *opcoes, int dir, int esq, int bai, int cim) {
	//Alocar memoria para o novo no
	info_sala *ptr = (info_sala *) malloc(sizeof(info_sala));
	if(ptr == NULL) {
		printf("\nERRO NA ALOCACAO DE MEMORIA!");
		fprintf(arquivo_saida, "\nERRO NA ALOCACAO DE MEMORIA!");
		return;
	}
	//Inicializar valores do novo no
	ptr->indice = indice;
	strcpy(ptr->texto, texto);
	ptr->tipo = tipo;
	ptr->n_op = n_opcoes;
	for(int i = 0; i < n_opcoes; i++)
		ptr->opcoes[i] = opcoes[i];
	ptr->ant = NULL;
	ptr->prox = NULL;
    ptr->direita=dir;
    ptr->esquerda=esq;
    ptr->baixo=bai;
    ptr->cima=cim;
	//Encadear novo no na lista
	info_sala *ptr_aux = ptr_inicio;
	if(ptr_aux != NULL) { //Lista nao-vazia
		while(ptr_aux->prox != NULL)
			ptr_aux = ptr_aux->prox;
		ptr_aux->prox = ptr;
		ptr->ant = ptr_aux;
	}
	else //Lista vazia
		ptr_inicio = ptr;
}

//Funcao que cadastra todos os nos da lista encadeada (carregamento da lista)
void cadastrar_salas() {
	opcao opcoes_0[1] = {{'D', 1}};
	cadastrar_sala(0, "\nPegue as tres reliquias e volte para vencer\nPRESSIONE D\n", inicio, 1, opcoes_0, 220, 32, 32, 32);
	opcao opcoes_1[3] = {{'A', 0}, {'D', 2}, {'S', 7}};
	cadastrar_sala(1, "Corredor - Escolha uma dentre as direcoes indicadas.\n", corredor, 3, opcoes_1, 220, 220, 220, 32);
	opcao opcoes_2[2] = {{'A', 1}, {'D', 3}};
	cadastrar_sala(2, "Corredor - Escolha uma dentre as direcoes indicadas.\n", corredor, 2, opcoes_2, 220, 220, 32, 32);
    opcao opcoes_3[3] = {{'A', 2}, {'D', 4}, {'S', 9}};
	cadastrar_sala(3, "Corredor - Escolha uma dentre as direcoes indicadas.\n", corredor, 3, opcoes_3, 220, 220, 220, 32);
    opcao opcoes_4[2] = {{'A', 3}, {'S', 10}};
	cadastrar_sala(4, "Corredor - Escolha uma dentre as direcoes indicadas.\n", monstro, 2, opcoes_4, 32, 220, 220, 32);
    opcao opcoes_5[1] = {{'S', 11}};
	cadastrar_sala(5, "Corredor - Escolha uma dentre as direcoes indicadas.\n", chave, 1, opcoes_5, 32, 32, 220, 32);
    opcao opcoes_6[2] = {{'D', 7}, {'S', 12}};
	cadastrar_sala(6, "Corredor - Escolha uma dentre as direcoes indicadas.\n", corredor, 2, opcoes_6, 220, 32, 220, 32);
    opcao opcoes_7[2] = {{'W', 1}, {'A', 6}};
	cadastrar_sala(7, "Corredor - Escolha uma dentre as direcoes indicadas.\n", corredor, 2, opcoes_7, 32, 220, 32, 220);
    opcao opcoes_8[2] = {{'D', 9}, {'S', 14}};
	cadastrar_sala(8, "Corredor - Escolha uma dentre as direcoes indicadas.\n", corredor, 2, opcoes_8, 220, 32, 220, 32);
    opcao opcoes_9[2] = {{'A', 8}, {'W', 3}};
	cadastrar_sala(9, "Corredor - Escolha uma dentre as direcoes indicadas.\n", corredor, 2, opcoes_9, 32, 220, 32, 220);
    opcao opcoes_10[2] = {{'W', 4}, {'D', 11}};
	cadastrar_sala(10, "Corredor - Escolha uma dentre as direcoes indicadas.\n", corredor, 2, opcoes_10, 220, 32, 32, 220);
    opcao opcoes_11[3] = {{'A', 10}, {'S', 17}, {'W', 5}};
	cadastrar_sala(11, "Corredor - Escolha uma dentre as direcoes indicadas.\n", corredor, 3, opcoes_11, 32, 220, 220, 220);
    opcao opcoes_12[2] = {{'W', 6}, {'S', 18}};
	cadastrar_sala(12, "Corredor - Escolha uma dentre as direcoes indicadas.\n", monstro, 2, opcoes_12, 32, 32, 220, 220);
    opcao opcoes_13[1] = {{'D', 14}};
	cadastrar_sala(13, "Corredor - Escolha uma dentre as direcoes indicadas.\n", chave, 1, opcoes_13, 220, 32, 32, 32);
    opcao opcoes_14[3] = {{'A', 13}, {'S', 20}, {'W', 8}};
	cadastrar_sala(14, "Corredor - Escolha uma dentre as direcoes indicadas.\n", corredor, 3, opcoes_14, 32, 220, 220, 220);
    opcao opcoes_15[1] = {{'D', 16}};
	cadastrar_sala(15, "Corredor - Escolha uma dentre as direcoes indicadas.\n", pedra, 1, opcoes_15, 220, 32, 32, 32);
    opcao opcoes_16[2] = {{'A', 15}, {'S', 22}};
	cadastrar_sala(16, "Corredor - Escolha uma dentre as direcoes indicadas.\n", corredor, 2, opcoes_16, 32, 220, 220, 32);
    opcao opcoes_17[2] = {{'W', 11}, {'S', 23}};
	cadastrar_sala(17, "Corredor - Escolha uma dentre as direcoes indicadas.\n", corredor, 2, opcoes_17, 32, 32, 220, 220);
    opcao opcoes_18[3] = {{'W', 12}, {'S', 24}, {'D', 19}};
	cadastrar_sala(18, "Corredor - Escolha uma dentre as direcoes indicadas.\n", corredor, 3, opcoes_18, 220, 32, 220, 220);
    opcao opcoes_19[2] = {{'A', 18}, {'S', 25}};
	cadastrar_sala(19, "Corredor - Escolha uma dentre as direcoes indicadas.\n", corredor, 2, opcoes_19, 32, 220, 220, 32);
    opcao opcoes_20[3] = {{'W', 14}, {'S', 26}, {'D', 21}};
	cadastrar_sala(20, "Corredor - Escolha uma dentre as direcoes indicadas.\n", corredor, 3, opcoes_20, 220, 32, 220, 220);
    opcao opcoes_21[2] = {{'A', 20}, {'D', 22}};
	cadastrar_sala(21, "Corredor - Escolha uma dentre as direcoes indicadas.\n", monstro, 2, opcoes_21, 220, 220, 32, 32);
    opcao opcoes_22[3] = {{'A', 21}, {'S', 28}, {'W', 16}};
	cadastrar_sala(22, "Corredor - Escolha uma dentre as direcoes indicadas.\n", corredor, 3, opcoes_22, 32, 220, 220, 220);
    opcao opcoes_23[2] = {{'W', 17}, {'S', 29}};
	cadastrar_sala(23, "Corredor - Escolha uma dentre as direcoes indicadas.\n", corredor, 2, opcoes_23, 32, 32, 220, 220);
    opcao opcoes_24[1] = {{'W', 18}};
	cadastrar_sala(24, "Corredor - Escolha uma dentre as direcoes indicadas.\n", pedra, 1, opcoes_24, 32, 32, 32, 220);
    opcao opcoes_25[1] = {{'W', 19}};
	cadastrar_sala(25, "Corredor sem saida - Volte por onde entrou.\n", corredor, 1, opcoes_25, 32, 32, 32, 220);
    opcao opcoes_26[2] = {{'W', 20}, {'D', 27}};
	cadastrar_sala(26, "Corredor - Escolha uma dentre as direcoes indicadas.\n", corredor, 2, opcoes_26, 220, 32, 32, 220);
    opcao opcoes_27[1] = {{'A', 26}};
	cadastrar_sala(27, "Corredor sem saida - Volte por onde entrou.\n", corredor, 1, opcoes_27, 32, 220, 32, 32);
    opcao opcoes_28[1] = {{'W', 22}};
	cadastrar_sala(28, "Corredor - Escolha uma dentre as direcoes indicadas.\n", chave, 1, opcoes_28, 32, 32, 32, 220);
    opcao opcoes_29[1] = {{'W', 23}};
	cadastrar_sala(29, "Corredor - Escolha uma dentre as direcoes indicadas.\n", pedra, 1, opcoes_29, 32, 32, 32, 220);
}

//Funcao que busca um no a partir do seu indice
info_sala *buscar_no(int indice) {
	info_sala *ptr_aux = ptr_inicio;
	if(ptr_aux == NULL)
		return NULL;
	else {
		while(ptr_aux != NULL) {
			if(ptr_aux->indice == indice)
				return ptr_aux;
			ptr_aux = ptr_aux->prox;
		}
		return NULL;
	}
}

//Funcao que le o indice do proximo no da lista a partir de uma opcao selecionada no no atual
int ler_indice_proximo_no(int opcao) {
	if(ptr_atual->opcoes[0].opcao_selecionada == '*') {
		return ptr_atual->opcoes[0].indice_proximo_no;
	}
	else {
		for(int i = 0; i < ptr_atual->n_op; i++) {
			if(ptr_atual->opcoes[i].opcao_selecionada == opcao){
				return ptr_atual->opcoes[i].indice_proximo_no;
			}
		}
		return -1;
	}
}

//Funcao que verifica se o acesso para um no de determinado indice esta liberado
int verificar_criterio_acesso(int indice) {
    if(indice == 15 && chave_1 == 0) {
		printf("\nEssa sala esta trancada! Encontre a chave | para abri-la.\n");
		fprintf(arquivo_saida, "\nEssa sala esta trancada! Encontre a chave | para abri-la.\n");
		return 0;
	}
	else if(indice == 24 && chave_2 == 0) {
		printf("\nEssa sala esta trancada! Encontre a chave %c para abri-la.\n", 30);
		fprintf(arquivo_saida, "\nEssa sala esta trancada! Encontre a chave %c para abri-la.\n", 30);
		return 0;
	}
    else if(indice == 29 && chave_3 == 0) {
		printf("\nEssa sala esta trancada! Encontre a chave O para abri-la.\n");
		fprintf(arquivo_saida, "\nEssa sala esta trancada! Encontre a chave O para abri-la.\n");
		return 0;
	}
	else if(indice == 5 && chave_1 == 0){
		printf("Voce encontrou a chave | para o Cajado dos Cajados!\nPressione qualquer tecla para pega-la.\n");
		fprintf(arquivo_saida, "Voce encontrou a chave | para o Cajado dos Cajados!\nPressione qualquer tecla para pega-la.\n");
	}
	else if(indice == 13 && chave_2 == 0){
		printf("Voce encontrou a chave %c para a Manta da Invisibilidade!\nPressione qualquer tecla para pega-la.\n", 30);
		fprintf(arquivo_saida, "Voce encontrou a chave %c para a Manta da Invisibilidade!\nPressione qualquer tecla para pega-la.\n", 30);
	}
	else if(indice == 28 && chave_3 == 0){
		printf("Voce encontrou a chave O para a Rocha Filosofal!\nPressione qualquer tecla para pega-la.\n");
		fprintf(arquivo_saida, "Voce encontrou a chave O para a Rocha Filosofal!\nPressione qualquer tecla para pega-la.\n");
	}
	else if(indice == 5 && chave_1 == 1){
		printf("Voce ja possui a chave |.\nPressione qualquer tecla + <ENTER>\n");
		fprintf(arquivo_saida, "Voce ja possui a chave |.\nPressione qualquer tecla + <ENTER>\n");
	}
	else if(indice == 13 && chave_2 == 1){
		printf("Voce ja possui a chave %c.\nPressione qualquer tecla + <ENTER>\n", 30);
		fprintf(arquivo_saida, "Voce ja possui a chave %c.\nPressione qualquer tecla + <ENTER>\n", 30);
	}
	else if(indice == 28 && chave_3 == 1){
		printf("Voce ja possui a chave O.\nPressione qualquer tecla + <ENTER>\n");
		fprintf(arquivo_saida, "Voce ja possui a chave O.\nPressione qualquer tecla + <ENTER>\n");
	}
		else if(indice == 15 && pedra_1 == 0 && chave_1==1){
		printf("Parabens! Voce encontrou o Cajado dos Cajados!\nPressione qualquer tecla para pega-lo.\n");
		fprintf(arquivo_saida, "Parabens! Voce encontrou o Cajado dos Cajados!\nPressione qualquer tecla para pega-lo.\n");
	}
	else if(indice == 24 && pedra_2 == 0 && chave_2==1){
		printf("Parabens! Voce encontrou a Manta da Invisibilidade!\nPressione qualquer tecla para pega-la.\n");
		fprintf(arquivo_saida, "Parabens! Voce encontrou a Manta da Invisibilidade!\nPressione qualquer tecla para pega-la.\n");
	}
	else if(indice == 29 && pedra_3 == 0 && chave_3==1){
		printf("Parabens! Voce encontrou a Rocha Filosofal!\nPressione qualquer tecla para pega-la.\n");
		fprintf(arquivo_saida, "Parabens! Voce encontrou a Rocha Filosofal!\nPressione qualquer tecla para pega-la.\n");
	}
	else if(indice == 15 && pedra_1 == 1){
		printf("Voce ja possui essa reliquia!\nPressione qualquer tecla + <ENTER>\n");
		fprintf(arquivo_saida, "Voce ja possui essa reliquia!\nPressione qualquer tecla + <ENTER>\n");
	}
	else if(indice == 24 && pedra_2 == 1){
		printf("Voce ja possui essa reliquia!\nPressione qualquer tecla + <ENTER>\n");
		fprintf(arquivo_saida, "Voce ja possui essa reliquia!\nPressione qualquer tecla + <ENTER>\n");
	}
	else if(indice == 29 && pedra_3 == 1){
		printf("Voce ja possui essa reliquia!\nPressione qualquer tecla + <ENTER>\n");
		fprintf(arquivo_saida, "Voce ja possui essa reliquia!\nPressione qualquer tecla + <ENTER>\n");
	}
	return 1;
}

//Funcao que atualiza as variaveis globais relacionadas aos criterios de acesso aos nos, caso necessario
void atualizar_criterios_globais(int indice) {
	if(indice == 5){
		chave_1 = 1;
    }
    else if(indice == 13){
        chave_2 = 1;
    }
    else if(indice == 28){
        chave_3 = 1;
    }
    else if(indice == 15){
        pedra_1 = 1;
    }
    else if(indice == 24){
        pedra_2 = 1;
    }
    else if(indice == 29){
        pedra_3 = 1;
    }
}

//Funcao que apaga toda a lista, liberando os espacos de memorias alocados
void apagar_lista() {
	info_sala *ptr_aux = ptr_inicio;
	if(ptr_aux != NULL) {
		while(ptr_aux->prox != NULL) {
			if(ptr_aux->ant != NULL) {
				free(ptr_aux->ant);
			}
			ptr_aux = ptr_aux->prox;
		}
		if(ptr_aux->ant != NULL) {
			free(ptr_aux->ant);
		}
		free(ptr_aux);
	}
	ptr_inicio = NULL;
}

void fmonstro(int indice){
	switch (indice){
		case 12:
			fala_monstro(pedra_1, 1);
			break;
		case 21:
			fala_monstro(pedra_2, 2);
			break;
		case 4:
			fala_monstro(pedra_3, 3);
			break;
		default:
			break;
    }
}

int dado(){
    int i;
    srand(time(NULL));
    return 1+ rand() % 6;
}

void fala_monstro(int pedra, int n){
	int lv;
	char reliquia[60];
    int sorte;
    char op;
	if(n==1){
		strcpy(reliquia, "do Cajado dos Cajados");
	}
	else if (n==2){
		strcpy(reliquia, "da Manta da Invisibilidade");
	}
	else if(n==3){
		strcpy(reliquia, "da Rocha Filosofal");
	}
	if(pedra == 1){
		lv = dado();
		printf("CUIDADO! Voce acabou de encontrar um guardiao nivel %d %s!\n", lv, reliquia);
		fprintf(arquivo_saida, "CUIDADO! Voce acabou de encontrar um guardiao nivel %d %s!\n", lv, reliquia);
		switch(lv){
			case 1:
            printf("         (`.  : \\               __..----..__\n\
          `.`.| |:          _,-':::''' '  `:`-._\n\
            `.:\\||       _,':::::'         `::::`-.\n\
              \\\\\\`|    _,':::::::'     `:.     `':::`.\n\
               ;` `-''  `::::::.                  `::\\\n\
            ,-'      .::'  `:::::.         `::..    `:\\\n\
          ,' /_) -.            `::.           `:.     |\n\
        ,'.:     `    `:.        `:.     .::.          \\\n\
   __,-'   ___,..-''-.  `:.        `.   /::::.         |\n\
  |):'_,--'           `.    `::..       |::::::.      ::\\\n\
   `-'                 |`--.:_::::|_____\\::::::::.__  ::|\n\
                       |   _/|::::|      \\::::::|::/\\  :|\n\
                       /:./  |:::/        \\__:::):/  \\  :\\\n\
                     ,'::'  /:::|        ,'::::/_/    `. ``-.__\n\
                    ''''   (//|/\\      ,';':,-'         `-.__  `'--..__\n\
                                                             `''---::::'\
 \n");
            fprintf(arquivo_saida, "         (`.  : \\               __..----..__\n\
          `.`.| |:          _,-':::''' '  `:`-._\n\
            `.:\\||       _,':::::'         `::::`-.\n\
              \\\\\\`|    _,':::::::'     `:.     `':::`.\n\
               ;` `-''  `::::::.                  `::\\\n\
            ,-'      .::'  `:::::.         `::..    `:\\\n\
          ,' /_) -.            `::.           `:.     |\n\
        ,'.:     `    `:.        `:.     .::.          \\\n\
   __,-'   ___,..-''-.  `:.        `.   /::::.         |\n\
  |):'_,--'           `.    `::..       |::::::.      ::\\\n\
   `-'                 |`--.:_::::|_____\\::::::::.__  ::|\n\
                       |   _/|::::|      \\::::::|::/\\  :|\n\
                       /:./  |:::/        \\__:::):/  \\  :\\\n\
                     ,'::'  /:::|        ,'::::/_/    `. ``-.__\n\
                    ''''   (//|/\\      ,';':,-'         `-.__  `'--..__\n\
                                                             `''---::::'\
 \n");
				break;
			case 2:
            printf("                              __\n\
                            .d$$b\n\
                          .' TO$;\\\n\
                         /  : TP._;\n\
                        / _.;  :Tb|\n\
                       /   /   ;j$j\n\
                   _.-\"       d$$$$\n\
                 .' ..       d$$$$;\n\
                /  /P'      d$$$$P. |\\\n\
               /   \"      .d$$$P' |\\^\"l\n\
             .'           `T$P^\"\"\"\"\"  :\n\
         ._.'      _.'                ;\n\
      `-.-\".-'-' ._.       _.-\"    .-\"\n\
    `.-\" _____  ._              .-\"\n\
   -(.g$$$$$$$b.              .'\n\
     \"\"^^T$$$P^)            .(:\n\
       _/  -\"  /.'         /:/;\n\
    ._.'-'`-'  \")/         /;/;\n\
 `-.-\"..--\"\"   \" /         /  ;\n\
.-\" ..--\"\"        -'          :\n\
..--\"\"--.-\"         (\\      .-(\\\n\
  ..--\"\"              `-\\(\\/;`\n\
    _.                      :\n\
                            ;`-\n\
                           :\\\n\
                           ;\n");
            fprintf(arquivo_saida, "                              __\n\
                            .d$$b\n\
                          .' TO$;\\\n\
                         /  : TP._;\n\
                        / _.;  :Tb|\n\
                       /   /   ;j$j\n\
                   _.-\"       d$$$$\n\
                 .' ..       d$$$$;\n\
                /  /P'      d$$$$P. |\\\n\
               /   \"      .d$$$P' |\\^\"l\n\
             .'           `T$P^\"\"\"\"\"  :\n\
         ._.'      _.'                ;\n\
      `-.-\".-'-' ._.       _.-\"    .-\"\n\
    `.-\" _____  ._              .-\"\n\
   -(.g$$$$$$$b.              .'\n\
     \"\"^^T$$$P^)            .(:\n\
       _/  -\"  /.'         /:/;\n\
    ._.'-'`-'  \")/         /;/;\n\
 `-.-\"..--\"\"   \" /         /  ;\n\
.-\" ..--\"\"        -'          :\n\
..--\"\"--.-\"         (\\      .-(\\\n\
  ..--\"\"              `-\\(\\/;`\n\
    _.                      :\n\
                            ;`-\n\
                           :\\\n\
                           ;\n");
				break;
			case 3:
            printf("                                    ___,,___\n\
                                ,d8888888888b,_\n\
                            _,d889'        8888b,\n\
                        _,d8888'          8888888b,\n\
                    _,d8889'           888888888888b,_\n\
                _,d8889'             888888889'688888, /b\n\
            _,d8889'               88888889'     `6888d 6,_\n\
         ,d88886'              _d888889'           ,8d  b888b,  d\\\n\
       ,d889'888,             d8889'               8d   9888888Y  )\n\
     ,d889'   `88,          ,d88'                 d8    `,88aa88 9\n\
    d889'      `88,        ,88'                   `8b     )88a88'\n\
   d88'         `88       ,88                   88 `8b,_ d888888\n\
  d89            88,      88                  d888b  `88`_  8888\n\
  88             88b      88                 d888888 8: (6`) 88')\n\
  88             8888b,   88                d888aaa8888, `   'Y'\n\
  88b          ,888888888888                 `d88aa `88888b ,d8\n\
  `88b       ,88886 `88888888                 d88a  d8a88` `8/\n\
   `q8b    ,88'`888  `888'\"`88          d8b  d8888,` 88/ 9)_6\n\
     88  ,88\"   `88  88p    `88        d88888888888bd8( Z~/\n\
     88b 8p      88 68'      `88      88888888' `688889`\n\
     `88 8        `8 8,       `88    888 `8888,   `qp'\n\
       8 8,        `q 8b       `88  88\"    `888b\n\
       q8 8b        \"888        `8888'\n\
        \"888                     `q88b\n\
                                  \"888'\n");
            fprintf(arquivo_saida, "                                    ___,,___\n\
                                ,d8888888888b,_\n\
                            _,d889'        8888b,\n\
                        _,d8888'          8888888b,\n\
                    _,d8889'           888888888888b,_\n\
                _,d8889'             888888889'688888, /b\n\
            _,d8889'               88888889'     `6888d 6,_\n\
         ,d88886'              _d888889'           ,8d  b888b,  d\\\n\
       ,d889'888,             d8889'               8d   9888888Y  )\n\
     ,d889'   `88,          ,d88'                 d8    `,88aa88 9\n\
    d889'      `88,        ,88'                   `8b     )88a88'\n\
   d88'         `88       ,88                   88 `8b,_ d888888\n\
  d89            88,      88                  d888b  `88`_  8888\n\
  88             88b      88                 d888888 8: (6`) 88')\n\
  88             8888b,   88                d888aaa8888, `   'Y'\n\
  88b          ,888888888888                 `d88aa `88888b ,d8\n\
  `88b       ,88886 `88888888                 d88a  d8a88` `8/\n\
   `q8b    ,88'`888  `888'\"`88          d8b  d8888,` 88/ 9)_6\n\
     88  ,88\"   `88  88p    `88        d88888888888bd8( Z~/\n\
     88b 8p      88 68'      `88      88888888' `688889`\n\
     `88 8        `8 8,       `88    888 `8888,   `qp'\n\
       8 8,        `q 8b       `88  88\"    `888b\n\
       q8 8b        \"888        `8888'\n\
        \"888                     `q88b\n\
                                  \"888'\n");
				break;
			case 4:
				printf("\
      .     .       .  .   . .   .   . .    +  .\n  .     .  :     .    .. :. .___---------___.\n\
       .  .   .    .  :.:. _\".^ .^ ^.  ..:\"-_. .\n\
    .  :       .  .  .:../:           . .^  :.: \\.\n\
        .   . :: +. :.:/: .   .    .        . . .:\\\n\
 .  :    .     . _ :::/:               .  ^ .  . .:\\\n\
  .. . .   . - : :.:./.                        .  .:\\\n\
  .      .     . :..|:                   .  .  ^. .:|\n\
    .       . : : ..||        .                . . !:|\n\
  .     . . . ::. ::\\(                           . :)/\n\
 .   .     : . : .:.|. ######              .#######::|\n\
  :.. .  :-  : .:  ::|.#######           ..#######,:|\n\
 .  .  .  ..  .  .. :\\ ########          :######## :/\n\
  .        .+ :: : -.:\\ ########       . ########.:/\n\
    .  .+   . . . . :.:\\. #######       #######..:/\n\
      :: . . . . ::.:..:.\\           .   .   ..:/\n\
   .   .   .  .. :  -::::.\\.       | |     . .:/\n\
      .  :  .  .  .-:.\":..:.:.\\            ..:/\n\
 .      -.   . . . .: .:::.:.\\.           .:/\n\
.   .   .  :      : ....::_:..:\\   ___.  :/\n\
   .   .  .   .:. .. .  .: :.:.:,\\       :/\n\
     +   .   .   : . ::. :.:. .:.|\\  .:/|\n\
     .         +   .  .  ...:: ..|  --.:|\n\
.      . . .   .  .  . ... :..:..\"(  ..)\"\n\
 .   .       .      :  .   .: ::/  .  .::\\\n");
            fprintf(arquivo_saida, "\
      .     .       .  .   . .   .   . .    +  .\n  .     .  :     .    .. :. .___---------___.\n\
       .  .   .    .  :.:. _\".^ .^ ^.  ..:\"-_. .\n\
    .  :       .  .  .:../:           . .^  :.: \\.\n\
        .   . :: +. :.:/: .   .    .        . . .:\\\n\
 .  :    .     . _ :::/:               .  ^ .  . .:\\\n\
  .. . .   . - : :.:./.                        .  .:\\\n\
  .      .     . :..|:                   .  .  ^. .:|\n\
    .       . : : ..||        .                . . !:|\n\
  .     . . . ::. ::\\(                           . :)/\n\
 .   .     : . : .:.|. ######              .#######::|\n\
  :.. .  :-  : .:  ::|.#######           ..#######,:|\n\
 .  .  .  ..  .  .. :\\ ########          :######## :/\n\
  .        .+ :: : -.:\\ ########       . ########.:/\n\
    .  .+   . . . . :.:\\. #######       #######..:/\n\
      :: . . . . ::.:..:.\\           .   .   ..:/\n\
   .   .   .  .. :  -::::.\\.       | |     . .:/\n\
      .  :  .  .  .-:.\":..:.:.\\            ..:/\n\
 .      -.   . . . .: .:::.:.\\.           .:/\n\
.   .   .  :      : ....::_:..:\\   ___.  :/\n\
   .   .  .   .:. .. .  .: :.:.:,\\       :/\n\
     +   .   .   : . ::. :.:. .:.|\\  .:/|\n\
     .         +   .  .  ...:: ..|  --.:|\n\
.      . . .   .  .  . ... :..:..\"(  ..)\"\n\
 .   .       .      :  .   .: ::/  .  .::\\\n");
				break;
			case 5:
				printf("\
                 ___====-_  _-====___\n\
           _--^^^#####//      \\\\#####^^^--_\n\
        _-^##########// (    ) \\\\##########^-_\n\
       -############//  |\\^^/|  \\\\############-\n\
     _/############//   (@::@)   \\\\############\\_\n\
    /#############((     \\\\//     ))#############\\\n\
   -###############\\\\    (oo)    //###############-\n\
  -#################\\\\  / VV \\  //#################-\n\
 -###################\\\\/      \\//###################-\n\
_#/|##########/\\######(   /\\   )######/\\##########|\\#_\n\
|/ |#/\\#/\\#/\\/  \\#/\\##\\  |  |  /##/\\#/  \\/\\#/\\#/\\#| \\|\n\
`  |/  V  V  `   V  \\#\\| |  | |/#/  V     V  V  \\|\n\
   `   `  `      `   / | |  | | \\\n\
                    (  | |  | |  )\n\
                   __\\ | |  | | /__\n\
                  (vvv(VVV)(VVV)vvv)\n");
            fprintf(arquivo_saida, "\
                 ___====-_  _-====___\n\
           _--^^^#####//      \\\\#####^^^--_\n\
        _-^##########// (    ) \\\\##########^-_\n\
       -############//  |\\^^/|  \\\\############-\n\
     _/############//   (@::@)   \\\\############\\_\n\
    /#############((     \\\\//     ))#############\\\n\
   -###############\\\\    (oo)    //###############-\n\
  -#################\\\\  / VV \\  //#################-\n\
 -###################\\\\/      \\//###################-\n\
_#/|##########/\\######(   /\\   )######/\\##########|\\#_\n\
|/ |#/\\#/\\#/\\/  \\#/\\##\\  |  |  /##/\\#/  \\/\\#/\\#/\\#| \\|\n\
`  |/  V  V  `   V  \\#\\| |  | |/#/  V     V  V  \\|\n\
   `   `  `      `   / | |  | | \\\n\
                    (  | |  | |  )\n\
                   __\\ | |  | | /__\n\
                  (vvv(VVV)(VVV)vvv)\n");
				break;
			case 6:
				printf("\
                            ,-.\n\
        ___---.__          /`|`\\          _,_,---,___\n\
    ,-`    \\`    `-.____,-`  |   `-.,____,--    //    \\\n\
  ,`        |           ~`\\     /`~           |        \\.\n\
 /      ___//              `. ,`          ,  , \\___      \\\n\
|    ,-   `-.__   _         |        ,,    __,-   `-.    |\n\
|   /          /\\_  `   .    |          _/\\          \\   |\n\
\\  |           \\ \\`-.___ \\   |   / ___,--/ /         |  /\n\
 \\  \\           | `._   `\\\\  |  //-   _, |,         /  /\n\
  `-.\\         /`  _ `---` , . ``---- _  -\\         /,/\n\
              |__  /|\\_,--.,-.--,--._/|\\ __|\n\
             /  `./  \\\\`\\ |  |  | /,//  \\   \\\n\
            /   /     ||--+--|--+-/-|     \\   \\\n\
           |   |     /\\_\\_\\ | /_/_/`\\     |   |\n\
            \\   \\__, \\_     `~`     _/ .__/   /\n\
            ,`-._-`   `-._______,-`   `-._,-`\n");
            fprintf(arquivo_saida, "\
                            ,-.\n\
        ___---.__          /`|`\\          _,_,---,___\n\
    ,-`    \\`    `-.____,-`  |   `-.,____,--    //    \\\n\
  ,`        |           ~`\\     /`~           |        \\.\n\
 /      ___//              `. ,`          ,  , \\___      \\\n\
|    ,-   `-.__   _         |        ,,    __,-   `-.    |\n\
|   /          /\\_  `   .    |          _/\\          \\   |\n\
\\  |           \\ \\`-.___ \\   |   / ___,--/ /         |  /\n\
 \\  \\           | `._   `\\\\  |  //-   _, |,         /  /\n\
  `-.\\         /`  _ `---` , . ``---- _  -\\         /,/\n\
              |__  /|\\_,--.,-.--,--._/|\\ __|\n\
             /  `./  \\\\`\\ |  |  | /,//  \\   \\\n\
            /   /     ||--+--|--+-/-|     \\   \\\n\
           |   |     /\\_\\_\\ | /_/_/`\\     |   |\n\
            \\   \\__, \\_     `~`     _/ .__/   /\n\
            ,`-._-`   `-._______,-`   `-._,-`\n");
				break;
			default:
				break;
		}
		do{
		printf("Conte com sua sorte e tente vence-lo jogando o dado.\nTire um numero maior ou igual ao nivel do guardiao para nao perder a reliquia.\nPressione X + <ENTER> para jogar o dado\n");
		fprintf(arquivo_saida, "Conte com sua sorte e tente vence-lo jogando o dado.\nTire um numero maior ou igual ao nivel do guardiao para nao perder a reliquia.\nPressione X + <ENTER> para jogar o dado\n");
		fflush(stdin);
		op=getchar();
		fprintf(arquivo_saida, "%c\n", op);
		if(op=='x'){op='X';}
		if(op=='X'){
			sorte=dado();
			printf("Voce tirou %d!\n", sorte);
			fprintf(arquivo_saida, "Voce tirou %d!\n", sorte);
		} else {
			printf("Opcao invalida!\n");
			fprintf(arquivo_saida, "Opcao invalida!\n");
		}
		} while (op!='X');
		if (sorte>=lv){
		printf("Parabens voce derrotou o guardiao e permaneceu com a reliquia!");
		fprintf(arquivo_saida, "Parabens voce derrotou o guardiao e permaneceu com a reliquia!");
		} else {
			switch (n){
			case 1:
				pedra_1=0;
				break;
			case 2:
				pedra_2=0;
				break;
			case 3:
				pedra_3=0;
				break;
			default:
				break;
			}
			printf("Voce foi derrotado e perdeu sua reliquia!");
			fprintf(arquivo_saida, "Voce foi derrotado e perdeu sua reliquia!");
		}

	}
}

void layout_sala(int e, int d, int c, int b){
    int C;
    int B;
	C = (c != 32) ? 219 : 32;
    B = (b != 32) ? 219 : 32;
    char a, f, g, h, k, l;
	a = (chave_1==1) ? '|' : ' ';
	f = (chave_2==1) ? 30 : ' ';
    g = (chave_3==1) ? 'o' : ' ';
	h = (pedra_1==1) ? '|' : ' ';
	k = (pedra_2==1) ? 30 : ' ';
	l = (pedra_3==1) ? 'o' : ' ';
    for(char i=0; i<26; i++){
        for(char j=0; j<54; j++){
			switch (layout[i][j]){
				case 0: printf(" "); fprintf(arquivo_saida, " "); break;
				case 1: printf("%c", 186); fprintf(arquivo_saida, "%c", 186); break;
				case 2: printf("%c", 205); fprintf(arquivo_saida, "%c", 205); break;
				case 3: printf("%c", 201); fprintf(arquivo_saida, "%c", 201); break;
				case 4: printf("%c", 204); fprintf(arquivo_saida, "%c", 204); break;
				case 5: printf("%c", 187); fprintf(arquivo_saida, "%c", 187); break;
				case 6: printf("%c", 185); fprintf(arquivo_saida, "%c", 185); break;
				case 7: printf("%c", 200); fprintf(arquivo_saida, "%c", 200); break;
				case 8: printf("%c", 188); fprintf(arquivo_saida, "%c", 188); break;
				case 'e': printf("%c", e); fprintf(arquivo_saida, "%c", e); break;
				case 'd': printf("%c", d); fprintf(arquivo_saida, "%c", d); break;
				case 'c': printf("%c", c); fprintf(arquivo_saida, "%c", c); break;
				case 'C': printf("%c", C); fprintf(arquivo_saida, "%c", C); break;
				case 'b': printf("%c", b); fprintf(arquivo_saida, "%c", b); break;
				case 'B': printf("%c", B); fprintf(arquivo_saida, "%c", B); break;
				case 'y': printf("Chave"); fprintf(arquivo_saida, "Chave"); break;
				case 't': printf("Reliquia"); fprintf(arquivo_saida, "Reliquia"); break;
				case 9: printf("%c", a); fprintf(arquivo_saida, "%c", a); break;
				case 10: printf("%c", f); fprintf(arquivo_saida, "%c", f); break;
				case 11: printf("%c", g); fprintf(arquivo_saida, "%c", g); break;
				case 12: printf("%c", h); fprintf(arquivo_saida, "%c", h); break;
				case 13: printf("%c", k); fprintf(arquivo_saida, "%c", k); break;
				case 14: printf("%c", l); fprintf(arquivo_saida, "%c", l); break;
			}
        } printf ("\n");
          fprintf(arquivo_saida, "\n");
	}
}

void desenho_sala(int indice){
    char aux;
	printf("\n");
	fprintf(arquivo_saida, "\n");
    if(indice==5 || indice==13 || indice==28){
        printf("               .---.\n\
              /    |\\________________\n\
             | ()  | ________   _   _)\n\
              \\    |/        | | | |\n\
               `---`         \"-\" |_|\n");
        fprintf(arquivo_saida, "               .---.\n\
              /    |\\________________\n\
             | ()  | ________   _   _)\n\
              \\    |/        | | | |\n\
               `---`         \"-\" |_|\n");
        fflush(stdin);
        aux=getchar();
        fprintf(arquivo_saida, "%c", aux);

    }
    else if((chave_1==1 && indice==15) || (chave_2==1 && indice==24) || (chave_3==1 && indice==29)){
        printf(" /\\                                 \\\n\
|  \\                                 \\\n\
|   \\_______________;;________________\\\n\
|   /                                 /\n\
|  /                                 /\n\
| /                                 /\n\
|/________________________________ /\n\
|\\     *  *  *  _________ *  *  *  \\\n\
| \\  *  *  * _ /_|_____|_\\ _ * * *  \\\n\
|  \\           \". \\   / .\"           \\\n\
|   \\ *  *  *    \".\\ /.\"   *  *  *    \\\n\
|    \\ *  *  *     \".\"       *  *  *   \\\n\
|     |---------------------------------|\n\
|     |                                 |\n\
|     |                                 |\n\
 \\    |                 ))              |\n\
  \\   |                 ))              |\n\
   \\  |                                 |\n\
    \\ |                                 |\n\
     \\|_________________________________|\n");
     fprintf(arquivo_saida, " /\\                                 \\\n\
|  \\                                 \\\n\
|   \\_______________;;________________\\\n\
|   /                                 /\n\
|  /                                 /\n\
| /                                 /\n\
|/________________________________ /\n\
|\\     *  *  *  _________ *  *  *  \\\n\
| \\  *  *  * _ /_|_____|_\\ _ * * *  \\\n\
|  \\           \". \\   / .\"           \\\n\
|   \\ *  *  *    \".\\ /.\"   *  *  *    \\\n\
|    \\ *  *  *     \".\"       *  *  *   \\\n\
|     |---------------------------------|\n\
|     |                                 |\n\
|     |                                 |\n\
 \\    |                 ))              |\n\
  \\   |                 ))              |\n\
   \\  |                                 |\n\
    \\ |                                 |\n\
     \\|_________________________________|\n");
        fflush(stdin);
        aux=getchar();
        fprintf(arquivo_saida, "%c", aux);
    }
	else if(indice==0 && pedra_1==1 && pedra_2==1 && pedra_3==1){
		printf("PARABENS!!! Voce conseguiu conquistar todas as reliquias e se tornou o mago mais poderoso do reino, use seus poderes com sabedoria!\n");
		fprintf(arquivo_saida, "PARABENS!!! Voce conseguiu conquistar todas as reliquias e se tornou o mago mais poderoso do reino, use seus poderes com sabedoria!\n");
		printf("                                           ,---.\n\
                                          /    |\n\
                                         /   | |\n\
                                        /      |\n\
                                       /     | |\n\
                                  ___,'        |\n\
      _-_-_                     <  -'          :\n\
     +' * `+                     `-.__..--'``-,_\\_\n\
      *;*,**                        |o/ ## :,.)_`>\n\
       `\\ `\\'                       :/ `     ||/)\n\
        `\\ `\\'       '* +'         (_.).__,-` |\\\n\
         `\\ `\\'     '.....'         /( `.``   `| :\n\
          `\\ `\\'    /+*+\"+\\         \\'`-.)  `  ; ;\n\
           `\\ `\\'  |\"+*'*+*|        | `       /-<\n\
            `\\ `\\'  \\+*+\"*/         |     `  /   `.\n\
             `\\ `\\ (\\._..____     /|  `    :__..-' \\\n\
               \\''\\ \\._.__\\ \\``-./ :`      ;        \\\n\
               `\\ `\\'    ` \\ \\:  (   `   /  ,   .    \\\n\
                `\\` \\'      \\ \\ |  | `     :  :    .\\ \\\n\
                 `\\ `\\'      )) :  ;      |  |     ):  :\n\
                  (`-.-'\\    || |\\ \\   `  ;  ;      | ||\n\
                   \\-_   `;;._   ( \\    /  /_       | ||\n\
                    `-.-.// ,'`-..__\\__/_,'         ; ||\n\
                     `\\'`\\:: :     /     `     ,   /   |\n\
                      `\\``|| |    (        ,' /   / / /|\n\
                       `\\`|| |              ,'   / / / |\n\
                        `\\|| |                  / / / /|\n\
                          || |                 / / / / ;\n");
            fprintf(arquivo_saida, "                                           ,---.\n\
                                          /    |\n\
                                         /   | |\n\
                                        /      |\n\
                                       /     | |\n\
                                  ___,'        |\n\
      _-_-_                     <  -'          :\n\
     +' * `+                     `-.__..--'``-,_\\_\n\
      *;*,**                        |o/ ## :,.)_`>\n\
       `\\ `\\'                       :/ `     ||/)\n\
        `\\ `\\'       '* +'         (_.).__,-` |\\\n\
         `\\ `\\'     '.....'         /( `.``   `| :\n\
          `\\ `\\'    /+*+\"+\\         \\'`-.)  `  ; ;\n\
           `\\ `\\'  |\"+*'*+*|        | `       /-<\n\
            `\\ `\\'  \\+*+\"*/         |     `  /   `.\n\
             `\\ `\\ (\\._..____     /|  `    :__..-' \\\n\
               \\''\\ \\._.__\\ \\``-./ :`      ;        \\\n\
               `\\ `\\'    ` \\ \\:  (   `   /  ,   .    \\\n\
                `\\` \\'      \\ \\ |  | `     :  :    .\\ \\\n\
                 `\\ `\\'      )) :  ;      |  |     ):  :\n\
                  (`-.-'\\    || |\\ \\   `  ;  ;      | ||\n\
                   \\-_   `;;._   ( \\    /  /_       | ||\n\
                    `-.-.// ,'`-..__\\__/_,'         ; ||\n\
                     `\\'`\\:: :     /     `     ,   /   |\n\
                      `\\``|| |    (        ,' /   / / /|\n\
                       `\\`|| |              ,'   / / / |\n\
                        `\\|| |                  / / / /|\n\
                          || |                 / / / / ;\n");
	}
}