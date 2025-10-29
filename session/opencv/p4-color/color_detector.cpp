#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main() {
    // Cargar imagen
    Mat img = imread("shapes_and_colors.png");
    if (img.empty()) {
        cerr << "Error: no se pudo cargar la imagen." << endl;
        return -1;
    }

    // Convertir de BGR a HSV
    Mat img_hsv;
    cvtColor(img, img_hsv, COLOR_BGR2HSV);

    // Definir rango de tono (hue)
    Scalar lower(7, 100, 100);
    Scalar upper(20, 255, 255);

    cout << "Lower: " << lower << endl;
    cout << "Upper: " << upper << endl;

    // Crear máscara en base al rango de color
    Mat mask;
    inRange(img_hsv, lower, upper, mask);
    /*
        void cv::inRange(
            InputArray src,   // [argin] Imagen de entrada (Mat) en cualquier tipo soportado
            InputArray lowerb,// [argin] Límite inferior del rango (Scalar o Mat)
            InputArray upperb,// [argin] Límite superior del rango (Scalar o Mat)
            OutputArray dst   // [argout] Imagen binaria de salida (Mat) con valores 0 o 255
        );
        ----------------
        - Compara cada píxel de la imagen 'img_hsv' con los límites 'lower' y 'upper'.
        - Si el píxel está dentro del rango HSV especificado, el valor en 'mask' será 255.
        - Si está fuera del rango, el valor será 0.
        Resultado: imagen binaria (blanco = dentro del rango, negro = fuera).
    */

    // Aplicar la máscara sobre la imagen original
    Mat result;
    bitwise_and(img, img, result, mask);
    /*
        void cv::bitwise_and(
            InputArray src1,      // [argin] Primera imagen de entrada
            InputArray src2,      // [argin] Segunda imagen de entrada
            OutputArray dst,      // [argout] Imagen de salida donde se aplicó el AND bit a bit
            InputArray mask = noArray() // [argin] Máscara opcional; sólo se procesan los píxeles distintos de 0
        );
        --------------------
        - Realiza una operación AND bit a bit entre las dos imágenes de entrada (aquí, 'img' y 'img').
        - Sólo los píxeles donde la máscara 'mask' sea distinta de cero (255) se conservan.
        - Los píxeles donde la máscara vale 0 se ponen a negro.
        Resultado: imagen filtrada que muestra sólo los colores dentro del rango definido.
    */

    // Mostrar resultados
    imshow("original", img);
    imshow("mask", mask);
    imshow("filtered", result);
    waitKey(0);
    destroyAllWindows();

    return 0;
}
