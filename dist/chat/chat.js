var chatArea;
var chatTextArea;
var chatInputArea;
var chatTextInput;
var chatBtnSubmit;

function initElements() {
	chatArea = document.getElementsByClassName("chat")[0];
	if(!chatArea){
		chatArea = document.createElement("div");
		chatArea.classList.add("chat");
		document.body.appendChild(chatArea);
	}
	//console.log(chatArea);
	
	chatTextArea = document.getElementsByClassName("chat-text-area")[0];
	if(!chatTextArea){
		chatTextArea = document.createElement("div");
		chatTextArea.classList.add("chat-text-area");
		chatArea.appendChild(chatTextArea);
	}
	//console.log(chatTextArea);
	
	chatInputArea = document.getElementsByClassName("chat-input")[0];
	if(!chatInputArea){
		chatInputArea = document.createElement("div");
		chatInputArea.classList.add("chat-input");
		chatArea.appendChild(chatInputArea);
	}
	
	chatTextInput = document.getElementsByClassName("chat-text-input")[0];
	if(!chatTextInput){
		chatTextInput = document.createElement("input");
		chatTextInput.classList.add("chat-text-input");
		chatInputArea.appendChild(chatTextInput);
	}
	//console.log(chatTextInput);
	
	chatBtnSubmit = document.getElementsByClassName("chat-submit-input")[0];
	if(!chatBtnSubmit){
		chatBtnSubmit = document.createElement("button");
		chatBtnSubmit.classList.add("chat-submit-input");
		chatBtnSubmit.innerHTML += "Send";
		chatInputArea.appendChild(chatBtnSubmit);
	}
	//console.log(chatBtnSubmit);

}

function chatIni() {
	const chatSocket = new WebSocket("wss://MyCppWebPort.alexandres18.repl.co/ws-connection/chat");

	chatSocket.onopen = function(e) {
		let name = prompt('Nome: ');
		if(name == "")
			return;

		let conn = {
			reconnect: false,
			message: {
				userName: name,
			},
			
		};
		
		chatSocket.send(JSON.stringify(conn));
		divChat.style.display = "block";
		
	}
  chatSocket.onclose = function(e) { 
		divChat.style.display = "none";
		console.log("closed"); 
		console.log(e); 
	}
  chatSocket.onmessage = function(e) { 
		console.log(e);
		let data = JSON.parse(e.data);
		console.log(data);
		addMessage(data.name, data.message, data.accepted);
	}

	chatTextInput.addEventListener("keydown", function (e) {
		console.log(e);
	});
	
	chatBtnSubmit.addEventListener("click", function (e) {
		e.preventDefault();

		chatSocket.send(JSON.stringify({ message: { message: chatTextInput.value}}));
		chatTextInput.value = "";
	});
	
}

function addMessage(user, message, isOwner) {
	if(isOwner){
		chatTextArea.innerHTML += `<p><b style='color: blue'>${user}</b>: ${message}</p>`;
		return;
	}
	chatTextArea.innerHTML += `<p><b>${user}</b>: ${message}</p>`;
}