function showBackdrop() {
	let uiBackdrop = document.getElementsByClassName("ui-backdrop")[0];
	console.log(uiBackdrop);
	uiBackdrop.classList.add("fade");
	uiBackdrop.classList.add("in");
}

function hideBackdrop() {
	let uiBackdrop = document.getElementsByClassName("ui-backdrop")[0];
	console.log(uiBackdrop);
	uiBackdrop.classList.remove("fade");
	uiBackdrop.classList.remove("in");
}