import tkinter as tk
from tkinter import ttk
import math
import random
import socket
import threading

# -----------------------------
# Seven-segment digit (single)
# -----------------------------
class SevenSegmentDisplay(tk.Canvas):
    """Canvas-based 7-segment digit."""
    def __init__(self, parent, scale=1.0, **kwargs):
        w, h = int(40 * scale), int(80 * scale)
        super().__init__(parent, width=w, height=h, bg="black", highlightthickness=0, **kwargs)
        s = scale
        self.color_on = "#FF2D2D"
        self.color_off = "#300000"

        self.segments = {}
        self.segments['A'] = self.create_polygon([8*s,4*s, 32*s,4*s, 27*s,9*s, 13*s,9*s], fill=self.color_off, outline="")
        self.segments['B'] = self.create_polygon([32*s,4*s, 36*s,8*s, 36*s,34*s, 32*s,30*s, 28*s,26*s, 28*s,9*s], fill=self.color_off, outline="")
        self.segments['C'] = self.create_polygon([32*s,40*s, 36*s,44*s, 36*s,70*s, 32*s,66*s, 28*s,61*s, 28*s,44*s], fill=self.color_off, outline="")
        self.segments['D'] = self.create_polygon([8*s,70*s, 32*s,70*s, 27*s,66*s, 13*s,66*s], fill=self.color_off, outline="")
        self.segments['E'] = self.create_polygon([4*s,40*s, 8*s,44*s, 12*s,49*s, 12*s,61*s, 8*s,66*s, 4*s,62*s, 4*s,44*s], fill=self.color_off, outline="")
        self.segments['F'] = self.create_polygon([4*s,8*s, 8*s,12*s, 12*s,18*s, 12*s,26*s, 8*s,30*s, 4*s,26*s, 4*s,12*s], fill=self.color_off, outline="")
        self.segments['G'] = self.create_polygon([8*s,36*s, 32*s,36*s, 27*s,32*s, 13*s,32*s], fill=self.color_off, outline="")

        self.map = {
            '0': 'ABCDEF',
            '1': 'BC',
            '2': 'ABGED',
            '3': 'ABCDG',
            '4': 'FGBC',
            '5': 'AFGCD',
            '6': 'AFGCDE',
            '7': 'ABC',
            '8': 'ABCDEFG',
            '9': 'ABCFG',
            '-': 'G',
            'C': 'AFED',
            '°': 'ABFG',
            ' ': ''
        }

    def display(self, ch):
        on = self.map.get(ch, '')
        for seg, obj in self.segments.items():
            color = self.color_on if seg in on else self.color_off
            self.itemconfig(obj, fill=color)


# -----------------------------
# Temperature display (with °C)
# -----------------------------
class TemperatureDisplay(ttk.Frame):
    def __init__(self, parent, label="TEMP", scale=1.6, numDec=1, numDigs=2, **kwargs):
        super().__init__(parent, **kwargs)
        ttk.Label(self, text=label, font=("Segoe UI", 12, "bold")).pack()
        row = ttk.Frame(self)
        row.pack()

        self.scale = scale

        # main digits (numDigs)
        self.NumDigits = numDigs
        self.digitsInt = [SevenSegmentDisplay(row, scale=scale) for _ in range(numDigs)]
        for d in self.digitsInt:
            d.pack(side="left", padx=1)
          
        # decimal dot
        dot_size = int(10 * scale)
        dot_h = int(80 * scale)
        self.dot = tk.Canvas(row, width=dot_size, height=dot_h, bg="black", highlightthickness=0)
        self.dot_id = self.dot.create_oval(2, int(dot_h*0.75), dot_size-2, int(dot_h*0.9), fill="#300000", outline="")
        self.dot.pack(side="left", padx=1)
        
        # Decimal (numDec)
        self.NumDecimals = numDec
        self.digitsDec = [SevenSegmentDisplay(row, scale=scale) for _ in range(numDec)]
        for d in self.digitsDec:
            d.pack(side="left", padx=1)
            
        # °C digits
        self.deg = SevenSegmentDisplay(row, scale=scale)
        self.deg.display('°')
        self.deg.pack(side="left", padx=1)
        self.cchar = SevenSegmentDisplay(row, scale=scale)
        self.cchar.display('C')
        self.cchar.pack(side="left", padx=1)

    def set_value(self, value):
        formatted = f"{value:{self.NumDigits + self.NumDecimals + 1}.{self.NumDecimals}f}".strip()
        # Separar parte entera y decimal
        if '.' in formatted:
            int_part, dec_part = formatted.split('.')
        else:
            int_part, dec_part = formatted, ''

        # Convertir a listas de caracteres
        digits = list(int_part)
        decimalChar = list(dec_part)
        
        while len(digits) < self.NumDigits:
            digits.insert(0, ' ')
        while len(decimalChar) < self.NumDecimals:
            decimalChar.insert(0, ' ')
        for d, ch in zip(self.digitsInt, digits):
            d.display(ch)
        if '.' in formatted:
            self.dot.itemconfig(self.dot_id, fill="#FF2D2D")
        else:
            self.dot.itemconfig(self.dot_id, fill="#300000")
        for d, ch in zip(self.digitsDec, decimalChar):
            d.display(ch)


# -----------------------------
# Circular metallic knob
# -----------------------------
class MetalKnob(tk.Canvas):
    def __init__(self, parent, diameter=220, min_val=0.0, max_val=99.9, initial=25.0, command=None, **kwargs):
        super().__init__(parent, width=diameter, height=diameter, bg="#dcdcdc", highlightthickness=2, highlightbackground="#4a4a4a", **kwargs)
        self.diameter = diameter
        self.r = diameter/2 * 0.8
        self.min_val, self.max_val = min_val, max_val
        self.value = initial
        self.command = command
        self.min_angle, self.max_angle = -135, 135
        self._draw_body()
        self._draw_pointer()
        self.bind("<B1-Motion>", self._drag)
        self.bind("<Button-1>", self._press)
        self.bind("<MouseWheel>", self._wheel)
        self.dragging = False

    def _draw_body(self):
        c = self.diameter/2
        cx = (self.diameter-c*0.2)/2
        cy = (self.diameter)/2
        #colors=["#dcdcdc", "#b0b0b0", "#7a7a7a", "#4a4a4a"]
        colors=["#dcdcdc", "#b0b0b0"]
        for i, color in enumerate(colors):
            r = self.r * (1 - i*0.15)
            self.create_oval(cx-r, cy-r, cx+r, cy+r, fill=color, outline="#111")
        #self.create_oval(cx-8, cy-8, cx+8, cy+8, fill="#111")

    def _draw_pointer(self):
        #cx, cy = self.center, self.center
        c = self.diameter/2
        cx = (self.diameter-c*0.2)/2
        cy = (self.diameter)/2
        a = self._value_to_angle(self.value)
        px = cx + math.sin(math.radians(a)) * self.r * 0.75
        py = cy - math.cos(math.radians(a)) * self.r * 0.75
        if hasattr(self, 'ptr'):
            self.delete(self.ptr)
        if hasattr(self, 'dot'):
            self.delete(self.dot)
        #self.ptr = self.create_line(cx, cy, px, py, width=6, fill="#222", capstyle=tk.ROUND)
        self.dot = self.create_oval(px-6, py-6, px+6, py+6, fill="#FFCC00", outline="#aa7700")

    def _value_to_angle(self, val):
        frac = (val - self.min_val)/(self.max_val - self.min_val)
        return self.min_angle + frac*(self.max_angle - self.min_angle)

    def _angle_to_value(self, ang):
        ang = max(self.min_angle, min(self.max_angle, ang))
        frac = (ang - self.min_angle)/(self.max_angle - self.min_angle)
        return self.min_val + frac*(self.max_val - self.min_val)

    def _press(self, e):
        self.dragging = True
        self._drag(e)

    def _drag(self, e):
        if not self.dragging:
            return
        #cx, cy = self.center, self.center
        c = self.diameter/2
        cx = (self.diameter-c*0.2)/2
        cy = (self.diameter)/2
        dx, dy = e.x - cx, cy - e.y
        
        ang = math.degrees(math.atan2(dx, dy))
        
        val = round(self._angle_to_value(ang)*10)/10
        self.set_value(val)

    def _wheel(self, e):
        delta = 0.5 if e.delta > 0 else -0.5
        self.set_value(self.value + delta)

    def set_value(self, val):
        val = max(self.min_val, min(self.max_val, val))
        self.value = val
        self._draw_pointer()
        if self.command:
            self.command(val)


# -----------------------------
# Main Application
# -----------------------------
class PIDInterface(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("PID Temperature Controller")
        self.configure(bg="black")
        self.geometry("700x450")

        self.setpoint = 25.0
        self.measured = 24.8
        
        # ----- CREAR SOCKET AQUI -----
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            self.sock.connect(("192.168.0.10", 5000))
            print("Conectado al servidor.")
        except Exception as e:
            print("Error al conectar:", e)
            self.sock = None
        # -------------------------------------------------------------
        
        # CREAR THREAD DE RECEPCION
        if self.sock:
            threading.Thread(target=self.receive_temperature, daemon=True).start()
        # -------------------------------------------------------------------------

        main = ttk.Frame(self)
        main.pack(padx=30, pady=20)

        # left: displays
        left = ttk.Frame(main)
        left.pack(side="left", padx=20)
        self.sp_disp = TemperatureDisplay(left, label="Setpoint", scale=1.8)
        self.sp_disp.pack(pady=15)
        self.meas_disp = TemperatureDisplay(left, label="Measured", scale=1.8)
        self.meas_disp.pack(pady=15)

        # right: knob
        right = ttk.Frame(main)
        right.pack(side="left", padx=30)
        self.knob = MetalKnob(right, diameter=160, initial=self.setpoint, command=self.knob_changed)
        self.knob.pack()

        self.update_loop()

    def knob_changed(self, val):
        self.setpoint = val
        self.send_setpoint()

    def update_loop(self):
        # Read Measured temperature value
        #self.measured += 0.1*(self.setpoint - self.measured) + random.uniform(-0.05, 0.05)
        #self.measured = round(self.measured, 1)
        
        # Draw
        self.sp_disp.set_value(self.setpoint)
        self.meas_disp.set_value(self.measured)
        self.knob.set_value(self.setpoint)
        
        # Schedule this function every 20ms
        self.after(50, self.update_loop)
    
    def send_setpoint(self):
        """Envia el setpoint al servidor"""
        if self.sock:
            try:
                msg = f"{self.setpoint:.1f}".encode()
                self.sock.sendall(msg)
            except Exception as e:
                print("Error enviando setpoint:", e)

    def receive_temperature(self):
        """Hilo que recibe la temperatura del controlador"""
        while True:
            try:
                # RECEPCION DE DATOS ------------------------
                data = self.sock.recv(1024)
                # -------------------------------------------
                if not data:
                    break
                self.measured = float(data.decode())
            except Exception as e:
                print("Error recibiendo temperatura:", e)
                break


if __name__ == "__main__":
    PIDInterface().mainloop()
