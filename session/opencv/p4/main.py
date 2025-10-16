import cv2
import numpy as np

# --- 1. Cargar imagen ---
img = cv2.imread("hand_dataset-5.png")
if img is None:
    print("No se pudo cargar la imagen")
    exit(-1)

# --- 2. Convertir a escala de grises ---
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# --- 3. Umbralización usando Otsu ---
_, mask = cv2.threshold(gray, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
mask = cv2.bitwise_not(mask)  # Invertir máscara

# --- 4. Detección de contornos ---
contours, hierarchy = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

# --- 5. Dibujar contornos sobre la imagen original ---
img_contours = img.copy()
cv2.drawContours(img_contours, contours, -1, (0, 255, 0), 2)

img_defects = img.copy()
for cnt in contours:
    # Eliminar contornos con área < 500
    if cv2.contourArea(cnt) < 500:
        continue

    # Calcular convex hull
    hull = cv2.convexHull(cnt, returnPoints=True)
    cv2.polylines(img_defects, [hull], True, (255, 0, 0), 2)

    # Calcular defectos de convexidad
    hull_indices = cv2.convexHull(cnt, returnPoints=False)
    if len(hull_indices) > 3:
        defects = cv2.convexityDefects(cnt, hull_indices)
        if defects is not None:
            for i in range(defects.shape[0]):
                s, e, f, d = defects[i, 0]
                depth = d / 256.0
                if depth < 10:
                    continue

                start = tuple(cnt[s][0])
                end = tuple(cnt[e][0])
                far = tuple(cnt[f][0])

                # Dibujar triángulo del defecto
                cv2.line(img_defects, start, far, (0, 255, 0), 1)
                cv2.line(img_defects, far, end, (0, 255, 0), 1)
                cv2.line(img_defects, start, end, (0, 255, 255), 1)

                # Mostrar profundidad
                text_pos = (far[0], far[1] + 15)
                cv2.putText(img_defects, f"{depth:.1f}", text_pos,
                            cv2.FONT_HERSHEY_SIMPLEX, 0.4, (255, 255, 255), 1, cv2.LINE_AA)

# --- 6. Mostrar resultados ---
cv2.imshow("Original", img)
cv2.imshow("Umbral", mask)
cv2.imshow("Contornos", img_contours)
cv2.imshow("Defectos", img_defects)

cv2.waitKey(0)
cv2.destroyAllWindows()
