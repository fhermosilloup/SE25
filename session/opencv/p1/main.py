import cv2

# Cargar imagen
img = cv2.imread("test.jpg")

# Verificar si se cargó correctamente
if img is None:
    print("Image not found!")
else:
    # Mostrar imagen
    cv2.imshow("Display", img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
