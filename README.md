# 🏗️ Proyecto de Control de Grúa mediante Interfaz Web 🌐

Este proyecto implementa un sistema de control remoto para una grúa a través de una interfaz web alojada en un **ESP32**. Utilizando una conexión Wi-Fi, los usuarios pueden controlar los movimientos de la grúa y sus ruedas desde cualquier dispositivo con un navegador. El enfoque principal está en ofrecer un control preciso y fácil de usar.

## 🚀 Características Principales

1. **Control Total de la Grúa**:
    - El sistema ofrece tres menús funcionales para controlar diferentes partes de la grúa (6 motores -cable, brazo, base, puente y movimiento).
    - Cada botón está diseñado para una respuesta rápida y precisa.

2. **Control del movimiento**:
    - El tercer menú se dedica al control del movimiento de la grúa, permitiendo desplazamientos en varias direcciones.
    - Incluye botones para avanzar, retroceder, girar a la izquierda y a la derecha.

3. **Función de Detener (Stop) Inmediato**:
    - Cada menú tiene un botón de 'Stop' que permite detener todos los movimientos de manera instantánea.
    - La funcionalidad ha sido mejorada para que, al soltar cualquier botón, el movimiento de las ruedas se detenga automáticamente.

4. **Mecanismo de Seguridad ante Desconexiones**:
    - Si se pierde la conexión con el ESP32, todos los movimientos de la grúa y las ruedas se detendrán automáticamente. Esto garantiza que no haya riesgos de funcionamiento incontrolado.

## 🛠️ Requisitos

- **ESP32**: El microcontrolador ESP32 es el encargado de alojar la interfaz web y recibir los comandos enviados por el usuario.
2. **L298N**: Un controlador de motores dual que permite controlar el movimiento de motores de corriente continua. El L298N puede manejar hasta dos motores a la vez Es ideal para proyectos que requieren el control de motores de corriente continua de 5V a 35V, y puede manejar una corriente máxima de 2A por canal.
- **Conexión Wi-Fi**: Es necesario que el ESP32 esté conectado a una red Wi-Fi para permitir la interacción remota.
- **Navegador Web**: Cualquier dispositivo con un navegador web (computadora, tablet, smartphone) puede ser utilizado para controlar la grúa.
  
## 📂 Estructura del Proyecto

El código del proyecto se divide en dos partes principales:

1. **Código del ESP32**: Maneja la lógica de red, la interfaz web y los comandos enviados al hardware de la grúa.
2. **Interfaz HTML**: La página web que se carga desde el ESP32 contiene botones interactivos para el control total de la grúa y sus ruedas.

## 🎯 Objetivos del Proyecto

Este proyecto tiene como objetivo proporcionar un sistema de control remoto confiable y fácil de usar para una grúa, permitiendo que los usuarios puedan manejar el equipo desde cualquier dispositivo conectado. Es especialmente útil para proyectos de automatización y prototipos donde se necesita controlar maquinaria de forma segura y eficiente.

## 💻 Cómo Empezar

1. Clona este repositorio:
    ```bash
    git clone https://github.com/tu-usuario/tu-repositorio.git
    ```
2. Sube el código al ESP32 usando el IDE de Arduino o PlatformIO.
3. Conecta el ESP32 a una red Wi-Fi y accede a la interfaz web desde cualquier navegador introduciendo la IP asignada al dispositivo.

---

Este proyecto es un excelente ejemplo de cómo los microcontroladores como el ESP32 pueden integrarse con tecnologías web para ofrecer soluciones de control remoto en tiempo real. ¡Espero que disfrutes experimentando con él y lo adaptes a tus propias necesidades! 🚧🤖






