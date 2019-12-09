#include <Matrix.h>

int **CreateIMatrix(int NumRows, int NumCols){
    int i;
	int  **Matrix;
	//Allocaz. vettore puntatori alle righe:
	Matrix=new int *[NumRows];
    if(Matrix==nullptr)
        return nullptr;
  //Allocaz. matrice:
	Matrix[0]=new int[NumRows*NumCols];
	for(i=1; i<NumRows; i++)
		Matrix[i]=Matrix[0]+i*NumCols;
	return Matrix;
}

void DeleteIMatrix(int  **Matrix){
    if(Matrix==nullptr)
        return;
	delete[] Matrix[0];
	delete[] Matrix;
}

float **CreateFMatrix(int NumRows, int NumCols){
	long i;
	float **Matrix;
	//Allocaz. vettore puntatori alle righe:
	Matrix=new float*[NumRows];
    if(Matrix==nullptr)
        return nullptr;
    if(Matrix==nullptr)
        return nullptr;
  //Allocaz. matrice:
	Matrix[0]=new float[NumRows*NumCols];
    if(Matrix[0]==nullptr)
        return nullptr;
	for(i=1; i<NumRows; i++)
		Matrix[i]=Matrix[0]+i*NumCols;
	return Matrix;
}

int DeleteFMatrix(float **Matrix){
    if(Matrix==nullptr)
        return 1;
	delete[] Matrix[0];
	delete[] Matrix;
	return 0;
}

char **CreateCMatrix(int NumRows, int NumCols){
    long i;
    char **Matrix;
    //Allocaz. vettore puntatori alle righe:
    Matrix=new char*[NumRows];
    if(Matrix==nullptr)
        return nullptr;
  //Allocaz. matrice:
    Matrix[0]=new char[NumRows*NumCols];
    for(i=1; i<NumRows; i++)
        Matrix[i]=Matrix[0]+i*NumCols;
    return Matrix;
}

void DeleteCMatrix(char **Matrix){
    if(Matrix==nullptr)
        return;
    delete[] Matrix[0];
    delete[] Matrix;
}

