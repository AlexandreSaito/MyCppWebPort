console.clear();

const root = document.querySelector(":root");
var contentWrapper = document.querySelector(".content-wrapper");
const openSidebar = document.querySelector(".open-sidebar");
const sidebarItemDdl = document.querySelectorAll(".sidebar-item-ddl");
const headerLinks = document.querySelectorAll(".header-item");
const mainContent = document.querySelector("#main-content");
const sidebarLeft = document.querySelector(".sidebar.left-sidebar");
var scriptWrapper;

const domController = {
  events: [],
  register: function (query, func) {
    this.events.push({ query: query, action: func });
  },
  unbind: function (query) {
    this.events = this.events.filter((x) => x.query != query);
  }
};

domController.register("h3", function (e) {
  console.log(this.el.innerHTML);
});

domController.register(`a[href]`, function (e) {
  e.preventDefault();
  let url = this.el.getAttribute("href");
  if (!url || url == "#") return;
  loadPage(url);
});

domController.register(`.sidebar-item-ddl`, function(e) {
	e.preventDefault();
	let el = this.el;
	
  el.classList.toggle("open");
  let listElement = el.parentElement.querySelector(".item-list");
	let height = "0px";
	if (el.classList.contains("open"))
		height = `${listElement.scrollHeight + 10}px`;
	listElement.style.height = height;
});

document.addEventListener("click", function (e) {
  let event = domController.events.find((x) => e.target.matches(x.query));
  if (event) {
    event.el = e.target;
    event.action(e);
  }
});

document.addEventListener("DOMContentLoaded", function (e) {
  resizeContentWrapper();
	scriptWrapper = document.querySelector("#script-wrapper");
});

window.addEventListener("resize", (event) => {
  resizeContentWrapper();
});

openSidebar.addEventListener("click", function (e) {
  e.preventDefault();

  let moreBtn = this.querySelector(".more-button");
  let sidebarWidth = "0px";
  moreBtn.classList.toggle("active");
  if (moreBtn.classList.contains("active")) sidebarWidth = "150px";
  root.style.setProperty("--left-sidebar-width", sidebarWidth);
});

showMoreButton();

// Context Menu
const contextMenuElement = document.querySelector("#contextMenu");

class ContextMenuItem {
  constructor({ name, classList, itens, action, onOpen }) {
    this.name = name;
    this.classList = classList;
    this.itens = itens;
    this.action = action;
    this.onOpen = onOpen;
  }
}

class ContextMenu {
  constructor(title) {
    this.title = title;
    this.opened = false;
    this.itens = [];
  }
  registerItem(contextMenuItem) {
    this.itens.push(contextMenuItem);
  }
  open(posX, posY) {
    let listElement = contextMenu.querySelector(".itens");
    listElement.innerHTML = "";

    if (contextMenuElement.classList.contains("open")) {
      contextMenuElement.classList.remove("open");
      contextMenuElement.classList.add("close");
    }

    if (this.itens.lenght == 0) return;

    contextMenu.querySelector(".title").innerHTML = this.title;

    this.createItemsElements(listElement, this.itens);

    contextMenuElement.classList.remove("close");
    contextMenuElement.classList.add("open");
    this.calculatePosition(contextMenuElement, posX, posY);
    this.opened = true;
  }
  close() {
    contextMenuElement.classList.remove("open");
    contextMenuElement.classList.add("close");
    this.opened = false;
  }
  createItemsElements(parent, list) {
    list.forEach((x) => {
      let item = document.createElement("li");
      let name = document.createElement("span");
      name.classList.add("item-name");
      name.innerText = x.name;
      item.append(name);
      if (x.itens) {
        item.classList.add("item-list");
        let listElement = document.createElement("ul");
        listElement.classList.add("itens");
        this.createItemsElements(listElement, x.itens);
        item.append(listElement);
        let obj = this;
        item.addEventListener("click", function (e) {
          e.preventDefault();

          let itemListElement = this.querySelector(".itens");
          this.classList.toggle("open");
          let cRect = this.getBoundingClientRect();
          let x = cRect.x + this.offsetHeight;
          obj.calculatePosition(itemListElement, cRect.x, cRect.y);
        });
      } else {
        item.classList.add("item");
        item.addEventListener("click", x.action);
        item.addEventListener("click", () => {
          this.close();
        });
      }
      parent.append(item);
      if (x.onOpen) x.onOpen(item);
    });
  }
  calculatePosition(element, posX, posY) {
    if (window.innerWidth < posX + element.offsetWidth)
      posX = `calc(100% - ${window.innerWidth - posX + element.offsetWidth}px)`;
    else posX = posX + "px";
    if (window.innerHeight < posY + element.offsetHeight)
      posY = `calc(100% - ${
        window.innerHeight - posY + element.offsetHeight
      }px)`;
    else posY = posY + "px";

    element.style.left = posX;
    element.style.top = posY;
  }
}

// Socket Handler
function createWebsocketMessage(message, options){
	let jsonObject = {message: message};
	if(options){
		if(options.reconnect)
			jsonObject.reconnect = options.reconnect;
		if(options.token)
			jsonObject.token = options.token;	
		if(options.op)
			jsonObject.op = options.op;	
	}
	return JSON.stringify(jsonObject);
}

// Functions
function loadPage(url, jsonData) {
  // async load
  let searchUrl = url;
  if (url.includes("?")) url += "&partial=true";
  else url += "?partial=true";

  contentWrapper.classList.add("dropping");

	var myHeaders = new Headers();
	myHeaders.append('Content-Type', 'text/html');

	var myInit = { method: 'GET',
               headers: myHeaders,
               mode: 'cors',
               cache: 'default' };
	fetch(url, myInit).then(function(response) {
	  return response.blob();
	}).then(function(blob) {
  	return blob.text();
	}).then(function(text){
    // parse HTML string to HTML DOM
    let wrapper = document.createElement("wrapper");
		wrapper.innerHTML = text;
		
    // clear Contents
    //mainContent.innerHTML = "";
    sidebarLeft.innerHTML = "";
		scriptWrapper.innerHTML = "";
		
    let currContentWrapper = document.createElement("div");
    currContentWrapper.classList.add("content-wrapper");
		
    let elements = wrapper.querySelectorAll("wrapper > *:not(#left-sidebar):not(script)");
    let leftSidebar = wrapper.querySelector("#left-sidebar");
    let scripts = wrapper.querySelectorAll("script");

		// add HTML elements to body
    for (let i = 0; i < elements.length; i++) currContentWrapper.append(elements[i]);

    mainContent.append(currContentWrapper);
    currContentWrapper.style.minHeight = contentWrapper.style.minHeight;
		
		// add script to body
    for (let i = 0; i < scripts.length; i++) {
			let script = document.createElement("script");
			script.setAttribute("type", scripts[i].getAttribute("type"));
			script.textContent  = scripts[i].innerHTML;
			scriptWrapper.append(script);
		}

		// add left side bar to body
    let showLSidebar = leftSidebar != null && leftSidebar != undefined;
    if (showLSidebar) {
      let lElements = leftSidebar.querySelectorAll("#left-sidebar > *");
      for (let i = 0; i < lElements.length; i++)
        sidebarLeft.append(lElements[i]);
    }

		// show More button if have left sidebar
    if (showLSidebar) showMoreButton();
    else hideMoreButton();

    history.pushState({ wrapperHTML: currContentWrapper.innerHTML }, "", searchUrl);
		setTimeout(()=>{
			let oldWrapper = contentWrapper;
      contentWrapper = currContentWrapper;
      contentWrapper.classList.add("fit");
			setTimeout(()=>{
				contentWrapper.classList.remove("fit");
				oldWrapper.remove();
      }, 480);
		}, 10);   
	});
}

function showMoreButton() {
  openSidebar.classList.add("in");
  openSidebar.classList.add("show");
  openSidebar.classList.remove("hide");
  setTimeout(() => {
    openSidebar.classList.remove("in");
  }, 300);
}

function hideMoreButton() {
  let moreBtn = openSidebar.querySelector(".more-button");
  if (moreBtn.classList.contains("active")) openSidebar.click();
  openSidebar.classList.add("in");
  openSidebar.classList.remove("show");
  openSidebar.classList.add("hide");
  setTimeout(() => {
    openSidebar.classList.remove("in");
  }, 310);
}

function resizeContentWrapper() {
  let clientHeight = window.innerHeight;
  let rs = getComputedStyle(root);
  let headerHeight = 0;
  let footerHeight = rs.getPropertyValue("--footer-height").replace("px", "");
  contentWrapper.style.minHeight = `${
    clientHeight - headerHeight - footerHeight
  }px`;
  mainContent.style.minHeight = `${clientHeight}px`;
}

function enterDoSubmit(inputSelector, buttonSelector, validate, validateFunc){
	let btn =	document.querySelector(buttonSelector);
	document.querySelector(inputSelector).addEventListener("keydown", function(e){
		if(e.key != "Enter")
			return;
		e.preventDefault();
		if(validate){
			if(!validateFunc)
				if(this.value == "")
					return;
			else
				if(!validateFunc(this.value))
					return;
		}
		
		btn.click();
	});
}
