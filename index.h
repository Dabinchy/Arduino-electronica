const char *HTML_CONTENT = R"=====(
<!DOCTYPE html>
<html>
<head>
<title>Control de Grua</title>
<meta name="viewport" content="width=device-width, initial-scale=0.7, maximum-scale=1, user-scalable=no">
<style type="text/css">
body { text-align: center; font-size: 24px; }
button { text-align: center; font-size: 24px; }
#container, #container2, #container3 {
    display: inline-block;
    width: 400px; 
    height: 400px;
    position: relative;
    margin: 0 50px; /* Espacio entre los menús */
}
div[class^='button'] { position: absolute; }
.button_up, .button_down { width: 214px; height: 104px; }
.button_left, .button_right { width: 104px; height: 214px; }
.button_stop { width: 178px; height: 178px; }
.button_up {
    background: url('https://esp32io.com/images/tutorial/up_inactive.png') no-repeat; 
    background-size: contain; 
    left:50%;
    top: 0px;
    transform: translateX(-50%);
}
.button_down {
    background: url('https://esp32io.com/images/tutorial/down_inactive.png') no-repeat; 
    background-size: contain; 
    left: 50%;
    bottom: 0px;
    transform: translateX(-50%);
}
.button_right {
    background: url('https://esp32io.com/images/tutorial/right_inactive.png') no-repeat;
    background-size: contain;   
    right: 0px;
    top: 50%;
    transform: translateY(-50%);
}
.button_left {
    background: url('https://esp32io.com/images/tutorial/left_inactive.png') no-repeat; 
    background-size: contain;
    left: 0px;
    top: 50%;
    transform: translateY(-50%);
}
.button_stop {
    background: url('https://esp32io.com/images/tutorial/stop_inactive.png') no-repeat; 
    background-size: contain;
    left: 50%;
    top: 50%;
    transform: translate(-50%, -50%);
}
.button_forward {
    background: url('https://esp32io.com/images/tutorial/up_inactive.png') no-repeat; 
    background-size: contain; 
    left: 50%;
    top: 0px;
    transform: translateX(-50%);
}
.button_backward {
    background: url('https://esp32io.com/images/tutorial/down_inactive.png') no-repeat; 
    background-size: contain; 
    left: 50%;
    bottom: 0px;
    transform: translateX(-50%);
}
.button_turn_left {
    background: url('https://esp32io.com/images/tutorial/left_inactive.png') no-repeat; 
    background-size: contain;
    left: 0px;
    top: 50%;
    transform: translateY(-50%);
}
.button_turn_right {
    background: url('https://esp32io.com/images/tutorial/right_inactive.png') no-repeat; 
    background-size: contain;   
    right: 0px;
    top: 50%;
    transform: translateY(-50%);
}
#log {
    width: 90%;
    height: 150px;
    margin: 20px auto;
    background-color: #f1f1f1;
    overflow-y: scroll;
    font-size: 16px;
    border: 1px solid #ccc;
    padding: 10px;
    box-shadow: 2px 2px 5px rgba(0,0,0,0.5);
    resize: none; /* No permite redimensionar */
}
</style>
<script>
var CMD_STOP        = 0;
var CMD_CABLE_UP    = 1;
var CMD_CABLE_DOWN  = 2;
var CMD_ARM_UP      = 4;
var CMD_ARM_DOWN    = 8;
var CMD_GEAR_LEFT   = 16;
var CMD_GEAR_RIGHT  = 32;
var CMD_BRIDGE_UP   = 64;
var CMD_BRIDGE_DOWN = 128;
var CMD_FORWARD     = 256;
var CMD_BACKWARD    = 512;
var CMD_TURN_LEFT   = 1024;
var CMD_TURN_RIGHT  = 2048;

var img_name_lookup = {
  [CMD_STOP]:        "stop",
  [CMD_CABLE_UP]:    "up",
  [CMD_CABLE_DOWN]:  "down",
  [CMD_ARM_UP]:      "left",
  [CMD_ARM_DOWN]:    "right",
  [CMD_GEAR_LEFT]:   "left",
  [CMD_GEAR_RIGHT]:  "right",
  [CMD_BRIDGE_UP]:   "up",
  [CMD_BRIDGE_DOWN]: "down",
  [CMD_TURN_LEFT]:   "left",
  [CMD_TURN_RIGHT]:  "right",
  [CMD_FORWARD]:     "up",
  [CMD_BACKWARD]:    "down"
}

var ws = null;

function init() {
  var containers = [document.querySelector("#container"), document.querySelector("#container2"), document.querySelector("#container3")];
  containers.forEach(container => {
    container.addEventListener("touchstart", mouse_down);
    container.addEventListener("touchend", mouse_up);
    container.addEventListener("touchcancel", mouse_up);
    container.addEventListener("mousedown", mouse_down);
    container.addEventListener("mouseup", mouse_up);
    container.addEventListener("mouseout", mouse_up);
  });
}

function ws_onmessage(e_msg) {
  e_msg = e_msg || window.event; // MessageEvent
  logMessage("Mensaje recibido: " + e_msg.data);
}

function ws_onopen() {
  logMessage("Conexión abierta");
  document.getElementById("wc_conn").innerHTML = "Desconectar";
}

function ws_onclose() {
  logMessage("Conexión cerrada");
  document.getElementById("wc_conn").innerHTML = "Conectar";
  console.log("socket was closed");

  // Enviamos el comando para detener todos los movimientos
  send_command(CMD_STOP);  // Detener movimientos si se pierde la conexión
  
  ws.onopen = null;
  ws.onclose = null;
  ws.onmessage = null;
  ws = null;
}

function wc_onclick() {
  if(ws == null) {
    ws = new WebSocket("ws://" + window.location.host + ":81");
    logMessage("Conectando...");
    ws.onopen = ws_onopen;
    ws.onclose = ws_onclose;
    ws.onmessage = ws_onmessage; 
  } else {
    ws.close();
  }
}

function mouse_down(event) {
  if (event.target !== event.currentTarget) {
    var id = event.target.id;
    send_command(id);  // Enviamos el comando correspondiente al botón presionado
    event.target.style.backgroundImage = "url('https://esp32io.com/images/tutorial/" + img_name_lookup[id] + "_active.png')";
    logMessage("Comando enviado: " + id);
  }
  event.stopPropagation();    
  event.preventDefault();    
}

function mouse_up(event) {
  if (event.target !== event.currentTarget) {
    send_command(CMD_STOP);  // Siempre enviamos el comando para detener el movimiento
    var id = event.target.id;
    event.target.style.backgroundImage = "url('https://esp32io.com/images/tutorial/" + img_name_lookup[id] + "_inactive.png')";
    logMessage("Comando STOP enviado");
  }
  event.stopPropagation();   
  event.preventDefault();    
}

function send_command(cmd) {   
  if(ws != null) {
    if(ws.readyState == 1) {
      ws.send(cmd + "\r\n");   
    }
  }
}

function logMessage(message) {
  const logArea = document.getElementById("log");
  logArea.value += message + "\n";  // Añade el mensaje al cuadro de texto
  logArea.scrollTop = logArea.scrollHeight;  // Desplaza hacia abajo para mostrar el último mensaje
}

window.onload = init;
</script>
</head>
<body>
<h2>ESP32 - Grua Via Web</h2>
<h4>↑↓-CABLE  ←→-BRAZO  //    ↑↓-PUENTE  ←→-GIRO-BASE //   ↑↓←→-DESPLAZAMIENTO</h4>

<!-- Primer menú -->
<div id="container">
  <div id="0" class="button_stop"></div>
  <div id="1" class="button_up"></div>
  <div id="2" class="button_down"></div>
  <div id="8" class="button_right"></div>
  <div id="4" class="button_left"></div>
</div>

<!-- Segundo menú, al lado del primero -->
<div id="container2">
  <div id="0" class="button_stop"></div>
  <div id="16" class="button_left"></div>
  <div id="32" class="button_right"></div>
  <div id="64" class="button_up"></div>
  <div id="128" class="button_down"></div>
</div>

<!-- Tercer menú, al lado de los anteriores -->
<div id="container3">
  <div id="0" class="button_stop"></div>
  <div id="256" class="button_up"></div>
  <div id="512" class="button_down"></div>
  <div id="1024" class="button_left"></div>
  <div id="2048" class="button_right"></div>
</div>

<!-- Cuadro de texto para mostrar los mensajes de depuración -->
<textarea id="log" readonly></textarea>

<p>
WebSocket : <span id="ws_state" style="color:blue">Close</span><br>
</p>
<button id="wc_conn" type="button" onclick="wc_onclick();">Desconectado</button>
<br>
<br>
<div class="sponsor">DR:CHE</a></div>
</body>
</html>
)=====";
