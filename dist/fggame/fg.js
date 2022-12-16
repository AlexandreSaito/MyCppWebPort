class World{
	constructor({width, height}){
		this.width = width;
		this.height = height;
	}
}

class Character{
	constructor({controller, object}){
		this.controller = {	
			up: false,
			down: false,
			right: false,
			left: false,
		};
		this.self = object;
		this.width = 50;
		this.height = 50;

		this.world = undefined;
	}

	move(){
		let pLeft = Number.parseInt(this.self.style.left.replace("px", ""));
		let pTop = Number.parseInt(this.self.style.top.replace("px", ""));

		//console.log(this.controller);
		//console.log(gameRules);
		//console.log({pLeft: pLeft, pTop: pTop });
		if(this.controller.right && !this.controller.left){
			if(pLeft + gameRules.velocity + this.width > world.width){
				return;
			}
			this.self.style.left = `${pLeft + gameRules.velocity}px`;
		}
		if(this.controller.left && !this.controller.right){
			if(pLeft - gameRules.velocity < 0){
				return;
			}
			this.self.style.left = `${pLeft - gameRules.velocity}px`;
		}
		if(this.controller.up && !this.controller.down){
			if(pTop - gameRules.velocity < 0){
				return;
			}
			this.self.style.top = `${pTop - gameRules.velocity}px`;
		}
		if(this.controller.down && !this.controller.up){
			if(pTop + gameRules.velocity  + this.height > world.height){
				return;
			}
			this.self.style.top = `${pTop + gameRules.velocity}px`;
		}
	}
}

class Player extends Character{
	constructor(params){
		super(params);
		this.registerCommands();
	}

	registerCommands(){		
		this.self.addEventListener("keydown", (e) => {
			switch(e.key.toUpperCase()){
				case playerKeys.up:
					this.controller.up = true;
					break;
				case playerKeys.down:
					this.controller.down = true;
					break;
				case playerKeys.right:
					this.controller.right = true;
					break;
				case playerKeys.left:
					this.controller.left = true;
					break;
			}
		});
		
		this.self.addEventListener("keyup", (e) => {
			switch(e.key.toUpperCase()){
				case playerKeys.up:
					this.controller.up = false;
					break;
				case playerKeys.down:
					this.controller.down = false;
					break;
				case playerKeys.right:
					this.controller.right = false;
					break;
				case playerKeys.left:
					this.controller.left = false;
					break;
			}
		});
	}
	
}

var gs = new WebSocket("wss://MyCppWebPort.alexandres18.repl.co/ws-connection/game"); // game socket

const player = document.querySelector("#player");
const content = document.querySelector(".content");
const world = new World({width: content.clientWidth, height:content.clientHeight});

const playerKeys = {
	up: 'W',
	down: 'S',
	right: 'D',
	left: 'A',
};

const gameRules = {
	velocity: 10,
};

const playerObj = new Player({object: player});
playerObj.world = world;
const characters = [];

console.log(playerObj);

export function gameLoop(){
	console.log("loop!");
	playerObj.move();
	for(var i = 0; i < characters.length; i++){
		characters[i].move();
	}
}
