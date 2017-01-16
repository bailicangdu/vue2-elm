import fetch from '../config/fetch'
import * as home from './tempdata/home'
import * as city from './tempdata/city'
import * as msite from './tempdata/msite'

/**
 * 获取首页默认地址
 */
const cityGuess = () => fetch('GET', '/v1/cities', {type: 'guess'})
/**
 * 获取首页热门城市
 */
const hotcity = () => fetch('GET', '/v1/cities', {type: 'hot'})
/**
 * 获取首页所有城市
 */
const groupcity = () => fetch('GET', '/v1/cities', {type: 'group'})
/**
 * 获取当前所在城市
 */
const currentcity = number => fetch('GET', '/v1/cities/' + number, {})
 /**
 * 获取搜索地址
 */
const searchplace = (cityid, value) => fetch('GET', '/v1/pois', {type: 'search', city_id: cityid, keyword: value})
/**
 * 获取msite页面地址信息
 */
//const msiteAdress = geohash => fetch('GET', '/v2/pois/' + geohash, {})
/**
 * 获取msite页面食品分类列表
 */
//const msiteFoodTypes = geohash => fetch('GET', '/v2/index_entry', {geohash, group_type:'1', 'flags[]':'F'})
/**
 * 获取msite商铺列表
 */
//const msiteShopList = (latitude, longitude, offset) => fetch('GET', '/shopping/restaurants', {latitude, longitude, offset, limit: '20', 'extras[]':'activities'})



//临时数据
const setpromise = data => {
	return new Promise((resolve, reject) => {
		resolve(data)
	})
}

// const cityGuess = () => setpromise(home.guesscity)
// const hotcity = () => setpromise(home.hotcity)
// const groupcity = () => setpromise(home.groupcity)
// const currentcity = number => setpromise(city.currentcity)
// const searchplace = (cityid, value) => setpromise(city.searchdata)
const msiteAdress = geohash => setpromise(msite.msiteAdress)
const msiteFoodTypes = geohash => setpromise(msite.foodTypes)
const msiteShopList = (latitude, longitude, offset) => setpromise(msite.shopList)


export {cityGuess, hotcity, groupcity, currentcity, searchplace, msiteAdress, msiteFoodTypes, msiteShopList}