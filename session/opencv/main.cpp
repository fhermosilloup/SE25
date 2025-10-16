#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Abrir stream de camara
    //cv::VideoCapture cap("http://192.168.1.3:4747/video?640x480"); // DroidCam (reemplaza por tu direccion IP)
    cv::VideoCapture cap(0); // Camara física
    if (!cap.isOpened()) {
        std::cerr << "No se pudo conectar al stream HTTP" << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (true) {
        bool ret = cap.read(frame);
        if (!ret || frame.empty()) {
            std::cerr << "No se pudo leer frame del stream" << std::endl;
            break;
        }
	      std::cout << frame.rows << "x" << frame.cols << std::endl;
        cv::imshow("DroidCam", frame);

        // Delay de 1ms y Salir si se presiona la tecla ESC (ESC=27 en ascii)
        if (cv::waitKey(1) == 27) {
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
