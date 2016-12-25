import {baseUrl} from './env'

export default (type = 'GET', url = '', data = {}) => {
	return new Promise((resolve, reject) => { //定义一个promise
		type = type.toUpperCase();
		url = baseUrl + url
		let requestObj = {
		  	method: type,
		  	headers: {
		      	'Accept': 'application/json',
      			'Content-Type': 'application/json'
		  	},
		  	mode: "no-cors",
		}

		if (type == 'GET') {
			let dataStr = ''; //数据拼接字符串
			Object.keys(data).forEach(key => {
				dataStr += key + '=' + data[key] + '&';
			})

			if (dataStr !== '') {
				dataStr = dataStr.substr(0, dataStr.lastIndexOf('&'));
				url = url + '?' + dataStr;
			}
		}else if (type == 'POST') {
			Object.defineProperty(requestObj, 'body', {
				value: JSON.stringify(data)
			})
		}else {
			reject('error type')
		}
		
		fetch(url, requestObj).then(res => {
		  	if (res.status == 200) {
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