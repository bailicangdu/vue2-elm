import fetch from '../config/fetch'
import * as home from './tempdata/home'
import * as city from './tempdata/city'
import * as msite from './tempdata/msite'
import * as search from './tempdata/search'
import * as food from './tempdata/food'
import * as shop from './tempdata/shop'
import * as login from './tempdata/login'
import * as confirm from './tempdata/confirm'
import * as order from './tempdata/order'


// /**
//  * 获取首页默认地址
//  */

// export const cityGuess = () => fetch('GET', '/v1/cities', {
// 	type: 'guess'
// });


// /**
//  * 获取首页热门城市
//  */

// export const hotcity = () => fetch('GET', '/v1/cities', {
// 	type: 'hot'
// });


// /**
//  * 获取首页所有城市
//  */

// export const groupcity = () => fetch('GET', '/v1/cities', {
// 	type: 'group'
// });


// /**
//  * 获取当前所在城市
//  */

// export const currentcity = number => fetch('GET', '/v1/cities/' + number, {});


// /**
//  * 获取搜索地址
//  */

// export const searchplace = (cityid, value) => fetch('GET', '/v1/pois', {
// 	type: 'search',
// 	city_id: cityid,
// 	keyword: value
// });


// /**
//  * 获取msite页面地址信息
//  */

// export const msiteAdress = geohash => fetch('GET', '/v2/pois/' + geohash, {});


// /**
//  * 获取msite页面食品分类列表
//  */

// export const msiteFoodTypes = geohash => fetch('GET', '/v2/index_entry', {
// 	geohash,
// 	group_type: '1',
// 	'flags[]': 'F'
// });


// /**
//  * 获取msite商铺列表
//  */

// export const shopList = (latitude, longitude, offset, restaurant_category_id = '', restaurant_category_ids = '', order_by = '', delivery_mode = '', support_ids = []) => {
// 	let supportStr = '';
// 	support_ids.forEach(item => {
// 		if (item.status) {
// 			supportStr += '&support_ids[]=' + item.id;
// 		}
// 	});
// 	let data = {
// 		latitude,
// 		longitude,
// 		offset,
// 		limit: '20',
// 		'extras[]': 'activities',
// 		keyword: '',
// 		restaurant_category_id,
// 		'restaurant_category_ids[]': restaurant_category_ids,
// 		order_by,
// 		'delivery_mode[]': delivery_mode + supportStr
// 	};
// 	return fetch('GET', '/shopping/restaurants', data);
// };


// /**
//  * 获取search页面搜索结果
//  */

// export const searchRestaurant = (geohash, keyword) => fetch('GET', '/v4/restaurants', {
// 	'extras[]': 'restaurant_activity',
// 	geohash,
// 	keyword,
// 	type: 'search'
// });


// /**
//  * 获取food页面的 category 种类列表
//  */

// export const foodCategory = (latitude, longitude) => fetch('GET', '/shopping/v2/restaurant/category', {
// 	latitude,
// 	longitude
// });


// /**
//  * 获取food页面的配送方式
//  */

// export const foodDelivery = (latitude, longitude) => fetch('GET', '/shopping/v1/restaurants/delivery_modes', {
// 	latitude,
// 	longitude,
// 	kw: ''
// });


// /**
//  * 获取food页面的商家属性活动列表
//  */

// export const foodActivity = (latitude, longitude) => fetch('GET', '/shopping/v1/restaurants/activity_attributes', {
// 	latitude,
// 	longitude,
// 	kw: ''
// });


// /**
//  * 获取shop页面商铺详情
//  */

// export const shopDetails = (shopid, latitude, longitude) => fetch('GET', '/shopping/restaurant/' + shopid, {
// 	latitude,
// 	longitude: longitude + '&extras[]=activities&extras[]=album&extras[]=license&extras[]=identification&extras[]=statistics'
// });



// /**
//  * 获取food页面的商家属性活动列表
//  */

// export const foodMenu = restaurant_id => fetch('GET', '/shopping/v2/menu', {
// 	restaurant_id
// });


// /**
//  * 获取商铺评价列表
//  */

// export const getRatingList = (offset, tag_name = '') => fetch('GET', '/ugc/v2/restaurants/834828/ratings', {
// 	has_content: true,
// 	offset,
// 	limit: 10,
// 	tag_name
// });


// /**
//  * 获取商铺评价分数
//  */

// export const ratingScores = shopid => fetch('GET', '/ugc/v2/restaurants/' + shopid + '/ratings/scores', {});


// /**
//  * 获取商铺评价分类
//  */

// export const ratingTags = shopid => fetch('GET', '/ugc/v2/restaurants/' + shopid + '/ratings/tags', {});


// /**
//  * 获取短信验证码
//  */

// export const mobileCode = phone => fetch('POST', '/v4/mobile/verify_code/send', {
// 	mobile: phone,
// 	scene: 'login',
// 	type: 'sms'
// });


// /**
//  * 手机号登陆
//  */

// // export const sendLogin = (code, mobile, validate_token) => fetch('POST', '/v1/login/app_mobile', {
// // 	code,
// // 	mobile,
// // 	validate_token
// // });



// /**
//  * 获取用户信息
//  */

// export const getUser = () => fetch('GET', '/v1/user', {});



// /**
//  * 获取图片验证码
//  */

// export const getcaptchas = () => fetch('POST', '/v1/captchas', {});


// /**
//  * 账号密码登陆
//  */

// export const accountLogin = (username, password, captcha_code) => fetch('POST', '/v2/login', {username, password, captcha_code});


// /**
//  * 检测帐号是否存在
//  */

// export const checkExsis = (checkNumber, type) => fetch('GET', '/v1/users/exists', {
// 	[type]: checkNumber,
// 	type
// });


// /**
//  * 发送帐号
//  */

// export const sendMobile = (sendData, captcha_code, type, password) => fetch('POST', '/v1/mobile/verify_code/send', {
// 	action: "send",
// 	captcha_code,
// 	[type]: sendData,
// 	type: "sms",
// 	way: type,
// 	password,
// });


// /**
//  * 确认订单
//  */

// export const checkout = (geohash, entities) => fetch('POST', '/v1/carts/checkout', {
// 	come_from: "web",
// 	geohash,
// 	entities,
// });


// /**
//  * 获取快速备注列表
//  */

// export const getRemark = (id, sig) => fetch('GET', '/v1/carts/' + id + '/remarks', {
// 	sig
// });


// /**
//  * 获取地址列表
//  */

// export const getAddress = (id, sig) => fetch('GET', '/v1/carts/' + id + '/addresses', {
// 	sig
// });


// /**
//  * 搜索地址
//  */

// export const searchNearby = keyword => fetch('GET', '/v1/pois', {
// 	type: 'nearby',
// 	keyword
// });


// /**
//  * 添加地址
//  */

// export const postAddAddress = (userId, address, address_detail, geohash, name, phone, phone_bk, poi_type, sex, tag, tag_type) => fetch('POST', '/v1/users/' + userId + '/addresses', {
// 	address,
// 	address_detail,
// 	geohash,
// 	name,
// 	phone,
// 	phone_bk,
// 	poi_type,
// 	sex,
// 	tag,
// 	tag_type,
// });


// /**
//  * 下订单
//  */

// export const placeOrders = (user_id, cart_id, address_id, description, entities, geohash, sig) => fetch('POST', '/v1/users/' + user_id + '/carts/' + cart_id + '/orders', {
// 	address_id,
// 	come_from: "mobile_web",
// 	deliver_time: "",
// 	description,
// 	entities,
// 	geohash,
// 	paymethod_id: 1,
// 	sig,
// });


// /**
//  * 重新发送订单验证码
//  */

// export const rePostVerify = (cart_id, sig, type) => fetch('POST', '/v1/carts/' + cart_id + '/verify_code', {
// 	sig,
// 	type,
// });



// /**
//  * 下订单
//  */

// export const validateOrders = ({
// 	user_id,
// 	cart_id,
// 	address_id,
// 	description,
// 	entities,
// 	geohash,
// 	sig,
// 	validation_code,
// 	validation_token
// }) => fetch('POST', '/v1/users/' + user_id + '/carts/' + cart_id + '/orders', {
// 	address_id,
// 	come_from: "mobile_web",
// 	deliver_time: "",
// 	description,
// 	entities,
// 	geohash,
// 	paymethod_id: 1,
// 	sig,
// 	validation_code,
// 	validation_token,
// });


// /**
//  * 重新发送订单验证码
//  */

// export const payRequest = (merchantOrderNo, userId) => fetch('GET', '/payapi/payment/queryOrder', {
// 	merchantId: 5,
// 	merchantOrderNo,
// 	source: 'MOBILE_WAP',
// 	userId,
// 	version: '1.0.0',
// });


// /**
//  * 获取订单列表
//  */

// // export const getOrderList = (user_id, offset) => fetch('GET', '/bos/v2/users/' + user_id + '/orders', {
// // 	limit: 10,
// // 	offset,
// // });


// /**
//  * 获取订单详情
//  */

// // export const getOrderDetail = (user_id, orderid) => fetch('GET', '/bos/v1/users/' + user_id + '/orders/' + orderid + '/snapshot', {});



/**
 * 以下是临时数据
 */
const setpromise = data => {
	return new Promise((resolve, reject) => {
		resolve(data)
	})
}

export const cityGuess = () => setpromise(home.guesscity);
export const hotcity = () => setpromise(home.hotcity);
export const groupcity = () => setpromise(home.groupcity);
export const currentcity = number => setpromise(city.currentcity);
export const searchplace = (cityid, value) => setpromise(city.searchdata);
export const msiteAdress = geohash => setpromise(msite.msiteAdress);
export const msiteFoodTypes = geohash => setpromise(msite.foodTypes);
export const shopList = (latitude, longitude, offset) => setpromise(msite.shopList);
export const searchRestaurant = (geohash, keyword) => setpromise(search.searchData);
export const foodCategory = (latitude, longitude) => setpromise(food.category);
export const foodDelivery = (latitude, longitude) => setpromise(food.delivery);
export const foodActivity = (latitude, longitude) => setpromise(food.activity);
export const shopDetails = (shopid, latitude, longitude) => setpromise(shop.shopDetails);
export const foodMenu = restaurant_id => setpromise(shop.shopMenu);
export const getRatingList = (offset, tag_name = '') => setpromise(shop.ratingList);
export const ratingScores = shopid => setpromise(shop.scores);
export const ratingTags = shopid => setpromise(shop.tage);
export const mobileCode = phone => setpromise(login.validate_token);
export const accountLogin = (username, password, captcha_code) => setpromise(login.userInfo);
export const checkExsis = (checkNumber, type) => setpromise(login.checkExsis);
export const sendMobile = (sendData, captcha_code, type, password) => setpromise(login.send);
export const checkout = (geohash, entities) => setpromise(confirm.checkout);
export const getRemark = (id, sig) => setpromise(confirm.remark);
export const getAddress = (id, sig) => setpromise(confirm.addressList);
export const getUser = () => setpromise(login.userInfo);
export const getcaptchas = () => setpromise(login.cpatchs);
export const searchNearby = keyword => setpromise(confirm.searchAddress);
export const postAddAddress = (userId, address, address_detail, geohash, name, phone, phone_bk, poi_type, sex, tag, tag_type) => setpromise(confirm.addAddress);
export const placeOrders = (user_id, cart_id, address_id, description, entities, geohash, sig) => setpromise(confirm.palceOrder);
export const rePostVerify = (cart_id, sig, type) => setpromise(confirm.verfiyCode);
export const validateOrders = ({
	user_id,
	cart_id,
	address_id,
	description,
	entities,
	geohash,
	sig,
	validation_code,
	validation_token
}) => setpromise(confirm.orderSuccess);
export const payRequest = (merchantOrderNo, userId) => setpromise(confirm.payDetail);
export const sendLogin = (code, mobile, validate_token) => setpromise(login.userInfo);
export const getOrderList = (user_id, offset) => setpromise(order.orderList);
export const getOrderDetail = (user_id, orderid) => setpromise(order.orderDetail);
