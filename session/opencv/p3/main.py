import cv2

# --- Abrir stream de cámara ---
# cap = cv2.VideoCapture("http://192.168.1.3:4747/video?640x480")  # DroidCam (reemplaza con tu IP)
cap = cv2.VideoCapture(0)  # Cámara física

if not cap.isOpened():
    print("No se pudo conectar al stream HTTP")
    exit(-1)

while True:
    ret, frame = cap.read()
    if not ret or frame is None:
        print("No se pudo leer frame del stream")
        break

    print(f"{frame.shape[0]}x{frame.shape[1]}")
    cv2.imshow("DroidCam", frame)

    # Delay de 1 ms y salir si se presiona ESC (27 en ASCII)
    if cv2.waitKey(1) & 0xFF == 27:
        break

cap.release()
cv2.destroyAllWindows()
