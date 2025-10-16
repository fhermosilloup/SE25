import cv2
import mediapipe as mp
import math

# --- Inicialización ---
mp_hands = mp.solutions.hands
mp_drawing = mp.solutions.drawing_utils

hands = mp_hands.Hands(
    static_image_mode=False,
    max_num_hands=1,                # Una mano para simplificar
    min_detection_confidence=0.7,
    min_tracking_confidence=0.7
)

# Función para calcular distancia entre dos puntos
def distance(p1, p2):
    return math.sqrt((p1.x - p2.x)**2 + (p1.y - p2.y)**2)

# Función para detectar gestos básicos
def detect_gesture(landmarks):
    tips = [4, 8, 12, 16, 20]  # Pulgar, índice, medio, anular, meñique
    fingers = []

    # Dedos extendidos si la punta está por encima de la base (y menor porque el origen es arriba)
    for i in range(1, 5):  # índice a meñique
        if landmarks[tips[i]].y < landmarks[tips[i]-2].y:
            fingers.append(1)
        else:
            fingers.append(0)

    # Pulgar (horizontal)
    if landmarks[4].x > landmarks[3].x:
        thumb = 1
    else:
        thumb = 0

    # Clasificación simple
    if fingers == [0,0,0,0] and thumb==1:
        return "Pulgar arriba 👍"
    elif fingers == [0,0,0,0] and thumb==0:
        return "Pulgar abajo 👎"
    elif fingers == [1,1,0,0]:
        return "Victoria ✌️"
    elif fingers == [0,0,0,0]:
        return "Puño cerrado ✊"
    else:
        return "Desconocido"

# --- Captura de cámara ---
cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    image_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    image_rgb.flags.writeable = False
    results = hands.process(image_rgb)
    image_rgb.flags.writeable = True
    image_bgr = cv2.cvtColor(image_rgb, cv2.COLOR_RGB2BGR)

    if results.multi_hand_landmarks:
        for hand_landmarks in results.multi_hand_landmarks:
            # Dibujar puntos y conexiones
            mp_drawing.draw_landmarks(image_bgr, hand_landmarks, mp_hands.HAND_CONNECTIONS)

            # Detectar gesto
            gesture = detect_gesture(hand_landmarks.landmark)
            cv2.putText(image_bgr, gesture, (10,50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0,255,0), 2)

    cv2.imshow("Hand Gesture Recognition", image_bgr)
    if cv2.waitKey(1) & 0xFF == 27:  # ESC
        break

cap.release()
cv2.destroyAllWindows()
