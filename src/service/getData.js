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
import * as service from './tempdata/service'
import * as addDetail from './tempdata/addDetail'
import * as addresspart from './tempdata/address'
import * as vip from './tempdata/vip'
import * as hongbao from './tempdata/hongbao'


let cityGuess;
let hotcity;
let groupcity;
let currentcity;
let searchplace;
let msiteAdress;
let msiteFoodTypes;
let shopList;
let searchRestaurant;
let foodCategory;
let foodDelivery;
let foodActivity;
let shopDetails;
let foodMenu;
let getRatingList;
let ratingScores;
let ratingTags;
let mobileCode;
let accountLogin;
let checkExsis;
let sendMobile;
let checkout;
let getRemark;
let getAddress;
let getcaptchas;
let searchNearby;
let postAddAddress;
let placeOrders;
let rePostVerify;
let validateOrders;
let payRequest;
let getService;
let vipCart;
let getHongbaoNum;
let getExpired;
let exChangeHongbao;
let getUser;
let sendLogin;
let getOrderList;
let getOrderDetail;
let getAddressList;
let getSearchAddress;


/**
 * 创建临时数据
 */
const setpromise = data => {
	return new Promise((resolve, reject) => {
		resolve(data)
	})
}

//编译环境使用真实数据
if (process.env.NODE_ENV == 'development') {

	/**
	 * 获取首页默认地址
	 */

	cityGuess = () => fetch('GET', '/v1/cities', {
		type: 'guess'
	});


	/**
	 * 获取首页热门城市
	 */

	hotcity = () => fetch('GET', '/v1/cities', {
		type: 'hot'
	});


	/**
	 * 获取首页所有城市
	 */

	groupcity = () => fetch('GET', '/v1/cities', {
		type: 'group'
	});


	/**
	 * 获取当前所在城市
	 */

	currentcity = number => fetch('GET', '/v1/cities/' + number, {});


	/**
	 * 获取搜索地址
	 */

	searchplace = (cityid, value) => fetch('GET', '/v1/pois', {
		type: 'search',
		city_id: cityid,
		keyword: value
	});


	/**
	 * 获取msite页面地址信息
	 */

	msiteAdress = geohash => fetch('GET', '/v2/pois/' + geohash, {});


	/**
	 * 获取msite页面食品分类列表
	 */

	msiteFoodTypes = geohash => fetch('GET', '/v2/index_entry', {
		geohash,
		group_type: '1',
		'flags[]': 'F'
	});


	/**
	 * 获取msite商铺列表
	 */

	shopList = (latitude, longitude, offset, restaurant_category_id = '', restaurant_category_ids = '', order_by = '', delivery_mode = '', support_ids = []) => {
		let supportStr = '';
		support_ids.forEach(item => {
			if (item.status) {
				supportStr += '&support_ids[]=' + item.id;
			}
		});
		let data = {
			latitude,
			longitude,
			offset,
			limit: '20',
			'extras[]': 'activities',
			keyword: '',
			restaurant_category_id,
			'restaurant_category_ids[]': restaurant_category_ids,
			order_by,
			'delivery_mode[]': delivery_mode + supportStr
		};
		return fetch('GET', '/shopping/restaurants', data);
	};


	/**
	 * 获取search页面搜索结果
	 */

	searchRestaurant = (geohash, keyword) => fetch('GET', '/v4/restaurants', {
		'extras[]': 'restaurant_activity',
		geohash,
		keyword,
		type: 'search'
	});


	/**
	 * 获取food页面的 category 种类列表
	 */

	foodCategory = (latitude, longitude) => fetch('GET', '/shopping/v2/restaurant/category', {
		latitude,
		longitude
	});


	/**
	 * 获取food页面的配送方式
	 */

	foodDelivery = (latitude, longitude) => fetch('GET', '/shopping/v1/restaurants/delivery_modes', {
		latitude,
		longitude,
		kw: ''
	});


	/**
	 * 获取food页面的商家属性活动列表
	 */

	foodActivity = (latitude, longitude) => fetch('GET', '/shopping/v1/restaurants/activity_attributes', {
		latitude,
		longitude,
		kw: ''
	});


	/**
	 * 获取shop页面商铺详情
	 */

	shopDetails = (shopid, latitude, longitude) => fetch('GET', '/shopping/restaurant/' + shopid, {
		latitude,
		longitude: longitude + '&extras[]=activities&extras[]=album&extras[]=license&extras[]=identification&extras[]=statistics'
	});



	/**
	 * 获取food页面的商家属性活动列表
	 */

	foodMenu = restaurant_id => fetch('GET', '/shopping/v2/menu', {
		restaurant_id
	});


	/**
	 * 获取商铺评价列表
	 */

	getRatingList = (offset, tag_name = '') => fetch('GET', '/ugc/v2/restaurants/834828/ratings', {
		has_content: true,
		offset,
		limit: 10,
		tag_name
	});


	/**
	 * 获取商铺评价分数
	 */

	ratingScores = shopid => fetch('GET', '/ugc/v2/restaurants/' + shopid + '/ratings/scores', {});


	/**
	 * 获取商铺评价分类
	 */

	ratingTags = shopid => fetch('GET', '/ugc/v2/restaurants/' + shopid + '/ratings/tags', {});


	/**
	 * 获取短信验证码
	 */

	mobileCode = phone => fetch('POST', '/v4/mobile/verify_code/send', {
		mobile: phone,
		scene: 'login',
		type: 'sms'
	});




	getcaptchas = () => fetch('POST', '/v1/captchas', {});


	/**
	 * 账号密码登陆
	 */

	accountLogin = (username, password, captcha_code) => fetch('POST', '/v2/login', {username, password, captcha_code});


	/**
	 * 检测帐号是否存在
	 */

	checkExsis = (checkNumber, type) => fetch('GET', '/v1/users/exists', {
		[type]: checkNumber,
		type
	});


	/**
	 * 发送帐号
	 */

	sendMobile = (sendData, captcha_code, type, password) => fetch('POST', '/v1/mobile/verify_code/send', {
		action: "send",
		captcha_code,
		[type]: sendData,
		type: "sms",
		way: type,
		password,
	});


	/**
	 * 确认订单
	 */

	checkout = (geohash, entities) => fetch('POST', '/v1/carts/checkout', {
		come_from: "web",
		geohash,
		entities,
	});


	/**
	 * 获取快速备注列表
	 */

	getRemark = (id, sig) => fetch('GET', '/v1/carts/' + id + '/remarks', {
		sig
	});


	/**
	 * 获取地址列表
	 */

	getAddress = (id, sig) => fetch('GET', '/v1/carts/' + id + '/addresses', {
		sig
	});


	/**
	 * 搜索地址
	 */

	searchNearby = keyword => fetch('GET', '/v1/pois', {
		type: 'nearby',
		keyword
	});


	/**
	 * 添加地址
	 */

	postAddAddress = (userId, address, address_detail, geohash, name, phone, phone_bk, poi_type, sex, tag, tag_type) => fetch('POST', '/v1/users/' + userId + '/addresses', {
		address,
		address_detail,
		geohash,
		name,
		phone,
		phone_bk,
		poi_type,
		sex,
		tag,
		tag_type,
	});


	/**
	 * 下订单
	 */

	placeOrders = (user_id, cart_id, address_id, description, entities, geohash, sig) => fetch('POST', '/v1/users/' + user_id + '/carts/' + cart_id + '/orders', {
		address_id,
		come_from: "mobile_web",
		deliver_time: "",
		description,
		entities,
		geohash,
		paymethod_id: 1,
		sig,
	});


	/**
	 * 重新发送订单验证码
	 */

	rePostVerify = (cart_id, sig, type) => fetch('POST', '/v1/carts/' + cart_id + '/verify_code', {
		sig,
		type,
	});



	/**
	 * 下订单
	 */

	validateOrders = ({
		user_id,
		cart_id,
		address_id,
		description,
		entities,
		geohash,
		sig,
		validation_code,
		validation_token
	}) => fetch('POST', '/v1/users/' + user_id + '/carts/' + cart_id + '/orders', {
		address_id,
		come_from: "mobile_web",
		deliver_time: "",
		description,
		entities,
		geohash,
		paymethod_id: 1,
		sig,
		validation_code,
		validation_token,
	});


	/**
	 * 重新发送订单验证码
	 */

	payRequest = (merchantOrderNo, userId) => fetch('GET', '/payapi/payment/queryOrder', {
		merchantId: 5,
		merchantOrderNo,
		source: 'MOBILE_WAP',
		userId,
		version: '1.0.0',
	});



	// /**
	//  * 获取服务中心信息
	//  */

	getService = () => fetch('GET', '/m.ele.me@json/profile/explain', {});



	/**
	*兑换会员卡
	*/
	vipCart= (id, number, password) => fetch('POST','/member/v1/users/' + id + '/delivery_card/physical_card/bind',{
		number,
		password
	})



	/**
	 * 获取红包数量
	*/

	getHongbaoNum= id => fetch('GET','/promotion/v2/users/' + id + '/hongbaos',{});



	/**
	 * 获取过期红包
	*/


	getExpired= id => fetch('GET','/promotion/v2/users/' + id + '/expired_hongbaos?limit=10&offset=0',{});


	/**
	 * 兑换红包
	*/

	exChangeHongbao= (id, exchange_code, captcha_code) => fetch('POST','/v1/users/' + id + '/hongbao/exchange',{
		exchange_code,
		captcha_code,
	});

	/**
	 * 获取用户信息
	 */

	getUser = () => fetch('GET', '/v1/user', {});



	// /**
	//  * 以下Api接口不需要或者不能进行反向代理
	//  */



	// /**
	//  * 手机号登陆
	//  */

	// sendLogin = (code, mobile, validate_token) => fetch('POST', '/v1/login/app_mobile', {
	// 	code,
	// 	mobile,
	// 	validate_token
	// });


	// /**
	//  * 获取订单列表
	//  */

	getOrderList = (user_id, offset) => fetch('GET', '/bos/v2/users/' + user_id + '/orders', {
		limit: 10,
		offset,
	});


	/**
	 * 获取订单详情
	 */

	getOrderDetail = (user_id, orderid) => fetch('GET', '/bos/v1/users/' + user_id + '/orders/' + orderid + '/snapshot', {});


	/**
	*个人中心里编辑地址
	*/
	getAddressList = (user_id) => fetch('GET', '/v1/users/'+user_id+'/addresses')

	/**
	*个人中心里搜索地址
	*/
	getSearchAddress=(keyword) => fetch('GET','v1/pois',{
		keyword:keyword,
		type:'nearby'
	})


}else{
	cityGuess = () => setpromise(home.guesscity);
	hotcity = () => setpromise(home.hotcity);
	groupcity = () => setpromise(home.groupcity);
	currentcity = number => setpromise(city.currentcity);
	searchplace = (cityid, value) => setpromise(city.searchdata);
	msiteAdress = geohash => setpromise(msite.msiteAdress);
	msiteFoodTypes = geohash => setpromise(msite.foodTypes);
	shopList = (latitude, longitude, offset) => setpromise(msite.shopList);
	searchRestaurant = (geohash, keyword) => setpromise(search.searchData);
	foodCategory = (latitude, longitude) => setpromise(food.category);
	foodDelivery = (latitude, longitude) => setpromise(food.delivery);
	foodActivity = (latitude, longitude) => setpromise(food.activity);
	shopDetails = (shopid, latitude, longitude) => setpromise(shop.shopDetails);
	foodMenu = restaurant_id => setpromise(shop.shopMenu);
	getRatingList = (offset, tag_name = '') => setpromise(shop.ratingList);
	ratingScores = shopid => setpromise(shop.scores);
	ratingTags = shopid => setpromise(shop.tage);
	mobileCode = phone => setpromise(login.validate_token);
	accountLogin = (username, password, captcha_code) => setpromise(login.userInfo);
	checkExsis = (checkNumber, type) => setpromise(login.checkExsis);
	sendMobile = (sendData, captcha_code, type, password) => setpromise(login.send);
	checkout = (geohash, entities) => setpromise(confirm.checkout);
	getRemark = (id, sig) => setpromise(confirm.remark);
	getAddress = (id, sig) => setpromise(confirm.addressList);
	getcaptchas = () => setpromise(login.cpatchs);
	searchNearby = keyword => setpromise(confirm.searchAddress);
	postAddAddress = (userId, address, address_detail, geohash, name, phone, phone_bk, poi_type, sex, tag, tag_type) => setpromise(confirm.addAddress);
	placeOrders = (user_id, cart_id, address_id, description, entities, geohash, sig) => setpromise(confirm.palceOrder);
	rePostVerify = (cart_id, sig, type) => setpromise(confirm.verfiyCode);
	validateOrders = ({
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
	payRequest = (merchantOrderNo, userId) => setpromise(confirm.payDetail);
	getService = () => setpromise(service.serviceData);
	vipCart= (id, number, password) => setpromise(vip.vipcart);
	getHongbaoNum= id => setpromise(hongbao.dataList);
	getExpired= id => setpromise(hongbao.expired);
	exChangeHongbao= (id, exchange_code, captcha_code) => setpromise(hongbao.exchange);
	getUser = () => setpromise(login.userInfo);
	getOrderList = (user_id, offset) => setpromise(order.orderList);
	getOrderDetail = (user_id, orderid) => setpromise(order.orderDetail);
	getAddressList = (user_id) => setpromise(addresspart.address);
	getSearchAddress=(keyword) => setpromise(addDetail.addData);
}


/**
 * 以下Api接口不需要进行反向代理
 */

sendLogin = (code, mobile, validate_token) => setpromise(login.userInfo);

export {cityGuess, hotcity, groupcity, currentcity, searchplace, msiteAdress, msiteFoodTypes,shopList, searchRestaurant, foodCategory, foodDelivery, foodActivity, shopDetails, foodMenu, getRatingList, ratingScores, ratingTags, mobileCode, accountLogin, checkExsis, sendMobile, checkout, getRemark, getAddress, getcaptchas, searchNearby, postAddAddress, placeOrders, rePostVerify, validateOrders, payRequest, getService, vipCart, getHongbaoNum, getExpired, exChangeHongbao, getUser, sendLogin, getOrderList, getOrderDetail, getAddressList, getSearchAddress, }
