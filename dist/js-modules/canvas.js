
var c;
var ctx;

function setCanvas(canvas, context){
	c = canvas;
	ctx = c.getContext(context);
}

function clearCanvas() {
	ctx.clearRect(0, 0, c.width, c.height);
}

function drawToCanvas({ fillStyle, position, width, height}){
	ctx.fillStyle = fillStyle;
	ctx.fillRect(position.x, position.y, width, height);
}

export { setCanvas, drawToCanvas, clearCanvas, ctx }