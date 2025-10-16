import cv2
from ultralytics import YOLO

# Cargar modelo
model = YOLO("runs/detect/train/weights/best.pt")  # reemplaza con tu ruta

# Fuente de imagen o cámara
cap = cv2.VideoCapture(0)  # 0 para cámara física, o ruta a video

while True:
    ret, frame = cap.read()
    if not ret:
        break

    # Inferencia
    results = model(frame, verbose=False)
    for det in results[0].boxes:
        if det.conf > 0.5:
            x1, y1, x2, y2 = map(int, det.xyxy[0])
            cls_id = int(det.cls[0])
            label = f"{model.names[cls_id]} {det.conf[0]:.2f}"
            cv2.rectangle(frame, (x1, y1), (x2, y2), (0,255,0), 2)
            cv2.putText(frame, label, (x1, y1-10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0,255,0), 2)

    cv2.imshow("YOLO Detection", frame)
    if cv2.waitKey(1) & 0xFF == 27:  # ESC para salir
        break

cap.release()
cv2.destroyAllWindows()
