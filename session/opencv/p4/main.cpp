#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Cargar imagen
    cv::Mat img = cv::imread("hand_dataset-5.png");
    if (img.empty()) {
        std::cerr << "No se pudo cargar la imagen" << std::endl;
        return -1;
    }

    // Convertir a escala de grises
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    // Umbralización usando Otsu
    cv::Mat mask;
    cv::threshold(gray, mask, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    cv::bitwise_not(mask, mask);  // Invertir máscara

    // Detección de contornos
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Dibujar contornos sobre la imagen original
    cv::Mat img_contours = img.clone();
    cv::drawContours(img_contours, contours, -1, cv::Scalar(0, 255, 0), 2);

    cv::Mat img_defects = img.clone();
    for (size_t i = 0; i < contours.size(); ++i) {
        // Eliminar contornos con área < 500
        if (cv::contourArea(contours[i]) < 500) continue;

        // Calcular el convex hull
        std::vector<cv::Point> hull;
        cv::convexHull(contours[i], hull, true);

        // Dibujar convex hull en azul
        cv::polylines(img_defects, hull, true, cv::Scalar(255, 0, 0), 2);

        // Calcular defectos de convexidad
        std::vector<int> hull_indices;
        cv::convexHull(contours[i], hull_indices, false);
        if (hull_indices.size() > 3) {
            cv::Mat defects;
            cv::convexityDefects(contours[i], hull_indices, defects);

            // Dibujar defectos con profundidad >= 10
            if (!defects.empty()) {
                for (int j = 0; j < defects.rows; ++j) {
                    int start_idx = defects.at<int>(j, 0);
                    int end_idx = defects.at<int>(j, 1);
                    int far_idx = defects.at<int>(j, 2);
                    float depth = defects.at<int>(j, 3) / 256.0f;

                    if (depth < 10) continue;

                    cv::Point start = contours[i][start_idx];
                    cv::Point end = contours[i][end_idx];
                    cv::Point far = contours[i][far_idx];

                    // Dibujar triángulo del defecto
                    cv::line(img_defects, start, far, cv::Scalar(0, 255, 0), 1);
                    cv::line(img_defects, far, end, cv::Scalar(0, 255, 0), 1);
                    cv::line(img_defects, start, end, cv::Scalar(0, 255, 255), 1);

                    // Mostrar profundidad
                    cv::putText(img_defects, cv::format("%.1f", depth),
                                cv::Point(far.x, far.y + 15),
                                cv::FONT_HERSHEY_SIMPLEX, 0.4,
                                cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
                }
            }
        }
    }

    // === 6. Mostrar resultados ===
    cv::imshow("Original", img);
    cv::imshow("Umbral", mask);
    cv::imshow("Contornos", img_contours);
    cv::imshow("Defectos", img_defects);

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
