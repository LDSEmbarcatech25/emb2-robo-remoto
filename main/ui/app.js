console.log("JS INICIOU");

function atualizarFrame()
{
    const img = document.getElementById("camera");

    img.onload = () => {

        setTimeout(atualizarFrame, 400);
    };

    img.onerror = () => {

        setTimeout(atualizarFrame, 1000);
    };

    img.src = "/stream?t=" + Date.now();
}

const socket = new WebSocket(
    "ws://" + window.location.host + "/ws"
);

socket.onopen = () => {

    console.log("WS CONECTADO");

    alert("WS CONECTADO");

    atualizarFrame();
};

socket.onerror = (e) => {

    console.log("ERRO WS", e);

    alert("ERRO WS");
};

socket.onclose = () => {

    console.log("WS FECHOU");

    alert("WS FECHOU");
};

function enviar(cmd)
{
    if(socket.readyState === WebSocket.OPEN)
    {
        socket.send(cmd);

        console.log("ENVIADO:", cmd);
    }
}

document.getElementById("frente").onclick = () => {

    enviar("FRENTE");
};

document.getElementById("tras").onclick = () => {

    enviar("TRAS");
};

document.getElementById("esquerda").onclick = () => {

    enviar("ESQUERDA");
};

document.getElementById("direita").onclick = () => {

    enviar("DIREITA");
};