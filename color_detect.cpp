/*
*	Objetivo: Desenvolver um programa que detecte um objeto de cor vermelha
*	e determinar sua área e sua posição no plano da imagem.
*
*	Arquivo: color_detect.cpp
*	
*	Autor: Eduardo Gomes
*
*/

//______________________ Bibliotecas ____________________________________________________
//_______________________________________________________________________________________

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>


//______________________ Namespaces _____________________________________________________
//_______________________________________________________________________________________

using namespace cv;
using namespace std;


//______________________ Função Principal _______________________________________________
//_______________________________________________________________________________________

int main(){

	Mat frame;					// Amarzena a captura do frame da WebCam
	Mat frameCopy;					// Armazena a cópia a matriz que amarzena a captura do frame da WebCam
	Mat mask;					// Armazena a matriz da mascara que define o que é vermelho na imagem

	Moments momento;				// Armazena os coordenadas do momento do contorno que delimita o vermelho da imagem

	double area;					// Armazena o valor da área do cortono
	double x, y;					// Armazena as coordendas do centro do contorno

	Point pointTextX   (30, 30);			// Armazena o ponto de partida para impressão de cada texto na imagem
	Point pointTextY   (30, 60);
	Point pointTextArea(30, 90);

	Scalar redLow  = Scalar(166, 90, 180);		// Define o limite inferior do vermelho
	Scalar redHigh = Scalar(186, 255, 255);		// Define o limite superior do vermelho


	VideoCapture webCam(0);				// Inicialização do construtor para captura da fonte de imagem



	if(!webCam.isOpened()) return -1;		// Valida a disponibilidade da fonte de vídeo
	else{
		while(1){																// loop infinito
			
			//---------- Detecção do objeto e obtenção de dados geométricos -------------

			webCam >> frame;							// O frame obtido, a cada loop, é armazenado em "frame"
			frame.copyTo(frameCopy);						// O frame é copiado, para que o frame original seja manipulado
			cvtColor(frame, frame, COLOR_BGR2HSV);					// Converte o frame original do espaço RGB para HSV
			inRange(frame, redLow, redHigh, mask);					// "Binarisa" a imagem:
												// Tudo que se enquadra no limite de "vermelho" torna-se branco	
												// Tudo que não se enquadra no limite de "vemelho" torna-se preto

			vector < vector < Point >> contours;
			findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);		// A partir da máscara binária determina-se a fonterira entre 0 e 1
																				// Essa fronteira chama-se contorno ("contours")
			
			for(int i = 0; i < contours.size(); i++){;				// Determina-se o memento e a área do contorno (objeto vermelho identificado)
				momento = moments(contours[i]);
				area = contourArea(contours[i]);
			}//end for
			
			x = momento.m10/momento.m00;						// Cálculo das coordenadas do centro do contorno
			y = momento.m01/momento.m00;
			
			Point center(x,y);							// Armazena o ponto central do contorno


			//---------- Manipulação da imgem -------------------------------------------

			drawContours(frameCopy, contours, -1, Scalar(0, 255, 0), 1, LINE_8);	// Desenha o controno obtido

			circle(frameCopy, center, 5, Scalar(0, 255, 0), FILLED, LINE_8);	// Desenha o ponto no centro do contorno obtido		
			
			string locX = "X = " + to_string(x);
			string locY = "Y = " + to_string(y);
			string sizeArea = "Area = " + to_string(area);

			putText(frameCopy, locX, pointTextX, FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 0), 2, LINE_8);
			putText(frameCopy, locY, pointTextY, FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 0), 2, LINE_8);
			putText(frameCopy, sizeArea, pointTextArea, FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 0), 2, LINE_8);
			
			imshow("contours", frameCopy);

			if(waitKey(30) >= 0) break;
		
		} //end while

	} //end else

	return 0;

} //end main
