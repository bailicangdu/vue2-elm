let baseUrl = ''

export default (type='GET', url='', data={}) => {
	return new Promise((resolve, reject) => { //定义一个promise
		type = type.toUpperCase();

		let dataStr = ''; //数据拼接字符串
		Object.keys(data).forEach( key => {
			dataStr += key + '=' + data[key] + '&';
		})
		dataStr = dataStr.substr(0, dataStr.lastIndexOf('&'));

		if (dataStr !== '') {
			url = baseUrl + url + '?' + dataStr;
		}else {
			url = baseUrl + url
		}
		
		fetch(url, {
		  	method: type,
		  	headers: {
		      	'Accept': 'application/json',
      			'Content-Type': 'application/json'
		  	},
		  	mode: "no-cors",
		  	// body: JSON.stringify({
		   //    	content: "留言内容"
		  	// })
		}).then(res => {
		  	if (res.status === 200) {
		      	return res.json()
		  	} else {
		      	return reject(res)
		  	}
		}).then(data => {
		  	resolve(data);
		}).catch(err => {
		  	reject(err);
		});
	})
}