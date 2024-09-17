#ifndef WEBPAGE_H
#define WEBPAGE_H

const char* webpage =  R"=====( 
<!DOCTYPE html> 
<html lang="en"> 
<head> 
    <meta charset="UTF-8"> 
    <meta name="viewport" content="width=device-width, initial-scale=1.0"> 
    <title>Joystick and Buttons</title> 
    <style> 
        body { 
            font-family: Arial, sans-serif; 
            display: flex; 
            align-items: center; 
            justify-content: center; 
            height: 100vh; 
            margin: 0; 
        } 

        .container { 
            position: relative; 
            display: flex; 
        } 

        #joystick { 
            width: 150px; 
            height: 150px; 
            background-color: #ddd; 
            border-radius: 50%; 
            position: relative; 
            margin-right: 40px; 
            border: 2px solid #333; 
            overflow: hidden; /* Prevents the stick from going outside */
        } 

        #stick { 
            width: 40px; 
            height: 40px; 
            background-color: #333; 
            border-radius: 50%; 
            position: absolute; 
            top: 50%; 
            left: 50%; 
            transform: translate(-50%, -50%); /* Center the stick initially */
        } 

        .button { 
            width: 50px; 
            height: 50px; 
            background-color: #333; 
            color: white; 
            border-radius: 50%; 
            display: flex; 
            align-items: center; 
            justify-content: center; 
            font-size: 20px; 
            border: none; 
            cursor: pointer; 
        } 

        .button-a { 
            position: absolute; 
            right: -60px; 
            top: 0; 
        } 

        .button-b { 
            position: absolute; 
            right: -60px; 
            bottom: 0; 
        } 
    </style> 
</head> 
<body> 
    
    <div class="container"> 
        <div id="joystick"> 
            <div id="stick"></div> 
        </div> 
        <button class="button button-a" id="buttonA">A</button> 
        <button class="button button-b" id="buttonB">B</button> 
    </div> 

        <script> 
        var ws; 
        var gateway = `ws://${window.location.hostname}/ws`; 
        var throttleTime = 50;  // Send WebSocket data every 50ms
        var lastSentTime = 0;
        var isCentered = false;  // Track if the stick is centered

        console.log('Trying to open a WebSocket connection…'); 
        websocket = new WebSocket(gateway); 
        websocket.onopen = onOpen; 
        websocket.onclose = onClose; 

        const joystick = document.getElementById("joystick"); 
        const stick = document.getElementById("stick"); 
        let joystickRect = joystick.getBoundingClientRect(); 
        let lastX = 0, lastY = 0;  // Store last values to avoid spamming if the values haven't changed

        window.addEventListener("resize", function() {
            joystickRect = joystick.getBoundingClientRect();  // Recalculate rect when window is resized
        });

        function sendWebSocketMessage(message) {
            if (websocket.readyState === WebSocket.OPEN) {
                websocket.send(message);
            }
        }

        function throttleSendMessage(message) {
            const now = Date.now();
            if (now - lastSentTime >= throttleTime) {
                sendWebSocketMessage(message);
                lastSentTime = now;
            }
        }

        joystick.addEventListener("mousemove", function(event) {
            const x = event.clientX - joystickRect.left - joystickRect.width / 2;
            const y = event.clientY - joystickRect.top - joystickRect.height / 2;

            const radius = joystickRect.width / 2;
            const distance = Math.sqrt(x * x + y * y);

            // Normalize the values to -1 to 1 range
            let clampedX = x / radius;
            let clampedY = -y / radius; // Invert Y to follow standard Cartesian coordinates

            if (distance > radius) {
                clampedX = clampedX / distance * radius;
                clampedY = clampedY / distance * radius;
            }

            // Move the stick within bounds
            stick.style.left = `${(clampedX + 1) * 50}%`;
            stick.style.top = `${(1 - clampedY) * 50}%`;

            // Send only if joystick values changed significantly or throttle time allows
            if (Math.abs(clampedX - lastX) > 0.01 || Math.abs(clampedY - lastY) > 0.01) {
                throttleSendMessage("JST: " + clampedX.toFixed(2) + ", " + clampedY.toFixed(2)); 
                lastX = clampedX;
                lastY = clampedY;
                isCentered = false;  // Joystick is not centered
            }
        }); 

        joystick.addEventListener("mouseleave", function() { 
            if (!isCentered) {
                sendWebSocketMessage("JST: 0, 0");
                stick.style.left = "50%";
                stick.style.top = "50%";
                lastX = 0;
                lastY = 0;
                isCentered = true;  // Joystick is centered now
                sendWebSocketMessage("JST: 0, 0");
            }
        }); 

        document.getElementById("buttonA").addEventListener("click", function() { 
            console.log("Button A pressed"); 
            sendWebSocketMessage("BTN: A"); 
        }); 

        document.getElementById("buttonB").addEventListener("click", function() { 
            console.log("Button B pressed"); 
            sendWebSocketMessage("BTN: B"); 
        }); 

        function onOpen(event) { 
            console.log('Connection opened'); 
        } 

        function onClose(event) { 
            console.log('Connection closed'); 
            setTimeout(initWebSocket, 2000);  // Reconnect after 2 seconds if connection closes 
        }

        function initWebSocket() { 
            console.log('Trying to open a WebSocket connection…'); 
            websocket = new WebSocket(gateway); 
            websocket.onopen = onOpen; 
            websocket.onclose = onClose; 
        } 
    </script>
</body> 
</html> 
)=====";

#endif