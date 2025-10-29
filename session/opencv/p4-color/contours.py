import cv2
import numpy as np

# Cargar imagen y convertir a escala de grises
img = cv2.imread("hand_dataset-5.png")
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# Calcular el valor medio de todos los píxeles
mean_val = cv2.mean(gray)[0]

# Crear una máscara binaria: 255 donde el píxel >= media, 0 en otro caso
#_, mask = cv2.threshold(img, mean_val, 255, cv2.THRESH_BINARY)
_, mask = cv2.threshold(gray, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
mask = cv2.bitwise_not(mask)

# === 4. Detectar contornos ===
contours, hierarchy = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

# === 5. Dibujar contornos sobre la imagen original ===
img_contours = img.copy()
cv2.drawContours(img_contours, contours, -1, (0, 255, 0), 2)  # verde, grosor 2 px
print("No. Objetos: ", len(contours))

img_defects = img.copy()
for cnt in contours:
    # Ignorar contornos muy pequeños (ruido)
    if cv2.contourArea(cnt) < 500:
        continue

    # === Calcular el convex hull ===
    hull = cv2.convexHull(cnt, returnPoints=True)

    # Dibujar el hull en azul
    cv2.polylines(img_defects, [hull], True, (255, 0, 0), 2)

    # === Calcular defectos de convexidad ===
    hull_indices = cv2.convexHull(cnt, returnPoints=False)
    if len(hull_indices) > 3:
        defects = cv2.convexityDefects(cnt, hull_indices)
        if defects is not None:
            for i in range(defects.shape[0]):
                s, e, f, d = defects[i, 0]
                start = tuple(cnt[s][0])
                end = tuple(cnt[e][0])
                far = tuple(cnt[f][0])
                depth = d / 256.0
                
                if depth < 10:
                    continue
                
                # === Dibujar triángulo del defecto ===
                cv2.line(img_defects, start, far, (0, 255, 0), 1)
                cv2.line(img_defects, far, end, (0, 255, 0), 1)
                cv2.line(img_defects, start, end, (0, 255, 255), 1)
                
                # === Mostrar la profundidad (d/256) debajo del punto far ===
                text_pos = (far[0], far[1] + 15)
                cv2.putText(img_defects, f"{depth:.1f}", text_pos, cv2.FONT_HERSHEY_SIMPLEX, 0.4, (255, 255, 255), 1, cv2.LINE_AA)

# === 6. Mostrar resultados ===
cv2.imshow("Original", img)
cv2.imshow("Umbral", mask)
cv2.imshow("Contornos", img_contours)
cv2.imshow("Defectos", img_defects)
cv2.waitKey(0)
cv2.destroyAllWindows()
