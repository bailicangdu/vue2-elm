import {
	RECORD_ADDRESS,
	ADD_CART,
	REDUCE_CART,
	INIT_BUYCART,
	CLEAR_CART,
	RECORD_SHOPDETAIL,
	RECORD_USERINFO,
	GET_USERINFO,
	CONFIRM_REMARK,
	CONFIRM_INVOICE,
	CHOOSE_SEARCH_ADDRESS,
	SAVE_GEOHASH,
	CHOOSE_ADDRESS,
	NEED_VALIDATION,
	SAVE_CART_ID_SIG,
	SAVE_ORDER_PARAM,
	CHANGE_ORDER_PARAM,
	ORDER_SUCCESS,
	SAVE_SHOPID,
	SAVE_ORDER,
	OUT_LOGIN,
	RETSET_NAME,
	SAVE_AVANDER,
	SAVE_ADDRESS,
} from './mutation-types.js'
import {
	setStore,
	getStore,
} from '../config/mUtils'

export default {
	// 记录当前经度纬度
	[RECORD_ADDRESS](state, {
		latitude,
		longitude
	}) {
		state.latitude = latitude;
		state.longitude = longitude;
	},

	[RECORD_SHOPDETAIL](state, detail) {
		state.shopDetail = detail;
	},
	// 加入购物车
	[ADD_CART](state, {
		shopid,
		category_id,
		item_id,
		food_id,
		name,
		price,
		specs,
		packing_fee,
		sku_id,
		stock
	}) {
		let cart = state.cartList;
		if (cart[shopid] && cart[shopid][category_id] && cart[shopid][category_id][item_id] && cart[shopid][category_id][item_id][food_id]) {
			cart[shopid][category_id][item_id][food_id]['num']++;
		} else if (cart[shopid] && cart[shopid][category_id] && cart[shopid][category_id][item_id]) {
			cart[shopid][category_id][item_id][food_id] = {};
			cart[shopid][category_id][item_id][food_id]['num'] = 1;
			cart[shopid][category_id][item_id][food_id]['id'] = food_id;
			cart[shopid][category_id][item_id][food_id]['name'] = name;
			cart[shopid][category_id][item_id][food_id]['price'] = price;
			cart[shopid][category_id][item_id][food_id]['specs'] = specs;
			cart[shopid][category_id][item_id][food_id]['packing_fee'] = packing_fee;
			cart[shopid][category_id][item_id][food_id]['sku_id'] = sku_id;
			cart[shopid][category_id][item_id][food_id]['stock'] = stock;
		} else if (cart[shopid] && cart[shopid][category_id]) {
			cart[shopid][category_id][item_id] = {};
			cart[shopid][category_id][item_id][food_id] = {};
			cart[shopid][category_id][item_id][food_id]['num'] = 1;
			cart[shopid][category_id][item_id][food_id]['id'] = food_id;
			cart[shopid][category_id][item_id][food_id]['name'] = name;
			cart[shopid][category_id][item_id][food_id]['price'] = price;
			cart[shopid][category_id][item_id][food_id]['specs'] = specs;
			cart[shopid][category_id][item_id][food_id]['packing_fee'] = packing_fee;
			cart[shopid][category_id][item_id][food_id]['sku_id'] = sku_id;
			cart[shopid][category_id][item_id][food_id]['stock'] = stock;
		} else if (cart[shopid]) {
			cart[shopid][category_id] = {};
			cart[shopid][category_id][item_id] = {};
			cart[shopid][category_id][item_id][food_id] = {};
			cart[shopid][category_id][item_id][food_id]['num'] = 1;
			cart[shopid][category_id][item_id][food_id]['id'] = food_id;
			cart[shopid][category_id][item_id][food_id]['name'] = name;
			cart[shopid][category_id][item_id][food_id]['price'] = price;
			cart[shopid][category_id][item_id][food_id]['specs'] = specs;
			cart[shopid][category_id][item_id][food_id]['packing_fee'] = packing_fee;
			cart[shopid][category_id][item_id][food_id]['sku_id'] = sku_id;
			cart[shopid][category_id][item_id][food_id]['stock'] = stock;
		} else {
			cart[shopid] = {};
			cart[shopid][category_id] = {};
			cart[shopid][category_id][item_id] = {};
			cart[shopid][category_id][item_id][food_id] = {};
			cart[shopid][category_id][item_id][food_id]['num'] = 1;
			cart[shopid][category_id][item_id][food_id]['id'] = food_id;
			cart[shopid][category_id][item_id][food_id]['name'] = name;
			cart[shopid][category_id][item_id][food_id]['price'] = price;
			cart[shopid][category_id][item_id][food_id]['specs'] = specs;
			cart[shopid][category_id][item_id][food_id]['packing_fee'] = packing_fee;
			cart[shopid][category_id][item_id][food_id]['sku_id'] = sku_id;
			cart[shopid][category_id][item_id][food_id]['stock'] = stock;
		}
		//返回一个新的对象，否则计算属性无法监听到数据的变化
		state.cartList = Object.assign({}, cart);
		//存入localStorage
		setStore('buyCart', state.cartList);
	},
	// 移出购物车
	[REDUCE_CART](state, {
		shopid,
		category_id,
		item_id,
		food_id,
		name,
		price,
		specs,
	}) {
		let cart = state.cartList;
		if (cart[shopid] && cart[shopid][category_id] && cart[shopid][category_id][item_id] && cart[shopid][category_id][item_id][food_id]) {
			if (cart[shopid][category_id][item_id][food_id]['num'] > 0) {
				cart[shopid][category_id][item_id][food_id]['num']--;
				//返回一个新的对象，否则计算属性无法监听到数据的变化
				state.cartList = Object.assign({}, cart);
				//存入localStorage
				setStore('buyCart', state.cartList);
			} else {
				//商品数量为0，则清空当前商品的信息
				cart[shopid][category_id][item_id][food_id] = null;
			}
		}
	},
	//网页初始化时从本地缓存获取购物车数据
	[INIT_BUYCART](state) {
		let initCart = getStore('buyCart');
		if (initCart) {
			state.cartList = JSON.parse(initCart);
		}
	},
	//清空当前商品的购物车信息
	[CLEAR_CART](state, shopid) {
		state.cartList[shopid] = null;
		state.cartList = Object.assign({}, state.cartList);
		setStore('buyCart', state.cartList);
	},
	// 记录用户信息
	[RECORD_USERINFO](state, info) {
		state.userInfo = info;
		state.login = true;
		let validity = 30;
		let now = new Date();
		now.setTime(now.getTime() + validity * 24 * 60 * 60 * 1000);
		document.cookie = "USERID=" + info.user_id + ";expires=" + now.toGMTString();
		document.cookie = "SID=huRyTRd9QLij7NkbpHJoj3PQrx1eRiO6bAiw" + ";expires=" + now.toGMTString();
	},
	//获取用户信息存入vuex
	[GET_USERINFO](state, info) {
		if (state.userInfo && (state.userInfo.username !== info.username)) {
			return;
		};
		if (!state.login) {
			return
		}
		if (!info.message) {
			state.userInfo = info;
		} else {
			state.userInfo = null;
		}
	},
	//修改用户名
	[RETSET_NAME](state,username) {
		state.userInfo = Object.assign({}, state.userInfo,{username})
	},
	//保存商铺id
	[SAVE_SHOPID](state, shopid) {
		state.shopid = shopid;
	},
	//记录订单页面用户选择的备注, 传递给订单确认页面
	[CONFIRM_REMARK](state, {
		remarkText,
		inputText
	}) {
		state.remarkText = remarkText;
		state.inputText = inputText;
	},
	//是否开发票
	[CONFIRM_INVOICE](state, invoice) {
		state.invoice = invoice;
	},
	//选择搜索的地址
	[CHOOSE_SEARCH_ADDRESS](state, place) {
		state.searchAddress = place;
	},
	//保存geohash
	[SAVE_GEOHASH](state, geohash) {
		state.geohash = geohash;
	},
	//选择的地址
	[CHOOSE_ADDRESS](state, {
		address,
		index
	}) {
		state.choosedAddress = address;
		state.addressIndex = index;
	},
	//保存下单需要验证的返回值
	[NEED_VALIDATION](state, needValidation) {
		state.needValidation = needValidation;
	},
	//保存下单后购物id 和 sig
	[SAVE_CART_ID_SIG](state, {
		cart_id,
		sig
	}) {
		state.cart_id = cart_id;
		state.sig = sig;
	},
	//保存下单参数，用户验证页面调用
	[SAVE_ORDER_PARAM](state, orderParam) {
		state.orderParam = orderParam;
	},
	//修改下单参数
	[CHANGE_ORDER_PARAM](state, newParam) {
		state.orderParam = Object.assign({}, state.orderParam, newParam);
	},
	//下单成功，保存订单返回信息
	[ORDER_SUCCESS](state, order) {
		state.orderMessage = order;
	},
	//进入订单详情页前保存该订单信息
	[SAVE_ORDER](state, orderDetail) {
		state.orderDetail = orderDetail;
	},
	//退出登陆
	[OUT_LOGIN](state) {
		state.userInfo = null;
		state.login = false;
	},
	//保存图片
	[SAVE_AVANDER](state, imgPath) {
		state.imgPath = imgPath;
	},
	//删除地址列表
	[SAVE_ADDRESS](state, newAdress) {
		state.removeAddress = newAdress
	},
}