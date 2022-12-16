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

export { ContextMenuItem, ContextMenu };