import * as Canvas from "/dist/js-modules/canvas.js";

function checkIsInside(a, b){
	let xInside = (a.position.x + a.width >= b.position.x && a.position.x <= b.position.x + b.width);
	let yInside = (a.position.y + a.height >= b.position.y && a.position.y <= b.position.y + b.height);
	//console.log({a: a, b: b, xInside: xInside, yInisde: yInside });
	if(xInside && yInside){
		return true;
	}

	return false;
}

function checkIsEscaping(a, b){
	let xOuter = (a.position.x < b.position.x || a.position.x + a.width >= b.position.x + b.width);
	let yOuter = (a.position.y <= b.position.y || a.position.y + a.height >= b.position.y + b.height);
	//console.log({a: a, b: b, xOuter: xOuter, yOuter: yOuter });
	if(xOuter || yOuter){
		return true;
	}

	return false;
}

class Camera{
	constructor({startPosition, canvas}){
		this.position = startPosition;
		this.width = canvas.width;
		this.height = canvas.height;
		this.canvas = canvas;
	}
}

class World{
	constructor({ width, height, canvasId, canvas, cameraStartPos }){
		this.width = width;
		this.height = height;

		if(canvas == undefined)
			canvas = document.querySelector(canvasId);
		Canvas.setCanvas(canvas, "2d");
		canvas.addEventListener("click", (e) => { canvas.focus(); });

		if(!cameraStartPos)
			cameraStartPos = {x: 0, y: 0};
		
		this.camera = new Camera({
			startPosition: cameraStartPos, 
			canvas: canvas
		});
		
		this.players = [];
		this.characters = [];

		// world props
		this.background;
		this.forwardElements = [];
		this.backwardElements = [];
		this.relativeElements = [];

		this.rules = {
			gravity: 10,
		};

		this.hasGravity = false;

		this.gameFPS = 0;
	}
	
	addPlayer({ id, position, controller, commands, fillStyle, hasCollision, onkeyup, onkeydown }){
		if(!id) id = this.players.length;
		
		this.players.push(new Player({
			controller: controller, 
			onkeyup: onkeyup,
			onkeydown: onkeydown,
			char: {
				id: id, 
				commands: commands, 
				gameObject: {
					startPosition: position,
					world: this, 
					fillStyle: fillStyle,
					isDrawable: true,
					hasCollision: hasCollision,
				}
			}
		}));
	}
	removePlayer(){
		
	}
	
	addCharacter({ id, position, commands, fillStyle, hasCollision }){
		if(!id) id = this.characters.length;
		
		this.characters.push(new Character({
			id: id, 
			commands: commands, 
			gameObject: {
				fillStyle: fillStyle,
				startPosition: position,
				world: this, 
				isDrawable: true,
				hasCollision: hasCollision,
			}
		}));
	}
	getCharacterById(id){
		return this.characters.find(x => x.id == id);
	}
	removeCharacter(){
		
	}

	update(){
		let toUpdate = [
			...this.players,
			...this.characters,
		];
		for(let i = 0; i < toUpdate.length; i++){
			toUpdate[i].update();
		}
	}
	
	draw() {
		let sorter = (a,b) => {
			if(a.zIndex == b.zIndex){
				if(a.correctIndex)
					return a.correctIndex(b);
			}
			return a.zIndex - b.zIndex;
		};
		
		let drawableRelative = [
			...this.characters.filter(x => x.needDraw()),
			...this.players.filter(x => x.needDraw()),
			...this.relativeElements.filter(x => x.needDraw()),
		].sort(sorter);

		let drawableBack = this.backwardElements.sort(sorter);
		let drawableFront = this.forwardElements.sort(sorter);

		let drawable = [
			...drawableBack,
			...drawableRelative,
			...drawableFront,
		];
		//console.log(drawable);
		
		Canvas.clearCanvas();

		// draw background 
		Canvas.drawToCanvas({ fillStyle: "#9d9d9d", position: { x: 0, y: 0}, width: this.camera.width, height: this.camera.height});
		
		for(let i = 0; i < drawable.length; i++){
			drawable[i].draw();
		}
		
	}
	
}

class Animation{
	// All animation is placed relative to gameObject position
	// image_ is relative to sprite sheet image
	// anim_ is relative to what will be shown
	// animOffset(x, y) is used to change position of the sprite relative to gameObject
	constructor({ image, imageId, imagePos, imageWidth, imageHeight, animWidth, animHeight, animOffset }){
		
	}
	
}

class GameObject{
	constructor({ startPosition, correctIndex, world, fillStyle, isDrawable, hasCollision, animations }){
		this.world = world;
		
		this.height = 50;
		this.width = 25;

		this.hasCollision = hasCollision;
		
		this.gravityAffect = true;
		
		this.zIndex = 0;
		this.correctIndex = correctIndex;
		
		if(!startPosition)
			startPosition = { x: 0, y: 0 };
		this.position = startPosition;

		this.lastPos = {x:0, y:0};
		this.lastPos.x = this.position.x;
		this.lastPos.y = this.position.y;
		
		this.rules = {};

		// to draw
		this.drawable = isDrawable;
		
		this.fillStyle = fillStyle;

		if(!animations)
			animations = {};
		
		this.animations = animations;

		this.currentAnimation = '';
		this.nextAnimation = '';
		this.frameIndex = 0;
	}

	update(){
		this.gravityEffect();
		this.manteinInsideWorld();

		if(this.position.x != this.lastPos.x){
			this.actions.moved = true;
			this.actions.lastMoved = 'x';
		}
		
		if(this.position.y != this.lastPos.y){
			this.actions.moved = true;
			this.actions.lastMoved = 'y';
		}
		
		this.customObjectUpdate();
		
		this.lastPos.x = this.position.x;
		this.lastPos.y = this.position.y;

		this.actions.moved = false;
	}

	customObjectUpdate(){console.log("gameObj");}
	
	needDraw(){
		if(this.drawable){
			if(checkIsInside(this, this.world.camera))
				return true;
		}
		return false;
	}
	
	draw(){
		Canvas.drawToCanvas({ fillStyle: this.fillStyle, position: this.position, width: this.width, height: this.height});
	}

	gravityEffect(){
		if(this.world.hasGravity){
				if(this.gravityAffect){
					this.position.y += this.world.rules.gravity;
					this.actions.moved = true;
				}
		}
	}
	
	manteinInsideWorld(){
		let worldPos = { position: { x: 0, y: 0 }, width: this.world.width, height: this.world.height };
		if(checkIsEscaping(this, worldPos)) {
			//console.log(worldPos.width - (this.position.x + this.width));
			let offSetX = worldPos.width - (this.position.x + this.width);
			let offSetY = worldPos.height - (this.position.y + this.height);
			if(offSetY < 0){
				this.position.y += offSetY;
			}
			if(offSetY > 0 && this.position.y < 0){
				this.position.y += (-1 * this.position.y);
			}
			if(offSetX < 0){
				this.position.x += offSetX;
			}
			if(offSetX > 0 && this.position.x < 0){
				this.position.x += (-1 * this.position.x);
			}
		}
		
	}
	
}

class Character extends GameObject{
	constructor({ id, commands, gameObject }){
		super(gameObject);
		
		if(!commands)
			commands = {};
		this.id = id;
		
		this.commands = commands;

		this.actions = {};
				
		this.height = 50;
		this.width = 25;

		this.rules.velocity = {x: 5, y: 10 };
		
	}

	update(){
		let commandsName = Object.getOwnPropertyNames(this.commands);
		for(let i = 0; i < commandsName.length; i ++){
			let command = this.commands[commandsName[i]];
			if(!command){
				continue;
			}
			if(command.validate(this)){
				command.action(this);
			}
		}
		
		super.update();
	}

	customObjectUpdate(){
		if(this.actions.moved){
			 let objects = [
				 ...this.world.forwardElements.filter(x => x.hasCollision), 
				 ...this.world.backwardElements.filter(x => x.hasCollision), 
				 ...this.world.relativeElements.filter(x => x.hasCollision),
				 ...this.world.characters.filter(x => x.hasCollision),
				 ...this.world.players.filter(x => x.hasCollision),
			 ];

			let index = objects.indexOf(this);
			if(index == -1)
				return;
			objects.splice(index, 1);
			//console.log(objects);
			
			for(let i = 0; i < objects.length; i++){
				let obj = objects[i];
				if(checkIsInside(this, obj)){
					console.log(`Colliding with a ${obj.constructor.name}`);
					// calc dif of distance to push object to one side
					let posDifX = this.position.x + this.width - obj.position.x;
					let posDifY = this.position.y + this.height - obj.position.y;
					
					//if(posDifX > posDifY){
					if(this.actions.lastMoved == 'x'){
						if(posDifX < obj.width / 2){
							this.position.x = this.position.x - posDifX;
						}
						else{
							this.position.x = this.position.x + (obj.width - (posDifX - this.width));
						}
					}
					
					if(posDifX < posDifY){
						if(posDifY < obj.height / 2){
							this.position.y = this.position.y - posDifY;
						}
						else{
							this.position.y = this.position.y + (obj.height - (posDifY - this.height));
						}
					}
					
				}
			}

		}
	}
	
}

class Player extends Character{
	constructor({ controller, char, onkeyup, onkeydown }){
		super(char);
		this.controller = controller;
		this.registerCommands();

		this.onkeyup = onkeyup;
		this.onkeydown = onkeydown;
		
		this.zIndex = -1;
	}
	
	registerCommands(){
		let p = this;
		p.world.camera.canvas.addEventListener("keydown", (e) => {
			let key = e.key.toUpperCase();
			let cmd = p.controller.find((x) => { return x.key == key });
			if(!cmd)
				return;
			p.actions[cmd.actionName] = true;
			if(p.onkeydown)
				p.onkeydown(p, cmd.actionName);
		});
		p.world.camera.canvas.addEventListener("keyup", (e) => {
			let key = e.key.toUpperCase();
			let cmd = p.controller.find((x) => { return x.key == key });
			if(!cmd)
				return;
			p.actions[cmd.actionName] = false;
			if(p.onkeyup)
				p.onkeyup(p, cmd.actionName);
		});
	}

	update(){
		super.update();
	}
	
}

export { Camera, World, Animation, GameObject, Character, Player, checkIsInside, checkIsEscaping }