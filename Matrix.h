/*Header del file Matrix.cpp.
  Questa coppia di file serve per l'allocazione e deallocazione dinamica di un vettore di float.
    Qualora dovessero servire funzioni analoghe per gli int o altri tipi, l'implementazione
    sarebbe banale.
    Sembrerebbe logico fare un'unica implementazione sfruttanto i "Templates"
    del C++, ma al momento (Dic '97) non sono riuscito ad ottenere questo risultato.
*/
#include <complex>

#define _abs(a)  (((a) > (0)) ? (a) : (-a))

float **CreateFMatrix(int numOfRows, int numOfCols);
int DeleteFMatrix(float **matrix);
float **ReallocFMatrix(float **m, int nr, int nc, int old_nr);
int **CreateIMatrix(int numRows, int numCols);
void DeleteIMatrix(int **Matrix);

char **CreateCMatrix(int NumRows, int NumCols);
void DeleteCMatrix(char **Matrix);
