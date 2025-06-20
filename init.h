#ifndef INIT
#define INIT

#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS (N_HABILIDADES * 5)
#define N_BASES (N_HEROIS / 5)
#define N_MISSOES (T_FIM_DO_MUNDO/100)
#define N_COMPOSTOS_V (N_HABILIDADES * 3)

//Numero de cada evento
#define E_CHEGA 1
#define E_ESPERA 2 
#define E_DESISTE 3
#define E_AVISA 4
#define E_ENTRA 5
#define E_SAI 6 
#define E_VIAJA 7
#define E_MORRE 8 
#define E_MISSAO 9
#define E_FIM 0

#endif