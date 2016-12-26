import fetch from '../config/fetch'

export default (type, url, data) => {
	return new Promise((resolve, reject) => {
		fetch(type, url, data).then(res => {
			resolve(res)
		}).catch(err => {
			reject(err)
		})
	})
}


