#include <opencv2/opencv.hpp>
using namespace cv;

/*
void TrackbarCallbackFunction(
    int pos,          // Valor actual del trackbar (posición del deslizador).
                      // Rango: [0, valor_máximo] definido al crear el trackbar.

    void* userdata    // Puntero genérico a datos del usuario.
                      // Puede usarse para pasar estructuras, objetos o variables
                      // adicionales al callback. Si no se necesita, puede ser nullptr.
);
*/
void nop(int, void*) {
    // función vacía para los callbacks de las trackbars
}

int main() {
    namedWindow("frame", WINDOW_AUTOSIZE);

    // Crear trackbars
    /*
    int cv::createTrackbar(
        const String& trackbarname,     // Nombre del control deslizante
        const String& winname,          // Nombre de la ventana a la que pertenece
        int* value,                     // Puntero a la variable asociada (puede ser nullptr)
        int count,                      // Valor máximo del control deslizante
        TrackbarCallback onChange = 0,  // Función callback llamada al cambiar el valor
        void* userdata = 0              // (opcional) datos de usuario pasados al callback
    );
    */
    createTrackbar("h", "frame", nullptr, 179, nop);
    createTrackbar("s", "frame", nullptr, 255, nop);
    createTrackbar("v", "frame", nullptr, 255, nop);

    while (true) {
        /*
        int cv::getTrackbarPos(
            const String& trackbarname,  // Nombre del control deslizante
            const String& winname        // Nombre de la ventana asociada
        );
        */
        int h = getTrackbarPos("h", "frame");
        int s = getTrackbarPos("s", "frame");
        int v = getTrackbarPos("v", "frame");

        // Crear imagen HSV con valores constantes
        Mat img_hsv(480, 640, CV_8UC3, Scalar(h, s, v));

        // Convertir a BGR para mostrar
        Mat img_bgr;
        cvtColor(img_hsv, img_bgr, COLOR_HSV2BGR);

        imshow("frame", img_bgr);

        // Salir con tecla ESC
        if (waitKey(1) == 27)
            break;
    }

    destroyAllWindows();
    return 0;
}
