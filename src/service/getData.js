import fetch from '../config/fetch'
import * as home from './tempdata/home'
/**
 * 获取首页默认地址
 */
// const cityGuess = () => fetch('GET', '/v1/cities', {type: 'guess'})
// const hotcity = () => fetch('GET', '/v1/cities', {type: 'hot'})
// const groupcity = () => fetch('GET', '/v1/cities', {type: 'group'})






//临时数据
const setpromise = data => {
	return new Promise((resolve, reject) => {
		resolve(data)
	})
}

const cityGuess = () => setpromise(home.guesscity)
const hotcity = () => setpromise(home.hotcity)
const groupcity = () => setpromise(home.groupcity)


export {cityGuess, hotcity, groupcity}