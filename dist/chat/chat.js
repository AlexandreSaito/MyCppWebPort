// Chat Functions
const chatElement = document.querySelector("#chat");
const headerChat = document.querySelector("#header-chat");
const users = document.querySelector("#chat .users");
const userNameForm = document.querySelector("#user-name-form");
const userNameBtn = document.querySelector("#chat-submit-user-name");
const chatUser = document.querySelector(".chat-user");
const chatUserName = chatUser.querySelector(".user-name");
const chatUserChat = chatUser.querySelector(".user-chat");
const btnSendChat = chatElement.querySelector("#send-chat");
const inputChat = chatElement.querySelector("#input-chat");
const historyElement = chatUser.querySelector(".history");

var chatSocket;
var userName;

enterDoSubmit("#input-chat", "#send-chat", true);

const chatContextMenu = new ContextMenu("chat!");
registerChatItens();

function registerChatWebsocket(){
	chatSocket = new WebSocket("wss://MyCppWebPort.alexandres18.repl.co/ws-connection/chat");
	
  chatSocket.onopen = function(e) { 
		chatSocket.send(createWebsocketMessage({userName: userName}, {op: 99})); 
		let text = '';
		for(let i = 0; i < 800; i++)
			text += 'a';
		chatSocket.send(createWebsocketMessage({userName: text}, {op: 1})); 
	}
  chatSocket.onclose = function(e) { 
		console.log("closed"); console.log(e); 
	}
  chatSocket.onmessage = function(e) { 
		//alert(e.data); 
		let data = JSON.parse(e.data);
		console.log(data);
		switch(data.op){
			case 1:
				headerChat.click(); 
				chatSocket.send(createWebsocketMessage({}, {op: 4})); 
				users.append(createUserSelector({ name: 'GLOBAL', id: 0 }));
				break;
			case 3:
				historyElement.append(createHistoryLine({name: data.name, text: data.message}));
				break;
			case 4:
				for (var i = 0; i < data.users.length; i++) {
				  users.append(createUserSelector({ name: data.users[i].name, id: data.users[i].id }));
				}
				break;
			case 5:
				users.append(createUserSelector({ name: data.user.name, id: data.user.id }));
				break;
		}
	}
}

function registerChatItens() {
  let hideItem = new ContextMenuItem({
    name: "Esconder chat.",
    action: () => {
      chatElement.classList.remove("open");
      if (!chatElement.classList.contains("close"))
        chatElement.classList.add("close");
      chatElement.classList.add("hidden");

      setTimeout(() => {
        headerChat.classList.add("show");
      }, 1500);
    }
  });

  let userNameItem = new ContextMenuItem({
    name: "",
    onOpen: (item) => {
      item.querySelector(
        ".item-name"
      ).innerHTML = `Alterar user name (${userName})`;
    },
    action: () => {
      userNameForm.classList.add("show");
    }
  });

  let teste = new ContextMenuItem({
    name: "teste",
    itens: [{ name: "T1" }, { name: "T4" }, { name: "T3" }, { name: "T2" }]
  });

  chatContextMenu.registerItem(userNameItem);
  chatContextMenu.registerItem(hideItem);
  //chatContextMenu.registerItem(teste);
}

domController.register(".user", selectChatUser);

function createUserSelector({ name, id }) {
  let temp = document.createElement("div");
  temp.classList.add("user");
  if (users.querySelector(".user") == undefined) temp.classList.add("focus");
  temp.innerHTML = `${name}`;
  //temp.addEventListener("click", selectChatUser);
  temp.dataset.name = name;
  temp.dataset.id = id;
  return temp;
}

function collapseSelectedUser() {
  chatUser.classList.add("close");

  if (
    chatUser.dataset.currentUserName &&
    chatUser.dataset.collapsed == "false"
  ) {
    let temp = createUserSelector({
      name: chatUser.dataset.currentUserName,
      id: chatUser.dataset.currentUserId
    });

    users.append(temp);
  }

  chatUser.dataset.collapsed = true;
}

function showSelectedUser(user, shownAction) {
  chatUser.dataset.currentUserId = user.id;
  chatUser.dataset.currentUserName = user.name;

  chatUserName.innerHTML = user.name;

  chatUser.classList.remove("close");
  chatUser.dataset.collapsed = false;

  // search text by user
	historyElement.innerHTML = "";
  //fillHistoryText();

  if (shownAction) shownAction();
}

function fillHistoryText() {
  historyElement.innerHTML = "";
  let history = [];

  for (var i = 0; i < 10; i++) history.push({ name: "a", text: "b" });

  history.forEach((x) => {
    historyElement.append(createHistoryLine({name: x.name, text: x.text}));
  });
}

function createHistoryLine({name, text}){
	let temp = document.createElement("p");
	temp.innerHTML = `<b class="name">${name}</b>: <span class="text">${text}</span>`;
	return temp;
}

function selectChatUser(e) {
  e.preventDefault();

  let focus = this.el;
  chat.classList.add("change-user");
  collapseSelectedUser();

  let rs = getComputedStyle(root);
  let closingTime = rs
    .getPropertyValue("--chat-user-close-timing")
    .replace("s", "");

  // converting second to millisecond
  closingTime = closingTime * 1000;

  setTimeout(() => {
    if (focus.nextElementSibling)
      focus.nextElementSibling.classList.add("focus");
    else if (focus.previousElementSibling)
      focus.previousElementSibling.classList.add("focus");
    focus.remove();

    showSelectedUser(focus.dataset, () => {
      setTimeout(() => {
        chat.classList.remove("change-user");
      }, 100);
    });
  }, closingTime);
}

btnSendChat.addEventListener("click", function(e){
	e.preventDefault();
	
	chatSocket.send(createWebsocketMessage({message: inputChat.value, to: chatUser.dataset.currentUserId}, {op: 3})); 
	inputChat.value = "";
});

headerChat.addEventListener("click", function (e) {
  e.preventDefault();

  if (!userName) {
    userNameForm.classList.add("show");
    return;
  }

  headerChat.classList.remove("show");
  chatElement.classList.remove("hidden");
  setTimeout(() => {
    chatElement.classList.add("open");
    chatElement.classList.remove("close");
  }, 1200);
});

userNameBtn.addEventListener("click", function (e) {
  e.preventDefault();
  let input = document.querySelector("#chat-user-name");
  if (!input || input.value == undefined || input.value == "") return;
  userName = input.value;
  userNameForm.classList.remove("show");
	registerChatWebsocket();
	
});

chat.addEventListener("click", function (e) {
  if (e.target != chat) return;
  this.classList.toggle("close");
  this.classList.toggle("open");
});

chat.addEventListener("contextmenu", function (e) {
    if (e.target != chat) return;
    e.preventDefault();
    if (!chatContextMenu.opened) chatContextMenu.open(e.clientX, e.clientY);
    else chatContextMenu.close();
    return false;
  }, false);

users.addEventListener("wheel", function (e) {
  e.preventDefault();

  let currentFocus = users.querySelector(".focus");
  if (currentFocus == undefined) currentFocus = users.querySelector(".user");

  let nextFocus;
  if (e.wheelDelta > 0) {
    nextFocus = currentFocus.previousElementSibling;
  } else {
    nextFocus = currentFocus.nextElementSibling;
  }

  if (nextFocus == undefined) return;
  nextFocus.classList.add("focus");
  currentFocus.classList.remove("focus");
});

chat.querySelector(".user-name").addEventListener("click", function (e) {
  collapseSelectedUser();
});
