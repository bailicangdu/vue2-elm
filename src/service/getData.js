import fetch from '../config/fetch'

/**
 * 获取首页默认地址
 */
const cityGuess = () => fetch('GET', '/v1/cities', {type: 'guess'})


export {cityGuess}