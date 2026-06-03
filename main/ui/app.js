console.log("JS INICIOU");

function atualizarFrame()
{
    const img = document.getElementById("camera");

    img.onload = () => {

        setTimeout(atualizarFrame, 30);
    };

    img.onerror = () => {

        setTimeout(atualizarFrame, 100);
    };

    img.src = "/stream?t=" + Date.now();
}

const socket = new WebSocket(
    "ws://" + window.location.host + "/ws"
);

socket.onopen = () => {

    console.log("WS CONECTADO");
    document.body.focus();
    atualizarFrame();
};

socket.onerror = (e) => {

    console.log("ERRO WS", e);
    alert("Erro na conexão com o WebSocket");
};

socket.onclose = () => {

    console.log("WS FECHOU");
};

function enviar(cmd)
{
    if(socket.readyState === WebSocket.OPEN)
    {
        socket.send(cmd);
        console.log(cmd);
    }
}

const frente = document.getElementById("frente");
    frente.onmousedown = () => {
        enviar("FRENTE");
    }
    frente.onmouseup = () => {
        enviar("PARAR");
    }


const tras =document.getElementById("tras");
    tras.onmousedown = () => {
        enviar("TRAS");
    }
    tras.onmouseup = () => { 
        enviar("PARAR");
    };

const esquerda =document.getElementById("esquerda");
    esquerda.onmousedown = () => {
    enviar("ESQUERDA");
    }
    esquerda.onmouseup = () => {
    enviar("PARAR");
    }   

const direita = document.getElementById("direita");
    direita.onmousedown = () => {
    enviar("DIREITA");
    }  
    direita.onmouseup = () => {     
    enviar("PARAR");
    };


document.addEventListener("keydown", (e) => {
    if(e.repeat)
        return;

    console.log("TECLA:", e.key);

    switch(e.key){
        
        case "w":
        case "W":
        case "ArrowUp":
            enviar("FRENTE");
            break;
        
        case "s":
        case "S":
        case "ArrowDown":
            enviar("TRAS");
            break;
        
        case "a":
        case "A":
        case "ArrowLeft":
            enviar("ESQUERDA");
            break;
        
        case "d":
        case "D":
        case "ArrowRight":
            enviar("DIREITA");
            break;
    }
});

document.addEventListener("keyup",(e)=>{

    switch(e.key){
        case "w":
        case "W":
        case "ArrowUp":
        
        case "s":
        case "S":
        case "ArrowDown":

        case "a":
        case "A":
        case "ArrowLeft":

        case "d":
        case "D":
        case "ArrowRight":

            enviar("PARAR");
            break;
    }
});

window.onload = function()
{
    document.body.focus();
};

socket.onmessage = (event) => {

    console.log(event.data);

    if(event.data.startsWith("Bateria:"))
    {
        document.getElementById("bateria")
            .innerText =
            event.data.replace("Bateria:", "");
    }

    if(event.data.startsWith("Velocidade:"))
    {
        document.getElementById("velocidade")
            .innerText =
            event.data.replace("Velocidade:", "");
    }

    if(event.data.startsWith("IP:"))
    {
        document.getElementById("ip")
            .innerText =
            event.data.replace("IP:", "");
    }

};

const botaoLogin =
    document.getElementById("entrar");

botaoLogin.onclick = () => {

    const senha =
        document.getElementById("senha").value;

    const usuario =
        document.getElementById("usuario").value;

    if(usuario=="login" && senha == "1234")
    {
        document.getElementById("login")
            .style.display = "none";

        document.getElementById("dashboard")
            .style.display = "block";
    }
    else
    {
        alert("Usuário ou senha incorretos");
    }
};