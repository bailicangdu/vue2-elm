import setPromise from './setPromise'

/**
 * 获取首页默认地址
 */
const cityGuess = (type = 'GET', url = '/v1/cities', data = {type: 'guess'}) => {
	return setPromise(type, url, data)
}

export {cityGuess}