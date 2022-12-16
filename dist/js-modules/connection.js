
export function createWebsocketMessage(message, options){
	let jsonObject = {message: message};
	if(options){
		if(options.reconnect)
			jsonObject.reconnect = options.reconnect;
		if(options.token)
			jsonObject.token = options.token;	
		if(options.op)
			jsonObject.op = options.op;	
	}
	return JSON.stringify(jsonObject) + "[ENDMESSAGE]";
}

export function fetchText({url, jsonData, method, headers, beforeSend, onDone}){
	
	var myHeaders = new Headers();
	if(headers){
		for(let i = 0; i < headers.length; i++){
			myHeaders.append(headers[i].header, headers[i].value);
		}
	}

	var myInit = { method: method,
               headers: myHeaders,
               mode: 'cors',
               cache: 'default' };
	
	if(beforeSend)
		beforeSend();
	
	fetch(url, myInit).then(function(r) {
		return r.blob();
	}).then(function(blob) {
  	return blob.text();
	}).then(function(text) {
		if(onDone)
			onDone(text);
	});
}
