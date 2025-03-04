const htmlElements = {
    "connectButton": document.getElementById("connect-button"),
    "connectionStatus": document.getElementById("connectionStatus"),
    "messageContainer": document.getElementById("message-container"),
}

let connectionStatus = htmlElements.connectionStatus
function ConnectButtonClick() {
    try {
        socket = new WebSocket('ws://localhost:9002');

        socket.onopen = function () {
            connectionStatus.textContent = 'Connected';
            addMessage('Connected to server', 'system-message');

            // Enable UI elements
            // messageInput.disabled = false;
            // sendButton.disabled = false;
            // connectButton.disabled = true;
            // disconnectButton.disabled = false;
        };

        socket.onmessage = function (event) {
            addMessage('Received: ' + event.data, 'received-message');
        };

        socket.onclose = function () {
            connectionStatus.textContent = 'Disconnected';
            addMessage('Disconnected from server', 'system-message');

            // Disable UI elements
            // messageInput.disabled = true;
            // sendButton.disabled = true;
            // connectButton.disabled = false;
            // disconnectButton.disabled = true;
        };

        socket.onerror = function (error) {
            addMessage('Error: ' + error, 'system-message');
        };
    } catch (error) {
        addMessage('Connection error: ' + error, 'system-message');
    }
}
// Add message to the container
function addMessage(message, type) {
    const messageElement = document.createElement('div');
    messageElement.classList.add(type);
    messageElement.textContent = message;
    htmlElements.messageContainer.appendChild(messageElement);
    htmlElements.messageContainer.scrollTop = htmlElements.messageContainer.scrollHeight;
}


htmlElements.connectButton.onclick = ConnectButtonClick