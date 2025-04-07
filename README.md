<!-- ============================== 
     Header Section: Maze Destroyer
================================== -->
<h2 align="center">Prototipo - Resolvedor de laberintos (Versión Boost XL6009)</h2>
<p align="center">
  <img width="100%" alt="GIF" src="https://github.com/user-attachments/assets/1b20f516-d102-4417-a13f-fc37040775b8" />
</p>
<p align="center">
  Este prototipo fue diseñado para resolver laberintos utilizando un algoritmo basado en la estrategia de seguir siempre la pared derecha. El sistema interpreta diferentes tipos de intersecciones analizando las lecturas de los sensores, junto con el tiempo y la memoria del recorrido.
</p>

<p align="center">
  Además, es capaz de seguir curvas e intersecciones complejas mediante un algoritmo PID.
</p>

<p align="center">
  <b>Aún presenta fallos en la interpretación de algunas intersecciones específicas, cuando el angulo es muy cerrado.</b>
</p>


<!-- ============================== 
     Header Section: Multimedia
================================== -->

- **Participación:**
  
  - https://youtu.be/P092oWwt9Sc
  
  - https://youtu.be/NX2z7nnBkeQ

<!-- ============================== 
     Header Section: Circuito
================================== -->
<h2 align="center">🔌 Conexiones</h2>

## Arduino Nano

| Pin Arduino | Conectado a                          |
|-------------|--------------------------------------|
| D2          | Arrancador (pin 4)                   |
| D3          | TB6672 → PWMA                        |
| D4          | TB6672 → AIN2                        |
| D5          | TB6672 → AIN1                        |
| D6          | TB6672 → STBY                        |
| D7          | TB6672 → BIN1                        |
| D8          | TB6672 → BIN2                        |
| D9          | Pulsador (botón)                     |
| D10         | Turbina                              |
| D11         | TB6672 → PWMB                        |
| D12         | Sensor 9                             |
| D13         | Sensor 10                            |
| A0          | Sensor 8                             |
| A1          | Sensor 7                             |
| A2          | Sensor 6                             |
| A3          | Sensor 5                             |
| A4          | Sensor 4                             |
| A5          | Sensor 3                             |
| A6          | Sensor 2                             |
| A7          | Sensor 1                             |
| VIN         | OUT+ del booster XL6009 (opcional)   |
| GND         | GND común (OUT- del booster XL6009)  |

---

## 📦 Driver de motor TB6672FNG

| Pin TB6672 | Conectado a                           |
|------------|---------------------------------------|
| VIN        | OUT+ del XL6009                       |
| VCC        | Arduino 5V                            |
| GND (x2)   | OUT- del XL6009                       |
| PWMA       | Arduino D3                            |
| AIN2       | Arduino D4                            |
| AIN1       | Arduino D5                            |
| STBY       | Arduino D6                            |
| BIN1       | Arduino D7                            |
| BIN2       | Arduino D8                            |
| PWMB       | Arduino D11                           |
| AO1 / AO2  | Motor MM1                             |
| BO1 / BO2  | Motor MM2                             |
| MD1, MD2, ML1, ML2 | GND o sin conexión            |

---

## 🧲 Conexión de Motores

| Motor | Conectado a            |
|-------|------------------------|
| MM1   | AO1 y AO2 del TB6672   |
| MM2   | BO1 y BO2 del TB6672   |

---

## ⚙️ Pulsador (Botón)

- **Pin 1:** Arduino D9  
- **Pin 2:** GND  
- **Entre D9 y GND:** Resistencia de 10kΩ (pull-down)

---

## 💡 Turbina

- **Positivo:** Arduino D10  
- **Negativo:** GND común

---

## 🧪 Sensores

| Sensor # | Pin Arduino |
|----------|-------------|
| 1        | A7          |
| 2        | A6          |
| 3        | A5          |
| 4        | A4          |
| 5        | A3          |
| 6        | A2          |
| 7        | A1          |
| 8        | A0          |
| 9        | D12         |
| 10       | D13         |

> Todos los sensores deben compartir **VCC (5V)** y **GND común** con el Arduino.

## ⚡ Módulo Boost Step-Up XL6009

| Pin XL6009 | Conectado a                                    |
|------------|------------------------------------------------|
| IN+        | Positivo de batería / fuente                   |
| IN-        | Negativo de batería / fuente                   |
| OUT+       | VM del TB6672 (y opcionalmente Arduino VIN)    |
| OUT-       | Tierra común (GND de Arduino, sensores, motores)   |

<!-- ============================== 
     Header Section: Contenido de estudio
================================== -->
<h2 align="center">Contenido de estudio - Anotaciones</h2>

## 1. Arduino Uno: Pines Digitales


Los pines digitales en un Arduino son aquellos que solo pueden leer o escribir dos estados posibles: **HIGH** (Alto - 5V) o **LOW** (Bajo - 0V), respectivamente (en la mayoría de las placas). Estos pines funcionan con lógica binaria, por lo que solo pueden estar encendidos o apagados.

### Salida Digital

También se pueden usar como salidas para encender o apagar LEDs, motores, relés, etc. El código cambia el estado del pin a **HIGH** o **LOW** para activar o desactivar el componente conectado.

#### Ejemplo de Código:

```cpp
int ledPin = 13; // Pin digital 13

void setup() {
  pinMode(ledPin, OUTPUT); 
  // Configura el pin como salida
}

void loop() {
  // Enciende el LED
  digitalWrite(ledPin, HIGH); 

  // Espera 1 segundo
  delay(1000); 

  // Apaga el LED
  digitalWrite(ledPin, LOW);
}
```

La función setup() se usa para preparar el entorno en el que trabaja el programa y loop() define un ciclo infinito de procesos. En este ejemplo, se usa un pin digital (13) para encender y apagar un LED con un retardo de 1 segundo entre estados.

- **Consideraciones:**
  - Los pines digitales solo reconocen 0V o 5V, no valores intermedios.
  
  - El número de pines digitales varía entre modelos de Arduino.

Arduino Uno, que tiene 14 pines digitales (de los cuales algunos pueden tener funciones especiales, como PWM).

## 2. Pines Analógicos

Los pines analógicos están etiquetados como `A0`, `A1`, `A2`, etc. y permiten leer valores variables en lugar de simplemente `HIGH` (5V) o `LOW` (0V).

- **Lectura analógica:**  
  Un pin analógico convierte una señal de voltaje continuo (entre `0V` y `5V`) en un valor digital que varía de `0` a `1023`.  
  - `0` equivale a `0V`
  - `1023` equivale a `5V`

Estos pines se usan para leer datos de sensores como potenciómetros, sensores de temperatura, luz, entre otros.

---

## 3. PWM (Modulación por Ancho de Pulso)

Aunque los pines digitales solo pueden estar en dos estados (encendido o apagado), se puede simular una salida analógica usando **PWM**.

### Conceptos Clave:

- **PWM:** Permite generar una señal que, a través de un ciclo de encendido/apagado, simula diferentes niveles de voltaje.
- **Duty Cycle (Ciclo de Trabajo):** Es la relación entre el tiempo en que la señal está activa y el periodo total.
  - Por ejemplo, un duty cycle del 50% significa que la señal está encendida la mitad del tiempo.

### Ejemplo de Señal PWM:

- **Periodo (T):** 40 ms (20 ms encendido + 20 ms apagado)
- **Frecuencia (f):** f = 1/T = 25 Hz

En otro ejemplo, se puede tener un duty cycle del 20%:
- La señal estará en 5V el 20% del tiempo y en 0V el 80% del tiempo.

---

## 4. Usos de los Pines Analógicos y PWM

- **Entrada Analógica:**  
  Permite leer valores de sensores variables.  
  *Ejemplo:* Un potenciómetro que ajusta la intensidad de un LED en función del valor leído.

- **Salida Analógica (PWM):**  
  Algunos pines digitales pueden simular una salida analógica usando PWM para controlar la intensidad de dispositivos, como el brillo de un LED.

---

## 5. Ejemplo de Código

```cpp
int BRILLO;

void setup() {
  pinMode(3, OUTPUT); // Configura el pin 3 como salida PWM
  // No es necesario inicializar pines analógicos
}

void loop() {
  // Lee el valor del potenciómetro en el pin analógico 0 y lo escala (0-1023 a 0-255)
  BRILLO = analogRead(0) / 4;
  // Envía el valor escalado al pin 3 para controlar el brillo del LED
  analogWrite(3, BRILLO);
}
```

En este ejemplo, se lee el valor de un potenciómetro y se utiliza para ajustar el brillo de un LED, mediante la función analogWrite() en el pin 3.
